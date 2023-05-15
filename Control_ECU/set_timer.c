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

#include "set_timer.h"
#include "avr/interrupt.h"

/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

/* Global variables to hold the address of the call back function in the application */
static volatile void (*g_callBack_Ptr)(void) = NULL_PTR;
uint8 g_tick = 0;	/* variable to count seconds */
uint8 g_sec;		/* variable to determine sec to be counted */

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 *  Function to count sec.
 */
static void Timer_countSec(void);

/*
 * Description :
 * Function to start timer based on seconds user entered
 */
void Timer_setSec(uint8 sec,void(*a_ptr)(void))
{
	SREG |= (1<<7); /* Enable global interrupts in MC */
	Timer1_ConfigType Timer1Config_Ptr = {INTIAL_VALUE,COMPARRE_VALUE,Timer1_Prescaler,Timer1_Mode};
	g_sec = sec;
	g_callBack_Ptr = a_ptr;
	Timer1_init(&Timer1Config_Ptr);
	Timer1_setCallBack(Timer_countSec);
}

/*
 * Description :
 *  Function to count sec.
 */
static void Timer_countSec(void)
{
	g_tick++;
	if(g_tick == g_sec)
	{
		Timer1_deInit();
		g_tick = 0;
		if(g_callBack_Ptr != NULL_PTR)
		{
			(*g_callBack_Ptr)();
		}
	}
}


