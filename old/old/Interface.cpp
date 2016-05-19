/*
*
*   Author: EARL Technologies
*
*   Created: May 01, 2016
*
*/

#include "/home/gregorio626/EARL/include/dynamixel/dynamixel.h"

#ifdef __cplusplus
extern "C" {
#endif

using namespace EARL;
using namespace Dynamixel;


 Interface* Interface::createInterface(int baud){
	Interface * interface = NULL;
	interface = new USB2AX(baud);
	interface->baud = baud;

	return interface;
}


#ifdef __cplusplus
}
#endif

