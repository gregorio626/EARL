/*
*
*	Author: Greg Roberts
*
*	Created: May 10, 2016
*
*/

#include "/home/gregorio626/EARL/include/dynamixel/dxl.h" /* The dynamixel header file */
 #include <string.h>  /* String function definitions */
#include <iostream>   /* Standard input/output for c++ */
#include <errno.h>


#ifdef _cplusplus
extern "C" {
#endif

using namespace EARL;
using namespace Dynamixel;

PortHandler::PortHandler() : interface(NULL), isBusy(false) {
 baud = (int)NULL; }


SerialInterface::Status PortHandler::initialize(int currentBoard, int iBaudNumber) {

	int iBaudrate = (int)(2000000.0 / (float)(iBaudNumber + 1));//takes care of converting the baudNumber to a baudrate

	const char * pkcPortName;
	switch (currentBoard) {
		case SerialInterface::usb2ax:
			pkcPortName = "/dev/ttyACM0";
			break;
	};
	interface = SerialInterface::createPortInterface(currentBoard, iBaudrate);//Here, we create an interface for the port.

	SerialInterface::Status _return;//create an object of the enum

	if((_return == interface->openPort(pkcPortName)) == (int)SerialInterface::ERR_NONE) {
		_return = SerialInterface::ERR_NONE;//if all is good, no error
		std::cout << "EARL::Dynamixel::PortHandler::initialize(\"" << pkcPortName << "\", " << iBaudrate << "): SUCCESS" << std::endl;
	}
	else{
		_return = SerialInterface::ERR_NONE;
		std::cout << "EARL::Dynamixel::PortHandler::initialize(\"" << pkcPortName << "\", " << iBaudrate << "): ERROR : " << errno << std::endl;
	}
	return _return;
}
#ifdef _cplusplus
}
#endif
