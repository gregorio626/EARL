/*
 * packet.cpp
 *
 *  Created on: May 25, 2016
 *      Author: gregorio626
 */

#include "dynamixel.h"
#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <sys/time.h>
#include <string.h>
#include <vector>

using namespace EARL;
using namespace Dynamixel;

#define BROADCAST_ID        (0xFE)

#define PKT_HEAD_0				(0)
#define PKT_HEAD_1				(1)
#define PKT_ID					(2)
#define PKT_LENGTH				(3)
#define PKT_INSTRUCTION			(4)
#define PKT_ERRBIT				(4)
#define PKT_PARAMETER			(5)
#define PKT_DEFAULT_BAUDNUMBER	(1)

#define MIN_SIZE				(6)

unsigned char Packet::getChecksum(const unsigned char * buffer) {
	unsigned char checksum = 0;
	int i;
	for (i = 2; i < (3 + buffer[PKT_LENGTH]); i++)
		checksum += buffer[i];
	return ~(checksum & 0xFF);
}

/*Create the packet header, set the id, and allocate for the length*/
void Packet::beginTxPacket(unsigned char id) {
	InstructionPacket.clear();

	InstructionPacket.push_back(0xff);
	InstructionPacket.push_back(0xff);
	setTxPacketID(id);
	InstructionPacket.push_back(0x00);//allocate for the length
}

void Packet::beginSyncWritePacket(unsigned char ucStartRegister, unsigned char ucNumBytes) {

	InstructionPacket.clear();

	InstructionPacket.push_back(0xFF);//header 0
	InstructionPacket.push_back(0xFF);//header 1
	setTxPacketID(BROADCAST_ID);//ID(Broadcast ID(0xFE)) 2
	InstructionPacket.push_back(0x00);//Allocate for length 3
	setTxPacketInstruction(SYNC_WRITE);//Instruction(0x83) 4
	setTxPacketParameter(ucStartRegister);//start register 5
	setTxPacketParameter(ucNumBytes);//number of bytes being written 6

}

void Packet::endTxPacket() {
	InstructionPacket[PKT_LENGTH] = InstructionPacket.size() - 3;//subtract three because of header 1, header 2, and length
	InstructionPacket.push_back(getChecksum(InstructionPacket.data()));
}

/*Sets the dynamixel ID*/
void Packet::setTxPacketID(unsigned char id) {
	InstructionPacket.push_back(id);
}

/*Set the Instruction*/
void Packet::setTxPacketInstruction(unsigned char instruction) {
	InstructionPacket.push_back(instruction);
}

void Packet::setTxPacketParameter(unsigned char val) {
	InstructionPacket.push_back(val);
}

int Packet::getLowByte(int iWord) {
	unsigned short temp;

	temp = iWord & 0xff;
	return (int)temp;
}

int Packet::getHighByte(int iWord) {
	unsigned short temp;

	temp = iWord & 0xff00;
	temp = temp >> 8;
	return (int)temp;
}

/*PING*/
////////////////////////////////////////////////////////////
const std::vector<unsigned char>& Packet::mk_Ping(unsigned char& id)
{
	beginTxPacket(id);//header, id, length

	setTxPacketInstruction(PING);//instruction

	endTxPacket();//footer(checksum)ERR_PACKET_EMPTY;
	return InstructionPacket;

}

/*READ*/
/////////////////////////////////////////////////////InstructionPacket.push_back(////////////////////////////////
const std::vector<unsigned char>& Packet::mk_ReadByte(unsigned char& id, unsigned char& reg) {

	beginTxPacket(id);//header, id, length

	setTxPacketInstruction(READ_DATA);//instruction

	InstructionPacket.push_back(reg);//register
	InstructionPacket.push_back(0x01);//read # of bytes

	endTxPacket();//footer(checksum)
	return InstructionPacket;
}

const std::vector<unsigned char>& Packet::mk_ReadWord(unsigned char& id, unsigned char& reg) {

	beginTxPacket(id);//header, id, length

	setTxPacketInstruction(READ_DATA);//instruction

	InstructionPacket.push_back(reg);//register
	InstructionPacket.push_back(0x02);//read # of bytes

	endTxPacket();//footer(checksum)
	return InstructionPacket;
}

/*WRITE*/
///////////////////////////////////////////////////////////////////////////////////////////////////////////
const std::vector<unsigned char>& Packet::mk_WriteByte(unsigned char& id, unsigned char& reg, unsigned int& val) {

	beginTxPacket(id);//header, id, length

	setTxPacketInstruction(WRITE_DATA);//instruction

	InstructionPacket.push_back(reg);//register
	InstructionPacket.push_back((unsigned char)val);//value

	endTxPacket();//footer(checksum)
	return InstructionPacket;
}

