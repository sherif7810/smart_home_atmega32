#ifndef ADC_H_
#define ADC_H_

void ADC_INIT(void);
u16 ADC_READ(void);
u16 ADC_READ_INT(void);
void ADC_START_CONVERSION_INT(u8);
void ADC_SET_CALL_BACK(void(*)(void));

#define AVCC 1
#define AREF_PIN 2
#define _2V 3

#define R_ADJUST 0
#define L_ADJUST 1

#define CHANNEL_0 0
#define CHANNEL_1 1
#define CHANNEL_2 2
#define CHANNEL_3 3
#define CHANNEL_4 4
#define CHANNEL_5 5
#define CHANNEL_6 6
#define CHANNEL_7 7

#endif /* ADC_H_ */
