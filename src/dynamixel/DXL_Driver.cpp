/*
 * DXL_Driver.cpp
 *
 *  Created on: Jun 17, 2016
 *      Author: gregorio626
 */

#include "dynamixel.h"
#include <errno.h>

using namespace EARL;
using namespace Dynamixel;

void DXL_Driver::setupPort(int boardType, const char * deviceName, int baudRate) {

	if(m_Debug == 1) {
		std::cerr << "#------>Initializing " << deviceName << " ..." << std::endl;
	}

	switch(boardType) {

	case USB2AX:
		if(master->initialize(deviceName, baudRate) != 1) {
			if(m_Debug == 1 || m_Debug == 3) {
				std::cerr << "Error: Failed to initialize \"" << deviceName << "\" for UART communication.(ERRNO: "<< errno << ")." << std::endl << std::endl;
			}
		}
	}
}

DXL_Driver::DXL_Driver(int board, const char * device, int baudRate, int numMotors, int iDebug) : m_NumMotors(numMotors), m_Debug(iDebug), master(NULL){

	Master * tmpMaster = new Master(iDebug, baudRate);

	if(tmpMaster == NULL) {
		if(m_Debug == 1 || m_Debug == 3) {
			std::cerr << "Error: EARL::Dynamixel::DXL_Driver::DXL_Driver(...)----> "
					"cannot assign empty object to pointer object, \"master\"." << std::endl;
			return;
		}
	}
	master = tmpMaster;

	setupPort(board, device, baudRate);

	setupMotors();

	if(m_Debug == 0) {
		std::cerr << "The USB port and the motors have successfully been setup." << std::endl;
	}
}

float DXL_Driver::getAverageMotorVoltage() {
	float averageVoltage = 0;
	int voltageSum = 0;
	std::vector<int> voltage;

	for(int ii = 1; ii < m_NumMotors + 1; ii++) {
		voltage.push_back(master->port->ReadByte(ii, 42));
	}
	for(unsigned int jj = 0; jj < voltage.size(); jj++) {
		voltageSum += voltage[jj];
		/*For debugging, used to view the voltage of motor ID 'ii + 1'*/
		//std::cerr << "voltage[" << jj << "]=>" << voltage[jj] << std::endl;
	}
	averageVoltage = (float)(voltageSum / m_NumMotors) / 10;

	return averageVoltage;
}


int DXL_Driver::setupMotors() {

	if(m_Debug > 1) {
		std::cerr << ">~~~~Initializing Dynamixel motors~~~~<" << std::endl << std::endl;
	}

	std::vector<int> failed2Connect;


	/*If we are not setting up any motors, kill the function*/
	if(m_NumMotors < 1) {
		if(m_Debug == 1 || m_Debug == 3) {
			std::cerr << "Error: EARL::Dynamixel::DXL_Driver::setupMotors(...)---->Attempting to setup " << m_NumMotors << " motors." << std::endl;
		}
		return -1;//ERROR: no motors to be connected to
	}

	if(m_Debug > 0 && m_Debug < 4) {
		std::cerr << "#------>Initializing " << m_NumMotors << " motors..." << std::endl;
	}

	for(int ii = 1; ii < m_NumMotors + 1; ii++) {
		if(master->port->Ping(ii) != 0) {
			failed2Connect.push_back(ii);
		}
	}

	if(failed2Connect.size() > 0) {
		if(m_Debug == 2 || m_Debug == 3) {
			std::cerr << "	Status: failed to connect " << failed2Connect.size() << " motors(";
			for(unsigned int jj = 0; jj < failed2Connect.size(); jj++) {
				if((jj + 1) < failed2Connect.size()) {
					std::cerr << failed2Connect[jj] << ", ";

				} else {
					std::cerr << failed2Connect[jj] << ")" << std::endl;

				}
			}
			std::cerr << "	---------------------------------" << std::endl;
			std::cerr << "		Number of motors:   N/A" << std::endl;
			std::cerr << "		Avg. voltage/motor: N/A" << std::endl;
			std::cerr << "		-------------------------" << std::endl << std::endl;
		} else if(m_Debug == 2 || m_Debug == 3) {
			std::cerr << " 	Status success" << std::endl;
			std::cerr << "	---------------------------------" << std::endl;
			std::cerr << "		Number of motors:   " << m_NumMotors << std::endl;
			std::cerr << "		Avg. voltage/motor: " << getAverageMotorVoltage() << "V"<< std::endl;
			std::cerr << "		-------------------------" << std::endl << std::endl;
		}
	}

	if(m_Debug == 4) {
		std::cerr << std::endl;

		std::cerr << "<~~~~~~~~~~~~~~~Success~~~~~~~~~~~~~~~>" << std::endl << std::endl;
	}
	return m_NumMotors;
}

void DXL_Driver::setDXLPosition(int iID, int iValue) {

	if((master->port->WriteWord(iID, 30, iValue)) == 0) {//write to the goal position register
		if(m_Debug > 0) {
			std::cerr << "Successfully written goal position value '" << iValue << "' to ID '" << iID << "'" << std::endl;
		}
	}
}

int DXL_Driver::getDXLPosition(int iID) {
	int position = master->port->ReadWord(iID, 30);

	return position;
}


void DXL_Driver::setDXLTorque(int iId, bool enable) {

	if((master->port->WriteByte(iId, 24, (int)enable)) == 0) {
		if(m_Debug > 0) {
			std::cerr << "DXL" << iId << " =>Torque ";
			if(enable == true) {
				std::cerr << "enabled" << std::endl;
				return;
			}
			std::cerr << "disabled" << std::endl;
			return;
		}
	}
}

std::vector<int> DXL_Driver::getAllRegisters(int iID, int numReg) {

	std::vector<int> regTable;
	if(m_Debug == 1 || m_Debug == 3) {
		if(numReg < 50) {
			std::cerr << "Error: EARL::Dynamixel::DXL_Driver::getAllRegisters(...)----> Cannot read less than 50 registers." << std::endl;
		} else if (iID > m_NumMotors || iID <= 0) {
			std::cerr << "Error: EARL::Dynamixel::DXL_Driver::getAllRegisters(...)----> Motor ID does not exist." << std::endl;
		}
	}


	for(int ii = 0; ii < numReg) {
		regTable.push_back(master->ReadByte(iID, ii));
	}
}


