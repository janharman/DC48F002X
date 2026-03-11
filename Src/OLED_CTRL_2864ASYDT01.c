//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
//  UG-2864ASYDT01 (2864-27E)
//  UG-2864ASYDT02 (2864-28E)
//  UG-2864ASYDT03 (2864-31E)
//  UG-2864ASYDT04 (2864-32E)
//  UG-2864ASYDT06 (2864-36E) Reference Code
//
//    Dot Matrix: 128*64
//    Driver IC : SSD1325 (Solomon Systech)
//    Interface : 8-bit 68XX/80XX Parallel, 4-wire SPI
//    Revision  :
//    Date      : 2007/11/25
//    Author    :
//    Editor    : Humphrey Lin
//
//  Copyright (c) Univision Technology Inc.
//
// Knihovna predelana tak, aby fungovala s jadrem Cortex M4.
// Jde o to, ze zapis do pinu daneho portu tady nejde provadet jednoduchym zapisem
// Port.Pin = 1 (nebo 0).
//
// Vyhozena je spousta funkci, ktere pro aplikaci nepotrebuji. Byli to zejmena demo funkce.
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stm32h5xx.h>
#include "dc48_Globals.h"
#include "OLED_Constants.h"
//#include "stdio.h"

#define	M68				// 8-bit 68XX Parallel
						//   BS1=0; BS2=1
//#define		I80				// 8-bit 80XX Parallel
						//   BS1=1; BS2=1
//#define	SPI				// 4-wire SPI
						//   BS1=0; BS2=0
						//   The unused pins should be connected with VSS mostly or floating (D2).
						//   Please refer to the SSD1329 specification for detail.


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Pin Definition
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#define RES_1		OLED_RES_1
#define RES_0		OLED_RES_0
#define CS_1		OLED_CS_1
#define CS_0		OLED_CS_0
#define DC_1		OLED_D_1
#define DC_0		OLED_D_0
#define RW_1		OLED_RW_1
#define RW_0		OLED_RW_0
#define E_1			OLED_E_1
#define E_0			OLED_E_0
#define xData(d)	GPIOA->BSRR = ((uint32_t)0xFF << 16) | (uint32_t)(d & 0xFF) //*(unsigned char *)&GPIOA->ODR = d


//#define RD	P3_0				// Read Signal
//#define WR	P3_1				// Write Signal

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Global Variables
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

unsigned char FontBrightness;	// General font brightness

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Local Variables
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

char NumberToDispArr[32];		// Obsahuje cisla (0x30..0x39) reprezentujici indexy cisel v poli znaku
void Draw_Horizontal_Line_1px(unsigned char x, unsigned char y, unsigned char w, unsigned char pattrn);
void Draw_Vertical_Line_1px(unsigned char x, unsigned char y, unsigned char h, unsigned char pattrn);
const char QuestionMarkAndSpaces[] = "       ?";

