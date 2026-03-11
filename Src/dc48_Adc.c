/*
 * dc4_Adc.c
 *
 *  Created on: 14. 2. 2019
 *      Author: J�n Harman
 */

/* Includes */
#include <math.h>
#include "stm32h523xx.h"
#include "dc48_Globals.h"

#define ADC1_CHAN__EXTERNAL_TMP		8
#define ADC1_CHAN__CPU_TEMP_SNS		16
#define ADC1_CHAN__INT_VOLT_REF		17
#define ADC1_CHAN__VDD_CORE_VOLT	2
#define ADC_START_CONVERSION		(ADC_CR_ADVREGEN | ADC_CR_ADEN | ADC_CR_ADSTART)
#define ADC_CPU_TEMPERATURE_V30_VOLTAGE			0.62
#define ADC_CPU_TEMPERATURE_AVG_SLOPE			0.002
#define ADC_MAX_12BIT_VALUE						4095.0
#define ADC_FILTER_EXP_CONST					20
#define TS_CAL1_TEMP							30.0
#define TS_CAL2_TEMP							130.0
#define ADC_INTERNAL_REFERENCE_VOLTAGE			1.216

// Toto tady mame protoze Worklight pouziva jen dva vstupy a DSL pouziva navic externi teplotu a ta je na ADC2.
#define ADC_MUM_OF_ADC_CHANS		4

ADC_TypeDef * const SEL_ADC = ADC1;


unsigned int 			Adc_Conversion_Idx;
unsigned int			Adc_Conversion_Chan;
float 					Adc_Filtered_Normalized[ADC_MUM_OF_ADC_CHANS];				// Values ADC filtered [mV]
//float 					Adc_Voltage_Values_NoFilter[ADC_MUM_OF_ADC_CHANS];				// Values ADC raw values [mV]
float 					Adc_Conversion_Ratio;	// Calculated from VREFINT = 1.216V
const short *			VRefIntCal=(const short *)0x08FFF810;	// +1
const short *			TS_CAL1=(const short *)0x08FFF814;		// +1
const short *			TS_CAL2=(const short *)0x08FFF818;		// +1
const int 				Adc_Channels[ADC_MUM_OF_ADC_CHANS] =
{
		ADC1_CHAN__INT_VOLT_REF,
		ADC1_CHAN__CPU_TEMP_SNS,
		ADC1_CHAN__EXTERNAL_TMP,
		ADC1_CHAN__VDD_CORE_VOLT,
};
#define ADC_IDX___IN_VOLT_REF		0
#define ADC_IDX___CPU_TEMPSNS		1
#define ADC_IDX___EXTERNAL_TMP		2
#define ADC_IDX___VDD_CORE_VOLT		3
unsigned int			ADC_RawADC_Data[ADC_MUM_OF_ADC_CHANS];

//#define ADC_TEMPERATURE_SENSOR_1000_Rup1500_OHM
#define ADC_TEMPERATURE_SENSOR_4700_Rup1500_OHM
#define ADC_NUM_OF_APPROX_POINTS		10
#define ADC_TEMPERATURE_APPROX_STEP		128

const struct {
	unsigned int T;
	float N;
}	adc_temp_points[ADC_NUM_OF_APPROX_POINTS] = {
#ifdef ADC_TEMPERATURE_SENSOR_1000_Rup1500_OHM
	{100, 0.5736},
	{228, 0.4239},
	{356, 0.2973},
	{484, 0.2027},
	{612, 0.1371},
	{740, 0.0932},
	{868, 0.0642},
	{996, 0.0449},
	{1124, 0.032},
	{1252, 0.0232},
#endif
#ifdef ADC_TEMPERATURE_SENSOR_4700_Rup1500_OHM
	{100, 0.8634},
	{228, 0.7757},
	{356, 0.6654},
	{484, 0.5444},
	{612, 0.4274},
	{740, 0.3257},
	{868, 0.2437},
	{996, 0.181},
	{1124, 0.1346},
	{1252, 0.1006},
#endif
};

void adc_process_val(int idx)
{
	float * f = &Adc_Filtered_Normalized[idx];
	ADC_RawADC_Data[idx] = SEL_ADC->DR;
	float adc_norm = (SEL_ADC->DR / ADC_MAX_12BIT_VALUE) * Adc_Conversion_Ratio;
	float dif = adc_norm-*f;
	*f += dif/ADC_FILTER_EXP_CONST;
	if (*f < 0.0001) *f = 0;
}

