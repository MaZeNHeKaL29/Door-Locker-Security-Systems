/******************************************************************************
 *
 * Module: Controller for HMI_ECU
 *
 * File Name: hmi_main.c
 *
 * Description: Source file for the controller for HMI_ECU
 *
 * Author: Mazen Hekal
 *
 *******************************************************************************/

#include "lcd.h"
#include "keypad.h"
#include "util/delay.h"
#include "uart.h"
#include "set_timer.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

#define MC1_READY 			0x11		/* Special Character for MC1 Ready    */
#define MC2_READY 			0x10		/* Special Character for MC2 Ready    */
#define MC_NOTREADY	        0x23		/* Special Character for MC Not Ready */
#define PASSWORD_SUCCESS	0x29		/* Special Character for Password entered successfully  */
#define PASSWORD_WRONG		0x30		/* Special Character for Password entered wrong         */
#define PASSWORD_WRONG_3	0x31		/* Special Character for Password entered wrong	3 times */
#ifndef	SUCCESS
#define	SUCCESS				0x01		/* Success						   */
#endif
#ifndef	ERROR
#define	ERROR				0x00		/* ERROR       					   */
#endif
#define PASSWORD_CREATE		0			/* State for creating password	   */
#define MAIN_MENU			1			/* State for main menu        	   */
#define OPEN_DOOR			2			/* State for Open door			   */
#define	PASSWORD_CHANGE		3  			/* State for changing password     */
#define	PASSWORD_ENTER		4   		/* State for Entering password	   */
#define	WAIT				5			/* State for Wait				   */
#define PASSWORD_ERROR		6			/* State for Wrong Password		   */


/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

/* a variable to indicate how many characters user entered in first entered password */
uint8 g_unitPassword1 = 0;
/* a variable to indicate how many characters user entered in reentered password */
uint8 g_unitPassword2 = 0;
/* a variable to loop over string of password */
uint8 g_unit = 0;
/* a flag to indicate that Enter Password is displayed on Screen or not */
uint8 g_flagEnterPass = 1;
/* a flag to indicate that ReEnter Password is displayed on Screen or not */
uint8 g_flagReEnterPass = 1;
/* a variable to indicate the state of operation */
uint8 g_state = PASSWORD_CREATE;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * Function to return to idle state
 */
void STATE_idle(void);


/*
 * Description :
 * Function to save password and display '*' on LCD
 */
uint8 PASSWORD_type(uint8 key, uint8* password);

/*
 * Description :
 * Function to create Password and send it to MC2
 */
void PASSWORD_create(uint8* password1, uint8* password2);

/*
 * Description :
 * Function to enter Password and send it to MC2
 */
void PASSWORD_enter(uint8 state, uint8* password);

/*
 * Description:
 * Function to display that door is unlocking or locking based on MC2
 */
void DOOR_open(void);

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*
 * Description :
 * Function to return to idle state
 */
void STATE_idle(void)
{
	while(UART_recieveByte() != MC2_READY){} /* wait for MC2 to be ready */
	/* Change state of operation to Main Menu */
	g_state = MAIN_MENU;
	LCD_clearScreen();
	LCD_displayString("+:Open Door");
	LCD_moveCursor2ndLine();
	LCD_displayString("-:Change Pass");
}


/*
 * Description :
 * Function to save password and display '*' on LCD
 */
uint8 PASSWORD_type(uint8 key, uint8* password)
{
	/* check if user entered a number or not, if not don't save character */
	if((key <= 9) && (key >= 0))
	{
		password[g_unit] = key + '0';
		g_unit++;
		LCD_displayCharacter('*');   /* display '*' as indication that character is entered */
		return SUCCESS;
	}
	return ERROR;
}

/*
 * Description :
 * Function to create Password and send it to MC2
 */
