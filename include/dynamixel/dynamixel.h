/*
 * dynamixel.h
 *
 *  Created on: May 25, 2016
 *      Author: gregorio626
 */

#ifndef DYNAMIXEL_H_
#define DYNAMIXEL_H_

#include <iostream>
#include <map>
#include <vector>

#define AX_Model_Number			(0)
#define AX_Firmware_Ver			(2)
#define AX_ID					(3)
#define AX_Baud					(4)
#define AX_Ret_Delay_Time		(5)
#define AX_CW_Ang_Lim			(6)
#define AX_CCW_Ang_Lim			(8)
#define EX106_Drive_Mode		(10)
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
#define AX_MAX_REG				(49)

namespace EARL {
namespace Dynamixel {

#define MAXNUM_TXPARAM      (150)
#define MAXNUM_RXPARAM      (225)

/*Communication status(for 'getResult()')*/
#define COMM_TX_SUCCESS      (0)
#define COMM_RX_SUCCESS      (1)
#define COMM_TX_FAIL     	(2)
#define COMM_RX_FAIL    		(3)
#define COMM_TX_ERROR        (4)
#define COMM_RX_WAITING      (5)
#define COMM_RX_TIMEOUT      (6)
#define COMM_RX_CORRUPT      (7)



class Packet {
public:
	typedef enum {
		ERRBIT_VOLTAGE = 1,
		ERRBIT_ANGLE = 2,
		ERRBIT_OVERHEAT = 4,
		ERRBIT_RANGE = 8,
		ERRBIT_CHECKSUM = 16,
		ERRBIT_OVERLOAD = 32,
		ERRBIT_INSTRUCTION = 64
	}PacketError;

	/*Instruction type*/
	enum {
		PING = 0x01,
		READ_DATA = 0x02,
		WRITE_DATA = 0x03,
		REG_WRITE = 0x04,
		ACTION = 0x05,
		SYNC_WRITE = 0x83
	};

	Packet() : m_Debug(true) {
	}

	void viewPacket() {
		std::cerr << "Current packet:";
		for(unsigned int ii = 0; ii < InstructionPacket.size(); ii++) {
			std::cerr << " " << (int)InstructionPacket.at(ii);
		}
		std::cerr << std::endl;
	}

	bool packet_IsDebug() {
		return this->m_Debug;
	}

	/*The instruction packet to be sent*/
	std::vector<unsigned char> InstructionPacket;

	/*HIGH LEVEL PACKET GENERATION*/
	/*(For commonly used packets)*/
	const std::vector<unsigned char>& mk_Ping(unsigned char& id);//make a PING packet

	const std::vector<unsigned char>& mk_ReadByte(unsigned char& id, unsigned char& reg);//make a READ_DATA packet for reading 1 byte
	const std::vector<unsigned char>& mk_ReadWord(unsigned char& id, unsigned char& reg);//make a READ_DATA packet for reading 2 bytes

	const std::vector<unsigned char>& mk_WriteByte(unsigned char& id, unsigned char& reg, unsigned int& val);//make a WRITE_DATA packet for writing 1 byte
	const std::vector<unsigned char>& mk_WriteWord(unsigned char& id, unsigned char& reg, unsigned int& val);//make a WRITE_DATA packet for writing 2 bytes

	const std::vector<unsigned char>& mk_RegWrite(unsigned char& id, unsigned char& reg, std::vector<int> iValue);

	const std::vector<unsigned char>& mk_Action();

	/*
	 * ucStartRegister=>the first register that we will be writing to
	 * ucNumBytes=>The number of bytes being written, starting from the start register
	 * ID=> A vector of Dynamixel ID's being written to
	 * Value=>A vector of values being written to the ID's
	 */
	const std::vector<unsigned char>& mk_SyncWrite(unsigned char& ucStartRegister, unsigned char& ucNumBytes, std::vector<unsigned char> ID, std::vector<unsigned char> Value);