int adc_process_temperature(float normal)
{
	int i, t;
	float e, d;
	if (normal >= adc_temp_points[0].N) return adc_temp_points[0].T;
	for (i=1; i<ADC_NUM_OF_APPROX_POINTS; i++)
	{
		if (normal >= adc_temp_points[i].N)
		{
			d = adc_temp_points[i-1].N - adc_temp_points[i].N;
			e = normal - adc_temp_points[i].N;
			t = e/d * ADC_TEMPERATURE_APPROX_STEP;
			return adc_temp_points[i].T - t;
		}
	}
	return adc_temp_points[ADC_NUM_OF_APPROX_POINTS-1].T+1;
}

void ADC_Job(void)
{
	float f, g;
	if (SEL_ADC->CR & ADC_CR_ADSTART) return;

	adc_process_val(Adc_Conversion_Idx);
	switch (Adc_Conversion_Idx)
	{
	case ADC_IDX___IN_VOLT_REF:
		if (ADC1->DR)
		{
			// Vysledek je takovy, ze po teto korekci meri asi dobre teplotu MCU, ale externi je spatna
			Adc_Conversion_Ratio = (*VRefIntCal) / (float)ADC1->DR;
		}
		break;
	case ADC_IDX___CPU_TEMPSNS:
		// obe metody mereni funguji dost podobne
		f = (TS_CAL2_TEMP - TS_CAL1_TEMP) / (float)(*TS_CAL2 - *TS_CAL1);
		g = Adc_Filtered_Normalized[ADC_IDX___CPU_TEMPSNS] * ADC_MAX_12BIT_VALUE - *TS_CAL1;
		f = f * g + TS_CAL1_TEMP;
//		f = ((Adc_Filtered_Normalized[ADC_IDX___CPU_TEMPSNS] * 3.297 - ADC_CPU_TEMPERATURE_V30_VOLTAGE)/ADC_CPU_TEMPERATURE_AVG_SLOPE)+TS_CAL1_TEMP;
		DisplayMcuTemperature =
		CpuTemperature = f*10;
		break;
	case ADC_IDX___EXTERNAL_TMP:
		// Externi teplomer meri moc
		DisplayExtTemperature =
		ExtTemperature = adc_process_temperature(Adc_Filtered_Normalized[Adc_Conversion_Idx]);
//		CpuVdd = Adc_Filtered_Normalized[Adc_Conversion_Idx] * 3297;
		break;
	case ADC_IDX___VDD_CORE_VOLT:
		// Baterie meri malo
//		CpuVdd = Adc_Filtered_Normalized[ADC_IDX___VDD_CORE_VOLT] * 4000 * 3.3;
		break;
	}
	// increment index and start another conversion
	Adc_Conversion_Idx++;
	if (Adc_Conversion_Idx >= ADC_MUM_OF_ADC_CHANS) Adc_Conversion_Idx = 0;
	Adc_Conversion_Chan = Adc_Channels[Adc_Conversion_Idx];
	SEL_ADC->SQR1 = (Adc_Conversion_Chan << 6);
	SEL_ADC->CR = ADC_START_CONVERSION;
}

void initAdc(void)
{
	RCC->AHB2ENR |= RCC_AHB2ENR_ADCEN; 	// turn on ADC clock

	// ----> ACD1
	ADC1->CR = 0;						// Reset ADC voltage regulator
	ADC1->CR = ADC_CR_ADVREGEN;			// START ADC voltage regulator
	for (int i=0;i<100;i++){};

	ADC1->CR |= ADC_CR_ADCAL; 			// start ADC calibration cycle
	while (ADC1->CR & ADC_CR_ADCAL){};	// wait for calibration to complete

	ADC1->CFGR = 0x80000000; // single, 12bit, right
	ADC1->SMPR1 = 0b00111111111111111111111111111111; // all = 0x07 = 601.5 ADC clock cycles
	ADC1->SMPR2 = 0b00000111111111111111111111111111; // all = 0x07 = 601.5 ADC clock cycles = we are not in a hurry

	ADC12_COMMON->CCR = ADC_CCR_CKMODE | ADC_CCR_VBATEN | ADC_CCR_TSEN | ADC_CCR_VREFEN;// | (1 << ADC_CCR_PRESC_Pos);
//	ADC1->OR = ADC_OR_OP1;		// enable VDD_CORE VOLTAGE for measurement
	ADC1->CR |= ADC_CR_ADEN;	// enable the ADC

	Adc_Conversion_Idx = 0;
	Adc_Conversion_Chan = 0;
}


