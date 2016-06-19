/*
 * main.cpp
 *
 *  Created on: Jun 17, 2016
 *      Author: gregorio626
 */

#include "dynamixel.h"

//#include <iostream>

/*             Debug Levels:
 *         0-Debug mode is disabled
 *         1-Errors
 *         2-General Output
 *         3-Errors and General Output
 *         4-Packet Level Only
 */

using namespace EARL;
using namespace Dynamixel;

int board = DXL_Driver::USB2AX;
const char * deviceName = "/dev/ttyACM0";
int baud = 1000000;
int numMotors = 18;
int debug = 2;


int main(int argc, char * argv[]) {

	////WELCOME & PORT INTIALIZATION////
	std::cerr << "//////////////////////////////" << std::endl;
	std::cerr << "//-------EARL Hexapod-------//" << std::endl;
	std::cerr << "//////////////////////////////" << std::endl << std::endl;

	DXL_Driver dxlDriver(board, deviceName, baud, numMotors, debug);

	return 0;
}