const std::vector<unsigned char>& Packet::mk_WriteWord(unsigned char& id, unsigned char& reg, unsigned int& val) {

	beginTxPacket(id);//header, ID, length

	setTxPacketInstruction(WRITE_DATA);//instruction

	InstructionPacket.push_back(reg);//register

	InstructionPacket.push_back((unsigned char)getLowByte(val));//value(Low byte)
	InstructionPacket.push_back((unsigned char)getHighByte(val));//value(High byte)

	endTxPacket();//checksum
	return InstructionPacket;
}

const std::vector<unsigned char>& Packet::mk_RegWrite(unsigned char& id, unsigned char& regStart, std::vector<int> iValue) {

	beginTxPacket(id);//header, ID, length

	setTxPacketInstruction(REG_WRITE);//Instruction

	InstructionPacket.push_back(regStart);//register
	for(unsigned int ii = 0; ii < iValue.size(); ii++) {

		if(iValue[ii] >= 0 && iValue[ii] <= 255) {

			/*push back as 1 byte*/
			InstructionPacket.push_back((unsigned char)iValue[ii]);//value
			InstructionPacket.push_back(0x00);

		} else if(iValue[ii] > 155 && iValue[ii] <= 1024) {

			/*push back as 2(high and low) bytes*/
			InstructionPacket.push_back(getLowByte(iValue[ii]));//Low byte
			InstructionPacket.push_back(getHighByte(iValue[ii]));//High byte
		}
	}

	endTxPacket();

	return InstructionPacket;
}

const std::vector<unsigned char>& Packet::mk_Action() {

	beginTxPacket(0xFE);//ID => 0xFE
	setTxPacketInstruction(ACTION);
	endTxPacket();

	return InstructionPacket;
}

const std::vector<unsigned char>& Packet::mk_SyncWrite(unsigned char & ucStartRegister, unsigned char& ucNumBytes, std::vector<unsigned char> ID, std::vector<unsigned char> Value) {

	/*Verify that neither vector is empty*/
	if(ID.size() == 0 || Value.size() == 0) {
		if(m_Debug == 1 || m_Debug == 3) {
			std::cerr << "Error: EARL::Dynamixel::Packet::mk_SyncWrite(...)----> Parameter(s) empty." << std::endl;
		}
		InstructionPacket.clear();
		return InstructionPacket;
	}
	/*Verify that we are not attempting to write to Registers that do not exist*/
	if((ucStartRegister + ucNumBytes) > AX_MAX_REG) {
		if(m_Debug == 1 || m_Debug == 3) {
			std::cerr << "Error: EARL::Dynamixel::Packet::mk_SyncWrite(...)----> Too many bytes being written." << std::endl;
		}
		InstructionPacket.clear();
		return InstructionPacket;
	}

	/*Verify that there are enough values for each ID, and visa versa*/
	if((ID.size() * ucNumBytes) != Value.size()) {
		if(m_Debug == 1 || m_Debug == 3) {
			std::cerr << "Error: EARL::Dynamixel::Packet::mk_SyncWrite(...)----> (ID.size() * ucNumBytes) != Value.size()." << std::endl;
		}
		InstructionPacket.clear();
		return InstructionPacket;
	}

	beginSyncWritePacket(ucStartRegister, ucNumBytes);//0-6

	do {
		InstructionPacket.push_back(ID.back());
		for(int nn = 0; nn < ucNumBytes; nn++) {
			InstructionPacket.push_back(Value.at(nn));
		}
		ID.pop_back();
		Value.erase(Value.begin(), Value.begin() + ucNumBytes);
	}while(ID.size() > 0);

	endTxPacket();

	return InstructionPacket;
}


bool Packet::checkPacket(const std::vector<unsigned char>& data) {
	if(data.size() < MIN_SIZE) {
		if(m_Debug == 1 || m_Debug == 3) {
			std::cerr << "Error: EARL::Dynamixel::Packet::checkPacket(...)---->data.size() < minimum size of valid packet(size of bad packet: " << data.size() << ")." << std::endl;
		}
		return false;//ERROR

	} else if(data[0] != 0xFF || data[1] != 0xFF) {
		if(m_Debug == 1 || m_Debug == 3) {
					std::cerr << "Error: EARL::Dynamixel::Packet::checkPacket(...)---->bad header." << std::endl;
		}
		return false;//ERROR

	} else if(data.size() != (unsigned int)(data[3] + 4)) {
		if(m_Debug == 1 || m_Debug == 3) {
					std::cerr << "Error: EARL::Dynamixel::Packet::checkPacket(...)---->data.size() = " << (int) data.size() << " != (data[3] + 4) = " << (int) (data[3] + 4) << std::endl;
		}
		return false;//ERROR
	}
	return getChecksum(data.data()) == data[3 + data[3]];
}

