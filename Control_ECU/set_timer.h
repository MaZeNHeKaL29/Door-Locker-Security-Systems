/******************************************************************************
 *
 * Module: Set Timer
 *
 * File Name: set_timer.h
 *
 * Description: Header file for the Setting Timer
 *
 * Author: Mazen Hekal
 *
 *******************************************************************************/

#ifndef SET_TIMER_H_
#define SET_TIMER_H_

#include "std_types.h"
#include "timer1.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

#define INTIAL_VALUE		0
#define COMPARRE_VALUE		7812
#define Timer1_Prescaler	F_CPU_1024
#define Timer1_Mode			CTC

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * Function to start timer based on seconds user entered
 */
void Timer_setSec(uint8 sec,void(*a_ptr)(void));






#endif /* SET_TIMER_H_ */
