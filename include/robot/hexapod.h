/*
 * hexapod.h
 *
 *  Created on: Jun 4, 2016
 *      Author: gregorio626
 */

#ifndef HEXAPOD_H_
#define HEXAPOD_H_

#include "dynamixel.h"
#include <vector>



namespace EARL {
namespace Robot {

class Hexapod {
private:
	Dynamixel::Master * dxlMaster;

	bool m_Debug;

	/*Uses 'EARL::Dynamixel::Port::Ping(...) to make sure all of the motors are present.*/
	const std::vector<int> motorRollCall(const unsigned int& numMotors);

	/*The number of motors connected*/
	int m_NumMotors;

public:
	Hexapod(bool bDebug);

	/*Sets the 'PortMaster' object*/
	void setMaster(Dynamixel::Master * master);

	bool Hexapod_IsDebug();


	/*Used to enable & disable debug mode for objects associated with the Dynamixel motors.
	 */
	void setDebug_Hexapod(bool bDebug);
	void setDebug_PortMaster(bool bDebug);
	void setDebug_Port(bool bDebug);
	void setDebug_Packet(bool bDebug);

	void enableDebugAll_DXL();
	void disableDebugAll_DXL();


	/*deals with setting up the motors for use, creating containers to store the registers for each of the motors, and verifying that all of the motors are connected
	 *returns the number of motors connected and active
	 */
	int setupMotors(int numMotors);

protected:
	float getAverageMotorVoltage();

	void relaxAll(int numberOfMotors);//disables the torque of all motors
};
}
}



#endif /* HEXAPOD_H_ */
