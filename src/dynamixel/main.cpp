/*
 * main.cpp
 *
 *  Created on: May 25, 2016
 *      Author: gregorio626
 */


#include <iostream>

#include "dynamixel.h"
#include "hexapod.h"

#include <string>
#include <vector>
#include <map>
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>





using namespace EARL;
using namespace Dynamixel;
using namespace Robot;

int main(int argc, char * argv[]) {

	int baudrate = 1000000;


	////WELCOME & PORT INTIALIZATION////
	std::cerr << "//////////////////////////////" << std::endl;
	std::cerr << "//-------EARL Hexapod-------//" << std::endl;
	std::cerr << "//////////////////////////////" << std::endl << std::endl;

	Hexapod hexapod(true);
	Master * master = new Master(true);

	hexapod.setMaster(master);//set the Dynamixel master object

	master->initialize("/dev/ttyACM0", baudrate);//initialize the serial port

	////MOTOR SETUP////
	hexapod.setupMotors(18);//setup 18 motors after verifying that they are all connected

	master->terminate();







	return 0;
}


