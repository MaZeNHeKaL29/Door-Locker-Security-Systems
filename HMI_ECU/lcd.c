/******************************************************************************
 *
 * Module: LCD
 *
 * File Name: lcd.c
 *
 * Description: Source file for LCD
 *
 * Author: Mazen Hekal
 *
 *******************************************************************************/

#include "lcd.h"
#include <util/delay.h>
#include "common_macros.h"
#include "gpio.h"

/*
 * Description :
 * Function to initialize LCD
 */
void LCD_init(void)
{
	/*set RS,Enable to output direction*/
	GPIO_setupPinDirection(LCD_RS_PORT_ID, LCD_RS_PIN_ID, PIN_OUTPUT);
	GPIO_setupPinDirection(LCD_ENABLE_PORT_ID, LCD_ENABLE_PIN_ID, PIN_OUTPUT);
#if(LCD_RW_CONNECTED_PIN == 1)
	/*Set RW = 0 */
	GPIO_setupPinDirection(LCD_RW_PORT_ID, LCD_RW_PIN_ID, PIN_OUTPUT);
	GPIO_writePin(LCD_RW_PORT_ID, LCD_RW_PIN_ID, LOGIC_LOW);
#endif
	/*Set DB to output direction*/
#if(LCD_DATA_BIT_MODE == 4)
	/* Configure 4 pins in the data port as output pins */
	GPIO_setupPinDirection(LCD_DB_PORT_ID,LCD_DB4_PIN_ID,PIN_OUTPUT);
	GPIO_setupPinDirection(LCD_DB_PORT_ID,LCD_DB5_PIN_ID,PIN_OUTPUT);
	GPIO_setupPinDirection(LCD_DB_PORT_ID,LCD_DB6_PIN_ID,PIN_OUTPUT);
	GPIO_setupPinDirection(LCD_DB_PORT_ID,LCD_DB7_PIN_ID,PIN_OUTPUT);

	/* Send for 4 bit initialization of LCD  */
	LCD_sendCommand(LCD_TWO_LINES_FOUR_BITS_MODE_INIT1);
	LCD_sendCommand(LCD_TWO_LINES_FOUR_BITS_MODE_INIT2);

	/* use 2-lines LCD + 4-bits Data Mode + 5*7 dot display Mode */
	LCD_sendCommand(LCD_TWO_LINES_FOUR_BITS_MODE);

#elif(LCD_DATA_BIT_MODE == 8)
	/* Configure the data port as output port */
	GPIO_setupPortDirection(LCD_DB_PORT_ID,PORT_OUTPUT);

	/* use 2-lines LCD + 8-bits Data Mode + 5*7 dot display Mode */
	LCD_sendCommand(LCD_TWO_LINES_EIGHT_BITS_MODE);

#endif
	/*Cursor OFF -> 0x0C*/
	LCD_sendCommand(LCD_CURSOR_OFF);
	/*Clear Screen -> 0x01*/
	LCD_sendCommand(LCD_CLEAR_SCREEN);
}

/*
 * Description :
 * Function to send Commands to LCD
 */
void LCD_sendCommand(uint8 command)
{
	/*Set RS = 0*/
	GPIO_writePin(LCD_RS_PORT_ID, LCD_RS_PIN_ID, LOGIC_LOW);
	_delay_ms(1);
	/*Set Enable = 1*/
	GPIO_writePin(LCD_ENABLE_PORT_ID, LCD_ENABLE_PIN_ID, LOGIC_HIGH);
	_delay_ms(1);
#if(LCD_DATA_BIT_MODE == 4)
	GPIO_writePin(LCD_DB_PORT_ID,LCD_DB4_PIN_ID,GET_BIT(command,4));
	GPIO_writePin(LCD_DB_PORT_ID,LCD_DB5_PIN_ID,GET_BIT(command,5));
	GPIO_writePin(LCD_DB_PORT_ID,LCD_DB6_PIN_ID,GET_BIT(command,6));
	GPIO_writePin(LCD_DB_PORT_ID,LCD_DB7_PIN_ID,GET_BIT(command,7));

	_delay_ms(1); /* delay for processing Tdsw = 100ns */
	GPIO_writePin(LCD_ENABLE_PORT_ID,LCD_ENABLE_PIN_ID,LOGIC_LOW); /* Disable LCD E=0 */
	_delay_ms(1); /* delay for processing Th = 13ns */
	GPIO_writePin(LCD_ENABLE_PORT_ID,LCD_ENABLE_PIN_ID,LOGIC_HIGH); /* Enable LCD E=1 */
	_delay_ms(1); /* delay for processing Tpw - Tdws = 190ns */

	GPIO_writePin(LCD_DB_PORT_ID,LCD_DB4_PIN_ID,GET_BIT(command,0));
	GPIO_writePin(LCD_DB_PORT_ID,LCD_DB5_PIN_ID,GET_BIT(command,1));
	GPIO_writePin(LCD_DB_PORT_ID,LCD_DB6_PIN_ID,GET_BIT(command,2));
	GPIO_writePin(LCD_DB_PORT_ID,LCD_DB7_PIN_ID,GET_BIT(command,3));

	_delay_ms(1); /* delay for processing Tdsw = 100ns */
	GPIO_writePin(LCD_ENABLE_PORT_ID,LCD_ENABLE_PIN_ID,LOGIC_LOW); /* Disable LCD E=0 */
	_delay_ms(1); /* delay for processing Th = 13ns */

#elif(LCD_DATA_BIT_MODE == 8)
	GPIO_writePort(LCD_DB_PORT_ID,command); /* out the required command to the data bus D0 --> D7 */
	_delay_ms(1); /* delay for processing Tdsw = 100ns */
	GPIO_writePin(LCD_ENABLE_PORT_ID,LCD_ENABLE_PIN_ID,LOGIC_LOW); /* Disable LCD E=0 */
	_delay_ms(1); /* delay for processing Th = 13ns */
#endif
}

