#ifndef REG_H_
#define REG_H_

#include "../LIB/STD_TYPES.h"
/*GPIO_REGISTERS*/
#define PORTA *(volatile u8*) 0x3B
#define DDRA *(volatile u8*) 0x3A
#define PINA *(volatile u8*) 0x39

#define PORTB *(volatile u8*) 0x38
#define DDRB *(volatile u8*) 0x37
#define PINB *(volatile u8*) 0x36

#define PORTC *(volatile u8*) 0x35
#define DDRC *(volatile u8*) 0x34
#define PINC *(volatile u8*) 0x33

#define PORTD *(volatile u8*) 0x32
#define DDRD *(volatile u8*) 0x31
#define PIND *(volatile u8*) 0x30

#define SREG (*((volatile u8*) 0x5F))
#define GICR (*((volatile u8*) 0x5B))
#define MCUCR (*((volatile u8*) 0x55))

/*ADC_REGISTERS*/
#define ADMUX (*((volatile u8*) 0x27))
#define ADCSRA (*((volatile u8*) 0x26))
#define ADCL (*((volatile u8*) 0x24))
#define ADCH (*((volatile u8*) 0x25))
#define ADC_VALUE (*((volatile u16*) 0x24))

/*TIMER_0_REGISTERS*/
#define TCCR0 (*((volatile u8*) 0x53))
#define TIMSK (*((volatile u8*) 0x59))
#define TCNT0 (*((volatile u8*) 0x52))
#define OCR0 (*((volatile u8*) 0x5C))

/*WDT_REGISTERS*/
#define WDTCR (*((volatile u8*) 0x41))

/*TIMER_1_REGISTERS*/
#define TCCR1A       (*((volatile u8*) 0x4F))
#define TCCR1B       (*((volatile u8*) 0x4E))
#define OCR1A_VALUE  (*((volatile u16*) 0x4A))
#define ICR1_VALUE   (*((volatile u16*) 0x46))

/*UART_REGISTERS*/
#define UCSRA       (*((volatile u8*) 0x2B))
#define UCSRB       (*((volatile u8*) 0x2A))
#define UCSRC       (*((volatile u8*) 0x40))
#define UBRRL       (*((volatile u8*) 0x29))
#define UBRRH       (*((volatile u8*) 0x40))
#define UDR         (*((volatile u8*) 0x2C))

/*SPI_REGISTERS*/
#define SPCR        (*((volatile u8*) 0x2D))
#define SPSR        (*((volatile u8*) 0x2E))
#define SPDR        (*((volatile u8*) 0x2F))

/*I2C_REGISTERS*/
#define TWSR        (*((volatile u8*) 0x21))
#define TWDR        (*((volatile u8*) 0x23))
#define TWAR        (*((volatile u8*) 0x22))
#define TWCR        (*((volatile u8*) 0x56))
#define TWBR        (*((volatile u8*) 0x20))
#endif /* REG_H_ */
