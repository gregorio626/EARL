/*
*
*   Author: EARL Technologies
*
*   Created: May 09, 2016
*
*/

#include "/home/gregorio626/EARL/include/dynamixel/dxl.h" /* The dynamixel header file */
#include <iostream>
#include <string.h>

#ifdef _cplusplus
extern "C" {
#endif

using namespace EARL;
using namespace Dynamixel;


void DXL_Motor::allocateMemory(size_t desiredNumberOfRegisters) {
	
	/*Check to see if they are the same size*/
	if(desiredNumberOfRegisters != numberOfRegisters) {
		/*First, we will create a temporary container, and make is have n number of containers, where n = numberOfRegisters*/
		unsigned char * tmpRegisters = new unsigned char [desiredNumberOfRegisters];
		unsigned char * tmpRegistersDesired = new unsigned char[desiredNumberOfRegisters];

		size_t smallestSize;
		if(desiredNumberOfRegisters < numberOfRegisters) {
			smallestSize = desiredNumberOfRegisters;
		} else {
			smallestSize = numberOfRegisters;
		}
	
		/*Now, we will copy all of the previous values*/
		memcpy(tmpRegisters, motorRegisters, smallestSize);
		memcpy(tmpRegistersDesired, motorRegistersDesired, smallestSize);

		unsigned char* trash = motorRegisters;
		motorRegisters = tmpRegisters;
		if(trash != NULL) {
			delete[] trash;
		}
		trash = motorRegistersDesired;
		motorRegistersDesired = tmpRegistersDesired;
		if (trash != NULL) {
			delete[] trash;
		}

		numberOfRegisters = desiredNumberOfRegisters;

	}
}

DXL_Motor::DXL_Motor(unsigned int uiID, Model model) : numberOfRegisters(2) {
	makeMap();
	motorRegisters = new unsigned char[2];
	motorRegistersDesired = new unsigned char[2];
	motorRegisters[0] = model & 0x00ff;//set model number low byte
	motorRegisters[1] = (model & 0xff00) >> 8;//set modle number high byte

	updateValues();

	for(int ii = 2; ii < numberOfRegisters; ii++) {
		motorRegisters[ii] = 255;
		motorRegistersDesired[ii] = 254;
	}

	motorRegisters[getMappedRegister(REG_ID)] = uiID;//set the ID of the DXL motor
	motorRegistersDesired[getMappedRegister(REG_ID)] = uiID;//set the ID of the DXL motor
}

DXL_Motor::~DXL_Motor() {
	if (motorRegisters != NULL) {
		delete[] motorRegisters;
	}

	if (motorRegisters != NULL) {
		delete[] motorRegistersDesired;
	}
}

unsigned short DXL_Motor::getWord(int iRegister) {
	iRegister = getMappedRegister(iRegister);
	return (int) motorRegisters[iRegister] + (((int) motorRegisters[iRegister + 1]) << 8);
}

void DXL_Motor::setWord(int iRegister, unsigned short newValue) {
	iRegister = getMappedRegister(iRegister);
	motorRegistersDesired[iRegister] = newValue & 0xFF;
	motorRegistersDesired[iRegister + 1] = (newValue >> 8) & 0xFF;
}

void DXL_Motor::setCalibration(double offset, bool reversed) {
	setup.setMotorOffset(offset);
	setup.setReverse(reversed);
}


void DXL_Motor::updateValues() {
	//		unsigned char currentID = get_value(REG_ID);
	Model model = (Model) getWord(0);
	setup.setup(model);

	unsigned int newNumberOfRegisterLength = 0;
	switch (model) {
	case MODEL_AX18A:
		newNumberOfRegisterLength = 50;
		break;
	}
	allocateMemory(newNumberOfRegisterLength);

	//		set_value(REG_ID, currentID);
}

std::string EARL::Dynamixel::modelToStr(Model model) {
	switch (model) {

	case MODEL_AX18A:
		return "AX18A";
	default:
		return "Unknown";
	}
}

std::string DXL_Motor::getDXLModel() {
	return modelToStr((Model) getWord(REG_Model_Number));
}

int DXL_Motor::getMappedRegister(int iRegister) {
	int result = iRegister;
	if (result >= 0x8000) {
		std::map<Model, std::map<int, RegisterValue> >::iterator it =
				reg_map.find(
						(Model) ((int) motorRegisters[0]
								| (((int) motorRegisters[1]) << 8)));

		if (it != reg_map.end()) {
			std::map<int, RegisterValue>::iterator it2 = it->second.find(iRegister);
			if (it2 != it->second.end()) {
				result = it2->second;
			}
		} else {
			result -= 0x8000; // What is this register?  OR are we in a different
							  // motor type?
		}
	}

	//		std::cerr << " - " << result << std::endl;

	return result;
}

std::map<Model, std::map<int, RegisterValue> > DXL_Motor::reg_map;

void DXL_Motor::makeMap() {
	if(reg_map.size() == 0) {
		{
			Model AX_Series[] = { MODEL_AX18A, };

			for (int ii = 0; ii < sizeof(AX_Series) / sizeof(*AX_Series); ii++) {
				reg_map[AX_Series[ii]][REG_Model_Number] = AX_Model_Number;
				reg_map[AX_Series[ii]][REG_Firmware_Ver] = AX_Firmware_Ver;
				reg_map[AX_Series[ii]][REG_ID] = AX_ID;
				reg_map[AX_Series[ii]][REG_Baud] = AX_Baud;
				reg_map[AX_Series[ii]][REG_Ret_Delay_Time] = AX_Ret_Delay_Time;
				reg_map[AX_Series[ii]][REG_CW_Ang_Lim] = AX_CW_Ang_Lim;
				reg_map[AX_Series[ii]][REG_CCW_Ang_Lim] = AX_CCW_Ang_Lim;
				reg_map[AX_Series[ii]][REG_Temp_Lim] = AX_Temp_Lim;
				reg_map[AX_Series[ii]][REG_Voltage_Lim_Low] = AX_Voltage_Lim_Low;
				reg_map[AX_Series[ii]][REG_Voltage_Lim_Hi] = AX_Voltage_Lim_Hi;
				reg_map[AX_Series[ii]][REG_Torque_Max] = AX_Torque_Max;
				reg_map[AX_Series[ii]][REG_Stat_Ret_Level] = AX_Stat_Ret_Level;
				reg_map[AX_Series[ii]][REG_Alarm_LED] = AX_Alarm_LED;
				reg_map[AX_Series[ii]][REG_Alarm_Shutdown] = AX_Alarm_Shutdown;
				reg_map[AX_Series[ii]][REG_Torque_EN] = AX_Torque_EN;
				reg_map[AX_Series[ii]][REG_LED] = AX_LED;
				reg_map[AX_Series[ii]][REG_CW_MARGIN] = AX_CW_Margin;
				reg_map[AX_Series[ii]][REG_CCW_MARGIN] = AX_CCW_Margin;
				reg_map[AX_Series[ii]][REG_CW_SLOPE] = AX_CW_Slope;
				reg_map[AX_Series[ii]][REG_CCW_SLOPE] = AX_CCW_Slope;
				reg_map[AX_Series[ii]][REG_Goal_Pos] = AX_Goal_Pos;
				reg_map[AX_Series[ii]][REG_Moving_Speed] = AX_Goal_Vel;
				reg_map[AX_Series[ii]][REG_Torque_Lim] = AX_Goal_Torque;
				reg_map[AX_Series[ii]][REG_Present_Pos] = AX_Present_Pos;
				reg_map[AX_Series[ii]][REG_Present_Speed] = AX_Present_Speed;
				reg_map[AX_Series[ii]][REG_Present_Load] = AX_Present_Load;
				reg_map[AX_Series[ii]][REG_Present_Voltage] = AX_Present_Voltage;
				reg_map[AX_Series[ii]][REG_Present_Temp] = AX_Present_Temp;
				reg_map[AX_Series[ii]][REG_Registered] = AX_Registered;
				reg_map[AX_Series[ii]][REG_Moving] = AX_Moving;
				reg_map[AX_Series[ii]][REG_Lock] = AX_Lock;
				reg_map[AX_Series[ii]][REG_Punch] = AX_Punch;
			}
		}
	}
}
#ifdef _cplusplus
}
#endif



