/*
 * hexapod.cpp
 *
 *  Created on: Jun 4, 2016
 *      Author: gregorio626
 */

#include "hexapod.h"

#include <vector>

using namespace EARL;
using namespace Robot;

#ifdef _cplusplus
extern "C" {
#endif

/*The constructor*/
Hexapod::Hexapod(bool bDebug) : dxlMaster(NULL), m_NumMotors(0) {
	setDebug_Hexapod(bDebug);

}

bool Hexapod::Hexapod_IsDebug() {

	return m_Debug;
}

void Hexapod::setDebug_Hexapod(bool bDebug) {

	this->m_Debug = bDebug;
}

void Hexapod::setMaster(Dynamixel::Master * master) {
	dxlMaster = master;

//	/*Make sure the object is not NULL*/
//	if(dxlMaster == NULL) {
//		return;
//	}
}

/*Uses 'EARL::Dynamixel::Port::Ping(...) to make sure all of the motors are present.
 * Returns the number of motors that were not found
 */
const std::vector<int> Hexapod::motorRollCall(const unsigned int& numMotors) {

	std::vector<int> failed2Connect;//stores the ID(s) of the dynamixel motor(s) that could not be found.

	int numFailed = 0;//the number of motors that could not be found

	if(Hexapod_IsDebug()) {
		std::cerr << "#------>Verifying motor connections" << std::endl;
	}
	for(unsigned int ii = 1; ii < numMotors + 1; ii++) {

		if((dxlMaster->port->Ping(ii)) != 0) {
			failed2Connect.push_back(ii);
		}
	}

	if(failed2Connect.size() != 0) {
		numFailed += 1;
	}

	return failed2Connect;
}

void Hexapod::relaxAll(int numberOfMotors) {
	for(int ii = 1; ii < numberOfMotors + 1; ii++) {
		dxlMaster->port->disableTorque(ii);
	}
}
int Hexapod::setupMotors(int numMotors) {


	/*If we are not setting up any motors, kill the function*/
	if(numMotors == 0) {
		if(Hexapod_IsDebug()) {
			std::cerr << "Error: EARL::Robot::Hexaod::setupMotors(...)---->Attempting to setup " << numMotors << " motors." << std::endl;
		}
		return -1;//ERROR: no motors to be connected to
	}

	std::vector<int> failedPing = motorRollCall(numMotors);

	if(failedPing.size() != 0) {

		if(Hexapod_IsDebug()) {

		std::cerr << std::endl;
		std::cerr << "Error: EARL::Robot::Hexapod::setupMotors(" << numMotors << ")----> Could not successfully ping all motors." << std::endl;

		std::cerr << "Motors ID(s)not found: ";
		for(unsigned int kk = 0; kk < failedPing.size(); kk++) {
			std::cerr << failedPing.at(kk);
			if(kk != (failedPing.size() - 1)) {
				std::cerr << ", ";
				}
			}
		std::cerr << std::endl;
		}
		return -1;
	}
	m_NumMotors = numMotors;//set the object member

	if(Hexapod_IsDebug()) {
		std::cerr << "	Status: success!" << std::endl;
		std::cerr << "	---------------------------------" << std::endl;
		std::cerr << "		Number of motors:   " << m_NumMotors << std::endl;
		std::cerr << "		Avg. voltage/motor: " << getAverageMotorVoltage() << "V"<< std::endl;
		std::cerr << "		-------------------------" << std::endl << std::endl;

	}
	relaxAll(m_NumMotors);//disable the torque of all motors

	return numMotors;
}

float Hexapod::getAverageMotorVoltage() {
	float averageVoltage = 0;
	int voltageSum = 0;
	std::vector<int> voltage;



	for(int ii = 1; ii < m_NumMotors + 1; ii++) {
		voltage.push_back(dxlMaster->port->ReadByte(ii, 42));
	}
	for(unsigned int jj = 0; jj < voltage.size(); jj++) {
		voltageSum += voltage[jj];
		/*For debugging, used to view the voltage of motor ID 'ii + 1'*/
		//std::cerr << "voltage[" << jj << "]=>" << voltage[jj] << std::endl;
	}
	averageVoltage = (float)(voltageSum / m_NumMotors) / 10;


	return averageVoltage;
}



#ifdef _cplusplus
}
#endif




