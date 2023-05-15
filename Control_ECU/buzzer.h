/******************************************************************************
 *
 * Module: Buzzer
 *
 * File Name: buzzer.h
 *
 * Description: Header file for Buzzer
 *
 * Author: Mazen Hekal
 *
 *******************************************************************************/

#ifndef BUZZER_H_
#define BUZZER_H_

#include "std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

/* PORT and PIN connected to buzzer in MC */
#define BUZZER_PORT_ID	PORTC_ID
#define BUZZER_PIN_ID	PIN2_ID

#define BUZZER_ON	LOGIC_HIGH
#define BUZZER_OFF	LOGIC_LOW

/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/

/*
 * Description:
 * Setup the direction for the buzzer pin as output pin through the GPIO driver.
 */
void Buzzer_init();

/*
 * Description:
 * Function to enable the Buzzer through the GPIO.
 */
void Buzzer_on(void);

/*
 * Description:
 * Function to disable the Buzzer through the GPIO.
 */
void Buzzer_off(void);


#endif /* BUZZER_H_ */
