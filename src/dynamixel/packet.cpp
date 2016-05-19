/*
*
*   Author: EARL Technologies
*
*   Created: May 03, 2016
*
*/


#include "dxl.h"
#include <vector>
#include <iostream>



#define ID					  (2)
#define LENGTH				  (3)
#define INSTRUCTION			  (4)
#define ERRBIT				  (4)
#define PARAMETER			  (5)


#define MAX_TXPARAM           (150)
#define MAX_RXPARAM           (225)

using namespace EARL;
using namespace Dynamixel;




#ifdef _cplusplus
extern "C" {
#endif

/*Calculate the checksum of the packet*/
unsigned char EARL::Dynamixel::getChecksum(const unsigned char * pkucBuffer) {
	unsigned char ucChecksum = 0;
	int ii;
	for(ii = 2; ii < (3 + pkucBuffer[3]); ii++)
		ucChecksum += pkucBuffer[ii];
	return ~(ucChecksum & 0xFF);

}

/*Create the packet header*/
void Packet::beginTxPacket(int iID)//create the header; 'dataSize' is either 
{
	packet.clear();//Clear content

	packet.push_back(0xFF);//header
	packet.push_back(0xFF);//header
	setTxPacketID(iID);//Set the dynamixel ID
	packet.push_back(0x00);//Length
}

/*Create the packet footer*/
void Packet::endTxPacket() {
	
	packet[LENGTH] = packet.size() - 3;
	packet.push_back(getChecksum(packet.data()));
}

/*Set the motor ID of the packet*/
void Packet::setTxPacketID(int iID) { 
	packet.push_back((unsigned char)(iID));//sets the 3rd container to the ID in hexadecimal.

}

/*Set the instruction value of the packet*/
void Packet::setTxPacketInstruction(unsigned char ucInstruction){
	if(packet.back() != 0x00){
		std::cout << "Error setting the packet instruction." << std::endl;
		return;
	}
	packet.push_back(ucInstruction);

}

/*Set the address value of the packet*/
void Packet::setTxPacketReg(int iRegister) {

	packet.push_back(iRegister);

}

/*Get the low byte for the packet*/
unsigned char Packet::getLowByte(int iWord){
	
	unsigned short usiTmp;
	usiTmp = iWord & 0xff;

	return (unsigned char)usiTmp;
}

/*Get the high byte for the packet*/
unsigned char Packet::getHighByte(int iWord){

	unsigned short usiTmp;
	usiTmp  = iWord & 0xff00;
	usiTmp = usiTmp >> 8;

	return (unsigned char)usiTmp;
}

void Packet::setTxPacketValue(int iValue) {

	if(iValue >= 0 && iValue <= 255){
		packet.push_back((unsigned char)(getLowByte(iValue));
	}else if(iValue >=0 && iValue <= 1024) {
		packet.push_back((unsigned char)(getLowByte(iValue)));
		packet.push_back((unsigned char)(getHighByte(iValue)));
	}

}
/*
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
*/

/*Packet to ping a motor*/
std::vector<unsigned char>& Packet::ping(int iID) {
	
	beginTxPacket(iID);//create header
	setTxPacketInstruction(INSTRUCTION_PING);
	endTxPacket();//create footer

	return packet;
}

/*Packet for reading a byte*/
std::vector<unsigned char>& Packet::readByte(int iID, int iReg){
	
	beginTxPacket(iID);//create the header
	setTxPacketInstruction(INSTRUCTION_READ);
	setTxPacketReg(iReg);
	packet.push_back(1);
	endTxPacket();//create the footer & set the checksum value

	return packet;
}

/*Packet for reading a word*/
std::vector<unsigned char>& Packet::readWord(int iID, int iReg){
	
	beginTxPacket(iID);//create the header
	setTxPacketInstruction(INSTRUCTION_READ);
	setTxPacketReg(iReg);
	packet.push_back(2);
	endTxPacket();//create the footer

	return packet;	
}

/*Packet for writing a byte*/
std::vector<unsigned char>& Packet::writeByte(int iID,int iRegister, int iValue){
	
	beginTxPacket(iID);//create the header
	setTxPacketInstruction(INSTRUCTION_WRITE);
	setTxPacketReg(iRegister);//The register that we will be writing a value to
	setTxPacketValue(iValue);//sets the container(s) for the value assocated with the specified register
	endTxPacket();//create the footer

	return packet;
}

/*Packet for writing a word*/
std::vector<unsigned char>& Packet::writeWord(int iID,int iRegister, int iValue){
	
	beginTxPacket(iID);//create the header
	setTxPacketInstruction(INSTRUCTION_WRITE);
	setTxPacketReg(iRegister);//The register that we will be writing a value to
	setTxPacketValue(iValue);//sets the container(s) for the value assocated with the specified register
	endTxPacket();//create the footer

	return packet;
}
/*Packet for resetting a motor*/
std::vector<unsigned char>& Packet::reset(int iID) {

	beginTxPacket(iID);//create the header
	setTxPacketInstruction(INSTRUCTION_RESET);
	endTxPacket();//create the footer

	return packet;
}


void Packet::viewPacket() {
	std::cout << "Current packet: ";
	for(unsigned int ii = 0; ii < packet.size(); ii++) {
		std::cout << (int)packet[ii] << " ";
	}
	std::cout << std::endl;
}


/*Instruction packet component:
			
			0xFF 0xFF :the beginnig of the packet
			0x01      :ID 1
			0x00      :length(in this case = 7)
			0x02      :instruction(in this case = 2 = READ)
			0x00 0x00 :begin from address 0
			0x00 0x02 :read 2 bytes


*/
#ifdef _cplusplus
}
#endif
