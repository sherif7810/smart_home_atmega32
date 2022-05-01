
#include "../../LIB/STD_TYPES.h"
#include "../../MCAL/ADC/ADC.h"

void LM35_INIT(void)
{
	ADC_INIT();
}

u8 LM35_READ(void)
{
	// LM035 max is 150.
	return ((u32) ADC_READ()) * 100 / 1023;
}
