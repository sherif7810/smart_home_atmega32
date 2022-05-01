#ifndef KEY_PAD_H_
#define KEY_PAD_H_

#include "../../LIB/STD_TYPES.h"

typedef enum {
	PRESSED = 0, RELEASED = 1
} KeyState_t;

void KEY_PAD_INIT(void);
KeyState_t KEY_PAD_READ(s8*);


#endif /* KEY_PAD_H_ */
