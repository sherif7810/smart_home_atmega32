#include "LIB/BIT_MATH.h"

#include "HAL/LCD/LCD.h"
#include "HAL/KEY_PAD/KEY_PAD.h"
#include "HAL/BUZZER/BUZZER.h"
#include "MCAL/TIMER_0/TIMER_0.h"
#include "MCAL/ADC/ADC.h"
#include "MCAL/REG.h"

#include "Free_RTOS/FreeRTOS.h"
#include "Free_RTOS/task.h"
#include "Free_RTOS/semphr.h"

#include <stdint.h>
#include <avr/eeprom.h>

void LoginTask(void *pv);
void ADCTask(void *pv);
void TemperatureTask(void *pv);
void FanTask(void *pv);
void DoorTask(void *pv);
void BuzzerTask(void *pv);
void LCDTask(void *pv);

void ADC_isr(void);

#define NUMBER_OF_FAILED_ATTEMPTS 3
#define PASSWORD 5124
#define PASSWORD_ADDRESS 0

xSemaphoreHandle LCDSem, ADCSem;
static u8 temperature = 0, door_opened_then_closed = 1, // 0, if door is not opened then closed. 1, if door is opened then closed.
		number_of_characters = 0, // Number of characters entered from key pad
		fan_on = 0, // 0, if fan is off. 1, if fan is on.
		alarm_on = 1, // 0, if alarm is not turned on. 1, if alarm is turned on. Initial value is 1, so that alarm is not turned on at the beginning.
		buzzer_on = 0, // 0, if buzzer is not turned on. 1, if buzzer is turned on.
		door_opened = 0; // 0, if door is closed. 1, if door is opened.
static u16 password = 0;
static u16 adc_reading = 0; // Reading from ADC.

int main(void) {
	// Initialize peripherals and devices
	LCD_INIT();
	KEY_PAD_INIT();
	BUZZER_INIT();
	// PWM
	PWM_0_INIT(); // Initialize Timer 0 PWM
	PWM_0_START(); // Start generating PWM connected to fan motor
	// Door motor
	DDRB |= 0b00110000; // Set motor pins as output

	// ADC
	ADC_INIT();
	// Set ADC call back
	ADC_SET_CALL_BACK(ADC_isr);
	// Start first conversion
	ADC_START_CONVERSION_INT(CHANNEL_0);

	// Create taken counting semaphore. It is given in ADC ISR.
	ADCSem = xSemaphoreCreateCounting(1, 0);
	// Create binary semaphore
	vSemaphoreCreateBinary(LCDSem);

	eeprom_write_word((uint16_t*) PASSWORD_ADDRESS, PASSWORD);
	password = eeprom_read_word((uint16_t*) PASSWORD_ADDRESS);

	/*
	 CREATE TASK OF THE 3 LEDS
	 Parameters:
	 1- Function name
	 2- Pointer to constant name for debugging
	 3- Stack depth
	 4- Task input parameter as pointer to void
	 5- Task priority
	 6- Task ID
	 */
	xTaskCreate(ADCTask, NULL, 60, NULL, 7, NULL);
	xTaskCreate(TemperatureTask, NULL, 60, NULL, 6, NULL);
	xTaskCreate(FanTask, NULL, 60, NULL, 5, NULL);
	xTaskCreate(LoginTask, NULL, 160, NULL, 4, NULL);
	xTaskCreate(DoorTask, NULL, 60, NULL, 3, NULL);
	xTaskCreate(BuzzerTask, NULL, 60, NULL, 2, NULL);
	xTaskCreate(LCDTask, NULL, 170, NULL, 1, NULL);

	/*2- START THE SCHEDULER*/
	vTaskStartScheduler();

	while (1) {
	}
}

/*
 * Task handles login scenario.
 * 1. Get 4 numbers from key pad.
 * 2. Check password.
 * 3. If password is correct, open door. Otherwise, go to step 1 for a limited number of times.
 * 4. If password is wrongly entered 3 times, turn buzzer on and reset the number of failed attempts.
 */
