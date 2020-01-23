//////////////////////////////////////////////////////////////////////////////
//																			//
//	PRC LIBRARY :															//
//  -------------															//
//																			//
//  Digital inputs functions												//
//																			//
//////////////////////////////////////////////////////////////////////////////


//	This function reads the state of a digital input and returns the result.
//	parameter :
//		digIn : the ID of the digital input to read ( 0 -> 7 )
//	return value :
//		state of the digital input ( 0 or 1 )
///////////////////////////////////////////////////////////////////////////////
void digInInit(void);
int digInRead(int digIn);
