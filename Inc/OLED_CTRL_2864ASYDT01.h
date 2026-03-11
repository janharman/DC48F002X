/*
 * OLED_CTRL_2864ASYDT01.h
 *
 *  Created on: 14. 10. 2014
 *      Author: Jan Harman
 */

#ifndef OLED_CTRL_2864ASYDT01_H_
#define OLED_CTRL_2864ASYDT01_H_

extern unsigned char FontBrightness;

void OLED_Init(void);
void Sleep(unsigned char a);
void DisplayLogo(int logo, unsigned char x, unsigned char xe, unsigned char y, unsigned char ye);
void Write_Number_To_Pos(int num, int posx, int posy, int digits);
void Write_Big_Number_To_Pos(int num, int posx, int posy, int digits);
void Write_ExtraBig_Number_To_Pos(int num, int posx, int posy, int digits);
void Write_Small_Number_To_Pos(int num, int posx, int posy, int digits, int dimm);
void Show_Pattern(char const *Data_Pointer, unsigned char x, unsigned char xe, unsigned char y, unsigned char ye);
void Show_Fixed_Pattern(unsigned char x, unsigned char y, unsigned char w, unsigned char h, unsigned char pattrn);
void Clear_Screen(void);
void Show_Small_Characters_String(char *Data_Pointer, unsigned char posx, unsigned char posy, unsigned char dimm);
void Show_Big_Numbers_String(char *Data_Pointer, unsigned char posx, unsigned char posy);
void Show_Big_Numbers_48ALL(unsigned int bigNr, unsigned char posx, unsigned char posy);
void Show_ExtraBig_Numbers_String(char *Data_Pointer, unsigned char posx, unsigned char posy);
void Show_String(char *Data_Pointer, unsigned char posx, unsigned char posy, char invert, int positions);
void Draw_Horizontal_Line(int col, int row, int length, char pattern);
void Draw_Vertical_Bar_Chart(unsigned char chart_val, unsigned char posx, unsigned char posy, unsigned char w, unsigned char h, unsigned char b, unsigned char g);
void Draw_1px_Frame(unsigned char x, unsigned char y, unsigned char w, unsigned char h, unsigned char round, unsigned char pattern);
void Show_Number_As_IP_Addr(unsigned long nr, unsigned char posx, unsigned char posy);
void Show_Chars_In_Integer(unsigned long chrs, unsigned char posx, unsigned char posy, unsigned char digits);
void Show_String_Yes_No(unsigned char nr, unsigned char posx, unsigned char posy);
void Show_6Bytes_As_Mac_Address(char* data, unsigned char posx, unsigned char posy);
void Show_WorkLightIcon(char posx, char posy, char OnOff);
void Show_CrmxSignalLevel(char posx, char posy, int Level);
void Show_CheckBox(char posx, char posy, int visible);
void Show_RadioButton(char posx, char posy, int visible);
void Show_EthernetStatus(char posx, char posy, int visible);
void Show_Button_Dimmed(char x, char y, char dimm, char button);
void Show_ArtNetUniverse(int universe, char x, char y);
void Write_Divided_Number_To_Pos(int num, int posx, int posy, int digits, int divisor);
void Show_ServiceIcon(char posx, char posy, char OnOff);
void Show_Small_Character(char znak, unsigned char posx, unsigned char posy, unsigned char dimm);
void Show_Bytes_In_Hex(char* data, unsigned char posx, unsigned char posy, unsigned char bytes);
void Write_Small_Bits_To_Pos(int num, int posx, int posy, int digits, int dimm);
void Write_Central_One_Chr_To_3Pos(int num, int posx, int posy, int digits);
void Draw_Vertical_Line_1px(unsigned char x, unsigned char y, unsigned char h, unsigned char pattrn);

//void Show_Dimmed_Pattern(unsigned char const *Data_Pointer, unsigned char x, unsigned char xe, unsigned char y, unsigned char ye, unsigned char dimm);


void Draw_Vertical_Bar_Chart_100_38(unsigned char chart_val, unsigned char posx, unsigned char posy, unsigned char w, unsigned char b);
void Draw_Vertical_Bar_Chart_100_40(unsigned char chart_val, unsigned char posx, unsigned char posy, unsigned char w, unsigned char b);

#endif /* OLED_CTRL_2864ASYDT01_H_ */
