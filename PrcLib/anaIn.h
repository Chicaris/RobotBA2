
//////////////////////////////////////////////////////////////////////////////
//																			//
//	PRC LIBRARY :															//
//  -------------															//
//																			//
//  Analog inputs functions													//
//																			//
//////////////////////////////////////////////////////////////////////////////


/// This function reads the voltage of a sensor connected the a Ain input
// and converts it to a numerical value.
// Parameter :
//		anaIn : the number of the Analog Input to read.
// Return value :
//		Input voltage conversion value : 0 -> 1023 <=> 0V -> 5V
//////////////////////////////////////////////////////////////////////////////
void anaInInit(void);
int anaInRead(int anaIn);