#pragma GCC push_options
#pragma GCC optimize ("O0")

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Delay Time
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void uDelay(volatile unsigned int l)
{
	while(l--);
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Read/Write Sequence
//
//	Tady pozor na rychlost procesoru!!!
//	Proto je to tady bez optimalizace. Ale mohlo by to blbnout.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#ifdef M68					// 8-bit 68XX Parallel
void Write_Command(unsigned char Data)
{
	DC_0;
	CS_0;
	RW_0;
	E_1;
	xData(Data);
	E_1;
	E_0;
	RW_1;
	CS_1;
	DC_1;
}

void Write_Data(unsigned char Data)
{
	DC_1;
	CS_0;
	RW_0;
	E_1;
	xData(Data);
	E_1;
	E_0;
	RW_1;
	CS_1;
	DC_1;
}
#endif

#pragma GCC pop_options

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Instruction Setting
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Set_Column_Address(unsigned char a, unsigned char b)
{
	Write_Command(0x15);			// Set Column Address
	Write_Command(a);			//   Default => 0x00
	Write_Command(b);			//   Default => 0x3F (Total Columns Devided by 2)
}


void Set_Row_Address(unsigned char a, unsigned char b)
{
	Write_Command(0x75);			// Set Row Address
	Write_Command(a);			//   Default => 0x00
	Write_Command(b);			//   Default => 0x4F
}


void Set_Contrast_Current(unsigned char d)
{
	Write_Command(0x81);			// Set Contrast Value
	Write_Command(d);			//   Default => 0x40
}


void Set_Current_Range(unsigned char d)
{
	Write_Command(0x84|d);			// Set Current Range
						//   Default => 0x84
						//     0x84 (0x00) => Quarter Current Range
						//     0x85 (0x01) => Half Current Range
						//     0x86 (0x02) => Full Current Range
}


void Set_Remap_Format(unsigned char d)
{
	Write_Command(0xA0);			// Set Re-Map & Data Format
	Write_Command(d);			//   Default => 0x00
						//     Column Address 0 Mapped to SEG0
						//     Disable Nibble Re-Map
						//     Horizontal Address Increment
						//     Scan from COM0 to COM[N-1]
						//     Disable COM Split Odd Even
}


void Set_Start_Line(unsigned char d)
{
	Write_Command(0xA1);			// Set Display Start Line
	Write_Command(d);			//   Default => 0x00
}


void Set_Display_Offset(unsigned char d)
{
	Write_Command(0xA2);			// Set Display Offset
	Write_Command(d);			//   Default => 0x00
}


void Set_Display_Mode(unsigned char d)
{
	Write_Command(0xA4|d);			// Set Display Mode
						//   Default => 0xA4
						//     0xA4 (0x00) => Normal Display
						//     0xA5 (0x01) => Entire Display On, All Pixels Turn On at GS Level 15
						//     0xA6 (0x02) => Entire Display Off, All Pixels Turn Off
						//     0xA7 (0x03) => Inverse Display
}


void Set_Multiplex_Ratio(unsigned char d)
{
	Write_Command(0xA8);			// Set Multiplex Ratio
	Write_Command(d);			//   Default => 0x5F
}


void Set_Master_Config(unsigned char d)
{
	Write_Command(0xAD);			// Set Master Configuration
	Write_Command(0x02|d);			//   Default => 0x03
						//     0x02 (0x00) => Select External VCC Supply
						//     0x03 (0x01) => Select Internal DC/DC Voltage Converter
}


void Set_Display_On_Off(unsigned char d)
{
	Write_Command(0xAE|d);			// Set Display On/Off
						//   Default => 0xAE
						//     0xAE (0x00) => Display Off
						//     0xAF (0x01) => Display On
}


void Set_Phase_Length(unsigned char d)
{
	Write_Command(0xB1);			// Phase 1 & 2 Period Adjustment
	Write_Command(d);			//   Default => 0x53 (5 Display Clocks [Phase 2] / 3 Display Clocks [Phase 1])
						//     D[3:0] => Phase 1 Period in 1~15 Display Clocks
						//     D[7:4] => Phase 2 Period in 1~15 Display Clocks
}


void Set_Frame_Frequency(unsigned char d)
{
	Write_Command(0xB2);			// Set Frame Frequency (Row Period)
	Write_Command(d);			//   Default => 0x25 (37 Display Clocks)
}


void Set_Display_Clock(unsigned char d)
{
	Write_Command(0xB3);			// Display Clock Divider/Osciallator Frequency
	Write_Command(d);			//   Default => 0x41
						//     D[3:0] => Display Clock Divider
						//     D[7:4] => Oscillator Frequency
}


void Set_Precharge_Compensation(unsigned char a, unsigned char b)
{
	Write_Command(0xB4);			// Set Pre-Charge Compensation Level
	Write_Command(b);			//   Default => 0x00 (No Compensation)

	if(a == 0x20)
	{
		Write_Command(0xB0);		// Set Pre-Charge Compensation Enable
		Write_Command(0x08|a);		//   Default => 0x08
						//     0x08 (0x00) => Disable Pre-Charge Compensation
						//     0x28 (0x20) => Enable Pre-Charge Compensation
	}
}


void Set_Precharge_Voltage(unsigned char d)
{
	Write_Command(0xBC);			// Set Pre-Charge Voltage Level
	Write_Command(d);			//   Default => 0x10 (Connect to VCOMH)
}


void Set_VCOMH(unsigned char d)
{
	Write_Command(0xBE);			// Set Output Level High Voltage for COM Signal
	Write_Command(d);			//   Default => 0x1D (0.81*VREF)
}


void Set_VSL(unsigned char d)
{
	Write_Command(0xBF);			// Set Segment Low Voltage Level
	Write_Command(0x02|d);			//   Default => 0x0E
						//     0x02 (0x00) => Keep VSL Pin Floating
						//     0x0E (0x0C) => Connect a Capacitor between VSL Pin & VSS
}


void Set_NOP()
{
	Write_Command(0xE3);			// Command for No Operation
}


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Global Variables
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#define Max_Column	0x3F			// 128/2-1 (Total Columns Devided by 2)
#define Max_Row		0x3F			// 64-1
#define	Brightness	0x7F

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Show Character (5x7)
//
//    b: Ascii
//    x: Start X Address
//    y: Start Y Address
//	  invert: Inversion of the character
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Show_Font57(unsigned char b, unsigned char x, unsigned char y, unsigned char invert)
{
unsigned char const *Src_Pointer;
unsigned char i,Font,MSB,LSB, bn;
unsigned char fontM = FontBrightness << 4;

	Src_Pointer=&Ascii_0[b][0];

	Set_Remap_Format(0x54);
	for(i=0;i<=4;i+=2)
	{
		LSB=*Src_Pointer;
		Src_Pointer++;
		if(i == 4)
		{
			MSB=0x00;
		}
		else
		{
			MSB=*Src_Pointer;
			Src_Pointer++;
		}
		if (invert)
		{
			MSB = ~MSB;
			LSB = ~LSB;
		}
 		Set_Column_Address(x,x);
		Set_Row_Address(y,y+7);

		for (bn=0;bn<8;bn++)
		{
				Font = 0;
				if (MSB&0x01) Font = fontM;
				if (LSB&0x01) Font |= FontBrightness;
				Write_Data(Font);
				MSB >>= 1;
				LSB >>= 1;
		}
		x++;
	}
	Set_Remap_Format(0x50);
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Show String
//
//    posx: Start X Address
//    posy: Start Y Address
//    * Must write "0" in the end...
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Show_String(char *Data_Pointer, unsigned char posx, unsigned char posy, char invert, int positions)
{
	int len = 0;
	int x2 = posx*2;

	while(*Data_Pointer && positions)
	{
		Show_Font57(*Data_Pointer, posx, posy, invert);
		Data_Pointer++;
		posx += 3;
		len += 6;
		positions--;
	}

	while (positions > 0)
	{
		Show_Font57(' ', posx, posy, invert);
		posx += 3;
		len += 6;
		positions--;
	}
	// if ivert, draw left and top line next to text
	// if not invert, draw black left and top line, to clear previous iverted area
	if (invert)
	{
		Draw_Horizontal_Line_1px(x2, posy-1, len, (FontBrightness<<4)|FontBrightness);
		Draw_Vertical_Line_1px(x2-1, posy-1, 9, FontBrightness);
	}
	else
	{
		Draw_Horizontal_Line_1px(x2, posy-1, len, 0);
		Draw_Vertical_Line_1px(x2-1, posy-1, 9, 0);
	}
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Show Pattern (Partial or Full Screen)
//
//    x: Column Address of Start
//    xe: Column Address of End (Total Columns Devided by 2)
//    y: Row Address of Start
//    ye: Row Address of End
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Show_Pattern(char const *Data_Pointer, unsigned char x, unsigned char xe, unsigned char y, unsigned char ye)
{
	char const *Src_Pointer;
	unsigned char i,j;
	
	Src_Pointer=Data_Pointer;
	Set_Column_Address(x,xe);
	Set_Row_Address(y,ye);

	for(i=0;i<(xe-x+1);i++)
		for(j=0;j<(ye-y+1);j++)
			Write_Data(*Src_Pointer++);
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Display Logo
//
//	  logo: not implemented (only if I will want more than one logo)
//    x: Column Address of Start
//    xe: Column Address of End (Total Columns Devided by 2)
//    y: Row Address of Start
//    ye: Row Address of End
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void DisplayLogo(int logo, unsigned char x, unsigned char xe, unsigned char y, unsigned char ye)
{
	Show_Pattern(TS_DIMMERS_Logo, x, xe, y, ye);
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Show Dimmed Pattern (Partial or Full Screen)
//
//    x: Column Address of Start
//    xe: Column Address of End (Total Columns Devided by 2)
//    y: Row Address of Start
//    ye: Row Address of End
//	  dimm: dimmed value
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Show_Dimmed_Pattern(char const *Data_Pointer, unsigned char x, unsigned char xe, unsigned char y, unsigned char ye, unsigned char dimm)
{
	char const *Src_Pointer;
	unsigned char i,j, c, d, diml;

	Src_Pointer=Data_Pointer;
	Set_Column_Address(x, xe);
	Set_Row_Address(y, ye);

	diml = dimm << 4;

	for(i=0;i<(xe-x+1);i++)
	{
		for(j=0;j<(ye-y+1);j++)
		{
			c = *Src_Pointer & 0xf0;
			if (c > diml) c = c-diml; else c = 0;
			d = *Src_Pointer & 0x0f;
			if (d > dimm) d = d-dimm; else d = 0;
			Write_Data(c+d);
			Src_Pointer++;
		}
	}
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Show Fixed Pattern
//
//    x: Column Address of Start
//    y: Row Address of Start
//    w: Number of Columns (Total Columns Devided by 2)
//    h: Number of Rows
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Show_Fixed_Pattern(unsigned char x, unsigned char y, unsigned char w, unsigned char h, unsigned char pattrn)
{
unsigned int i;

	Set_Column_Address(x, x+w-1);
	Set_Row_Address(y, y+h-1);

	i = w*h;
	while (i-- > 0)
			Write_Data(pattrn);
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Show EXTRA Big Numbers String
//
//    posx: Start X Address
//    posy: Start Y Address
//    * Must write "0" in the end...
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Show_ExtraBig_Numbers_String(char *Data_Pointer, unsigned char posx, unsigned char posy)
{
	char znak = *Data_Pointer++;
	while ((znak != 0) && (posx <= 57))
	{
		if (znak == 0x20)
			Show_Pattern(ExtraBigNumbers[10], posx, posx+10, posy, posy+22);
		else
		if (znak == '-')
			Show_Pattern(ExtraBigNumbers[11], posx, posx+10, posy, posy+22);
		else
		{
			znak = znak-0x30;
			if (znak > 9) break;
			Show_Pattern(ExtraBigNumbers[(int)znak], posx, posx+10, posy, posy+22);
		}
		posx += 11;
		znak = *Data_Pointer++;
	}
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Show Big Numbers String
//
//    posx: Start X Address
//    posy: Start Y Address
//    * Must write "0" in the end...
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Show_Big_Numbers_String(char *Data_Pointer, unsigned char posx, unsigned char posy)
{
	unsigned char znak = *Data_Pointer++;
	while ((znak != 0) && (posx <= 57))
	{
		switch (znak)
		{
		case 0x20:
			Show_Pattern(Big_Numbers[10], posx, posx+5, posy, posy+14);
			break;
		case '-':
			Show_Pattern(Big_Numbers[11], posx, posx+5, posy, posy+14);
			break;
		case 'A':
			Show_Pattern(Big_Numbers[12], posx, posx+5, posy, posy+14);
			break;
		case 'L':
			Show_Pattern(Big_Numbers[13], posx, posx+5, posy, posy+14);
			break;
		default:
			znak = znak-0x30;
			if (znak > 9) break;
			Show_Pattern(Big_Numbers[znak], posx, posx+5, posy, posy+14);
		}
		posx += 6;
		znak = *Data_Pointer++;
	}
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Show Big Numbers String
//
//    posx: Start X Address
//    posy: Start Y Address
//    * Must write "0" in the end...
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

const char DimmerNumbersAll[13][3] = {" 1", " 2", " 3", " 4", " 5", " 6", " 7", " 8", " 9", "10", "11", "12", "AL"};

void Show_Big_Numbers_48ALL(unsigned int bigNr, unsigned char posx, unsigned char posy)
{
	unsigned char znak[4];
	unsigned char * z = znak;
	int i = 0;
	bigNr++;
	if (bigNr <= 9)
	{
		znak[i++] = ' ';
		znak[i++] = '0' + bigNr;
	}
	else if (bigNr <= 48)
	{
		znak[i++] = (bigNr / 10) + '0';
		znak[i++] = (bigNr % 10) + '0';
	}
	else
	{
		znak[i++] = 'A';
		znak[i++] = 'L';
	}
	while (i-- && (posx <= 57))
	{
		switch (*z)
		{
		case 0x20:
			Show_Pattern(Big_Numbers[10], posx, posx+5, posy, posy+14);
			break;
		case '-':
			Show_Pattern(Big_Numbers[11], posx, posx+5, posy, posy+14);
			break;
		case 'A':
			Show_Pattern(Big_Numbers[12], posx, posx+5, posy, posy+14);
			break;
		case 'L':
			Show_Pattern(Big_Numbers[13], posx, posx+5, posy, posy+14);
			break;
		default:
			*z = *z-0x30;
			if (*z > 9) break;
			Show_Pattern(Big_Numbers[*z], posx, posx+5, posy, posy+14);
		}
		posx += 6;
		z++;
	}
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Show Small Numbers String
//
//    posx: Start X Address
//    posy: Start Y Address
//    * Must write "0" in the end...
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Show_Small_Characters_String(char *Data_Pointer, unsigned char posx, unsigned char posy, unsigned char dimm)
{
	char znak = *Data_Pointer++;
	while ((znak != 0) && (posx <= 62))
	{
		znak -= 0x20;
		if (znak >= MAX_SMALL_CHARACTER_IDX) break; // out of table
		if (dimm == 0)
			Show_Pattern(Small_Characters[(int)znak], posx, posx+1, posy, posy+4);
		else
			Show_Dimmed_Pattern(Small_Characters[(int)znak], posx, posx+1, posy, posy+4, dimm);
		posx += 2;
		znak = *Data_Pointer++;
	}
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Show Small character - one character
//
//    znak: the only character
//    posx: Start X Address
//    posy: Start Y Address
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Show_Small_Character(unsigned char znak, unsigned char posx, unsigned char posy, unsigned char dimm)
{
	if ((znak != 0) && (posx <= 62) && (posy <= 59))
	{
		znak -= 0x20;
		if (znak >= MAX_SMALL_CHARACTER_IDX) return; // out of table
		if (dimm == 0)
			Show_Pattern(Small_Characters[znak], posx, posx+1, posy, posy+4);
		else
			Show_Dimmed_Pattern(Small_Characters[znak], posx, posx+1, posy, posy+4, dimm);
	}
}

void Draw_OneCoupleOfPixels(int col, int row, char pattrn)
{
	Set_Column_Address(col, col);
	Set_Row_Address(row, row);
	Write_Data(pattrn);
}

void Draw_Horizontal_Line(int col, int row, int length, char pattern)
{
	int i;

	Set_Column_Address(col, 63);
	Set_Row_Address(row, 63);

	for(i=0; i<length; i++)
			Write_Data(pattern);
}

void Draw_Horizontal_Line_1px(unsigned char x, unsigned char y, unsigned char w, unsigned char pattrn)
{
	unsigned char ww;
	unsigned char xd2 = x>>1;

	// zacatek na liche adrese, musime nakreslit jednu dvojici pixelu svitici vpravo = 0xf0
	if (x & 1)
	{
			Draw_OneCoupleOfPixels(xd2, y, pattrn<<4);
			w--;
			xd2++;
	}
	if (w > 2)
	{
			ww = w >> 1;
			Draw_Horizontal_Line(xd2, y, ww, pattrn | (pattrn<<4));
			if (w & 1)
			{
					Draw_OneCoupleOfPixels(xd2+ww, y, pattrn);
			}
	}
}

void Draw_Vertical_Line_1px(unsigned char x, unsigned char y, unsigned char h, unsigned char pattrn)
{
	Set_Remap_Format(0x54);

	Set_Column_Address(x>>1, x>>1);
	Set_Row_Address(y, y+h-1);

	if (x&1) pattrn <<= 4;

	while (h-- > 0)
			Write_Data(pattrn);

	Set_Remap_Format(0x50);
}

void Clear_Screen(void)
{
	//	Fill_RAM(0xff); // toto nejak nefunguje, uvidime, treba se to casem rozchodi :-)
		Draw_Horizontal_Line(0, 0, 4096, 0); // 64*64=4096
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Convert signed number (integer = 4 Bytes) to character array
//
//    num: signed integer
//    en_neg: enable negative numbers
//    digits: number of digits. If num is higher then number of digits, ++++ is replaced
//    in string. If num is lower then number of digits -1 minus sign is replaced in result.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Number_To_Indexed_Array(int num, unsigned int en_neg, unsigned int digits)
{
	int i;
	int zero_suppress = !(digits & NO_ZERO_SUPPRESSION);
	int pos = 0;
	int minus = (num < 0) && (en_neg != 0);
	int divisor = 1;
	unsigned char over;
	int digit_nr; // to store original number of digits

	digits &= 0x0f;
	if (digits > 9) digits = 9;
	digit_nr = digits;

	if (minus)
	{
		*NumberToDispArr = ' ';
		num = -num;
		pos = 1;
		digits--;
	}
	while (digits-- > 1) divisor = divisor * 10;
	i = num / divisor;
	if (i > 9) // number is too high to display in given number of positions
	{
		if (minus) over = '-'; else over = '+';
		pos = 0;
		while (digit_nr--) NumberToDispArr[pos++] = over;
		divisor = 0;
	}

	while (divisor > 0)
	{
			i = num / divisor;
			num = num - i*divisor;
			if ((i != 0) || (!zero_suppress) || (divisor == 1))
			{
					if (minus)
					{
							NumberToDispArr[pos-1] = '-';
							minus = 0;
					}
					NumberToDispArr[pos] = 0x30 + i;
					zero_suppress = 0;
			}
			else NumberToDispArr[pos] = ' ';
			pos++;
			divisor = divisor / 10;
	}
	NumberToDispArr[pos] = 0;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Unsigned Byte to array
//
//    num: unsigned char
//    arr: array to store result
//	  Result is zero supressed, left aligned
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
char * Char_To_CharArr(unsigned char num, char * arr)
{
	int divisor = 100;
	int i;
	int zero_suppress = 1;

	while (divisor > 0)
	{
			i = num / divisor;
			num = num - i*divisor;
			if ((i != 0) || (!zero_suppress) || (divisor == 1))
			{
					*arr++ = 0x30 + i;
					zero_suppress = 0;
			}
			divisor = divisor / 10;
	}
	*arr = 0;
	return arr;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Bytes to Hexadecimal
//
//    data: pointer to data to convert
//    arr: array to store result
//	  count: number of data to be converted
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Bytes_To_Hexadecimal(char * data, char * arr, unsigned char count, unsigned char byte_delimiter)
{
	while (count--)
	{
			*arr++ = HexadecimalNumbers[((*data) & 0xf0)>>4];
			*arr++ = HexadecimalNumbers[(*data++) & 0x0f];
			if (byte_delimiter && count)
				*arr++ = byte_delimiter;
	}
	*arr = 0;
}

//**********************************************************************
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
//	PUBLIC FUNCTIONS
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//**********************************************************************

void Write_Number_To_Pos(int num, int posx, int posy, int digits)
{
	Number_To_Indexed_Array(num, 1, digits);
	Show_String(NumberToDispArr, posx, posy, digits & 0x80, digits & 0x0f);
}

/*
 * If no zero suppresion is needed, digits must contain NO_ZERO_SUPPRESSION
 */

void Write_ExtraBig_Number_To_Pos(int num, int posx, int posy, int digits)
{
	Number_To_Indexed_Array(num, 0, digits);
	Show_ExtraBig_Numbers_String(NumberToDispArr, posx, posy);
}

/*
 * If no zero suppresion is needed, digits must contain NO_ZERO_SUPPRESSION
 */

void Write_Big_Number_To_Pos(int num, int posx, int posy, int digits)
{
	Number_To_Indexed_Array(num, 0, digits);
	Show_Big_Numbers_String(NumberToDispArr, posx, posy);
}

/*
 * If no zero suppresion is needed, digits must contain NO_ZERO_SUPPRESSION
 */

void Write_Small_Number_To_Pos(int num, int posx, int posy, int digits, int dimm)
{
	Number_To_Indexed_Array(num, 1, digits);
	Show_Small_Characters_String(NumberToDispArr, posx, posy, dimm);
}

void Draw_1px_Frame(unsigned char x, unsigned char y, unsigned char w, unsigned char h, unsigned char round, unsigned char pattern)
{
	if (!round)
	{
			Draw_Horizontal_Line_1px(x, y, w, pattern);
			Draw_Horizontal_Line_1px(x, y+h-1, w, pattern);
	}
	else
	{
			Draw_Horizontal_Line_1px(x+1, y, w-2, pattern);
			Draw_Horizontal_Line_1px(x+1, y+h-1, w-2, pattern);
	}
	Draw_Vertical_Line_1px(x, y+1, h-2, pattern);
	Draw_Vertical_Line_1px(x+w-1, y+1, h-2, pattern);
}

//---------------------------------------------------------------------------------
// Draws simple vertical bar - !!! without clearing any region above !!! Use Draw_Vertical_Bar_Chart to draw chart according to chart value.
//
// posx = x position in pixels [0..127]
// posy = row
// w = bar width in pixels
// h = height of the bar -> must be in range [1..64]
// b = brightness (both nibbles have to be filled. fox ex: 0xdd, not only 0x0d)
//---------------------------------------------------------------------------------
void Draw_Vertical_Bar(unsigned char posx, unsigned char posy, unsigned char w, unsigned char h, unsigned char b)
{
	unsigned char ww;
	unsigned char x = posx >> 1;

	if (posx & 1)	// zacatek na lichem sloupci
	{
			Show_Fixed_Pattern(x, posy, 1, h, b&0xf0); // udelat caru na pravem pixelu
			x++;
			w--;
	}
	if (w >= 2)	// plne dvojice pixelu
	{
			ww = w >> 1;
			Show_Fixed_Pattern(x, posy, ww, h, b);
			x += ww;
	}
	if (w & 1)	// posledni lichy pixel (levy)
	{
			Show_Fixed_Pattern(x, posy, 1, h, b&0x0f);

	}
}

//---------------------------------------------------------------------------------
// Draw dynamic vertical bar.
//
// chart_val = value of the chart -> must be less or qual h
// posx = x position in pixels [0..127]
// posy = row
// w = bar width in pixels
// h = height of the bar -> must be in range [1..64]
// b = brightness (both nibbles have to be filled. fox ex: 0xdd, not only 0x0d)
// g = line between bar and space
//---------------------------------------------------------------------------------
void Draw_Vertical_Bar_Chart(unsigned char chart_val, unsigned char posx, unsigned char posy, unsigned char w, unsigned char h, unsigned char b, unsigned char g)
{
	unsigned char ww;
	unsigned char x;
	unsigned char hmv;
	unsigned char gg;


	if (g) chart_val++;
	hmv = h-chart_val;

	if (h > chart_val)
	{
			ww = w;
			x = posx;
			if (x & 1)	// zacatek na lichem sloupci
			{
					ww++;		// zacnu na sudem, mazat se muze i o jeden sloupec vedle
					x--;
					if (ww & 1) ww++;
			}
			if (ww > 1) ww >>= 1;
			x >>= 1;
			Show_Fixed_Pattern(x, posy, ww, hmv, 0);
	}
	if (chart_val > 0)
	{
			if (chart_val >= h)
			{
				chart_val = h;
				g = 0;
			}
			else
				posy += hmv;
			x = posx >> 1;

			if (g)
			{
					gg = 0xff-b;
					if (g > gg) g -= gg; else g = 0;
					Draw_Vertical_Bar(posx, posy, w, 1, g);
					posy++;
					chart_val--;
			}
			if (chart_val)
					Draw_Vertical_Bar(posx, posy, w, chart_val, b);
	}
}

//---------------------------------------------------------------------------------
// Draw dynamic vertical bar 100/38.
// Maximum input value is 100
// Chart height is 38
//
// chart_val = value of the chart -> maximum is 100
// posx = x position in pixels [0..127]
// posy = row
// w = bar width in pixels
// b = brightness (both nibbles have to be filled. fox ex: 0xdd, not only 0x0d)
//
// chart_val se deli 0.38, aby se dalo dosahnout 38 pri vstupu 100. Cele se to jeste vynasobi 16-ti, aby bylo na caru rozhrani.
// 24904/65536=0.38
//---------------------------------------------------------------------------------
void Draw_Vertical_Bar_Chart_100_38(unsigned char chart_val, unsigned char posx, unsigned char posy, unsigned char w, unsigned char b)
{
	int		value16 = (chart_val * 16 * 24904) >> 16;
	int		chart_value = value16 >> 4;
	int		to_gray = value16 & 0x0f;

	to_gray |= to_gray << 4;
	Draw_Vertical_Bar_Chart(chart_value, posx, posy, w, 38, b, to_gray);
}

void Draw_Vertical_Bar_Chart_100_40(unsigned char chart_val, unsigned char posx, unsigned char posy, unsigned char w, unsigned char b)
{
	int		value16 = (chart_val * 16 * 26215) >> 16;
	int		chart_value = value16 >> 4;
	int		to_gray = value16 & 0x0f;

	to_gray |= to_gray << 4;
	Draw_Vertical_Bar_Chart(chart_value, posx, posy, w, 40, b, to_gray);
}

//---------------------------------------------------------------------------------
// Shows Number in dot notation like an IP address. For example 192.168.1.253
//
// Nr = 4 byte number
// posx = x position (x divided by 2)
// posy = row
//---------------------------------------------------------------------------------
void Show_Number_As_IP_Addr(unsigned long nr, unsigned char posx, unsigned char posy)
{
	int i;
	char * uc = NumberToDispArr;
	char * pnr = (char *)&nr +3;
	for (i=0;i<4;i++)
	{
			uc = Char_To_CharArr(*pnr--, uc);
			*uc++ = '.';
	}
	*--uc = 0;
	Show_String(NumberToDispArr, posx, posy, 0, -1);
}

//---------------------------------------------------------------------------------
// Shows characters stored in one 32-bit number (chrs)
//
// chrs = 4 byte number
// posx = x position (x divided by 2)
// posy = row
//---------------------------------------------------------------------------------
void Show_Chars_In_Integer(unsigned long chrs, unsigned char posx, unsigned char posy, unsigned char digits)
{
	*(unsigned long *)NumberToDispArr = chrs;
	NumberToDispArr[digits] = 0;
	Show_String(NumberToDispArr, posx, posy, 0, digits);
}

void Show_String_Yes_No(unsigned char nr, unsigned char posx, unsigned char posy)
{
	Show_String((void*)StringsYesNo[nr>0], posx, posy, 0, 3);
}

//---------------------------------------------------------------------------------
// Shows 6 Bytes data as MAC Address (with double dots notation)
//
// data: pointer to data
// posx = x position (x divided by 2)
// posy = row
//---------------------------------------------------------------------------------
void Show_6Bytes_As_Mac_Address(char* data, unsigned char posx, unsigned char posy)
{
	Bytes_To_Hexadecimal(data, NumberToDispArr, 6, ':');
	NumberToDispArr[17] = 0;
	Show_String(NumberToDispArr, posx, posy, 0, 17);
}

void Show_WorkLightIcon(char posx, char posy, char OnOff)
{
	if (OnOff)
		Show_Pattern(WorkLightIcon, posx, posx+4, posy, posy+5);
	else
		Show_Fixed_Pattern(posx, posy, 5, 6, 0);
}

void Show_CheckBox(char posx, char posy, int visible)
{
	Show_Pattern(NotOK_OK_Symbol[visible], posx, posx+3, posy, posy+4);
}

void Show_RadioButton(char posx, char posy, int visible)
{
	Show_Pattern(RadioButton[visible != 0], posx, posx+2, posy, posy+4);
}

/* definice jsou v a2Ethernet.h
#define ETHERNET_STS_ENABLED_SACN		1
#define ETHERNET_STS_ENABLED_ARTNET		2
#define ETHERNET_STS_PLUGIN_MODULE		4
#define ETHERNET_STS_LINK				8
#define ETHERNET_STS_PROTOCOL			16
#define ETHERNET_STS_DMX_OK				32
 */
void Show_EthernetStatus(char posx, char posy, int visible)
{
	if (!(visible & 3))									// Both Ethernet protocols are disabled
	{
			Show_Fixed_Pattern(posx, posy, 7, 5, 0); 	// Disabled
	}
	else
	if (!(visible & 4))									// Some protocol is enabled, but module is not present
	{
			Show_CheckBox(posx, posy, 0);				// No Module
			Show_Fixed_Pattern(posx+4, posy, 3, 5, 0);
	}
	else
	if (!(visible & 8))									// Module is present, but no ethernet connection
	{
//			Show_RadioButton(posx, posy, 0);			// No Line
			Show_CheckBox(posx, posy, 0);
			Show_CheckBox(posx+3, posy, 0);
	}
	else
	if (!(visible & 16))								// Ethernet connection is OK, but no protocol is present
	{
			Show_RadioButton(posx, posy, 0);			// No Protocol
			Show_CheckBox(posx+3, posy, 0);
	}
	else
	if (!(visible & 32))								// Protocol is present, but no desired universe
	{
			Show_RadioButton(posx, posy, 1);			// Protocol + Universe
			Show_CheckBox(posx+3, posy, 0);
	}
	else												// Universe is present, everything is OK
	{
			Show_RadioButton(posx, posy, 1);			// Protocol + Universe
			Show_CheckBox(posx+3, posy, 1);
	}
}

void Show_CrmxSignalLevel(char posx, char posy, int Level)
{
	if (Level & 0x00000001) // Enabled CRMX (Wi-Fi)
	{
			if (Level & 0x00000002)
			{
					Show_Pattern(CrmxSignalLevelPattern[(Level & 0xF0000000) >> 28], posx, posx+5, posy+1, posy+4);
					Show_CheckBox(posx+6, posy, !!(Level & 0x08));
			}
			else	// No module present
			{
					Show_CheckBox(posx, posy, 0);
					Show_Fixed_Pattern(posx+4, posy, 6, 5, 0);
			}
	}
	else // Disabled
			Show_Fixed_Pattern(posx, posy, 10, 5, 0);
}

void Show_Button_Dimmed(char x, char y, char dimm, char button)
{
	Show_Dimmed_Pattern(ButtonPatterns[(int)button], (unsigned char)x, (unsigned char)(x+4), (unsigned char)y, (unsigned char)(y+7), (unsigned char)dimm);
}

// Shows ArtNet Universe number. It consists of:
// - Network [0..127]
// - SubNetwork [0..15]
// - Universe Number [0..15]
//
void Show_ArtNetUniverse(int universe, char x, char y)
{
	char * s = Char_To_CharArr(universe>>8, &NumberToDispArr[4]);
	*s++ = ':';
	s = Char_To_CharArr((universe>>4)&15, s);
	*s++ = ':';
	s = Char_To_CharArr(universe&15, s);
	*(int*)NumberToDispArr = 0x20202020;
	Show_String(s-9, x, y, 0, 9);
}

void Write_Divided_Number_To_Pos(int num, int posx, int posy, int digits, int divisor)
{
	int dig = digits >> 4;
	if (num >= 0)
	{
			Write_Number_To_Pos(num/divisor, posx, posy, dig);
			posx += 3*dig;
			Show_Font57('.', posx, posy, 0);
			posx += 3;
			dig = digits & 0xf;
			Write_Number_To_Pos(num%divisor, posx, posy, dig | 0x10);
	}
	else
	{
			dig += (digits & 0xf)+1;
			Show_String((char*)&QuestionMarkAndSpaces[8-dig], posx, posy, 0, -1);
	}
}

void Show_ServiceIcon(char posx, char posy, char OnOff)
{
	if (OnOff)
		Show_Pattern(ServiceIcon, posx, posx+3, posy, posy+7);
	else
		Show_Fixed_Pattern(posx, posy, 4, 8, 0);
}

void Show_Bytes_In_Hex(char* data, unsigned char posx, unsigned char posy, unsigned char bytes)
{
	Bytes_To_Hexadecimal(data, NumberToDispArr, bytes, 0);
	NumberToDispArr[17] = 0;
	Show_String(NumberToDispArr, posx, posy, 0, bytes*2);
}

/* Writes bit representation of the number.
 *
 * Inputs:
 * 		digits = number of bits to write (maximum is 16, minimmum is 1)
 * */

void Write_Small_Bits_To_Pos(int num, int posx, int posy, int digits, int dimm)
{
	char * c = &NumberToDispArr[17];

	*c = 0;
	while (digits--)
	{
		c--;
		*c = num&1?'1':'0';
		num >>= 1;
	}
	Show_Small_Characters_String(c, posx, posy, dimm);
}

void Write_Central_One_Chr_To_3Pos(int num, int posx, int posy, int digits)
{
	NumberToDispArr[3] = 0;
	NumberToDispArr[2] = ' ';
	NumberToDispArr[1] = num?'0'+num:'-';
	NumberToDispArr[0] = ' ';
	Show_String(NumberToDispArr, posx, posy, digits & 0x80, digits & 0x0f);
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Sleep Mode
//
//    "0x01" Enter Sleep Mode
//    "0x00" Exit Sleep Mode
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Sleep(unsigned char a)
{
	switch(a)
	{
		case 0: // Exit Sleep Mode
			Set_Display_On_Off(0x01);
			Set_Display_Mode(0x00);
			break;
		case 1:	// Enter Sleep Mode
			Set_Display_Mode(0x02);
			Set_Display_On_Off(0x00);
			break;
	}
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Gray Scale Table Setting (Full Screen)
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#define gss 0x33
void Set_Gray_Scale_Table()
{
	Write_Command(0xB8);			// Set Gray Scale Table

	Write_Command(0x01);			//   Gray Scale Level 1
	Write_Command(0x11);			//   Gray Scale Level 3 & 2
	Write_Command(0x22);			//   Gray Scale Level 5 & 4
	Write_Command(0x32);			//   Gray Scale Level 7 & 6
	Write_Command(0x43);			//   Gray Scale Level 9 & 8
	Write_Command(0x54);			//   Gray Scale Level 11 & 10
	Write_Command(0x65);			//   Gray Scale Level 13 & 12
	Write_Command(0x76);			//   Gray Scale Level 15 & 14
/*
	Write_Command(gss);			//   Gray Scale Level 1
	Write_Command(gss);			//   Gray Scale Level 3 & 2
	Write_Command(gss);			//   Gray Scale Level 5 & 4
	Write_Command(gss);			//   Gray Scale Level 7 & 6
	Write_Command(gss);			//   Gray Scale Level 9 & 8
	Write_Command(gss);			//   Gray Scale Level 11 & 10
	Write_Command(gss);			//   Gray Scale Level 13 & 12
	Write_Command(gss);			//   Gray Scale Level 15 & 14
*/
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Initialization : THIS IS PUBLIC FUNCTION AS WELL
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void OLED_Init()
{
	CS_0;		// docasne, potom zrusit az bude mit hardwarove CS porad na nule
	RES_0;
	uDelay(3000);
	RES_1;
	uDelay(500);

	Set_Display_On_Off(0x00);		// Display Off (0x00/0x01)
	Set_Display_Clock(0x91);		// Set Clock as 135 Frames/Sec
	Set_Multiplex_Ratio(63);		// 1/64 Duty (0x0F~0x5F)
	Set_Display_Offset(76);			// Shift Mapping RAM Counter (0x00~0x5F)
	Set_Start_Line(0x00);			// Set Mapping RAM Display Start Line (0x00~0x5F)
	Set_Master_Config(0x00);		// Disable Embedded DC/DC Converter (0x00/0x01)
	Set_Remap_Format(0x50);			// Set Column Address 0 Mapped to SEG0
						//     Disable Nibble Remap
						//     Horizontal Address Increment
						//     Scan from COM[N-1] to COM0
						//     Enable COM Split Odd Even
	Set_Current_Range(0x02);		// Set Full Current Range
	Set_Gray_Scale_Table();			// Set Pulse Width for Gray Scale Table
	Set_Contrast_Current(Brightness);	// Set Scale Factor of Segment Output Current Control
	Set_Frame_Frequency(0x46);		// Set Frame Frequency
	Set_Phase_Length(0x22);			// Set Phase 1 as 2 Clocks & Phase 2 as 2 Clocks
	Set_Precharge_Voltage(0x10);	// Set Pre-Charge Voltage Level
	Set_Precharge_Compensation(0x20,0x07);	// Set Pre-Charge Compensation
	Set_VCOMH(0x02);				// Set High Voltage Level of COM Pin
	Set_VSL(0x0C);					// Set Low Voltage Level of SEG Pin
	Set_Display_Mode(0x00);			// Normal Display Mode (0x00/0x01/0x02/0x03)

//	Fill_RAM(0x00);					// Clear Screen
	Show_Fixed_Pattern(0, 0, 64, 64, 0x00);

	Set_Display_On_Off(0x01);		// Display On (0x00/0x01)
	uDelay(400);
	FontBrightness = 0x0f;
}

// end of file