void PASSWORD_create(uint8* password1, uint8* password2)
{
	uint8 key; /* a variable to get key pressed in keypad */
	uint8 password_case;	/* a variable to indicate the case of password (wrong or right) */
	/* Display Enter Password on LCD */
	if(g_flagEnterPass == 1)
	{
		LCD_clearScreen();
		LCD_displayString("Enter Pass : ");
		LCD_moveCursor2ndLine();
		/* set flag to zero to indicate that Enter Password is displayed on Screen */
		g_flagEnterPass = 0;
	}

	key = KEYPAD_getPressedKey(); /* get key from keypad */

	/* Enter Password */
	if(g_unitPassword1 != 5)
	{
		if(PASSWORD_type(key, password1) == SUCCESS)
		{
			g_unitPassword1++;
		}
	}
	else if(g_unitPassword1 == 5 && g_unitPassword2 == 0 && g_flagReEnterPass == 1 && key == 13)
	{
		password1[5] = '#'; /* add '#' to indicate end of password to MC2 */
		password1[6] = '\0'; /* end of string of Password */
		/* Display ReEnter Password on LCD */
		LCD_clearScreen();
		LCD_displayString("ReEnter Pass :");
		LCD_moveCursor2ndLine();
		/* set flag to zero to indicate that ReEnter Password is displayed on Screen */
		g_flagReEnterPass = 0;
		g_unit = 0;
	}
	/* ReEnter Password */
	else if(g_unitPassword1 == 5 && g_unitPassword2 != 5 && g_flagReEnterPass == 0)
	{
		if(PASSWORD_type(key, password2) == SUCCESS)
		{
			g_unitPassword2++;
		}
	}
	else if(g_unitPassword1 == 5 && g_unitPassword2 == 5 && key == 13)
	{
		password2[5] = '#'; /* add '#' to indicate end of password to MC2 */
		password2[6] = '\0'; /* end of string of Password */
		/* communicate with MC2 that MC1 is ready to send first entered password */
		UART_sendByte(MC1_READY);
		while(UART_recieveByte() != MC2_READY){} /* wait for MC2 to be ready */
		_delay_ms(10);
		UART_sendString(password1); /* send first entered password */
		/* communicate with MC2 that MC1 is ready to send reentered password */
		UART_sendByte(MC1_READY);
		while(UART_recieveByte() != MC2_READY){}	/* wait for MC2 to be ready */
		_delay_ms(10);
		UART_sendString(password2); /* send first entered password */
		while(UART_recieveByte() != MC2_READY){} /* wait for MC2 to be ready */
		password_case = UART_recieveByte();
		/* check if operation of creating password is successed or not */
		if(password_case == SUCCESS)
		{
			/* display 'Success' on Screen for 2 seconds */
			LCD_clearScreen();
			LCD_displayString("Success");
			_delay_ms(2000);
			/* Change state of operation to Main Menu */
			g_state = MAIN_MENU;
			LCD_clearScreen();
			LCD_displayString("+:Open Door");
			LCD_moveCursor2ndLine();
			LCD_displayString("-:Change Pass");
		}
		else if(password_case == ERROR)
		{
			/* display 'Two Passwords are not matched' on Screen for 2 seconds */
			LCD_clearScreen();
			LCD_displayString("Two Passwords ");
			LCD_moveCursor2ndLine();
			LCD_displayString("are not matched");
			_delay_ms(2000);
			/* Change state of operation to Main Menu */
		}
		g_unit = 0;
		g_unitPassword1 = 0;
		g_unitPassword2 = 0;
		g_flagEnterPass = 1;
		g_flagReEnterPass = 1;
	}
	/* wait 400 ms after pressing keypad */
	_delay_ms(400);
}

/*
 * Description :
 * Function to enter Password and send it to MC2
 */
