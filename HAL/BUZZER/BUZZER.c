#include "BUZZER_CFG.h"
#include "BUZZER.h"
#include "../../MCAL/DIO/DIO.h"

void BUZZER_INIT(void)
{
	PIN_MODE(BUZZER_PIN,OUTPUT);
}

void BUZZER_ON(void)
{
	PIN_WRITE(BUZZER_PIN,HIGH);
}

void BUZZER_OFF(void)
{
	PIN_WRITE(BUZZER_PIN,LOW);
}

void BUZZER_TOG(void)
{
	PIN_TOG(BUZZER_PIN);
}
