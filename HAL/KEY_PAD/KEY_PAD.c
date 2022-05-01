#include "../../LIB/STD_TYPES.h"

#include "KEY_PAD_CFG.h"
#include "KEY_PAD.h"
#include "../../MCAL/DIO/DIO.h"
#include <util/delay.h>

void KEY_PAD_INIT(void) {
	PIN_MODE(KEY_PAD_R0, OUTPUT);
	PIN_MODE(KEY_PAD_R1, OUTPUT);
	PIN_MODE(KEY_PAD_R2, OUTPUT);
	PIN_MODE(KEY_PAD_R3, OUTPUT);

	PIN_MODE(KEY_PAD_C0, INPUT);
	PIN_MODE(KEY_PAD_C1, INPUT);
	PIN_MODE(KEY_PAD_C2, INPUT);

	PIN_WRITE(KEY_PAD_C0, HIGH);
	PIN_WRITE(KEY_PAD_C1, HIGH);
	PIN_WRITE(KEY_PAD_C2, HIGH);
}

KeyState_t KEY_PAD_READ(s8* key) {
	u8 r = 0;
	u8 c = 0;
	KeyState_t value = RELEASED;
	for (r = KEY_PAD_R0; r <= KEY_PAD_R3; r++) {
		PIN_WRITE(r, LOW);
		for (c = KEY_PAD_C0; c <= KEY_PAD_C2; c++) {
			value = PIN_READ(c);
			if (value == PRESSED) {
				_delay_ms(120);
				value = PIN_READ(c);
				if (value == PRESSED) {
					while (PIN_READ(c) == PRESSED)
						;

					if (r == KEY_PAD_R0 && c == KEY_PAD_C0)
						*key = '3';
					else if (r == KEY_PAD_R0 && c == KEY_PAD_C1)
						*key = '2';
					else if (r == KEY_PAD_R0 && c == KEY_PAD_C2)
						*key = '1';
					else if (r == KEY_PAD_R1 && c == KEY_PAD_C0)
						*key = '6';
					else if (r == KEY_PAD_R1 && c == KEY_PAD_C1)
						*key = '5';
					else if (r == KEY_PAD_R1 && c == KEY_PAD_C2)
						*key = '4';
					else if (r == KEY_PAD_R2 && c == KEY_PAD_C0)
						*key = '9';
					else if (r == KEY_PAD_R2 && c == KEY_PAD_C1)
						*key = '8';
					else if (r == KEY_PAD_R2 && c == KEY_PAD_C2)
						*key = '7';
					else if (r == KEY_PAD_R3 && c == KEY_PAD_C0)
						*key = '#';
					else if (r == KEY_PAD_R3 && c == KEY_PAD_C1)
						*key = '0';
					else if (r == KEY_PAD_R3 && c == KEY_PAD_C2)
						*key = '*';

					return PRESSED;
				}
			}
		}
		PIN_WRITE(r, HIGH);
	}
	return RELEASED;
}
