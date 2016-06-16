/*
 * PortControl.cpp
 *
 *  Created on: May 25, 2016
 *      Author: gregorio626
 */


#include <iostream>
#include "dynamixel.h"
#include <string>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <linux/serial.h>
#include <string.h>
#include <stdio.h>
#include <vector>

#ifdef _cplusplus
extern "C" {
#endif

#define ERR_BYTE (4)

using namespace EARL;
using namespace Dynamixel;


int Port::openPort(const char * pkcPortName)
{
	strcpy(m_PortName, pkcPortName);

	if(Port_IsDebug()) {
		std::cerr << "#------>Opening \"" << m_PortName << "\"" << std::endl;
		std::cerr << "	Status: ";
	}
	struct termios newtio;

	memset(&newtio, 0, sizeof(newtio));
	closePort();

	if((m_fd = ::open(m_PortName, O_RDWR|O_NOCTTY|O_NONBLOCK)) < 0) {
		if(Port_IsDebug()) {
			std::cerr << "device open error: " <<  m_PortName << std::endl;
			std::cerr << "	---------------------------------------" << std::endl;
		}
		return 0;
	}

	newtio.c_cflag		= B1000000|CS8|CLOCAL|CREAD;
	newtio.c_iflag		= IGNPAR;
	newtio.c_oflag		= 0;
	newtio.c_lflag		= 0;
	newtio.c_cc[VTIME]	= 0;	// time-out 값 (TIME * 0.1초) 0 : disable
	newtio.c_cc[VMIN]	= 0;	// MIN 은 read 가 return 되기 위한 최소 문자 개수

	clearPort();
	tcsetattr(m_fd, TCSANOW, &newtio);

	if(m_fd == -1)
		return 0;

	closePort();

	memset(&newtio, 0, sizeof(newtio));
	closePort();

	if((m_fd = ::open(m_PortName, O_RDWR|O_NOCTTY|O_NONBLOCK)) < 0) {
		if(Port_IsDebug()) {
			std::cerr << "device open error: " <<  m_PortName << std::endl;
			std::cerr << "	---------------------------------------" << std::endl;
		}
		closePort();
		return 0;
	}
	if(Port_IsDebug()) {
		std::cerr << " success!" << std::endl;//successful
		std::cerr << "	---------------------------------" << std::endl;
	}

	newtio.c_cflag		= B1000000|CS8|CLOCAL|CREAD;
	newtio.c_iflag		= IGNPAR;
	newtio.c_oflag		= 0;
	newtio.c_lflag		= 0;
	newtio.c_cc[VTIME]	= 0;	// time-out 값 (TIME * 0.1초) 0 : disable
	newtio.c_cc[VMIN]	= 0;	// MIN 은 read 가 return 되기 위한 최소 문자 개수

	clearPort();
	tcsetattr(m_fd, TCSANOW, &newtio);

	if(Port_IsDebug()) {
		std::cerr << "		Port name: \"" << m_PortName << "\"" << std::endl;
		std::cerr << "		Baudrate: " << m_Baud << std::endl;
		std::cerr << "		-------------------------" << std::endl << std::endl;

	}

	m_PortIsOpen = true;
	return 1;

}

void Port::closePort()
{
	if(m_fd != -1)
		::close(m_fd);
	m_fd = -1;
}
void Port::clearPort() {
	tcflush(m_fd, TCIFLUSH);
}

Port::~Port() {

}

unsigned int Port::writePacket(std::vector<unsigned char> instructPacket) {
	if(isOpen() == false) {

		if(Packet_IsDebug()) {
			std::cerr << "Error: EARL::Dynamixel::Port::writePort(...)----> Port is not open." << std::endl;
		}
		return -1;//Error: The port is closed
	}
	int numBytesWritten = ::write(m_fd, instructPacket.data(), instructPacket.size());//write the packet. the return will be the number of bytes that have been written

	if(Packet_IsDebug() == true) {
		if(numBytesWritten > 0) {
			std::cerr << "Number of bytes written: " << numBytesWritten << std::endl;
		} else if(numBytesWritten == 0) {
			if(Port_IsDebug())
			std::cerr << "Error: EARL::Dynamixel::Port::writePort(...)----> 0 bytes have been written." << std::endl;
		}
	}
	tcdrain(m_fd);

	return numBytesWritten;
}

unsigned int Port::readPacket(std::vector<unsigned char>* statPacket, unsigned int iNumBytesToRead) {

	unsigned int bytesRead;

	if(iNumBytesToRead <= 0) {
		if(Port_IsDebug()) {
			std::cerr << "EARL::Dynamixel::Port::readPort(...)---->Attempting to read less than 1 byte" << std::endl;
		}
	}
	if(statPacket == NULL) {//if the vector is NULL, or 0, return error
		if(Port_IsDebug()){
			std::cerr << "Error: EARL::Dynamixel::Port::readPort(...)---->Empty packet." << std::endl;
		}
		return -1;//Error: The packet trying to be read is empty
	}


	usleep(15000);//is this too long?

	unsigned char ucPacket[iNumBytesToRead];//we need an array

	if(isOpen() == true) {
		bytesRead = ::read(m_fd, ucPacket, iNumBytesToRead);
	}
	for(unsigned int ii = 0; ii < bytesRead; ii++) {
		statPacket->push_back(ucPacket[ii]);//build the vector that stores the packet read
	}

	return bytesRead;
}

/*
 *
 * 1. Make sure the vector parameter is not empty(should be set as the instruction packet that corresponds
 * 2. Clear the vector parameter
 * 3. Reads the returned status packet & store it as
 *
 *
 */

Port::Status Port::getResponse(std::vector<unsigned char>* buffer) {
	int packetLength = 0;//the length of the packet in the buffer
	int numberOfBytes = 4;

	/*1. check if buffer is empty*/
	if(buffer == NULL) {

		if(Port_IsDebug()) {

			std::cerr << "Error: EARL::Dynamixel::Port::getError(...)----> Empty packet." << std::endl;

		}

		return ERROR;//Error: the packet is empty
	}
	/*2. since we have made it this far, we know the buffer isnt empty. so now we clear it*/
	buffer->clear();

	usleep(5000);

	/*3. Read the packet. return the number of bytes the packet being read, contains*/
	packetLength = readPacket(buffer, numberOfBytes);//read the incoming status packet

	if(packetLength > 0 && buffer->at(0) != 0xFF) {
		buffer->erase(buffer->begin());
		packetLength -= 1;
		packetLength += readPacket(buffer, 1);
	}
	if(Packet_IsDebug()) {
		std::cerr << "Packet size: " << buffer->size() << std::endl;
	}

	if(packetLength == numberOfBytes) {
		packetLength += readPacket(buffer, buffer->at(numberOfBytes - 1));
	} else {
		if(Port_IsDebug()) {
			std::cerr << "Error: EARL::Dynamixel::Port::getResponse(...)----> Read " << packetLength << " bytes:";
			for(unsigned int ii = 0; ii < buffer->size(); ii++) {
				std::cerr << " " << (int)buffer->at(ii);
			}std::cerr << std::endl;
		}
		return ERROR;//Error: bad packet length
	}
	if(Packet_IsDebug()) {
		std::cerr << "Incoming status packet:";
		for(unsigned int ii = 0; ii < buffer->size(); ii++) {
			std::cerr << " " << (int)buffer->at(ii);
		}
		std::cerr << std::endl << std::endl;
	}
	if(!packet.checkPacket(*buffer)) {
		return ERROR;//Error: bad checksum
	}
	return ERR_NONE;
}

int Port::Ping(int id) {
	int status;
	unsigned char tmpID = (unsigned char)id;
	std::vector<unsigned char> tmpPacket;

	/*Generate the packet*/
	tmpPacket = packet.mk_Ping(tmpID);

	if(Packet_IsDebug()) {
		packet.viewPacket();
	}

	/*make sure that the number of bytes written is the same as the number of bytes in the packet*/
	if (writePacket(tmpPacket) != tmpPacket.size()) {
		return ERROR;//Error: bad length
	}

	/*Make sure that there are no errors*/
	if ((status = getResponse(&tmpPacket)) == ERR_NONE) {
			status = tmpPacket[ERR_BYTE];//set it as the error
			return ERR_NONE;
	}

	return status;
}

int Port::ReadByte(int id, int reg) {
	int status;
	unsigned char tmpID = (unsigned char)id;
	unsigned char tmpRegister = (unsigned char)reg;
	std::vector<unsigned char> tmpPacket;

	if(Packet_IsDebug()) {
		std::cerr << "Read byte: ";
	}

	/*Generate the packet*/
	tmpPacket = packet.mk_ReadByte(tmpID, tmpRegister);

	if(Packet_IsDebug()) {
		packet.viewPacket();
	}

	/*make sure that the number of bytes written is the same as the number of bytes in the packet*/
	if (writePacket(tmpPacket) != tmpPacket.size()) {
		if(Port_IsDebug()) {
			std::cerr << "Error: EARL::Dynamixel::Port::ReadByte(...)----> Bad packet length." << std::endl;
		}

		return ERROR;//Error: bad length
	}

	/*Make sure that there are no errors*/
	if ((status = getResponse(&tmpPacket)) == ERR_NONE) {

		status = tmpPacket[ERR_BYTE];//set as the error byte of the status packet
		return tmpPacket[5];//return the value read, if there is one
	}

	return status;//return error if there is one
}

int Port::ReadWord(int id, int reg) {
	int status;
	unsigned char tmpID = (unsigned char)id;
	unsigned char tmpRegister = (unsigned char)reg;
	std::vector<unsigned char> tmpPacket;

	if(Packet_IsDebug()) {
		std::cerr << "Read word: ";
	}

	/*Generate the packet*/
	tmpPacket = packet.mk_ReadWord(tmpID, tmpRegister);

	if(Packet_IsDebug()) {
		packet.viewPacket();
	}

	/*make sure that the number of bytes written is the same as the number of bytes in the packet*/
	if (writePacket(tmpPacket) != tmpPacket.size()) {

		return ERROR;//Error: bad length
	}

	/*Make sure that there are no errors*/
	if ((status = getResponse(&tmpPacket)) == ERR_NONE) {

		status = tmpPacket[4];//set as the error byte of the status packet
		return tmpPacket[5] + (tmpPacket[6] << 8);//return the value read, if there is one
	}

	return status;
}


int Port::WriteByte(int id, int reg, int val) {
	int status;
	unsigned char tmpID = (unsigned char)id;
	unsigned char tmpRegister = (unsigned char)reg;
	unsigned int tmpVal = (unsigned int)val;
	std::vector<unsigned char> tmpPacket;

	if(Packet_IsDebug()) {
		std::cerr << "Write byte: " << std::endl;
	}

	/*Generate the packet*/
	tmpPacket = packet.mk_WriteByte(tmpID, tmpRegister, tmpVal);

	if(Packet_IsDebug()) {
		packet.viewPacket();
	}

	/*make sure that the number of bytes written is the same as the number of bytes in the packet*/
	if (writePacket(tmpPacket) != tmpPacket.size()) {

		return ERROR;//Error: bad length
	}
	/*Make sure that there are no errors*/
	if ((status = getResponse(&tmpPacket)) == ERR_NONE) {

		status = tmpPacket[4];
		return ERR_NONE;
	}

	return status;
}

int Port::WriteWord(int id, int reg, int val) {
	int status;
	unsigned char tmpID = (unsigned char)id;
	unsigned char tmpRegister = (unsigned char)reg;
	unsigned int tmpVal = (unsigned int)val;
	std::vector<unsigned char> tmpPacket;

	if(Packet_IsDebug()) {
		std::cerr << "Write byte: ";
	}
	/*Generate the packet*/
	tmpPacket = packet.mk_WriteWord(tmpID, tmpRegister, tmpVal);

	if(Packet_IsDebug()) {
		packet.viewPacket();
	}

	/*make sure that the number of bytes written is the same as the number of bytes in the packet*/
	if (writePacket(tmpPacket) != tmpPacket.size()) {

		return ERROR;//Error: bad length
	}

	/*Make sure that there are no errors*/
	if ((status = getResponse(&tmpPacket)) == ERR_NONE) {

		status = tmpPacket[4];
		return ERR_NONE;
	}

	return status;
}

int Port::RegWrite(int iID, int iRegister, std::vector<int> iValue) {
	int status;
	unsigned char ucTmpID = (unsigned char)iID;
	unsigned char ucTmpRegister = (unsigned char)iRegister;
	std::vector<unsigned char> tmpPacket;


	if(Packet_IsDebug()) {
		std::cerr << "Reg write: ";
	}
	/*Generate the packet*/
	tmpPacket = packet.mk_RegWrite(ucTmpID, ucTmpRegister, iValue);

	if(Packet_IsDebug()) {
		packet.viewPacket();
	}

	/*make sure that the number of bytes written is the same as the number of bytes in the packet*/
	if (writePacket(tmpPacket) != tmpPacket.size()) {

		return ERROR;//Error: bad length
	}

	/*Make sure that there are no errors*/
	if ((status = getResponse(&tmpPacket)) == ERR_NONE) {

		status = tmpPacket[4];
		return ERR_NONE;
	}

	return status;
}

int Port::Action() {
	int status;
	std::vector<unsigned char> tmpPacket;

	if(Packet_IsDebug()) {
		std::cerr << "Action: ";
	}

	/*Generate the packet*/
	tmpPacket = packet.mk_Action();

	if(Packet_IsDebug()) {
		packet.viewPacket();
	}

	/*make sure that the number of bytes written is the same as the number of bytes in the packet*/
	if (writePacket(tmpPacket) != tmpPacket.size()) {

		return ERROR;//Error: bad length
	}


	return status;

}

void Port::setPositionValue(int iID, int iValue) {

	if((WriteWord(iID, 30, iValue)) == 0) {//write to the goal position register
		if(Port_IsDebug()) {
			std::cerr << "Successfully written goal position value '" << iValue << "' to ID '" << iID << "'" << std::endl;
		}
		else {
			if(Port_IsDebug()) {
				std::cerr << "Error writing goal position value '" << iValue << "' to ID '" << iID << "'" << std::endl;
			}
		}
	}
}

void Port::enableTorque(int iID) {

	if((WriteByte(iID, 24, 1)) == 0) {
		if(Port_IsDebug()) {
			std::cerr << "Torque enabled DXL" << iID << std::endl;
		}
		else {
			if(Port_IsDebug()) {
				std::cerr << "Error enabling torque DXL" << iID << std::endl;
			}
		}
	}
}

void Port::disableTorque(int iID) {

	if((WriteByte(iID, 24, 0)) == 0) {
		if(Port_IsDebug()) {
			std::cerr << "Torque disabled DXL" << iID << std::endl;
		}
		else {
			if(Port_IsDebug()) {
				std::cerr << "Error disabling torque DXL" << iID << std::endl;
			}
		}
	}
}




#ifdef _cplusplus
}
#endif








