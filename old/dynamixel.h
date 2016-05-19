/*
*
*	Author: Greg Roberts(With help from Matt Bunting)
*
*	Created: May 10, 2016
*
*/

#ifndef _DYNAMIXEL_H_
#define _DYNAMIXEL_H_


#include <iostream>
#include <map>
#include <vector>

namespace EARL{
	namespace Dynamixel{

/*Dynamixel Motor Models
*/
typedef enum {
	MODEL_AX18A = 18
} Model;

enum {
	REG_Model_Number = 0x8000,
	//	REG_Model_Number_H		= 0x8001,
	REG_Firmware_Ver = 0x8002,
	REG_ID = 0x8003,
	REG_Baud = 0x8004,
	REG_Ret_Delay_Time = 0x8005,
	REG_CW_Ang_Lim = 0x8006,
	//	REG_CW_Ang_Lim_H		= 0x8007,
	REG_CCW_Ang_Lim = 0x8008,
	//	REG_CCW_Ang_Lim_H   	= 0x8009,
	REG_Temp_Lim = 0x800B,
	REG_Voltage_Lim_Low = 0x800C,
	REG_Voltage_Lim_Hi = 0x800D,
	REG_Torque_Max = 0x800E,
	//	REG_Torque_Max_H		= 0x800F,
	REG_Stat_Ret_Level = 0x8010,
	REG_Alarm_LED = 0x8011,
	REG_Alarm_Shutdown = 0x8012,

	REG_Torque_EN = 0x8018,
	REG_LED = 0x8019,
	REG_Goal_Pos = 0x801E,
	//	REG_Goal_Pos_H			= 0x801F,
	REG_Moving_Speed = 0x8020,
	//	REG_Moving_Speed_H		= 0x8021,
	REG_Torque_Lim = 0x8022,
	//	REG_Torque_Lim_H		= 0x8023,
	REG_Present_Pos = 0x8024,
	//	REG_Present_Pos_H		= 0x8025,
	REG_Present_Load = 0x8026,
	//	REG_Present_Load_H		= 0x8027,
	REG_Present_Speed = 0x8028,
	//	REG_Present_Speed_H		= 0x8029,
	REG_Present_Voltage = 0x802A,
	REG_Present_Temp = 0x802B,
	REG_Registered = 0x802C,

	REG_Moving = 0x802E,
	REG_Lock = 0x802F,
	REG_Punch = 0x8030,
	//	REG_Punch_H				= 0x8031,

	REG_CW_MARGIN = 0x881A,
	REG_CCW_MARGIN = 0x881B,
	REG_CW_SLOPE = 0x881C,
	REG_CCW_SLOPE = 0x801D,
};

// AX, EX, DX, RX series:
#define AX_Model_Number			(0)
#define AX_Firmware_Ver			(2)
#define AX_ID					(3)
#define AX_Baud					(4)
#define AX_Ret_Delay_Time		(5)
#define AX_CW_Ang_Lim			(6)
#define AX_CCW_Ang_Lim			(8)

#define AX_Temp_Lim				(11)
#define AX_Voltage_Lim_Low		(12)
#define AX_Voltage_Lim_Hi		(13)
#define AX_Torque_Max			(14)
#define AX_Stat_Ret_Level		(16)
#define AX_Alarm_LED			(17)
#define AX_Alarm_Shutdown		(18)

#define AX_Torque_EN			(24)
#define AX_LED					(25)
#define AX_CW_Margin			(26)
#define AX_CCW_Margin			(27)
#define AX_CW_Slope				(28)
#define AX_CCW_Slope			(29)
#define AX_Goal_Pos				(30)
#define AX_Goal_Vel				(32)
#define AX_Goal_Torque			(34)
#define AX_Present_Pos			(36)
#define AX_Present_Speed		(38)
#define AX_Present_Load			(40)
#define AX_Present_Voltage		(42)
#define AX_Present_Temp			(43)
#define AX_Registered			(44)
#define AX_Moving				(46)
#define AX_Lock					(47)
#define AX_Punch				(48)

class DXL_Motor;


/*A class that stores the value of a register
*/
class RegisterValue {
public:

	int value;//The value stored at the register

	RegisterValue() :
			value(-1) {
	}

	RegisterValue & operator =(const int& value) {
		this->value = value;
		return *this;
	}

	operator int() {
		return value;
	}

};

class DXL_Setup {
public:


	void setup(Model model);


	void convert(double position, unsigned char* buffer);


	double invert(unsigned char* buffer);


	void setMotorOffset(double offset) {
		this->offset = offset;
	}


	void setReverse(bool reversed) {
		this->reverse = reversed;
	}

	DXL_Setup() :
			type(MODEL_AX18A), offset(0), reverse(false) {
	}

private:
	Model type;//The modle of th motor
	double offset;
	bool reverse;

};



unsigned char getChecksum(const unsigned char * pkucBuffer);

}//namespace Dynamixel
}//namespace EARL

#endif
