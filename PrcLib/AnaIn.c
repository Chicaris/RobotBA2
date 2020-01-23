//////////////////////////////////////////////////////////////////////////////
//																			//
//	PRC LIBRARY :															//
//  -------------															//
//																			//
//  Analog inputs functions													//
//																			//
//////////////////////////////////////////////////////////////////////////////


// This file contains the definitions for our processor
#include <xc.h>
//  This file contains the prototype of __delay32()
#include <libpic30.h>




// This function initialises the Analog-to-Digital Converter peripheral
// to accept the analog sensors
//	parameter : none
//	return : none
//////////////////////////////////////////////////////////////////////////////
void anaInInit(void)
{
	AD1CON1bits.AD12B	= 0;		// 10 bits, 1-channel ADC operation
	AD1CON3bits.ADCS	= 2;		// Tcy = 25ns ; Tad = (ADCS+1)*TCY >= 75ns -> ADCS = 2
	AD1CHS0bits.CH0SA	= 1;		// selects the input of CH0
	AD1CON1bits.ADON	= 1;		// enables CAN1
	AD1CHS0bits.CH0NA	= 0;		//selects Vref- as negative input of CH0
}


// This function reads the voltage of a sensor connected the a Ain input
// and converts it to a numerical value.
// Parameter :
//		anaIn : the number of the Analog Input to read.
// Return value :
//		Input voltage conversion value : 0 -> 1023 <=> 0V -> 5V
//////////////////////////////////////////////////////////////////////////////
int anaInRead(int anaIn)
{
	int tmp;
	
	
	
	switch ( anaIn )
	{
		case 0 :
			AD1CHS0bits.CH0SA = 15;		// selects AN15 input as positive input of CH0
		break;
		case 1 :
			AD1CHS0bits.CH0SA = 14;		// selects AN14 input as positive input of CH0
		break;
		case 2 :
			AD1CHS0bits.CH0SA = 13;		// selects AN13 input as positive input of CH0
		break;
		case 3 :
			AD1CHS0bits.CH0SA = 12;		// selects AN12 input as positive input of CH0
		break;
		case 4 :
			AD1CHS0bits.CH0SA = 11;		// selects AN11 input as positive input of CH0
		break;
		case 5 :
			AD1CHS0bits.CH0SA = 10;		// selects AN10 input as positive input of CH0
		break;
		case 6 :
			AD1CHS0bits.CH0SA = 9;		// selects AN9 input as positive input of CH0
		break;
		case 7 :
			AD1CHS0bits.CH0SA = 8;		// selects AN8 input as positive input of CH0
		break;
	}
	
	AD1CON1bits.SAMP = 1;				// starts the conversion
	__delay32(12);						//  waits 12 Tcy ( = 4 Tad ) to ensure that the conversion has the time to end correctly (12Tcy is minimum for delay32)
	AD1CON1bits.SAMP = 0;				// start Converting
	while (!AD1CON1bits.DONE);			// conversion done?

	tmp =( 21*ADC1BUF0 ) >> 4;			// scales the result to fit between 0 and 1023
	if ( tmp > 1023 )					// test for overflow
		tmp = 1023;
	return ( tmp );						// returns the result of the conversion
}
