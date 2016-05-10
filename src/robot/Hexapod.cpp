/*
*
*	Author: EARL Technologies
*
*	Created: May 05, 2016
*
*/


#include "/home/gregorio626/EARL/include/dynamixel/dynamixel.h"
#include "/home/gregorio626/EARL/include/robot/Hexapod.h"

using namespace EARL;
using namespace Robot;

void Hexapod::setupHandler(Dynamixel::Handler* pHandlerObj, unsigned int motorCount) {

	dynamixelHandler = pHandlerObj;

	if(dynamixelHandler == NULL)
		return;

	for(unsigned int ii = 0; ii < motorCount; ii++){
		dynamixelHandler->forceAddDynamixel(ii + 1);
	}
}