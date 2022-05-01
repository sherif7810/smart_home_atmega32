#include "LCD_CFG.h"
#include "LCD.h"
#include "../../MCAL/DIO/DIO.h"

#include "avr/delay.h"

void LCD_INIT(void)
{
	PIN_MODE(LCD_RS_PIN,OUTPUT);
	PIN_MODE(LCD_EN_PIN,OUTPUT);
	#if LCD_MODE == _8_BIT_MODE
	PORT_MODE(LCD_DATA_PORT,OUTPUT);
	_delay_ms(100);
	// To select 8-bit mode.
	LCD_WRITE_CMD(0x38);
	#elif LCD_MODE == _4_BIT_MODE
	PIN_MODE(LCD_DATA_PIN_4,OUTPUT);
	PIN_MODE(LCD_DATA_PIN_5,OUTPUT);
	PIN_MODE(LCD_DATA_PIN_6,OUTPUT);
	PIN_MODE(LCD_DATA_PIN_7,OUTPUT);
	_delay_ms(100);
	LCD_WRITE_CMD(0x33);
	LCD_WRITE_CMD(0x32);
	LCD_WRITE_CMD(0x28);
	#endif
	LCD_WRITE_CMD(0x0C);
	LCD_WRITE_CMD(0x01);
	LCD_WRITE_CMD(0x06);
	LCD_WRITE_CMD(0x02);
}

void LCD_WRITE_CMD(u8 command)
{
	#if LCD_MODE == _8_BIT_MODE
	// To select command register.
	PIN_WRITE(LCD_RS_PIN,LOW);
	PORT_WRITE(LCD_DATA_PORT,command);
	// To make enable pin high.
	PIN_WRITE(LCD_EN_PIN,HIGH);
	_delay_ms(1);
	// To make enable pin low.
	PIN_WRITE(LCD_EN_PIN,LOW);
	_delay_ms(5);
	#elif LCD_MODE == _4_BIT_MODE
	// To select command register.
	PIN_WRITE(LCD_RS_PIN,LOW);
	/*To send the left half.*/
	PORT_WRITE(LCD_DATA_PORT,(((command >> 1) & 0b01111000) | (PORT_READ(LCD_DATA_PORT) & 0b10000111)));
	// To make enable pin high.
	PIN_WRITE(LCD_EN_PIN,HIGH);
	_delay_ms(1);
	// To make enable pin low.
	PIN_WRITE(LCD_EN_PIN,LOW);
	/*To send the right half.*/
	PORT_WRITE(LCD_DATA_PORT,(((command <<3) & 0b01111000) | (PORT_READ(LCD_DATA_PORT) & 0b10000111)));
	// To make enable pin high.
	PIN_WRITE(LCD_EN_PIN,HIGH);
	_delay_ms(1);
	// To make enable pin low.
	PIN_WRITE(LCD_EN_PIN,LOW);
	_delay_ms(5);
	#endif
}

void LCD_WRITE_CHR(u8 character)
{
	#if LCD_MODE == _8_BIT_MODE
	// To select data register.
	PIN_WRITE(LCD_RS_PIN,HIGH);
	PORT_WRITE(LCD_DATA_PORT,character);
	// To make enable pin high.
	PIN_WRITE(LCD_EN_PIN,HIGH);
	_delay_ms(1);
	// To make enable pin low.
	PIN_WRITE(LCD_EN_PIN,LOW);
	_delay_ms(5);
	#elif LCD_MODE == _4_BIT_MODE
	// To select data register.
	PIN_WRITE(LCD_RS_PIN,HIGH);
	/*To send the left half.*/
	PORT_WRITE(LCD_DATA_PORT,(((character >> 1) & 0b01111000) | (PORT_READ(LCD_DATA_PORT) & 0b10000111)));
	// To make enable pin high.
	PIN_WRITE(LCD_EN_PIN,HIGH);
	_delay_ms(1);
	// To make enable pin low.
	PIN_WRITE(LCD_EN_PIN,LOW);
	/*To send the right half.*/
	PORT_WRITE(LCD_DATA_PORT,(((character << 3) & 0b01111000) | (PORT_READ(LCD_DATA_PORT) & 0b10000111)));
	// To make enable pin high.
	PIN_WRITE(LCD_EN_PIN,HIGH);
	_delay_ms(1);
	// To make enable pin low.
	PIN_WRITE(LCD_EN_PIN,LOW);
	_delay_ms(5);
	#endif
}

void LCD_WRITE_STR(u8* p)
{
	u8 i = 0;
	while (p[i])
	{
		LCD_WRITE_CHR(p[i]);
		i++;
	}
}

void LCD_WRITE_INT(u32 number)
{
	if (number == 0)
	{
		LCD_WRITE_CHR('0');
		return;
	}
	
	u8 remainder = 0;
	s8 i = 0;
	u8 arr[10] = {0};
	while (number != 0)
	{
		remainder = number % 10;
		arr[i] = remainder;
		i++;
		number /= 10;
	}
	i--;
	while(i >= 0)
	{
		LCD_WRITE_CHR(arr[i] + 48);
		i--;
	}
}

void LCD_WRITE_NUM(s32 number)
{
	if (number < 0)
	{
		LCD_WRITE_CHR('-');
		number *= -1;
		LCD_WRITE_INT(number);
	} else
	{
		LCD_WRITE_INT(number);
	}
}

void LCD_CLR(void)
{
	LCD_WRITE_CMD(0x01);
}

void LCD_GOTO(u8 row,u8 col)
{
	u8 arr[] = {0x80, 0xC0, 0x90, 0xD0};
	LCD_WRITE_CMD(arr[row] + col);
}

void LCD_WRITE_FLOAT(f64 number,u8 n)
{
	if (number < 0.0)
	{
		LCD_WRITE_CHR('-');
		number *= -1.0;
	}
	
	// Display integer part.
	u32 integer_part = (u32) number;
	LCD_WRITE_NUM(integer_part);
	
	LCD_WRITE_CHR('.');
	
	f64 float_part = number - integer_part;
	
	/*
	while (float_part != 0.0f)
	{
		float_part *= 10.0f;
		integer_part = (u32) float_part;
		LCD_WRITE_NUM(integer_part);
		float_part -= (f32) integer_part;
	}
	*/
	
	u8 i = 0;
	u32 j = 10;
	for (; i < n; i++)
	{
		if (i != n - 1)
			LCD_WRITE_NUM(((u32) (float_part * j)) % 10);
		else // Add 1 to the last digit.
			LCD_WRITE_NUM(((u32) (float_part * j)) % 10 + 1);
		j *= 10;
	}
}
