/******************************************************************************
 *
 * Module: Controller for Control_ECU
 *
 * File Name: control_main.c
 *
 * Description: Source file for the Controller for Control_ECU
 *
 * Author: Mazen Hekal
 *
 *******************************************************************************/

#include "external_eeprom.h"
#include "util/delay.h"
#include "twi.h"
#include "dc_motor.h"
#include "uart.h"
#include "buzzer.h"

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
#define	PASSWORD_CREATE		0			/* State for creating password and saving password */
#define PASSWORD_CHECK		1			/* State for Checking password 					   */
#define	OPEN_DOOR			2			/* State for opening door						   */
#define	PASSWORD_CHANGE		3  			/* State for changing password     				   */
#define	WAIT				5			/* State for Wait				   				   */
#define	PASSWORD_ERROR		6			/* State for Wrong Password						   */

/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

/* a variable to indicate the state of operation */
uint8 g_state = PASSWORD_CREATE;
/* a variable to count number of wrong password entered */
uint8 g_passwordWrongCount = 0;
/* a string to save password */
uint8 g_password[6];
/* value to read for EEPROM    */
uint8 g_val = 0;
/* variable to determine case of entered password */
uint8 g_password_case = SUCCESS;

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
 * Function to save password in EEPROM
 */
void PASSWORD_create(void);

/*
 * Description :
 * Function to check password from EEPROM
 */
void PASSWORD_check(void);

/*
 * Description :
 * Function to rotate Motor CW for 15 seconds
 */
void DC_MOTOR_rotateCW15(void);

/*
 * Description :
 * Function to hold DC Motor for 3 seconds
 */
void DC_MOTOR_hold3(void);

/*
 * Description :
 * Function to rotate Motor ACW for 15 seconds
 */
void DC_MOTOR_rotateACW15(void);




/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*
 * Description :
 * Function to return to idle state
 */
void STATE_idle(void)
{
	DC_MOTOR_rotate(DC_MOTOR_STOP, 0);
	Buzzer_off();
	g_passwordWrongCount = 0;
	g_state = PASSWORD_CHECK;
	UART_sendByte(MC2_READY);
}

/*
 * Description :
 * Function to save password in EEPROM
 */
void PASSWORD_create(void)
{
	while(UART_recieveByte() != MC1_READY){} /* wait for MC1 to be ready */
	/* Send MC2_READY byte to MC1 to ask it to send the string */
	UART_sendByte(MC2_READY);

	_delay_ms(10);

	/* Receive String from MC1 through UART */
	UART_receiveString(g_password);
	/* Send MC_NOTREADY byte to MC1 */
	UART_sendByte(MC_NOTREADY);
	for(int i = 0; i < 5; i++)
	{
		EEPROM_writeByte(0x0311 + i, g_password[i] - '0'); /* Write 0x0F in the external EEPROM */
		_delay_ms(10);
	}
	while(UART_recieveByte() != MC1_READY){}
	/* Send MC2_READY byte to MC1 to ask it to send the string */
	UART_sendByte(MC2_READY);
	_delay_ms(10);

	/* Receive String from MC1 through UART */
	UART_receiveString(g_password);
	/* Send MC_NOTREADY byte to MC1 */
	UART_sendByte(MC_NOTREADY);
	g_password_case = SUCCESS;
	/* check if reentered password is the same as first entered password */
	for(int i = 0; i < 5; i++)
	{
		EEPROM_readByte(0x0311 + i, &g_val);
		if(g_val != (g_password[i] - '0'))
		{
			g_password_case = ERROR;
			break;
		}
		_delay_ms(10);
	}
	/* Send MC2_READY byte to MC1 */
	UART_sendByte(MC2_READY);
	/* Send password case to MC1 */
	UART_sendByte(g_password_case);
	if(g_password_case == SUCCESS)
	{
		g_state = PASSWORD_CHECK;
	}
}

/*
 * Description :
 * Function to check password from EEPROM
 */
