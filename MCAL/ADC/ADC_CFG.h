#ifndef ADC_CFG_H_
#define ADC_CFG_H_

/*V_REF options -> [AVCC,AREF_PIN,_2V]*/
#define V_REF AVCC
/*ADJUST options -> [R_ADJUST,L_ADJUST]*/
#define ADJUST R_ADJUST

/*ADC_CHANNEL options -> [CHANNEL_0 ,..., CHANNEL_7]*/
#define ADC_CHANNEL CHANNEL_0

/*PRESCALER_D_FACTOR options -> [2 ,..., 128]*/
#define PRESCALER_D_FACTOR 128

#endif /* ADC_CFG_H_ */