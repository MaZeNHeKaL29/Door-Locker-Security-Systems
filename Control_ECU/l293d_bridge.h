/******************************************************************************
 *
 * Module: L293D H-Bridge
 *
 * File Name: l29d_bridge.h
 *
 * Description: Header file for L293D H-Bridge
 *
 * Author: Mazen Hekal
 *
 *******************************************************************************/

#ifndef L293D_BRIDGE_H_
#define L293D_BRIDGE_H_

#include "std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

/* PORT Connected to L293D in MC */
#define L293D_PORT	PORTB
/* Shift Required for IN1, IN2 of L293D connected in MC */
#define L293_PIN_SHIFT	0

#define L293D_IN1_PORT_ID	PORTB_ID
#define L293D_IN1_PIN_ID	PIN0_ID

#define L293D_IN2_PORT_ID	PORTB_ID
#define L293D_IN2_PIN_ID	PIN1_ID



#endif /* L293D_BRIDGE_H_ */