void LoginTask(void *pv) {
	u8 logged_in = 0; // 0, if it is not logged in. 1, if it is logged in.
	s8 character = 0; // Buffer of character entered from key pad
	u8 number_of_failed_attempts = 0; // Number of failed attempts of entering password
	static u16 entered_password = 0; // Password entered from key pad

	while (1) {
		if (logged_in == 0 && door_opened_then_closed == 1) { // Not logged in and door is opened then closed
		// Display password string
			if (pdPASS == xSemaphoreTake(LCDSem, 35)) {
				LCD_GOTO(0, 0); // Set cursor
				LCD_WRITE_STR((u8*) "Password: ");
				// Give semaphore
				xSemaphoreGive(LCDSem);
			}

			if (number_of_characters < 4) { // Password is fully entered
				if (PRESSED == KEY_PAD_READ(&character)) { // If a key is pressed, store the value in character
					if (character != '*' || character != '#') { // Ignore characters that are not numbers
						entered_password = entered_password * 10
								+ (character - 48); // Append entered number

						// Display '*', when a character is entered.
						if (pdPASS == xSemaphoreTake(LCDSem, 35)) {
							LCD_GOTO(0, 10 + number_of_characters); // Set cursor
							LCD_WRITE_CHR('*');
							// Give semaphore
							xSemaphoreGive(LCDSem);
						}

						number_of_characters++; // Increase number of entered characters
					}
				}
			} else if (PRESSED == KEY_PAD_READ(&character)) { // If 4 numbers are received, receive characters
			/*
			 * If '*' is entered, check password.
			 */
				if (character == '*') {
					if (password == entered_password) { // Check password
						// Display welcome string
						if (pdPASS == xSemaphoreTake(LCDSem, 35)) {
							LCD_GOTO(0, 0); // Set cursor
							LCD_WRITE_STR((u8*) "              "); // Clear previous string
							LCD_GOTO(0, 0); // Set cursor
							LCD_WRITE_STR((u8*) "Welcome.");
							// Give semaphore
							xSemaphoreGive(LCDSem);
						}
						// Remove welcome string
						vTaskDelay(1000);
						if (pdPASS == xSemaphoreTake(LCDSem, 35)) {
							LCD_GOTO(0, 0); // Set cursor
							LCD_WRITE_STR((u8*) "        "); // Clear previous string
							xSemaphoreGive(LCDSem);
						}
						// Open and close door
						logged_in = 1;
					} else { // Password was not entered correctly
						number_of_failed_attempts++; // Increase number of failed attempts to enter password

						// Display wrong password was entered and the number of failed attempts
						if (pdPASS == xSemaphoreTake(LCDSem, 35)) {
							LCD_GOTO(0, 0); // Set cursor
							LCD_WRITE_STR((u8*) "Wrong pass (");
							LCD_WRITE_INT(number_of_failed_attempts);
							LCD_WRITE_STR((u8*) ").");
							xSemaphoreGive(LCDSem);
						}
						vTaskDelay(1000); // Wait a second
						// Remove displayed text
						if (pdPASS == xSemaphoreTake(LCDSem, 35)) {
							LCD_GOTO(0, 0); // Set cursor
							LCD_WRITE_STR((u8*) "               "); // Clear previous string
							xSemaphoreGive(LCDSem);
						}

						if (number_of_failed_attempts
								== NUMBER_OF_FAILED_ATTEMPTS) { // Check whether maximum number of failed attempts to enter password was reached
							alarm_on = 0; // Turn buzzer on
							number_of_failed_attempts = 0; // Reset number of number of failed attempts to enter password
						}
					}

					entered_password = 0; // Reset password
					number_of_characters = 0; // Reset number of entered characteres
				}
			}

		} else if (logged_in == 1) {
			door_opened_then_closed = 0; // Open door then close door

			logged_in = 0; // Ready to login again
			number_of_failed_attempts = 0; // Reset number of number of failed attempts to enter password
		}

		vTaskDelay(200); // Periodicity
	}
}

/*
 * Converts ADC reading to temperature.
 * Display temperature.
 */
void ADCTask(void *pv) {
	while (1) {
		if (pdPASS == xSemaphoreTake(ADCSem, 35)) {
			adc_reading = ADC_READ_INT(); // Read ADC value

			// Start conversion
			ADC_START_CONVERSION_INT(CHANNEL_0);
		}

		vTaskDelay(150); // Periodicity
	}

}

void TemperatureTask(void *pv) {
	while (1) {
		// Calculate temperature from ADC reading
		if ((adc_reading >= 2 && adc_reading <= 10)
				|| (adc_reading >= 45 && adc_reading <= 49)) {
			temperature = (((u32) adc_reading * 5000) / 1023) / 10 + 1;
		} else {
			temperature = (((u32) adc_reading * 5000) / 1023) / 10;
		}

		vTaskDelay(160); // Periodicity
	}
}

/*
 * Task handles fan.
 */
void FanTask(void *pv) {
	while (1) {
		if (temperature > 30) {
			// Turn fan and buzzer on
			if (temperature <= 100) {
				PWM_0_SET_DUTY_CYCLE(temperature); // Set duty cycle to temperature value
			} else {
				// Maximum duty cycle is 100
				PWM_0_SET_DUTY_CYCLE(100);
			}

			fan_on = 1; // Set flag
		} else {
			PWM_0_SET_DUTY_CYCLE(0); // Stop fan
			fan_on = 0; // Reset flag
		}

		vTaskDelay(200); // Periodicity
	}
}

/*
 * Task handles door
 */
