/*
 * lcd.h
 *
 *  Created on: Dec 8, 2012
 *      Author: lars
 */

#ifndef LCD_H_
#define LCD_H_

#include "common.h"

#include <avr/pgmspace.h>


void lcd_Initialize(void);

void lcd_Clear(void);
void lcd_CursorSet(uint8_t row, uint8_t column);

void lcd_Write(uint8_t c);
void lcd_WriteRepeat(uint8_t c, uint8_t repeat);

uint8_t lcd_WriteString(char const *s);
uint8_t lcd_WriteString_P(char const *s);


// LCD definitions

#define LCD_COLUMNS 20
#define LCD_ROWS    4


// Special HD44780 characters

#define LCD_CHAR_RIGHTARROW 0x7E
#define LCD_CHAR_LEFTARROW 0x7F

#define LCD_CHAR_FILLED_RECT 0xFF
#define LCD_CHAR_OPEN_RECT   0xDB



#endif /* LCD_H_ */
