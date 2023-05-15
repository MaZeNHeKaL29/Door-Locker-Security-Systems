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

#ifndef PWM_H_
#define PWM_H_

#include "std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

#define TIMER0_MAXIMUM_COUNT 255

/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/

/*
 * Description :
 * Function to initialize Timer0 with Pulse Width Modulation mode
 */
void PWM_Timer0_Start(uint8 duty_cycle);


#endif /* PWM_H_ */
