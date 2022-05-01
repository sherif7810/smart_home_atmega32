#ifndef LCD_H_
#define LCD_H_

#include "../../LIB/STD_TYPES.h"

void LCD_INIT(void);
void LCD_WRITE_CMD(u8);
void LCD_WRITE_CHR(u8);
void LCD_WRITE_STR(u8*);
void LCD_WRITE_INT(u32);
void LCD_WRITE_NUM(s32);
void LCD_CLR(void);
void LCD_GOTO(u8,u8);
void LCD_WRITE_FLOAT(f64,u8);

#define _4_BIT_MODE 4 // For example.
#define _8_BIT_MODE 8

#endif /* LCD_H_ */
