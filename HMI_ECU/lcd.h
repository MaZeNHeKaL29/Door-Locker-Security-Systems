/******************************************************************************
 *
 * Module: LCD
 *
 * File Name: lcd.h
 *
 * Description: Header file for LCD
 *
 * Author: Mazen Hekal
 *
 *******************************************************************************/

#ifndef LCD_H_
#define LCD_H_

#include "std_types.h"

/*******************************************************************************
 *                               Types Declaration                             *
 *******************************************************************************/
#define LCD_DATA_BIT_MODE	8

#define LCD_RW_CONNECTED_PIN	1

#if((LCD_DATA_BIT_MODE != 4) && (LCD_DATA_BIT_MODE != 8))

#error "Number of Data bits should be equal to 4 or 8"

#endif

/*LCD HW PORTS and PIN IDS*/
#define LCD_RS_PORT_ID	PORTB_ID
#define LCD_RS_PIN_ID	PIN0_ID

#if(LCD_RW_CONNECTED_PIN == 1)

#define LCD_RW_PORT_ID	PORTB_ID
#define LCD_RW_PIN_ID	PIN1_ID

#endif

#define LCD_ENABLE_PORT_ID	PORTB_ID
#define LCD_ENABLE_PIN_ID	PIN2_ID

#if(LCD_RW_CONNECTED_PIN == 1)

#define LCD_RW_PORT_ID	PORTB_ID
#define LCD_RW_PIN_ID	PIN1_ID

#endif

#define LCD_DB_PORT_ID	PORTC_ID

#if (LCD_DATA_BIT_MODE == 4)

#define LCD_DB4_PIN_ID PIN4_ID
#define LCD_DB5_PIN_ID PIN5_ID
#define LCD_DB6_PIN_ID PIN6_ID
#define LCD_DB7_PIN_ID PIN7_ID

#endif


/*LCD Commands*/
#define LCD_CLEAR_SCREEN	0x01
#define LCD_RETURN_HOME	0x02
#define LCD_SHIFT_CURSOR_LEFT	0x04
#define LCD_SHIFT_CURSOR_RIGHT	0x05
#define LCD_CURSOR_OFF	0x0C
#define LCD_CURSOR_BLINK	0x0F
#define LCD_SET_CURSOR_LOCATION	0x80
#define LCD_CURSOR_1ST_LINE	0x80
#define LCD_CURSOR_2ND_LINE	0xC0
#define LCD_TWO_LINES_EIGHT_BITS_MODE	0x38
#define LCD_TWO_LINES_FOUR_BITS_MODE	0x28
#define LCD_TWO_LINES_FOUR_BITS_MODE_INIT1	0x33
#define LCD_TWO_LINES_FOUR_BITS_MODE_INIT2	0x32



/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/

/*
 * Function to initialize LCD
 */
void LCD_init(void);
/*
 * Description :
 * Function to send Commands to LCD
 */
void LCD_sendCommand(uint8 command);
/*
 * Description :
 * Function to display character on LCD
 */
void LCD_displayCharacter(uint8 character);
/*
 * Description :
 * Function to display String
 */
void LCD_displayString(char* string);
/*
 * Description :
 * Function to move cursor
 */
void LCD_moveCursor(uint8 row,uint8 col);
/*
 * Description :
 * Function to display string at certain row and column
 */
void LCD_displayStringRowColumn(char* string, uint8 row, uint8 col);
/*
 * Description:
 * Function to clear screen
 */
void LCD_clearScreen(void);
/*
 * Description:
 * Function to display integer numbers
 */
void LCD_displayInteger(int data);
/*
 * Description:
 * Function to move cursor to 1st line
 */
void LCD_moveCursor1stLine(void);
/*
 * Description:
 * Function to move cursor to 2nd line
 */
void LCD_moveCursor2ndLine(void);


#endif /* LCD_H_ */
