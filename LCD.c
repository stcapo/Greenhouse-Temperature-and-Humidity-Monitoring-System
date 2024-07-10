#include "LCD.h"


unsigned char code pic[8][8]={
{0x04,0x0E,0x15,0x04,0x04,0x04,0x04,0x00},//��
{0x00,0x04,0x04,0x04,0x04,0x15,0x0E,0x04},//��
{0x00,0x04,0x08,0x1F,0x08,0x04,0x00,0x00},//��
{0x00,0x04,0x02,0x1F,0x02,0x04,0x00,0x00},//��
{0x04,0x04,0x0A,0x1F,0x1F,0x0A,0x04,0x04},//ռλ��
{0x00,0x1f,0x00,0x1f,0x0a,0x0a,0x13,0x00},// Ԫ
{0,0,0,0x1f,0,0,0,0}, // -
{0,0,0,0x1f,0x1f,0,0,0} // =
}; 
unsigned char code test[8] = {0x07,0x05,0x07,0x00,0x00,0x00,0x00,0x00};

#define LCD_DATA P1

sbit RS = P2^0;
sbit EN = P2^1;



void lcd_command(unsigned char command) {
    LCD_DATA = command;
    RS = 0;
    EN = 1;
    delay_ms(1);
    EN = 0;
}

void lcd_data(unsigned char dat) {
    LCD_DATA = dat;
    RS = 1;
    EN = 1;
    delay_ms(1);
    EN = 0;
}

void lcd_init() {
	//unsigned char num; // �ڼ������ݣ� ��ʾ����Ļ�ϴ������ҵ�����
    delay_ms(15); // ��ʼ����ʱ
    lcd_function_set();
    delay_ms(5); // ������ʱ
    lcd_display_set();
	
    delay_ms(5); // ������ʱ
    lcd_entry_mode_set();
	
    delay_ms(5); // ������ʱ
    lcd_clear_display();
    delay_ms(5); // ������ʱ
	
	load_customized_char(test); // ���� 1 ���Զ����ַ��� CGRAM
	
    /*for (num = 0; num < 8; num++) {
        load_customized_char_array8(num, pic[num]); // ���� 8 ���Զ����ַ��� CGRAM
    }*/
}

void lcd_clear_display(){
	lcd_command(0x01); // write 20H(data) to DDRAM to display a space; set AC to 00H
}

void lcd_return_home(){
	lcd_command(0x20);// set address counter(AC) to 00H, return the cursor to original position(the lefttop); return the screen to original position if it shifted;  not changing the DDRAM;
}
void lcd_entry_mode_set(){
	lcd_command(0x06);// keep screen position and right roll for the cursor when entry; 
}
void lcd_display_set(){ // control the screen and cursor switch
	lcd_command(0x0c); // blink on; cursor off; screen on;
}
void lcd_cursor_or_screen_shift(){
	lcd_command(0x18); // screen right shift will cause the string left shift;
	// 0x18 in binary: 0001 1000
	// Bit 4 (1) - check the screen or cursor shift
	// Bit 3 (1) - S/C : Screen/Cursor (1 = Screen, 0 = cursor)
	// Bit 2 (0) - L/R : Shift direction (1 = Left, 0 = Right)
	// Bit 1 (0) - Unused
	// Bit 0 (0) - Unused
}
void lcd_function_set(){
	lcd_command(0x38); // 8λģʽ, 2��, 5x7����
	// 0x38 in binary: 0011 1000
	// Bit 7 (0) - Unused
	// Bit 6 (0) - Unused
	// Bit 5 (1) - 8-bit interface
	// Bit 4 (1) - 2 lines display
	// Bit 3 (1) - 5x7 dots format
	// Bits 2-0 (000) - Unused
}
void lcd_set_CGRAM_address(unsigned char offset){
	// set CGRAM address to AC
	if(offset>0x40) offset = 0;
	lcd_command(0x40+offset); 
}
void lcd_set_DDRAM_address(unsigned char offset){
	// set DDRAM address to AC
	if(offset>0x80) offset = 0;
	lcd_command(0x80+offset);
}