void DoorTask(void *pv) {
	while (1) {
		if (door_opened_then_closed == 0) {
			/*
			 * Open door, wait then close door
			 */
			SET_BIT(PORTB, 4); // Open door
			// Door opening
			vTaskDelay(2000); // It takes 2 seconds to open door
			CLR_BIT(PORTB, 4); // Stop door
			door_opened = 1; // Set flag

			vTaskDelay(2000); // Wait 2 seconds
			SET_BIT(PORTB, 5); // Close door
			//Door closing
			vTaskDelay(2000); // It takes 2 seconds to close door
			CLR_BIT(PORTB, 5); // Stop door
			door_opened = 0; // Reset flag

			door_opened_then_closed = 1; // Flag is set to indicate door was opened
		}

		vTaskDelay(200); // Periodicity
	}
}

/*
 * Task handles buzzer
 */
void BuzzerTask(void *pv) {
	while (1) {
		if (alarm_on == 0) { // Check if buzzer needs to be turned on
			// Turn buzzer on 3 times;
			BUZZER_ON(); // Turn buzzer on
			buzzer_on = 1; // Set flag
			vTaskDelay(200); // Wait 200 ms

			BUZZER_OFF(); // Turn buzzer off
			buzzer_on = 0; // Reset flag
			vTaskDelay(200); // Wait 200 ms

			BUZZER_ON(); // Turn buzzer on
			buzzer_on = 1; // Set flag
			vTaskDelay(200); // Wait 200 ms

			BUZZER_OFF(); // Turn buzzer off
			buzzer_on = 0; // Reset flag
			vTaskDelay(200); // Wait 200 ms

			BUZZER_ON(); // Turn buzzer on
			buzzer_on = 1; // Set flag
			vTaskDelay(200); // Wait 200 ms

			BUZZER_OFF(); // Turn buzzer off
			buzzer_on = 0; // Reset flag

			alarm_on = 1; // Flag is set to indicate buzzer was turned on
		}
		vTaskDelay(200); // Periodicity
	}
}

void LCDTask(void *pv) {
	// The 2 variables are used to display fan state one time per change
	u8 fan_on_displayed = 0, // 0, if fan being on is not displayed. 1, if fan being on is displayed.
			fan_off_displayed = 0; // 0, if fan being off is not displayed. 1, if fan being off is displayed.

	// Display string that does not change
	if (pdPASS == xSemaphoreTake(LCDSem, 35)) {
		LCD_GOTO(1, 0); // Set cursor
		LCD_WRITE_STR((u8*) "Fan:o");
		LCD_GOTO(1, 9); // Set cursor
		LCD_WRITE_STR((u8*) "Bz:Off.");
		LCD_GOTO(2, 0); // Set cursor
		LCD_WRITE_STR((u8*) "Temperature: ");
		LCD_GOTO(2, 15); // Set cursor
		LCD_WRITE_STR((u8*) "C");
		LCD_GOTO(3, 0); // Set cursor
		LCD_WRITE_STR((u8*) "Door closed."); // Initial door state

		xSemaphoreGive(LCDSem);
	}
	while (1) {
		if (pdPASS == xSemaphoreTake(LCDSem, 35)) {
			// Fan state display
			LCD_GOTO(1, 5); // Set cursor
			if (fan_on == 0 && fan_off_displayed == 0) {
				LCD_WRITE_STR((u8*) "ff.");

				fan_off_displayed = 1; // Set to indicate, fan being off was displayed
				fan_on_displayed = 0; // Reset flag to indicate fan being on is not displayed
			} else if (fan_on == 1 && fan_on_displayed == 0) {
				LCD_WRITE_STR((u8*) "   "); // Clear previous string
				LCD_GOTO(1, 5); // Set cursor
				LCD_WRITE_STR((u8*) "n.");

				fan_on_displayed = 1; // Set to indicate, fan being on was displayed
				fan_off_displayed = 0; // Reset flag to indicate fan being off is not displayed
			}

			// Buzzer state display
			LCD_GOTO(1, 13); // Set cursor
			if (buzzer_on == 0) {
				LCD_WRITE_STR((u8*) "ff.");
			} else if (buzzer_on == 1) {
				LCD_WRITE_STR((u8*) "   "); // Clear previous string
				LCD_GOTO(1, 13); // Set cursor
				LCD_WRITE_STR((u8*) "n.");
			}

			// Temperature display
			LCD_GOTO(2, 12); // Set cursor
			LCD_WRITE_STR((u8*) "   "); // Clear previous string
			LCD_GOTO(2, 12); // Set cursor
			LCD_WRITE_INT(temperature);

			// Door state display
			LCD_GOTO(3, 5); // Set cursor
			if (door_opened == 0) {
				LCD_WRITE_STR((u8*) "clos");
			} else if (door_opened == 1) {
				LCD_WRITE_STR((u8*) "open");
			}

			xSemaphoreGive(LCDSem);
		}

		vTaskDelay(150); // Periodicity
	}
}

void ADC_isr(void) {
	xSemaphoreGive(ADCSem); // Give semaphore
}
