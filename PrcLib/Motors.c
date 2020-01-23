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


// "Motor Enable" pins definition
#define motorEnable0	PORTAbits.RA2
#define motorEnable1	PORTGbits.RG15
#define motorEnable3	PORTDbits.RD13
#define motorEnable2	PORTDbits.RD12
#define motorEnable5	PORTCbits.RC13
#define motorEnable4	PORTDbits.RD9
#define motorEnable7	PORTCbits.RC14
#define motorEnable6	PORTAbits.RA14

// "Motor Direction" pins definition
#define motorDirection0 PORTGbits.RG2
#define motorDirection1 PORTAbits.RA3
#define motorDirection2 PORTAbits.RA4
#define motorDirection3 PORTDbits.RD11
#define motorDirection4 PORTDbits.RD10
#define motorDirection5 PORTDbits.RD8
#define motorDirection6 PORTAbits.RA15
#define motorDirection7 PORTAbits.RA5

// NXT quadrature encoders pins definition
#define CH0A	PORTGbits.RG7
#define CH0B	PORTGbits.RG6
#define CH1A	PORTGbits.RG9
#define CH1B	PORTGbits.RG8



//	This function initialises the PWM generators
//	parameter : none
//	return : none
//////////////////////////////////////////////////////////////////////////////
void motorInit(void)
{
	//  MotorEnable pins are configured as outputs
	TRISGbits.TRISG15 = 0;
	TRISAbits.TRISA2 = 0;
	TRISDbits.TRISD13 = 0;
	TRISDbits.TRISD12 = 0;
	TRISCbits.TRISC13 = 0;
	TRISDbits.TRISD9 = 0;
	TRISCbits.TRISC14 = 0;
	TRISAbits.TRISA14 = 0;

	//  MotorDirection pins are configured as outputs
	TRISGbits.TRISG2 = 0;
	TRISAbits.TRISA3 = 0;
	TRISAbits.TRISA4 = 0;
	TRISDbits.TRISD11 = 0;
	TRISDbits.TRISD10 = 0;
	TRISDbits.TRISD8 = 0;
	TRISAbits.TRISA15 = 0;
	TRISAbits.TRISA5 = 0;


	//  Timer2 is the clock source for the output compare modules
	//  The desired PWM frequency is 20kHz.
	//  => PR2 = T_PWM/(prescaler*Tcy)-1 = 50ms/25ns-1 = 19999
	//  (with a prescaler of 1:1)
	//  We takes PR2 = 2047 to have a full 10-bit range for the duty cycle
	//////////////////////////////////////////////////////////////////////
	//  Timer2 is stopped to be configured
	T2CONbits.TON = 0;
	//  Timer2 is driven by the primary ocillator
	T2CONbits.TCS = 0;
	// Prescaler 1:1
	T2CONbits.TCKPS = 0;
	//  Timer2 is reset
	TMR2 = 0;
	//  Timer2 period is set to 2048 clock cycles
	PR2 = 2047;
	//  Timer2 is started
	T2CONbits.TON = 1;

	//  Output compare modules are configured to work as PWM modulators,
	//  clocked by Timer2
	////////////////////////////////////////////////////////////////////
	//  OC1
	//  OC module is disabled to be configured
	OC1CONbits.OCM = 0;
	//  Clock source si Timer2
	OC1CONbits.OCTSEL = 0;
	//  Duty cycle is set to 0 (0V applied to the motor)
	OC1RS = 0;
	// "PWM without fault protection" mode is selected
	OC1CONbits.OCM = 6;
	// Motor direction is initialised to the positive one
	motorDirection7 = 0;
	//OC2
	OC2CONbits.OCM = 0;
	OC2CONbits.OCTSEL = 0;	
	OC2RS = 0;	
	OC2CONbits.OCM = 6;
	motorDirection6 = 0;
	//OC3
	OC3CONbits.OCM = 0;
	OC3CONbits.OCTSEL = 0;	
	OC3RS = 0;	
	OC3CONbits.OCM = 6;
	motorDirection5 = 0;
	//OC4
	OC4CONbits.OCM = 0;
	OC4CONbits.OCTSEL = 0;	
	OC4RS = 0;
	OC4CONbits.OCM = 6;
	motorDirection4 = 0;
	//OC5
	OC5CONbits.OCM = 0;
	OC5CONbits.OCTSEL = 0;	
	OC5RS = 0;
	OC5CONbits.OCM = 6;
	motorDirection3 = 0;
	//OC6
	OC6CONbits.OCM = 0;
	OC6CONbits.OCTSEL = 0;	
	OC6RS = 0;
	OC6CONbits.OCM = 6;
	motorDirection2 = 0;
	//OC7
	OC7CONbits.OCM = 0;
	OC7CONbits.OCTSEL = 0;	
	OC7RS = 0;
	OC7CONbits.OCM = 6;
	motorDirection1 = 0;
	//OC8
	OC8CONbits.OCM = 0;
	OC8CONbits.OCTSEL = 0;	
	OC8RS = 0;
	OC8CONbits.OCM = 6;
	motorDirection0 = 0;
}



