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

namespace EARL {
namespace Dynamixel {

#define AX_MAX_REG				(49)

#define MAXNUM_TXPARAM      (150)
#define MAXNUM_RXPARAM      (225)

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

	Packet() : m_Debug(false){
	}

	void viewPacket() {
		if(m_Debug == 4) {
			std::cerr << "Current packet:";
			for(unsigned int ii = 0; ii < InstructionPacket.size(); ii++) {
				std::cerr << " " << (int)InstructionPacket.at(ii);
			}
			std::cerr << std::endl;
		}
	}

	bool Packet_IsDebug() {
		if(m_Debug < 4) {
		return false;
		}

		return true;
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

	/*0 = Debug Disabled
	 *1 = DXL_Driver Only
	 *2 = ^^^^ & PortMaster
	 *3 = ^^^^ & Port
	 *4 = ^^^^ & Packet*/
	int m_Debug;

private:

	/////////////////////////////////////
	//*-----MAKE THE PACKET-----*//
	/////////////////////////////////////

	/*Creates the header for PING, READ, WRITE, ACTION, REG_WRITE packets*/
	void beginTxPacket(unsigned char id);

	/*Creates the header for a SYNC_WRITE packet.*/
	void beginSyncWritePacket(unsigned char ucStartRegister, unsigned char ucNumBytes);//used to begin the generation of a SYNC_WRITE packet

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

};

class Port {

public:

	typedef enum {
		ERR_NONE = 0,/*There is no error*/
		ERROR = -1
	}Status;

private:

	int m_fd;//file descriptor

	/*Write a Dynamixel packet to serial port*/
	unsigned int writePacket(std::vector<unsigned char> packet);

	/*Reads a Dynamixel packet that has been received by the serial port*/
	unsigned int readPacket(std::vector<unsigned char> * packet, unsigned int size);

	/*Gets the returned packet and returns a response*/
	Port::Status getResponse(std::vector<unsigned char> * buffer);

	bool Port_IsDebug() {
		if(m_Debug < 3) {
			return false;
		} else {
			return true;
		}
	}

public:

	/*Generates dynamixel packets*/
	Packet packet;


	/*The constructor. Initializes the unsigned char ucStartRegister, unsigned char ucNumBytese baudrate, and sets the debug mode
	 */
	Port(int baudRate, int iDebug) : m_fd(-1), m_Debug(iDebug), m_IsOpen(false){
		packet.m_Debug = iDebug;
		m_Baud = baudRate;
	}

	/*0 = Debug Disabled
	 *1 = DXL_Driver Only
	 *2 = ^^^^ & PortMaster
	 *3 = ^^^^ & Port
	 *4 = ^^^^ & Packet*/
	int m_Debug;

	~Port();
	int openPort(const char * pkcPortName);//open the serial port
	void closePort();//close the serial port
	void clearPort();//clear the serial port

	/*Create and send a PING packet, and process any response*/
	int Ping(int id);

	/*Create and send a READ packet, and process any response
	 *Returns the value being read
	 */
	int ReadByte(int id, int reg);
	int ReadWord(int id, int reg);
	int WriteByte(int id, int reg, int val);
	int WriteWord(int id, int reg, int val);
	int RegWrite(int iID, int iRegister, std::vector<int> iValue);
	int Action();



protected:

	/*Baudrate*/
	int m_Baud;
	/*the status of the port being open and active*/
	bool m_IsOpen;

	char m_DeviceName[20];

};

/*High Level class that deals with the communication port
 */
class Master {

public:

	/*All-things Serial Port*/
	Port * port;

	/*0 = Debug Disabled
	 *1 = DXL_Driver Only
	 *2 = ^^^^ & PortMaster
	 *3 = ^^^^ & Port
	 *4 = ^^^^ & Packet*/
	int m_Debug;


	Master(int iDebug, int baudRate) : port(NULL), m_Debug(iDebug), m_Baud(0) {


	}

	/*Initialize the serial port */
	int initialize(const char * pkcPortName, int iBaud);

	/*Terminated the serial port's connection */
	void terminate();

protected:

	/*The baudrate*/
	int m_Baud;

};

/*The highest-level class that deals with Dynamixel motors*/
class DXL_Driver {

public:

	/*The board being used for dxl communication*/
	typedef enum {
		USB2AX
	}Board;

	/*Implements 'setupPort()' and 'setupMotors'*/
	DXL_Driver(int board, const char * deviceName, int baudRate, int numMotors, int iDebug);

	float getAverageMotorVoltage();
	void setDXLPosition(int iID, int iValue);
	int getDXLPosition(int iID);
	void setDXLTorque(int iId, bool enable);
	std::vector<int> getAllRegisters(int iID, int numReg);



private:

	int m_NumMotors;

	/*0 = Debug Disabled
	 *1 = DXL_Driver Only
	 *2 = ^^^^ & PortMaster
	 *3 = ^^^^ & Port
	 *4 = ^^^^ & Packet*/
	int m_Debug;

	Master * master;

	/*Used to configure the serial port*/
	void setupPort(int boardType, const char * deviceName, int baudRate);
	/*Used to configure the Dynamixel motors*/
	int setupMotors();



};

	}//namespace Dynamixel;
}//namespace EARl;

#endif /* DYNAMIXEL_H_ */