void goto_XY(unsigned char x,unsigned char y){
	// DDRAM address counter(AC) ---- coordinate of the display
	if(x>0x27) x = 0x27;
	
    if (y == 0) {
        lcd_set_DDRAM_address(x);
    } else {
        lcd_set_DDRAM_address(x+0x40); 
    }
}
void displaydec_lt9(unsigned char num){
	num %=10;
	lcd_data(num+'0');
}
void displaydec_lt256(unsigned char num){ // 253  --- {3,5,2} 2 --- {2}
	unsigned char a[4];
	unsigned char i=0;
	if(num!=0){
		while(num>0){
			a[i++] = num%10 +'0';
			num/=10;
		}
	}else{
		a[i] = '0';
	}
	while(i){
		lcd_data(a[--i]);
	}
}
void displaydec_lt65536(unsigned int num){ 
	unsigned char a[6];
	unsigned char i=0;
	if(num!=0){
		while(num>0){
			a[i++] = num%10 +'0';
			num/=10;
		}
	}else{
		a[i] = '0';
	}
	while(i){
		lcd_data(a[--i]);
	}
}
void displaydec_lt4294967296(unsigned long num){ 
	unsigned char a[11];
	unsigned char i=0;
	if(num!=0){
		while(num>0){
			a[i++] = num%10 +'0';
			num/=10;
		}
	}else{
		a[i] = '0';
	}
	while(i){
		lcd_data(a[--i]);
	}
}
void displayhex_ltFF(unsigned char hex){ // 3F 0011 1111  ---- 0011 
	unsigned char temp;
	temp = hex>>4;
	temp>9?lcd_data(temp-10+'A'):lcd_data(temp+'0');
	temp = hex&0x0F;
	temp>9?lcd_data(temp-10+'A'):lcd_data(temp+'0');
}
void display_string(char *s){
	while(*s!='\0'){
		lcd_data(*s++);
	}
}
void load_customized_char(unsigned char * _test){
	char offset;
	lcd_set_CGRAM_address(offset);
	for(offset=0;offset<8;offset++){
		lcd_data(_test[offset]); // lcd_data��ʱ��LCD�ȿ���û������CGRAM�ĵ�ַ����������ˣ��Ͱ�����д�뵽�����õĵ�ַ�����û���ã�������д��DDRAM��ַ����ʾ��
	}
	lcd_set_DDRAM_address(0);
}
void display_customized_char(){
	lcd_data(0); // lcd_data��ʱ��LCD�ȿ�CGRAM�е�0��ַ��û�����ݣ�����о�д�뵽DDRAM��ַ����ʾ���У�û�о���DDRAM��ַ����ʾ����ʾascii��Ϊ0���ַ�
}
void load_customized_char_array8(unsigned char i0to7, unsigned char * _code) {
    char offset;
    i0to7 = i0to7 << 3; // ���� 3 λ���൱�ڳ��� 8���õ���ǰ�Զ����ַ�����ʼ��ַ

    for (offset = 0; offset < 8; offset++) {
        lcd_set_CGRAM_address(i0to7+offset); // ���� CGRAM ��ַ��0x40 ������ǰ׺
        lcd_data(_code[offset]); // ���ַ�����д�� CGRAM ÿ��_code[i]����һ���ֽڣ������һ���ֽڣ�0x04�ĺ���λ�������е�һ��λ������
    }
	lcd_set_DDRAM_address(0); // ���� DDRAM ��ַΪ 0x80
}

void display_customized_char_array8() {
    unsigned char num;
    for (num = 0; num < 8; num++) {
        lcd_data(num); // ��ʾ 8 ���Զ����ַ�
    }
}




/*
00000
11111
00000
11111
01010
01010
10011
00000
	
  1  
 111
1 1 1
  1
  1
  1
  1
  1
  
00000
00000
00000
11111
00000
00000
00000
00000	

00111
00101
00111
00000
00000
00000
00000
00000	

*/




