/******************************************************************************
 *
 * Module: DC Motor
 *
 * File Name: dc_motor.c
 *
 * Description: Source file for DC Motor
 *
 * Author: Mazen Hekal
 *
 *******************************************************************************/


#include "dc_motor.h"
#include "gpio.h"
#include "l293d_bridge.h"
#include "pwm.h"
#include <avr/io.h>

/*
 * Description :
 * The Function responsible for setup the direction for the two
 * motor pins through the GPIO driver.
 */
void DC_MOTOR_init(void)
{
	/* Setup Output direction for IN1 & IN2 in L293D H-Bridge pins connected to MC */
	GPIO_setupPinDirection(L293D_IN1_PORT_ID, L293D_IN1_PIN_ID, PIN_OUTPUT);
	GPIO_setupPinDirection(L293D_IN2_PORT_ID, L293D_IN2_PIN_ID, PIN_OUTPUT);
	/* Stop DC Motor */
	L293D_PORT = (L293D_PORT & ~(0x03 << L293_PIN_SHIFT)) | (DC_MOTOR_STOP << L293_PIN_SHIFT);
}

/*
 * Description :
 * The function responsible for rotate the DC Motor CW/ or A-CW or
 * stop the motor based on the state input state value.
 */
void DC_MOTOR_rotate(DC_MOTOR_State state, uint8 speed)
{
	/* Setup State of DC Motor */
	L293D_PORT = (L293D_PORT & ~(0x03 << L293_PIN_SHIFT)) | (state << L293_PIN_SHIFT);
	/* Setup speed for DC Motor */
	PWM_Timer0_Start(speed);
}
