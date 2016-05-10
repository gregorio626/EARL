/*
*
*   Author: EARL Technologies
*
*   Created: May 03, 2016
*
*/

#include "/home/gregorio626/EARL/include/dynamixel/dynamixel.h"
#include <iostream>
#include <string.h>

#ifdef _cplusplus
extern "C" {
#endif

using namespace EARL;
using namespace Dynamixel;

Handler::Handler() : interface(NULL), isBusy(false) {
	baud = (int)NULL;


}

Handler::~Handler() {

}

Interface::Status Handler::openInterface(const char * field, int baudrate){
	baud = baudrate;
	interface = Interface::createInterface(baudrate);

	Interface::Status ret;
	if((ret = interface->openPort(field)) == Interface::ERR_NONE) {
		ret = Interface::ERR_NONE;
		std::cout << "EARL::Dynamixel::Handler::openInterface(\"" << field << "\", " << baudrate << "): NONE" << std::endl;
	}
	else{
		ret = Interface::ERR_UNKNOWN;
		std::cerr << "EARL::Dynamixel::Handler::openInterface(\"" << field << "\", " << baudrate << "): ERROR" << std::endl;

	}

	return ret;
}


#ifdef _cplusplus
}
#endif
