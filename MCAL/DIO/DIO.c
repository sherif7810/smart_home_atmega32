#include "DIO.h"
#include "../../LIB/STD_TYPES.h"
#include "../../LIB/BIT_MATH.h"
#include "../REG.h"

void PIN_MODE(u8 x,u8 status) {
	u8 pin = x % 10;
	u8 port = x / 10;
	switch (status)
	{
		case INPUT:
		switch (port)
		{
			case A:
			CLR_BIT(DDRA,pin);
			break;
			case B:
			CLR_BIT(DDRB,pin);
			break;
			case C:
			CLR_BIT(DDRC,pin);
			break;
			case D:
			CLR_BIT(DDRD,pin);
			break;
			default:
			break;
		}
		break;
		case OUTPUT:
		switch (port)
		{
			case A:
			SET_BIT(DDRA,pin);
			break;
			case B:
			SET_BIT(DDRB,pin);
			break;
			case C:
			SET_BIT(DDRC,pin);
			break;
			case D:
			SET_BIT(DDRD,pin);
			break;
			default:
			break;
		}
		break;
		default:
		break;
	}
}

void PIN_WRITE(u8 x,u8 status) {
	u8 pin = x % 10;
	u8 port = x / 10;
	switch (status)
	{
		case LOW:
		switch (port)
		{
			case A:
			CLR_BIT(PORTA,pin);
			break;
			case B:
			CLR_BIT(PORTB,pin);
			break;
			case C:
			CLR_BIT(PORTC,pin);
			break;
			case D:
			CLR_BIT(PORTD,pin);
			break;
			default:
			break;
		}
		break;
		case HIGH:
		switch (port)
		{
			case A:
			SET_BIT(PORTA,pin);
			break;
			case B:
			SET_BIT(PORTB,pin);
			break;
			case C:
			SET_BIT(PORTC,pin);
			break;
			case D:
			SET_BIT(PORTD,pin);
			break;
			default:
			break;
		}
		break;
		default:
		break;
	}
}

void PIN_TOG(u8 x)
{
	u8 pin = x % 10;
	u8 port = x / 10;
	switch (port)
	{
		case A:
		TOG_BIT(PORTA,pin);
		break;
		case B:
		TOG_BIT(PORTB,pin);
		break;
		case C:
		TOG_BIT(PORTC,pin);
		break;
		case D:
		TOG_BIT(PORTD,pin);
		break;
		default:
		break;
	}
}

void PIN_PULL_UP(u8 x,u8 status)
{
	u8 pin = x % 10;
	u8 port = x / 10;
	switch (status)
	{
		case DISABLE:
		switch (port)
		{
			case A:
			CLR_BIT(PORTA,pin);
			break;
			case B:
			CLR_BIT(PORTB,pin);
			break;
			case C:
			CLR_BIT(PORTC,pin);
			break;
			case D:
			CLR_BIT(PORTD,pin);
			break;
			default:
			break;
		}
		break;
		case ENABLE:
		switch (port)
		{
			case A:
			SET_BIT(PORTA,pin);
			break;
			case B:
			SET_BIT(PORTB,pin);
			break;
			case C:
			SET_BIT(PORTC,pin);
			break;
			case D:
			SET_BIT(PORTD,pin);
			break;
			default:
			break;
		}
		break;
		default:
		break;
	}
}

u8 PIN_READ(u8 x)
{
	u8 value = 0;
	u8 pin = x % 10;
	u8 port = x / 10;
	switch (port)
	{
		case A:
		value = GET_BIT(PINA,pin);
		break;
		case B:
		value = GET_BIT(PINA,pin);
		break;
		case C:
		value = GET_BIT(PINA,pin);
		break;
		case D:
		value = GET_BIT(PIND,pin);
		break;
		default:
		break;
	}
	return value;
}

void PORT_MODE(u8 port,u8 status)
{
	switch (status)
	{
		case INPUT:
		switch (port)
		{
			case A:
			DDRA = 0;
			break;
			case B:
			DDRB = 0;
			break;
			case C:
			DDRC = 0;
			break;
			case D:
			DDRD = 0;
			break;
		}
		break;
		case OUTPUT:
		switch (port)
		{
			case A:
			DDRA = 255;
			break;
			case B:
			DDRB = 255;
			break;
			case C:
			DDRC = 255;
			break;
			case D:
			DDRD = 255;
			break;
		}
		break;
	}
}

//void PORT_WRITE(u8 port,u8 status)
void PORT_WRITE(u8 port,u8 value)
{
	/*
	switch (status)
	{
		case LOW:
		switch (port)
		{
			case A:
			PORTA = 0;
			break;
			case B:
			PORTB = 0;
			break;
			case C:
			PORTC = 0;
			break;
			case D:
			PORTD = 0;
			break;
		}
		break;
		case HIGH:
		switch (port)
		{
			case A:
			PORTA = 255;
			break;
			case B:
			PORTB = 255;
			break;
			case C:
			PORTC = 255;
			break;
			case D:
			PORTD = 255;
			break;
		}
		break;
	}
	*/
	switch (port)
	{
		case A:
		PORTA = value;
		break;
		case B:
		PORTB = value;
		break;
		case C:
		PORTC = value;
		break;
		case D:
		PORTD = value;
		break;
	}
}

void PORT_TOG(u8 port)
{
	switch (port)
	{
		case A:
		PORTA ^= 255;
		break;
		case B:
		PORTB ^= 255;
		break;
		case C:
		PORTC ^= 255;
		break;
		case D:
		PORTD ^= 255;
		break;
	}
}

u8 PORT_READ(u8 port)
{
	u8 value = 0;
	switch (port)
	{
		case A:
		value = PINA;
		break;
		case B:
		value = PINB;
		break;
		case C:
		value = PINC;
		break;
		case D:
		value = PIND;
		break;
		default:
		break;
	}
	return value;
}