	bool checkPacket(const std::vector<unsigned char>& data);
private:



	/*The status packet being received*/
	std::vector<unsigned char> StatusPacket;

	/////////////////////////////////////
	//*-----MAKE THE PACKET-----*//
	/////////////////////////////////////

	/*Create the header of the packet, set the Dynamixle ID, and allocate space for the length of the packet*/
	void beginTxPacket(unsigned char id);

	void beginTxSyncWritePacket(unsigned char ucStartRegister, unsigned char ucNumBytes);//used to begin the generation of a SYNC_WRITE packet

	/*Creates the footer of the packet. This sets the lengths, and sets the checksum value*/
	void endTxPacket();

	/*Set the Dynamixel ID of the instruction packet*/
	void setTxPacketID(unsigned char id);

	/*Set the instruction type of the instruction packet*/
	void setTxPacketInstruction(unsigned char instruction);

	void setTxPacketParameter(unsigned char val);

	/*Sets the number of bytes to be read in a READ_DATA packet*/
	void setTxPacketReadLength(unsigned char length);

	/*Returns the low byte*/
	int getLowByte(int iWord);

	/*Returns the high byte*/
	int getHighByte(int iWord);

	/*returns the calculated checksum*/
	unsigned char getChecksum(const unsigned char * buffer);

	bool m_Debug;

};

class Port {

public:

	typedef enum {
		ERR_NONE = 0,/*There is no error*/
		ERROR = -1
	}Status;

private:
	int m_fd;//file descriptor

	/*Write a packet to the port
	 * Returns the number of bytes that have been written, or the error code(errors are negative values)*/
	unsigned int writePacket(std::vector<unsigned char> packet);
	unsigned int readPacket(std::vector<unsigned char> * packet, unsigned int size);

	/*Gets the returned packet and returns a response*/
	Port::Status getResponse(std::vector<unsigned char> * buffer);

	/*True = port is open, false = port is closed*/
	bool isOpen() {
		return m_PortIsOpen;
	}

	bool m_Debug;

public:




	/*The constructor. Initializes the unsigned char ucStartRegister, unsigned char ucNumBytese baudrate, and sets the debug mode
	 */
	Port(int baudRate, bool bDebug) : m_fd(-1), m_Debug(bDebug), m_PortIsOpen(false){
		m_Baud = baudRate;
	}

	~Port();
	int openPort(const char * pkcPortName);//open the serial port
	void closePort();//close the serial port
	void clearPort();//clear the serial port

	int Ping(int id);
	int ReadByte(int id, int reg);
	int ReadWord(int id, int reg);
	int WriteByte(int id, int reg, int val);
	int WriteWord(int id, int reg, int val);
	int RegWrite(int iID, int iRegister, std::vector<int> iValue);
	int Action();

	void setPositionValue(int iID, int iValue);
	void enableTorque(int iID);
	void disableTorque(int iID);

	/*True = 'Port'debug mode enabled . false = disabled*/
	bool Port_IsDebug() {
		return m_Debug;
	}

	/*Returns true if debug mode is enabled, false if disabled
	 */
	bool Packet_IsDebug() {
		return packet.packet_IsDebug();
	}


	/*Generates dynamixel packets*/
	Packet packet;

protected:


	/*Baudrate*/
	int m_Baud;

	bool m_PortIsOpen;//the status of the port being open and active

	char m_PortName[20];


};


/*The main class that deals with the communication port
 */
class Master {

public:

	Port * port;//pointer to object
	Master(bool bDebug) : port(NULL), m_Debug(bDebug), m_Baud(0) {
	}

	/*Initialize the serial port */
	int initialize(const char * pkcPortName, int iBaud);

	/*Terminated the serial port's connection */
	void terminate();

	/*debug mode*/
	bool m_Debug;

protected:

	/*The baudrate*/
	int m_Baud;

};



	}//namespace Dynamixel;
}//namespace EARl;

#endif /* DYNAMIXEL_H_ */
