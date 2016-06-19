/*
 * PortMaster.cpp
 *
 *  Created on: May 25, 2016
 *      Author: gregorio626
 */

#include "dynamixel.h"

using namespace EARL;
using namespace Dynamixel;

/*Initializes the serial port's connection
 */
int Master::initialize(const char * pkcPortName, int iBaud) {

	if(m_Debug > 1) {
		std::cerr << std::endl;
		std::cerr << ">~~~~~~~Initializing UART port~~~~~~~~<" << std::endl << std::endl;
	}
	m_Baud = iBaud;
	port = new Port(iBaud, m_Debug);

	int rtrn;

	if((rtrn = port->openPort(pkcPortName)) != 1) {
		return 0;//failed
	}
	if(m_Debug == 4) {
		std::cerr << "<~~~~~~~~~~~~~~~Success~~~~~~~~~~~~~~~>" << std::endl << std::endl;
	}
	return true;//success
}

/*Terminates the serial port's connection
 */
void Master::terminate() {
	std::cerr << std::endl;
	if(m_Debug > 1) {
		std::cerr << "~~~~Terminating UART port~~~~" << std::endl;
	}
	port->closePort();
}











