#ifndef TIMER_0_H_
#define TIMER_0_H_

#include "../../LIB/STD_TYPES.h"

void TIMER_0_INIT(void);
void TIMER_0_SET_TIME(u32);
void TIMER_0_START(void);
void TIMER_0_STOP(void);

void PWM_0_INIT(void);
void PWM_0_SET_DUTY_CYCLE(u8);
void PWM_0_START(void);
void PWM_0_STOP(void);

void SET_TIMER_0_CALL_BACK(void(*)(void));

#define NORMAL_MODE 1
#define CTC_MODE 2
#define FAST_PWM 3
#define PHASE_CORRECT_PWM 4
#define NON_INVERTED 5
#define INVERTED 6

#endif /* TIMER_0_H_ */
