#include "ADC_CFG.h"
#include "../../LIB/STD_TYPES.h"
#include "../../LIB/BIT_MATH.h"
#include "../DIO/DIO.h"
#include "../REG.h"
#include <avr/interrupt.h>

static void(*ADC_CALL_BACK)(void);

void ADC_INIT(void)
{
	#if V_REF == AVCC
	CLR_BIT(ADMUX,7);
	SET_BIT(ADMUX,6);
	#elif V_REF == AREF_PIN
	CLR_BIT(ADMUX,7);
	CLR_BIT(ADMUX,6);
	#elif V_REFF == _2V
	SET_BIT(ADMUX,7);
	SET_BIT(ADMUX,6);
	#endif
	#if ADJUST == R_ADJUST
	CLR_BIT(ADMUX,5);
	#elif ADJUST == L_ADJUST
	SET_BIT(ADMUX,5);
	#endif
	#if ADC_CHANNEL == CHANNEL_0
	CLR_BIT(ADMUX,4);
	CLR_BIT(ADMUX,3);
	CLR_BIT(ADMUX,2);
	CLR_BIT(ADMUX,1);
	CLR_BIT(ADMUX,0);
	#endif
	// Prescaler division factor
	#if PRESCALER_D_FACTOR == 128
	SET_BIT(ADCSRA,0);
	SET_BIT(ADCSRA,1);
	SET_BIT(ADCSRA,2);
	#endif
	
	// To enable auto trigger mode
	//SET_BIT(ADCSRA,5);
	// To enable ADC
	SET_BIT(ADCSRA,7);

	// Enable interrupt
	SET_BIT(ADCSRA,3);

	// Enable global interrupt
	SET_BIT(SREG,7);
}

u16 ADC_READ(void)
{
	SET_BIT(ADCSRA,6);
	while (GET_BIT(ADCSRA,4) == 0);
	return ADC_VALUE;
}

u16 ADC_READ_INT(void)
{
	return ADC_VALUE;
}

void ADC_START_CONVERSION_INT(u8 channel)
{
	// Select channel
	channel &= 0x07;
	ADMUX &= 0xE0;
	ADMUX |= channel;
	// Start conversion
	SET_BIT(ADCSRA,6);
}

void ADC_SET_CALL_BACK(void(*ptr)(void))
{
	ADC_CALL_BACK = ptr;
}

ISR(ADC_vect)
{
	ADC_CALL_BACK();
}
