/******************************************************************************
 *
 * Module: Pulse Width Modulation
 *
 * File Name: pwm.c
 *
 * Description: Source file for Pulse Width Modulation
 *
 * Author: Mazen Hekal
 *
 *******************************************************************************/

#include "pwm.h"
#include "gpio.h"
#include "common_macros.h"
#include "avr/io.h"

/*
 * Description :
 * Function to initialize Timer0 with Pulse Width Modulation mode
 */
void PWM_Timer0_Start(uint8 duty_cycle)
{
	/* Set Bits WGM00 -> 0 , WGM01 -> 1 to generate PWM mode */
	TCCR0 = (1<<WGM00) | (1<<WGM01);
	/* PWM Non_Inverting Mode */
	SET_BIT(TCCR0,COM01);
	/* Set OC0(PB3) as output pin */
	GPIO_setupPinDirection(PORTB_ID, PIN3_ID, PIN_OUTPUT);
	/* Setup the prescaler with F_CPU/8 */
	SET_BIT(TCCR0,CS01);
	/* Start Counting from zero */
	TCNT0 = 0;
	/* Set Output Compare Register */
	OCR0 = (uint8) ((TIMER0_MAXIMUM_COUNT) * ((float32)duty_cycle/100));
}


