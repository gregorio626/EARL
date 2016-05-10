/*
*
*   Author: EARL Technologies
*
*   Created: May 03, 2016
*
*/

#include "/home/gregorio626/EARL/include/dynamixel/dynamixel.h"

#ifdef _cplusplus
extern "C" {
#endif

using namespace EARL;
using namespace Dynamixel;

#define ID					  (2)
#define LENGTH				  (3)
#define INSTRUCTION			  (4)
#define ERRBIT				  (4)
#define PARAMETER			  (5)

#define INSTRUCTION_PING      (0x01) //No execution. It is used when controller is ready to receive Status Packet
#define INSTRUCTION_READ      (0x02) //This command reads data from Dynamixel
#define INSTRUCTION_WRITE     (0x03) //This command writes data to Dynamixel
#define INSTRUCTION_REG_WRITE (0x04) //It is similar to WRITE, but it remains in the standby state without being executed until the ACTION command arrives.
#define INSTRUCTION_ACTION    (0x05) //This command initiates motions registered with REG WRITE
#define INSTRUCTION_RESET     (0x06) //This command restores the state of Dynamixel to the factory default setting.

#define MAX_TXPARAM           (150)
#define MAX_RXPARAM           (225)

/*Calculate the checksum of the packet*/
unsigned char EARL::Dynamixel::getChecksum(const unsigned char * pkucBuffer) {
	unsigned char ucChecksum = 0;
	int ii;
	for(ii = 2; ii < (3 + pkucBuffer[3]); ii++)
		ucChecksum += pkucBuffer[ii];
	return ~(ucChecksum & 0xFF);

}

/*Create the packet header*/
void Packet::beginPacket(int iID)//create the header
{
	packet.clear();//clear content

	packet.push_back(0xFF);//header
	packet.push_back(0xFF);//header
	
	setPacketID(iID);
	
	packet.push_back(0x00);//Length
}

/*Create the packet footer*/
void Packet::endPacket() {
	
	packet[3] = packet.size() - 3;
	packet.push_back(getChecksum(packet.data()));
}

/*Set the motor ID of the packet*/
void Packet::setPacketID(int iID) { 
	
	packet.push_back((unsigned char)(iID));
}

/*Set the instruction value of the packet*/
void Packet::setPacketInstruction(unsigned char ucInstruction){
	
	packet.push_back(ucInstruction);
}

/*Set the address value of the packet*/
void Packet::setPacketAddress(int iAddress) {

	packet.push_back((unsigned char)(iAddress));
}

/*Set the high and low byte values of the packet*/
void Packet::setPacketLowHighByte(int iWord){

	unsigned char ucLow = (unsigned char)(getPacketLowByte(iWord));
	unsigned char ucHigh = (unsigned char)(getPacketHighByte(iWord));


	packet.push_back(ucHigh);
	packet.push_back(ucLow);
}

/*Get the low byte for the packet*/
int Packet::getPacketLowByte(int iWord){
	
	unsigned short usiTmp;
	usiTmp = iWord & 0xff;

	return (int)usiTmp;
}

/*Get the high byte for the packet*/
int Packet::getPacketHighByte(int iWord){

	unsigned short usiTmp;
	usiTmp  = iWord & 0xff00;
	usiTmp = usiTmp >> 8;

	return (int)usiTmp;
}

/*Packet to ping a motor*/
std::vector<unsigned char>& Packet::ping(int iID) {
	
	beginPacket(iID);//create header
	setPacketInstruction(INSTRUCTION_PING);
	endPacket();//create footer

	return packet;
}

/*Packet for reading a byte*/
std::vector<unsigned char>& Packet::readByte(int iID, int iAddress){
	
	beginPacket(iID);//create the header
	setPacketInstruction(INSTRUCTION_READ);
	setPacketAddress(iAddress);
	packet.push_back(1);
	endPacket();//create the footer

	return packet;
}

/*Packet for reading a word*/
std::vector<unsigned char>& Packet::readWord(int iID, int iAddress){
	
	beginPacket(iID);//create the header
	setPacketInstruction(INSTRUCTION_READ);
	setPacketAddress(iAddress);
	packet.push_back(2);
	endPacket();//create the footer

	return packet;	
}

/*Packet for writing a byte*/
std::vector<unsigned char>& Packet::writeByte(int iID,int iAddress, int iValue){
	
	beginPacket(iID);//create the header
	setPacketInstruction(INSTRUCTION_WRITE);
	setPacketAddress(iAddress);
	packet.push_back((unsigned char)(iValue));
	endPacket();//create the footer

	return packet;
}

/*Packet for writing a word*/
std::vector<unsigned char>& Packet::writeWord(int iID,int iAddress, int iValue){
	
	beginPacket(iID);//create the header
	setPacketInstruction(INSTRUCTION_WRITE);
	setPacketAddress(iAddress);
	setPacketLowHighByte(iValue);
	endPacket();//create the footer

	return packet;
}
/*Packet for resetting a motor*/
std::vector<unsigned char>& Packet::reset(int iID) {

	beginPacket(iID);//create the header
	setPacketInstruction(INSTRUCTION_RESET);
	endPacket();//create the footer

	return packet;
}

/*Packet for writing to a motor*/
void Packet::write(int iID, int iAddress, int iValue){
	
	if(iAddress > 0){
		if(iValue >= 0 && iValue <= 255)
			writeByte(iID, iAddress, iValue);
		else if(iValue >= 255 && iValue <= 1024)
			writeWord(iID, iAddress, iValue);
		else
			std::cerr << "Error: invalid value in 'EARL::Dynamxel::Packet::Write(...)'" << std::endl;
	}
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
