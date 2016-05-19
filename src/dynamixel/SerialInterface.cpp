/*
*
*	Author: Greg Roberts
*
*	Created: May 10, 2016
*
*/

#include "/home/gregorio626/EARL/include/dynamixel/dxl.h"

#ifdef _cplusplus
extern "C" {
#endif

using namespace EARL;
using namespace Dynamixel;

SerialInterface::SerialInterface(int baud) : board(0), bCurrentlyOpen(false), portName(NULL){
this->baud = baud;
}
SerialInterface::~SerialInterface() {
}

SerialInterface*  SerialInterface::createPortInterface(int currentBoard, int baud) {
		SerialInterface * interface = NULL;

	switch(currentBoard) {

		case usb2ax:
			interface = new USB2AX(baud);
			interface->baud = baud;
			break;
			/*
		case PRO:
			SerialInterface * interface = NULL;
			interface = new SerialInterface(baud);
			interface->baud = baud;
			break;
			*/
		default:
			break;
	}


	return interface;
}


#ifdef _cplusplus
}
#endif
