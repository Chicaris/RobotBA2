//////////////////////////////////////////////////////////////////////////////
//																			//
//	PRC LIBRARY :															//
//  -------------															//
//																			//
//  LEDs functions															//
//																			//
//////////////////////////////////////////////////////////////////////////////



//	This function enables one motor
//	parameters :
//		motor : ID of the motor
//	return : none
//////////////////////////////////////////////////////////////////////////////
void motorEnable(int motor);


//	This function disables one motor
//	parameters :
//		motor : ID of the motor
//	return : none
//////////////////////////////////////////////////////////////////////////////
void motorDisable(int motor);


//	This function sets the voltage applied to one motor
//	parameters :
//		motor : ID of the motor
//		voltage : new value of the voltage : -2047 ->2047 <=> -9V -> 9V
//	return : none
//////////////////////////////////////////////////////////////////////////////
void motorSetVoltage(int motor, int voltage);


//	This function returns the value measured by the NXT rotation sensor
//	parameter :
//		channel : the ID of the encoder to be read ( 0 -> 1 )
//	return :
//		angle measured by the encoder since its initialisation, in degrees
//////////////////////////////////////////////////////////////////////////////
long encoderRead(int channel);