/*
 * Description :
 * Function to display character on LCD
 */
void LCD_displayCharacter(uint8 character)
{
	/*Set RS = 1*/
	GPIO_writePin(LCD_RS_PORT_ID, LCD_RS_PIN_ID, LOGIC_HIGH);
	_delay_ms(1);
	/*Set Enable = 1*/
	GPIO_writePin(LCD_ENABLE_PORT_ID, LCD_ENABLE_PIN_ID, LOGIC_HIGH);
	_delay_ms(1);
#if(LCD_DATA_BIT_MODE == 4)
	GPIO_writePin(LCD_DB_PORT_ID,LCD_DB4_PIN_ID,GET_BIT(character,4));
	GPIO_writePin(LCD_DB_PORT_ID,LCD_DB5_PIN_ID,GET_BIT(character,5));
	GPIO_writePin(LCD_DB_PORT_ID,LCD_DB6_PIN_ID,GET_BIT(character,6));
	GPIO_writePin(LCD_DB_PORT_ID,LCD_DB7_PIN_ID,GET_BIT(character,7));

	_delay_ms(1); /* delay for processing Tdsw = 100ns */
	GPIO_writePin(LCD_ENABLE_PORT_ID,LCD_ENABLE_PIN_ID,LOGIC_LOW); /* Disable LCD E=0 */
	_delay_ms(1); /* delay for processing Th = 13ns */
	GPIO_writePin(LCD_ENABLE_PORT_ID,LCD_ENABLE_PIN_ID,LOGIC_HIGH); /* Enable LCD E=1 */
	_delay_ms(1); /* delay for processing Tpw - Tdws = 190ns */

	GPIO_writePin(LCD_DB_PORT_ID,LCD_DB4_PIN_ID,GET_BIT(character,0));
	GPIO_writePin(LCD_DB_PORT_ID,LCD_DB5_PIN_ID,GET_BIT(character,1));
	GPIO_writePin(LCD_DB_PORT_ID,LCD_DB6_PIN_ID,GET_BIT(character,2));
	GPIO_writePin(LCD_DB_PORT_ID,LCD_DB7_PIN_ID,GET_BIT(character,3));

	_delay_ms(1); /* delay for processing Tdsw = 100ns */
	GPIO_writePin(LCD_ENABLE_PORT_ID,LCD_ENABLE_PIN_ID,LOGIC_LOW); /* Disable LCD E=0 */
	_delay_ms(1); /* delay for processing Th = 13ns */

#elif(LCD_DATA_BIT_MODE == 8)
	GPIO_writePort(LCD_DB_PORT_ID,character); /* out the required character to the data bus D0 --> D7 */
	_delay_ms(1); /* delay for processing Tdsw = 100ns */
	GPIO_writePin(LCD_ENABLE_PORT_ID,LCD_ENABLE_PIN_ID,LOGIC_LOW); /* Disable LCD E=0 */
	_delay_ms(1); /* delay for processing Th = 13ns */
#endif
}

/*
 * Description :
 * Function to display String
 */
void LCD_displayString(char* string)
{
	uint8 i = 0;
	while(string[i] != '\0'){
		LCD_displayCharacter(string[i]);
		i++;
	}
}

/*
 * Description :
 * Function to move cursor
 */
void LCD_moveCursor(uint8 row,uint8 col)
{
	uint8 lcd_memory_address;

	/* Calculate the required address in the LCD DDRAM */
	switch(row)
	{
	case 0:
		lcd_memory_address=col;
		break;
	case 1:
		lcd_memory_address=col+0x40;
		break;
	case 2:
		lcd_memory_address=col+0x10;
		break;
	case 3:
		lcd_memory_address=col+0x50;
		break;
	}
	/* Move the LCD cursor to this specific address */
	LCD_sendCommand(lcd_memory_address | LCD_SET_CURSOR_LOCATION);
}

/*
 * Description :
 * Function to display string at certain row and column
 */
void LCD_displayStringRowColumn(char* string, uint8 row, uint8 col)
{
	LCD_moveCursor(row, col);
	LCD_displayString(string);
}

/*
 * Description:
 * Function to clear screen
 */
void LCD_clearScreen(void)
{
	LCD_sendCommand(LCD_CLEAR_SCREEN);
}

/*
 * Description:
 * Function to display integer numbers
 */
void LCD_displayInteger(int data)
{
	char buff[16];
	itoa(data,buff,10);	//convert integer to string ASCII using decimal mode
	LCD_displayString(buff);
}

/*
 * Description:
 * Function to move cursor to 1st line
 */
void LCD_moveCursor1stLine(void)
{
	LCD_sendCommand(LCD_CURSOR_1ST_LINE);
}


/*
 * Description:
 * Function to move cursor to 2nd line
 */
void LCD_moveCursor2ndLine(void)
{
	LCD_sendCommand(LCD_CURSOR_2ND_LINE);
}
