#include "TIMER_0_CFG.h"
#include "TIMER_0.h"
#include "../../LIB/STD_TYPES.h"
#include "../../LIB/BIT_MATH.h"
#include "../REG.h"
#include "../DIO/DIO.h"
#if TIMER_0_MODE != FAST_PWM
#include <avr/interrupt.h>
#endif

// My changes to silence warnings
#if TIMER_0_MODE == NORMAL_MODE
static u32 num_of_ov = 0;
#elif TIMER_0_MODE == CTC_MODE
static u32 total_ticks = 0;
static u32 num_of_cm = 0;
#endif
#if TIMER_0_MODE == FAST_PWM
static u32 total_ticks = 0;
#endif
void(*TIMER_0_CALL_BACK)(void);

void TIMER_0_INIT(void)
{
	#if TIMER_0_MODE == NORMAL_MODE
	CLR_BIT(TCCR0,3);
	CLR_BIT(TCCR0,6);
	// To enable timer0 overflow int
	SET_BIT(TIMSK,0);
	#elif TIMER_0_MODE == CTC_MODE
	SET_BIT(TCCR0,3);
	CLR_BIT(TCCR0,6);
	// To enable timer0 compare match int
	SET_BIT(TIMSK,1);
	#endif
	// To enable global int
	SET_BIT(SREG,7);
}

void TIMER_0_SET_TIME(u32 desired_time)
{
	u32 tick_time =  (TIMER_0_D_FACTOR / CRYSTAL_FREQUENCY);
	total_ticks = (desired_time * 1000) / tick_time;
	#if TIMER_0_MODE == NORMAL_MODE
	num_of_ov = total_ticks / 256;
	if (total_ticks % 256 != 0)
	{
		// Set first overflow only
		TCNT0 = 256 - (total_ticks % 256);
		num_of_ov++;
	}
	#elif TIMER_0_MODE == CTC_MODE
	u8 ocr_value = 255;
	while ((total_ticks % ocr_value) != 0)
	{
		ocr_value--;
	}
	num_of_cm = total_ticks / ocr_value;
	OCR0 = ocr_value - 1;
	#endif
}

void TIMER_0_START(void)
{
	#if TIMER_0_D_FACTOR == 1024
	SET_BIT(TCCR0,0);
	CLR_BIT(TCCR0,1);
	SET_BIT(TCCR0,2);
	#elif TIMER_0_D_FACTOR == 256
	CLR_BIT(TCCR0,0);
	CLR_BIT(TCCR0,1);
	SET_BIT(TCCR0,2);
	#endif
}

void TIMER_0_STOP(void)
{
	CLR_BIT(TCCR0,0);
	CLR_BIT(TCCR0,1);
	CLR_BIT(TCCR0,2);
}

void PWM_0_INIT(void)
{
	SET_BIT(DDRB,3);
	#if PWM_0_MODE == FAST_PWM
	SET_BIT(TCCR0,3);
	SET_BIT(TCCR0,6);
		#if COMPARE_OUTPUT_MODE == NON_INVERTED
		CLR_BIT(TCCR0,4);
		SET_BIT(TCCR0,5);
		#elif COMPARE_OUTPUT_MODE == INVERTED
		SET_BIT(TCCR0,4);
		SET_BIT(TCCR0,5);
		#endif
	#elif PWM_0_MODE == PHASE_CORRECT_PWM
	CLR_BIT(TCCR0,3);
	SET_BIT(TCCR0,6);
		#if COMPARE_OUTPUT_MODE == NON_INVERTED
		CLR_BIT(TCCR0,4);
		SET_BIT(TCCR0,5);
		#elif COMPARE_OUTPUT_MODE == INVERTED
		SET_BIT(TCCR0,4);
		SET_BIT(TCCR0,5);
		#endif
	#endif
}

void PWM_0_SET_DUTY_CYCLE(u8 duty_cycle)
{
	#if PWM_0_MODE == FAST_PWM
		#if COMPARE_OUTPUT_MODE == NON_INVERTED
		OCR0 = (((256 * duty_cycle) / 100) - 1);
		#elif COMPARE_OUTPUT_MODE == INVERTED
		OCR0 = (255 - ((duty_cycle * 256) / 100));
		#endif
	#elif PWM_0_MODE == PHASE_CORRECT_PWM
		#if COMPARE_OUTPUT_MODE == NON_INVERTED
		OCR0 = ((duty_cycle * 255) / 100);
		#elif COMPARE_OUTPUT_MODE == INVERTED
		OCR0 = (255 - ((255 * duty_cycle) / 100));
		#endif
	#endif
}

void PWM_0_START(void)
{
	TIMER_0_START();
}

void PWM_0_STOP(void)
{
	TIMER_0_STOP();
}

void SET_TIMER_0_CALL_BACK(void(*ptr)(void))
{
	TIMER_0_CALL_BACK = ptr;
}

#if TIMER_0_MODE != FAST_PWM

#if TIMER_0_MODE == NORMAL_MODE
ISR(TIMER0_OVF_vect)
#elif TIMER_0_MODE == CTC_MODE
ISR(TIMER0_COMP_vect)
#endif
{
	static u32 counter = 0;
	counter++;
	#if TIMER_0_MODE == NORMAL_MODE
	if (counter == num_of_ov)
	{
		TIMER_0_CALL_BACK();
		counter = 0;
		TCNT0 = 256 - total_ticks % 256;
	}
	#elif TIMER_0_MODE == CTC_MODE
	if (counter == num_of_cm)
	{
		TIMER_0_CALL_BACK();
		counter = 0;
	}
	#endif
}

#endif
