#ifndef DIO_H_
#define DIO_H_

#include "../../LIB/STD_TYPES.h"

void PIN_MODE(u8,u8);
void PIN_WRITE(u8,u8);
void PIN_TOG(u8);
void PIN_PULL_UP(u8,u8);
u8 PIN_READ(u8);
void PORT_MODE(u8,u8);
void PORT_WRITE(u8,u8);
void PORT_TOG(u8);
u8 PORT_READ(u8);

#define INPUT 0
#define OUTPUT 1

#define PA0 10
#define PA1 11
#define PA2 12
#define PA3 13
#define PA4 14
#define PA5 15
#define PA6 16
#define PA7 17

#define PB0 20
#define PB1 21
#define PB2 22
#define PB3 23
#define PB4 24
#define PB5 25
#define PB6 26
#define PB7 27

#define PC0 30
#define PC1 31
#define PC2 32
#define PC3 33
#define PC4 34
#define PC5 35
#define PC6 36
#define PC7 37

#define PD0 40
#define PD1 41
#define PD2 42
#define PD3 43
#define PD4 44
#define PD5 45
#define PD6 46
#define PD7 47

#define A 1
#define B 2
#define C 3
#define D 4

#define LOW 0
#define HIGH 1

#define DISABLE 0
#define ENABLE 1

#endif /* DIO_H_ */