void PASSWORD_enter(uint8 state, uint8* password)
{
	uint8 key; /* a variable to get key pressed in keypad */
	uint8 password_case;	/* a variable to indicate the case of password (wrong or right) */
	/* Display Enter Password on LCD */
	if(g_flagEnterPass == 1)
	{
		LCD_clearScreen();
		LCD_displayString("Enter Pass : ");
		LCD_moveCursor2ndLine();
		/* set flag to zero to indicate that Enter Password is displayed on Screen */
		g_flagEnterPass = 0;
	}

	key = KEYPAD_getPressedKey(); /* get key from keypad */

	/* Enter Password */
	if(g_unitPassword1 != 5)
	{
		if(PASSWORD_type(key, password) == SUCCESS)
		{
			g_unitPassword1++;
		}
	}
	else if(g_unitPassword1 == 5 && key == 13)
	{
		password[5] = '#'; /* add '#' to indicate end of password to MC2 */
		password[6] = '\0'; /* end of string of Password */
		/* communicate with MC2 that MC1 is ready to send entered password */
		UART_sendByte(MC1_READY);
		while(UART_recieveByte() != MC2_READY){} /* wait for MC2 to be ready */
		_delay_ms(10);
		UART_sendString(password); /* send password */
		while(UART_recieveByte() != MC2_READY){} /* wait for MC2 to be ready */
		password_case = UART_recieveByte();
		if(password_case == PASSWORD_SUCCESS)
		{
			/* display 'Correct Password' for 2 seconds */
			LCD_clearScreen();
			LCD_displayString("Correct Password");
			_delay_ms(2000);
			/* Change state of operation to next state */
			g_state = state;
			while(UART_recieveByte() != MC2_READY){} /* wait for MC2 to be ready */
			/* communicate with MC2 that MC1 is ready to state */
			UART_sendByte(MC1_READY);
			/* send state to MC2 */
			UART_sendByte(state);
		}
		else if(password_case == PASSWORD_WRONG)
		{
			/* display 'Wrong Password'  for 2 seconds */
			LCD_clearScreen();
			LCD_displayString("Wrong Password");
			LCD_moveCursor2ndLine();
			LCD_displayString("Try Again");
			_delay_ms(2000);
		}
		else if(password_case == PASSWORD_WRONG_3)
		{
			g_state = PASSWORD_ERROR;
		}
		g_unit = 0;
		g_unitPassword1 = 0;
		g_unitPassword2 = 0;
		g_flagEnterPass = 1;
		g_flagReEnterPass = 1;
	}
	/* wait 400 ms after pressing keypad */
	_delay_ms(400);
}

/*
 * Description:
 * Function to display that door is unlocking or locking based on MC2
 */
void DOOR_open(void)
{
	/* Display 'Door is Unlocking' on Screen */
	LCD_clearScreen();
	LCD_displayString("Door is");
	LCD_moveCursor2ndLine();
	LCD_displayString("Unlocking");
	/* communicate with MC2 that MC1 is ready */
	UART_sendByte(MC1_READY);
	while(UART_recieveByte() != MC2_READY){} /* wait for MC2 to be ready */
	/* Display 'Door is locking' on Screen */
	LCD_clearScreen();
	LCD_displayString("Door is");
	LCD_moveCursor2ndLine();
	LCD_displayString("Locking");
	/* return to idle state */
	STATE_idle();
}

/*******************************************************************************
 *                      Main Function                                          *
 *******************************************************************************/

int main(void)
{
	uint8 key;			/* a variable to get key pressed in keypad */
	uint8 password1[7]; /* a string to save first entered password */
	uint8 password2[7]; /* a string to save reentered password */
	uint8 state_next;	/* a variable to indicate next state after finishing certain operation */
	UART_ConfigType UART_configType = {PARITY_DISABLED,STOP_1_BIT,CHARACTER_8_BIT,UART_BAUDRATE_9600};
	UART_init(&UART_configType); /* initialize UART Driver */
	LCD_init();					/* initialize LCD		   */
	while(1)
	{
		switch(g_state)
		{
		/* State to wait for user to create password */
		case PASSWORD_CREATE:
			PASSWORD_create(password1, password2);
			break;
		/* State to wait for user to open door or change password (IDLE STATE) */
		case MAIN_MENU:
			key = KEYPAD_getPressedKey();
			if(key == '+')
			{
				state_next = OPEN_DOOR;
				g_state = PASSWORD_ENTER;
			}
			else if(key == '-')
			{
				state_next = PASSWORD_CHANGE;
				g_state = PASSWORD_ENTER;
			}
			break;
		/* state to wait for user to enter password */
		case PASSWORD_ENTER:
			PASSWORD_enter(state_next, password1);
			break;
		/* state to unlock and then lock door */
		case OPEN_DOOR:
			DOOR_open();
			break;
		/* state to wait for user to change password */
		case PASSWORD_CHANGE:
			PASSWORD_create(password1, password2);
			break;
		/* state for indication of system alert as user entered password wrong 3 times */
		case PASSWORD_ERROR:
			while(UART_recieveByte() != MC2_READY){} /* wait for MC2 to be ready */
			/* communicate with MC2 that MC1 is ready */
			UART_sendByte(MC1_READY);
			/* display 'Error' on Screen for 1 minute */
			LCD_clearScreen();
			LCD_displayString("ERROR");
			Timer_setSec(60, STATE_idle);
			g_state = WAIT;
			break;
		/* state to wait until a certain operation is completed */
		case WAIT:
			break;
		}
	}
}

