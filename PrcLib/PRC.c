//////////////////////////////////////////////////////////////////////////////
//																			//
//	PRC LIBRARY :															//
//  -------------															//
//																			//
//  PRC initialisation function and timing functions						//
//																			//
//////////////////////////////////////////////////////////////////////////////
#include "clock.h"


///////////////////////////////////////////////////////////////////////////////
//		DEFINES
///////////////////////////////////////////////////////////////////////////////
// Definition the number of clock cycle in 1ms
#define UNE_MS FCY/1000

///////////////////////////////////////////////////////////////////////////////
//		INCLUDE FILES
///////////////////////////////////////////////////////////////////////////////
//  This file contains the definitions for our processor
#include <xc.h>
//  This file contains the prototype of __delay32()
#include <libpic30.h>

//	This function initialises the digital outputs that controls the LEDs.
//	All LEDs are off when the function ends.
//	parameter : none
//	return : none
//////////////////////////////////////////////////////////////////////////////
void ledInit(void);


//	This function initialises the digital inputs where the digital sensors
//  can be connected.
//	parameter : none
//	return : none
//////////////////////////////////////////////////////////////////////////////
void digInInit(void);


// This function initialises the Analog-to-Digital Converter peripheral
// to accept the analog sensors
//	parameter : none
//	return : none
//////////////////////////////////////////////////////////////////////////////
void anaInInit(void);


//	This function initialises the PWM generators
//	parameter : none
//	return : none
//////////////////////////////////////////////////////////////////////////////
void motorInit(void);


//  This function initialises the peripherals and the software associated to
//	the NXT encoders (Enocders handling by interruption).
//	parameter : none
//	return : none
//////////////////////////////////////////////////////////////////////////////
void encodersInit(void);

///////////////////////////////////////////////////////////////////////////////
//		GLOBAL VARIABLES
///////////////////////////////////////////////////////////////////////////////

// Software timer : this variable holds the number of ms elapsed since the call
// of TimerInit
unsigned long timerCounter;


///////////////////////////////////////////////////////////////////////////////
//		PRC INITIALISATION FUNCTION AND CONFIGURATION BITS
///////////////////////////////////////////////////////////////////////////////
//  The dsPIC starts clocked by the internal oscillator
_FOSCSEL(FNOSC_FRC);
//  "Clock switching" enabled and 
//  primary oscillator configured to use an external crystal (->10MHz)
_FOSC(FCKSM_CSECMD & OSCIOFNC_OFF & POSCMD_XT);
//  Watchdog disabled
_FWDT(FWDTEN_OFF)
//  JTAG interface disabled
_FICD(JTAGEN_OFF);



//	This function initialises the internal peripheral of the PRC (oscillator
// and TIMER1 as a 1ms timer
//
//	parameter :
//		none
//	return :
//		none
///////////////////////////////////////////////////////////////////////////////
void prcInit(void) {
	pllConfig();	// configure la PLL de l'horloge interne pour tourner à 40MIPS
	// configures unused analog inputs as digital  I/Os 
	AD1PCFGH = 0xFFFF;
	AD1PCFGL = 0x0FFF;	// AN<15:12> used for ADC1
	AD2PCFGL = 0xF0FF;	// AN<11:8> used for ADC2
	// Configures timer1 to generate an interrupt every ms
	T1CONbits.TON = 0;		// stop Timer1
	TMR1 = 0;				// clear timer1
	PR1 = 39999;			// PR1 = period/Tcy - 1 = 1ms/25ns - 1 = 40E3 - 1
	T1CONbits.TCS = 0; 		// Timer1 uses internal clock (Fcy = Fosc/2 = 40MHz)
	T1CONbits.TSIDL = 1; 	// Timer1 is stopped in IDLE mode
	T1CONbits.TCKPS = 0;	// no prescaler
	T1CONbits.TGATE = 0;	// we don't use external enable signal
//	_T1IP = 4;		// set Timer1 interrupt priority to 4 : 0 (lowest) -> 7 (highest)
	_T1IF = 0;		// clear the interrupt flag
	_T1IE = 1;		// enable the interrupt
	
	anaInInit();
	digInInit();
	ledInit();
	motorInit();
	encodersInit();
}


///////////////////////////////////////////////////////////////////////////////
//		TIMING FUNCTIONS
///////////////////////////////////////////////////////////////////////////////

//	This function waits for a given number of milliseconds
//
//	parameters :
//		delay : number of milliseconds to wait
//	return :
//		none
///////////////////////////////////////////////////////////////////////////////
void delay_ms(long delay) {
	while (delay > 0) {
		__delay32(UNE_MS);
		delay--;
	}	
}


//	This function returns the value of the software timer
//
//	parameters :
//		none
//	return :
//		value of the software timer
///////////////////////////////////////////////////////////////////////////////
long timerRead(void) {
	return (timerCounter);
}

//	This function resets and starts the timer.
//
//	parameters :
//		none
//	return :
//		none
///////////////////////////////////////////////////////////////////////////////
void timerStart(void) {
	timerCounter = 0;		// Reset the software timer
	TMR1 = 0;				// Reset TIMER1
	T1CONbits.TON = 1;		// start TIMER1
}

//	This function stops the timer.
//
//	parameters :
//		none
//	return :
//		none
///////////////////////////////////////////////////////////////////////////////
void timerStop(void) {
	T1CONbits.TON = 0;		// stop Timer1
}



//	The TIMER1 ISR is called every ms to increment the software timer
//
//	parameters :
//		none
//	return :
//		none
///////////////////////////////////////////////////////////////////////////////
void __attribute__ (( interrupt, no_auto_psv )) _T1Interrupt(void) {
	_T1IF = 0;
	timerCounter++;
}
