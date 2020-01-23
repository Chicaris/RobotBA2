//////////////////////////////////////////////////////////////////////////////
//																			//
//	PRC LIBRARY :															//
//  -------------															//
//																			//
//  Digital inputs functions												//
//																			//
//////////////////////////////////////////////////////////////////////////////


// This file contains the definitions for our processor
#include <xc.h>


// Digital input pins definition
#define digIn0  PORTFbits.RF0
#define digIn1  PORTFbits.RF1
#define digIn2  PORTGbits.RG1
#define digIn3  PORTGbits.RG0
#define digIn4  PORTGbits.RG12
#define digIn5  PORTGbits.RG14
#define digIn6  PORTAbits.RA7
#define digIn7  PORTAbits.RA6



//	This function initialises the digital inputs where the digital sensors
//  can be connected.
//	parameter : none
//	return : none
//////////////////////////////////////////////////////////////////////////////
void digInInit(void)
{
	TRISFbits.TRISF0 = 1;	// DigIn0
	TRISFbits.TRISF1 = 1;	// DigIn1
	TRISGbits.TRISG1 = 1;	// DigIn2
	TRISGbits.TRISG0 = 1;	// DigIn3
	TRISGbits.TRISG12 = 1;	// DigIn4
	TRISGbits.TRISG14 = 1;	// DigIn5
	TRISAbits.TRISA7 = 1;	// DigIn6
	TRISAbits.TRISA6 = 1;	// DigIn7
}



//	This function reads the state of a digital input and returns the result.
//	parameter :
//		digIn : the ID of the digital input to read ( 0 -> 7 )
//	return value :
//		state of the digital input ( 0 or 1 )
///////////////////////////////////////////////////////////////////////////////
int digInRead(int digIn)
{
	int tmp;
	
	switch ( digIn )
	{
		case 0 :
			tmp = digIn0;
		break;
		case 1 :
			tmp = digIn1;
		break;
		case 2 :
			tmp = digIn2;
		break;
		case 3 :
			tmp = digIn3;
		break;
		case 4 :
			tmp = digIn4;
		break;
		case 5 :
			tmp = digIn5;
		break;
		case 6 :
			tmp = digIn6;
		break;
		case 7 :
			tmp = digIn7;
		break;
	}	
	return tmp;
}
