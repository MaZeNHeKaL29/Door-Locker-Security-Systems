/******************************************************************************
 *
 * Module: DC Motor
 *
 * File Name: dc_motor.h
 *
 * Description: Header file for DC Motor
 *
 * Author: Mazen Hekal
 *
 *******************************************************************************/

#ifndef DC_MOTOR_H_
#define DC_MOTOR_H_

#include "std_types.h"

/*******************************************************************************
 *                               Types Declaration                             *
 *******************************************************************************/

typedef enum{
	DC_MOTOR_STOP,DC_MOTOR_CW,DC_MOTOR_A_CW
}DC_MOTOR_State;

/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/

/*
 * Description :
 * The Function responsible for setup the direction for the two
 * motor pins through the GPIO driver.
 */
void DC_MOTOR_init(void);

/*
 * Description :
 * The function responsible for rotate the DC Motor CW/ or A-CW or
 * stop the motor based on the state input state value.
 */
void DC_MOTOR_rotate(DC_MOTOR_State state, uint8 speed);

#endif /* DC_MOTOR_H_ */
