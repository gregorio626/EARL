/*
 * PortMaster.cpp
 *
 *  Created on: May 25, 2016
 *      Author: gregorio626
 */

#include "dynamixel.h"

#include <unistd.h>
#include <vector>
#include <iostream>

using namespace EARL;
using namespace Dynamixel;

/*Initializes the serial port's connection
 */
int Master::initialize(const char * pkcPortName, int iBaud) {


	std::cerr << "~~~~initializing UART port~~~~" << std::endl << std::endl;
	m_Baud = iBaud;
	port = new Port(iBaud, m_Debug);

	int rtrn;

	if((rtrn = port->openPort(pkcPortName)) != 1) {
		return 0;//failed
	}

	if(port->Port_IsDebug()) {
		std::cerr << "#------>'" << pkcPortName << "' has been successfully initialized." << std::endl;
	}
	return true;//success
}

/*Terminates the serial port's connection
 */
void Master::terminate() {
	std::cerr << std::endl;
	std::cerr << "~~~~Terminating UART port~~~~" << std::endl;
	port->closePort();
}











