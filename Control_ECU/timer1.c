/******************************************************************************
 *
 * Module: Timer1
 *
 * File Name: timer1.c
 *
 * Description: Source file for the Timer1 AVR driver
 *
 * Author: Mazen Hekal
 *
 *******************************************************************************/

#include "timer1.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "common_macros.h"

/* Global variables to hold the address of the call back function in the application */
static volatile void (*g_callBack_Ptr)(void) = NULL_PTR;

/*******************************************************************************
 *                       Interrupt Service Routines                            *
 *******************************************************************************/

ISR(TIMER1_OVF_vect)
{
	if(g_callBack_Ptr != NULL_PTR)
	{
		(*g_callBack_Ptr)();
	}
}

ISR(TIMER1_COMPA_vect)
{
	if(g_callBack_Ptr != NULL_PTR)
	{
		(*g_callBack_Ptr)();
	}
}



/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*
 * Description :
 * Function to initialize the Timer driver
 */
void Timer1_init(const Timer1_ConfigType * Config_Ptr)
{
	TCNT1 = Config_Ptr->initial_value;		/* Set timer1 initial count to zero */
	OCR1A = Config_Ptr->compare_value;    /* Set the Compare value */
	if(Config_Ptr->mode == NORMAL)
	{
		SET_BIT(TIMSK, TOIE1); 		/* Enable Timer1 Normal Interrupt */
	}
	else if(Config_Ptr->mode == CTC)
	{
		SET_BIT(TIMSK, OCIE1A); 		/* Enable Timer1 Compare A Interrupt */
	}
	/* Configure timer control register TCCR1A
	 * 1. Disconnect OC1A and OC1B  COM1A1=0 COM1A0=0 COM1B0=0 COM1B1=0
	 * 2. FOC1A=1 FOC1B=0
	 * 3. Set WGM10 and WGM11 depending on mode of operation
	 */
	TCCR1A = (1<<FOC1A);
	TCCR1A = (TCCR1A & ~(0x03)) | (Config_Ptr->mode & 0x03);
	/* Configure timer control register TCCR1B
	 * 1. Set WGM12=1 and WGM13=0 depending on mode of operation
	 * 2. Set CS10, CS11, CS12 depending on prescaler
	 */
	TCCR1B = 0;
	TCCR1B = (TCCR1B & ~(0x07)) | (Config_Ptr->prescaler);
	TCCR1B = (TCCR1B & ~(0x18)) | (((Config_Ptr->mode & 0x0C) >> 2) << 3);
}

/*
 * Description :
 * Function to disable the Timer1.
 */
void Timer1_deInit(void)
{
	/* Clear All Timer1 Registers */
	TCNT1 = 0;
	OCR1A = 0;
	TCCR1A = 0;
	TCCR1B = 0;

	/* Clear Interrupts for Timer1 Unit */
	TIMSK = 0;
}

/*
 * Description :
 *  Function to set the Call Back function address.
 */
void Timer1_setCallBack(void(*a_ptr)(void))
{
	g_callBack_Ptr = a_ptr;
}

