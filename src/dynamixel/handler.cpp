/*
*
*   Author: EARL Technologies
*
*   Created: April 27, 2016
*
*/

#include "/home/gregorio626/EARL/include/dynamixel/dynamixel.h"
#include <unistd.h>

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
	interface = Interface::create(interfaceType, baud);
	Interface::StatusError ret;
	if((ret = interface->openPort(devPort)) == Interface::ERR_NONE)
	{
		if(!start())
			{
				ret = Interface::ERR_PORT_DISCONNECTED;
			}
			else
			{
				std::cerr << "Error: Handler::initInterface() interfcae->openPort() failed" << std::endl;
			}
			return ret;
	}
}


#ifdef _cplusplus
}
#endif