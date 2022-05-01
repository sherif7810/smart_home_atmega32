#ifndef TIMER_0_CFG_H_
#define TIMER_0_CFG_H_

/*TIMER_0_MODE options -> [ NORMAL_MODE, CTC_MODE, FAST_PWM ]*/
#define TIMER_0_MODE FAST_PWM

/*TIMER_0_D_FACTOR options -> [1024,256]*/
#define TIMER_0_D_FACTOR 1024

#define CRYSTAL_FREQUENCY 16

/*PWM_0_MODE options -> [ FAST_PWM, PHASE_CORRECT_PWM ]*/
#define PWM_0_MODE PHASE_CORRECT_PWM
/*COMPARE_OUTPUT_MODE options -> [ NON_INVERTED, INVERTED ]*/
#define COMPARE_OUTPUT_MODE NON_INVERTED

#endif /* TIMER_0_CFG_H_ */