//	This function enables one motor
//	parameters :
//		motor : ID of the motor
//	return : none
//////////////////////////////////////////////////////////////////////////////
void motorEnable(int motor)
{
	switch( motor )
	{
		case 0 : motorEnable0 = 1;   break;
		case 1 : motorEnable1 = 1;   break;
		case 2 : motorEnable2 = 1;   break;
		case 3 : motorEnable3 = 1;   break;
		case 4 : motorEnable4 = 1;   break;
		case 5 : motorEnable5 = 1;   break;
		case 6 : motorEnable6 = 1;   break;
		case 7 : motorEnable7 = 1;   break;
	}
}



//	This function disables one motor
//	parameters :
//		motor : ID of the motor
//	return : none
//////////////////////////////////////////////////////////////////////////////
void motorDisable(int motor)
{
	switch( motor )
	{
		case 0 : motorEnable0 = 0;   break;
		case 1 : motorEnable1 = 0;   break;
		case 2 : motorEnable2 = 0;   break;
		case 3 : motorEnable3 = 0;   break;
		case 4 : motorEnable4 = 0;   break;
		case 5 : motorEnable5 = 0;   break;
		case 6 : motorEnable6 = 0;   break;
		case 7 : motorEnable7 = 0;   break;
	}
}



//	This function sets the voltage applied to one motor
//	parameters :
//		motor : ID of the motor
//		voltage : new value of the voltage : -2047 ->2047 <=> -9V -> 9V
//	return : none
//////////////////////////////////////////////////////////////////////////////
void motorSetVoltage(int motor, int voltage)
{
	int direction, dutyCycle;
	
	if (voltage < 0) {
		dutyCycle = -voltage;
		direction = 0;
	} else {
		dutyCycle = PR2 + 1 -voltage;
		direction = 1;
	}	
	
	if (dutyCycle > PR2+1)
		dutyCycle = PR2 + 1;
	else if (dutyCycle < 0)
		dutyCycle = 0;

	switch (motor)
	{
		case 0 :
			motorDirection0 = direction;
			OC8RS = dutyCycle;
		break;
		case 1 :
			motorDirection1 = direction;
			OC7RS = dutyCycle;
		break;
		case 2 :
			motorDirection2 = direction;
			OC6RS = dutyCycle;
		break;
		case 3 :
			motorDirection3 = direction;
			OC5RS = dutyCycle;
		break;
		case 4 :
			motorDirection4 = direction;
			OC4RS = dutyCycle;
		break;
		case 5 :
			motorDirection5 = direction;
			OC3RS = dutyCycle;
		break;
		case 6 :
			motorDirection6 = direction;
			OC2RS = dutyCycle;
		break;
		case 7 :
			motorDirection7 = direction;
			OC1RS = dutyCycle;
		break;
	}
}




int cnIsrTmp, oldCH0A, oldCH1A;
long tmpStep0, tmpStep1;

//  This function initialises the peripherals and the software associated to
//	the NXT encoders (Enocders handling by interruption).
//	parameter : none
//	return : none
//////////////////////////////////////////////////////////////////////////////
void encodersInit(void)
{
	// Initialises the encoders
	tmpStep0 = 0;			// init step counter for encoder0
	oldCH0A = CH0A;			// read initial value of CHA for encoder0
	tmpStep1 = 0;			// init step counter for encoder1
	oldCH1A = CH1A;			// read initial value of CHA for encoder1
	
	CNEN1bits.CN9IE = 1;	// enable CH0A CN interrupt
	CNEN1bits.CN11IE = 1;	// enable CH1A CN interrupt
	_CNIF = 0;				// clear the interrupt flag before...
	_CNIE = 1;				// enable the interrupt
}



void __attribute__ (( interrupt, no_auto_psv )) _CNInterrupt( void )
{
	_CNIF = 0;	// clear the interrupt flag at the beginning of the ISR to be 
				// reinterrupted in case of another event (both encoders are asynchronous)

	cnIsrTmp = 	CH0A;				// we read input pin only once in the ISR
	if ( cnIsrTmp != oldCH0A )		// if input pin has changed
	{
		oldCH0A = cnIsrTmp;			// store the new value
		if ( CH0B ^ cnIsrTmp ) 
			tmpStep0++;
		else
			tmpStep0--;
	}
	
	cnIsrTmp = 	CH1A;				// we read input pin only once in the ISR
	if ( cnIsrTmp != oldCH1A )		// if input pin has changed
	{
		oldCH1A = cnIsrTmp;			// store the new value
		if ( CH1B ^ cnIsrTmp ) 
			tmpStep1--;
		else
			tmpStep1++;
	}
}



//	This function returns the value measured by the NXT rotation sensor
//	parameter :
//		motor : the ID of the encoder to be read ( 0 -> 1 )
//	return :
//		angle measured by the encoder since its initialisation, in degrees
//////////////////////////////////////////////////////////////////////////////
long encoderRead(int motor)
{
	long value;
	
	if ( motor == 0 )
	{
		_CNIE = 0;			// disable interrupt to read the value safely
		value = tmpStep0;	// read the value
		_CNIE = 1;			// re-enable the interrupt
	}
	else if ( motor == 1 )
	{
		_CNIE = 0;			// disable interrupt to read the value safely
		value = tmpStep1;	// read the value
		_CNIE = 1;			// re-enable the interrupt
	}
	else
		value = 0;

	return( value );
}
