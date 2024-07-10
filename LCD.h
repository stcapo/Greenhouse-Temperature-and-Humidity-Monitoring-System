#ifndef _LCD_H
#define _LCD_H
#include "public.h"
#include <reg52.h>
#define LCD_DATA P1


// basic funtions:
void lcd_home(); // Return cursor to home position

void lcd_command(unsigned char command);
void lcd_data(unsigned char dat);
void lcd_init();
void lcd_clear_display();
void lcd_return_home();
void lcd_entry_mode_set();
void lcd_display_set();
void lcd_cursor_or_screen_shift();
void lcd_function_set();
void lcd_set_CGRAM_address(unsigned char offset);
void lcd_set_DDRAM_address(unsigned char offset);



void goto_XY(unsigned char x,unsigned char y);

void displaydec_lt9(unsigned char num);
void displaydec_lt256(unsigned char num);
void displaydec_lt65536(unsigned int num);
void displaydec_lt4294967296(unsigned long num);
void displayhex_ltFF(unsigned char hex);
void display_string(char *s);

void load_customized_char(unsigned char * _test);
void display_customized_char();
void load_customized_char_array8(unsigned char i0to7,unsigned char * _code);
void display_customized_char_array8();

