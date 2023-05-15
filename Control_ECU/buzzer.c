/******************************************************************************
 *
 * Module: Buzzer
 *
 * File Name: buzzer.c
 *
 * Description: Source file for Buzzer
 *
 * Author: Mazen Hekal
 *
 *******************************************************************************/

#include "buzzer.h"
#include "gpio.h"
#include "common_macros.h"

/*
 * Description:
 * Setup the direction for the buzzer pin as output pin through the GPIO driver.
 */
void Buzzer_init()
{
	/* Setup Output direction for Buzzer pin connected to MC */
	GPIO_setupPinDirection(BUZZER_PORT_ID, BUZZER_PIN_ID, PIN_OUTPUT);
	/* Turn off Buzzer */
	GPIO_writePin(BUZZER_PORT_ID, BUZZER_PIN_ID, BUZZER_OFF);
}

/*
 * Description:
 * Function to enable the Buzzer through the GPIO.
 */
void Buzzer_on(void)
{
	/* Turn on Buzzer */
	GPIO_writePin(BUZZER_PORT_ID, BUZZER_PIN_ID, BUZZER_ON);
}

/*
 * Description:
 * Function to disable the Buzzer through the GPIO.
 */
void Buzzer_off(void)
{
	/* Turn off Buzzer */
	GPIO_writePin(BUZZER_PORT_ID, BUZZER_PIN_ID, BUZZER_OFF);
}


