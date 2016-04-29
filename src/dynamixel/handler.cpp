/*
*
*   Author: EARL Technologies
*
*   Created: April 27, 2016
*
*/

#include "dynamixel.h"
#include <unistd.h>
#include <iostream>

#ifdef _cplusplus
extern "C" {
#endif

using namespace EARL;
using namespace Dynamixel;

Handler::Handler() : interface(NULL), isBusy(false)
{

}

Handler::~Handler()
{

}

Interface::StatusError Handler::initInterface(int interfaceType, int baudrate, const char * devPort)
{
	baud = baudrate;
	//interface = Interface::create(interfaceType, baud);
	Interface::StatusError ret;
	if((ret = interface->openPort(devPort)) == Interface::ERR_NONE)
	{
			return ret;
	}
	return ret;
}


#ifdef _cplusplus
}
#endif
