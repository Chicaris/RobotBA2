//////////////////////////////////////////////////////////////////////////////
//																			//
//	PRC LIBRARY :															//
//  -------------															//
//																			//
//  LEDs functions															//
//																			//
//////////////////////////////////////////////////////////////////////////////


// This file contains the definitions for our processor
#include <xc.h>


// defines LEDs output pins
#define led0	LATBbits.LATB4
#define led1	LATBbits.LATB5
#define led2	LATAbits.LATA13
#define led3	LATAbits.LATA12
#define led4	LATAbits.LATA0
#define led5	LATEbits.LATE7
#define led6	LATEbits.LATE6
#define led7	LATEbits.LATE5


//	This function initialises the digital outputs that controls the LEDs
//	parameter : none
//	return : none
//////////////////////////////////////////////////////////////////////////////
void ledInit(void)
{
	TRISBbits.TRISB4 = 0;	// LED0
	TRISBbits.TRISB5 = 0;	// LED1
	TRISAbits.TRISA13 = 0;	// LED2
	TRISAbits.TRISA12 = 0;	// LED3
	TRISAbits.TRISA0 = 0;	// LED4
	TRISEbits.TRISE7 = 0;	// LED5
	TRISEbits.TRISE6 = 0;	// LED6
	TRISEbits.TRISE5 = 0;	// LED7
}



//	This function sets the state of one LED
//	parameters :
//		led : ID of the LED
//		state : new state of the LED
//	return : none
//////////////////////////////////////////////////////////////////////////////
void ledWrite(int led, int state)
{
	switch( led )
	{
		case 0 :
			led0 = state;
		break;
		case 1 :
			led1 = state;
		break;
		case 2 :
			led2 = state;
		break;
		case 3 :
			led3 = state;
		break;
		case 4 :
			led4 = state;
		break;
		case 5 :
			led5 = state;
		break;
		case 6 :
			led6 = state;
		break;
		case 7 :
			led7 = state;
		break;
	}
}



//	This function sets the state of all LEDs
//	parameters :
//		value : new state of the LEDs, coded as a binary value
//	return : none
//////////////////////////////////////////////////////////////////////////////
void ledWriteByte(int value)
{
	if ( ( value >= 0 ) && ( value < 256 ) )
	{
		led0 = value & 0x01;
		value >>= 1;
		led1 = value & 0x01;
		value >>= 1;
		led2 = value & 0x01;
		value >>= 1;
		led3 = value & 0x01;
		value >>= 1;
		led4 = value & 0x01;
		value >>= 1;
		led5 = value & 0x01;
		value >>= 1;
		led6 = value & 0x01;
		value >>= 1;
		led7 = value & 0x01;
	}
}



//	This function toggles the state of one LED
//	parameters :
//		led : ID of the LED
//	return : none
//////////////////////////////////////////////////////////////////////////////
void ledToggle(int led)
{
	switch( led )
	{
		case 0 :
			led0 = !led0;
		break;
		case 1 :
			led1 = !led1;
		break;
		case 2 :
			led2 = !led2;
		break;
		case 3 :
			led3 = !led3;
		break;
		case 4 :
			led4 = !led4;
		break;
		case 5 :
			led5 = !led5;
		break;
		case 6 :
			led6 = !led6;
		break;
		case 7 :
			led7 = !led7;
		break;
	}
}