void PASSWORD_check(void)
{
	while(UART_recieveByte() != MC1_READY){} /* wait for MC1 to be ready */
	/* Send MC2_READY byte to MC1 to ask it to send the string */
	UART_sendByte(MC2_READY);

	_delay_ms(10);

	/* Receive String from MC1 through UART */
	UART_receiveString(g_password);
	/* Send MC_NOTREADY byte to MC1 */
	UART_sendByte(MC_NOTREADY);
	g_password_case = PASSWORD_SUCCESS;
	/* check if password is correct */
	for(int i = 0; i < 5; i++)
	{
		EEPROM_readByte(0x0311 + i, &g_val);
		if(g_val != (g_password[i] - '0'))
		{
			g_password_case = PASSWORD_WRONG;
			break;
		}
		_delay_ms(10);
	}
	if(g_password_case == PASSWORD_WRONG)
	{
		g_passwordWrongCount++;
		if(g_passwordWrongCount == 3)
		{
			g_password_case = PASSWORD_WRONG_3;
		}
	}
	/* Send MC2_READY byte to MC1 */
	UART_sendByte(MC2_READY);
	/* Send password case to MC1 */
	UART_sendByte(g_password_case);
	/*
	 * if password is correct, go to next next
	 * if password entered wrong 3 times, activate system alert
	 */
	if(g_password_case == PASSWORD_SUCCESS)
	{
		g_passwordWrongCount = 0;
		/* Send MC2_READY byte to MC1 */
		UART_sendByte(MC2_READY);
		while(UART_recieveByte() != MC1_READY){} /* wait for MC1 to be ready */
		g_state = UART_recieveByte();		/* receive state from MC1 */
	}
	else if(g_password_case == PASSWORD_WRONG_3)
	{
		g_state = PASSWORD_ERROR;
	}
}

/*
 * Description :
 * Function to rotate Motor CW for 15 seconds
 */
void DC_MOTOR_rotateCW15(void)
{
	DC_MOTOR_rotate(DC_MOTOR_CW, 50);
	Timer_setSec(15, DC_MOTOR_hold3);
}

/*
 * Description :
 * Function to hold DC Motor for 3 seconds
 */
void DC_MOTOR_hold3(void)
{
	DC_MOTOR_rotate(DC_MOTOR_STOP, 0);
	Timer_setSec(3, DC_MOTOR_rotateACW15);
}

/*
 * Description :
 * Function to rotate DC Motor ACW for 15 seconds
 */
void DC_MOTOR_rotateACW15(void)
{
	/* Send MC2_READY byte to MC1 */
	UART_sendByte(MC2_READY);
	DC_MOTOR_rotate(DC_MOTOR_A_CW, 50);
	Timer_setSec(15, STATE_idle);
}

/*******************************************************************************
 *                      Main Function                                          *
 *******************************************************************************/

int main(void)
{
	TWI_ConfigType config_Type = {FIRST_ADDRESS,FAST_MODE};
	UART_ConfigType UART_configType = {PARITY_DISABLED,STOP_1_BIT,CHARACTER_8_BIT,UART_BAUDRATE_9600};
	UART_init(&UART_configType);	/* initialize UART Driver */

	/* Initialize the TWI/I2C Driver */
	TWI_init(&config_Type);

	DC_MOTOR_init();	/* initialize DC Motor  */
	Buzzer_init();		/* initialize Buzzer	*/

	while(1)
	{
		switch(g_state)
		{
		/* state to create password and save it on EEPROM */
		case PASSWORD_CREATE:
			PASSWORD_create();
			break;
		/* state to check if password entered is correct or not */
		case PASSWORD_CHECK:
			PASSWORD_check();
			break;
		/* state to unlock door and then lock door */
		case OPEN_DOOR:
			while(UART_recieveByte() != MC1_READY){} /* wait for MC1 to be ready */
			DC_MOTOR_rotateCW15();
			g_state = WAIT;
			break;
		/* state to change password and save it on EEPROM */
		case PASSWORD_CHANGE:
			PASSWORD_create();
			break;
		/* state for indication of system alert as user entered password wrong 3 times */
		case PASSWORD_ERROR:
			/* Send MC2_READY byte to MC1 to ask it to send the string */
			UART_sendByte(MC2_READY);
			while(UART_recieveByte() != MC1_READY){} /* wait for MC1 to be ready */
			Buzzer_on();
			Timer_setSec(60, STATE_idle);
			g_state = WAIT;
			break;
		/* state to wait until a certain operation is completed */
		case WAIT:
			break;
		}

	}
	return 0;
}
