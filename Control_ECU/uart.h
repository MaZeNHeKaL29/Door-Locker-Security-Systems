 /******************************************************************************
 *
 * Module: UART
 *
 * File Name: uart.h
 *
 * Description: Header file for the UART AVR driver
 *
 * Author: Mazen Hekal
 *
 *******************************************************************************/

#ifndef UART_H_
#define UART_H_

#include "std_types.h"

/*******************************************************************************
 *                               Types Declaration                             *
 *******************************************************************************/

typedef enum{
	PARITY_DISABLED,PARITY_EVEN = 0x02, PARITY_ODD
}UART_Parity;

typedef enum{
	STOP_1_BIT, STOP_2_BIT
}UART_StopBitSelect;

typedef enum{
	CHARACTER_5_BIT,CHARACTER_6_BIT,CHARACTER_7_BIT,CHARACTER_8_BIT
}UART_CharacterSize;

typedef enum{
	UART_BAUDRATE_2400 = 2400,UART_BAUDRATE_4800 = 4800,UART_BAUDRATE_9600 = 9600
}UART_BaudRate;

typedef struct{
	UART_Parity parity;
	UART_StopBitSelect stopBitSelect;
	UART_CharacterSize characterSize;
	UART_BaudRate baudRate;
}UART_ConfigType;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * Functional responsible for Initialize the UART device by:
 * 1. Setup the Frame format like number of data bits, parity bit type and number of stop bits.
 * 2. Enable the UART.
 * 3. Setup the UART baud rate.
 */
void UART_init(UART_ConfigType *configType_Ptr);

/*
 * Description :
 * Functional responsible for send byte to another UART device.
 */
void UART_sendByte(const uint8 data);

/*
 * Description :
 * Functional responsible for receive byte from another UART device.
 */
uint8 UART_recieveByte(void);

/*
 * Description :
 * Send the required string through UART to the other UART device.
 */
void UART_sendString(const uint8 *str);

/*
 * Description :
 * Receive the required string until the '#' symbol through UART from the other UART device.
 */
void UART_receiveString(uint8 *str); // Receive until #

#endif /* UART_H_ */
