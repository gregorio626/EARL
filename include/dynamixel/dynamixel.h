/*
*
*   Author: EARL Technologies
*
*   Created: May 01, 2016
*
*/


#ifndef _DYNAMIXEL_H_
#define _DYNAMIXEL_H_

#include <iostream>
#include <string>
#include <vector>
#include <map>

namespace EARL {
namespace Dynamixel {

class Interface {
public:
	typedef enum {
	    ERR_NONE                = 0,//There is no error
	    COMMAND_UNSUPPORTED     = -1,//The command is unsupported
	    PORT_IS_DISCONNECTED    = -2,//The port is disconnected
	    PACKET_LENGTH_ERR       = -3,//The packet is too short
	    PACKET_CHECKSUM_ERR     = -4,//Bad incoming checksum
	    PACKET_NULL             = -5,//The packet is empty/NULL
	    ERR_BAD_DEVICE_NAME     = -6,//There is no such device type(see com::open(...))
	    DEVICE_OPEN_ERROR       = -7,//Device open error(for usb2ax code)
	    PORT_IS_CLOSED          = -8,//The communcation port is not open
	    ERR_UNKNOWN             = -9 //Unknown error
	 }Status;

public:
	
	static Interface* createInterface(int baud);

	Interface(int baudrate) : currentlyOpen(false){
		baud = baudrate;
	}

	virtual Status openPort(const char * field) = 0;

	virtual void closePort() = 0;

	int baud;

protected:

	bool currentlyOpen;

};

class USB2AX : public Interface {
public:
	USB2AX(int baudrate) : Interface(baudrate), fid(-1) {
	}
	Status openPort(const char * field);//must execute 'sudo usermod -a -G dialout $USERNAME' before this will work, if you have not already added yourself to the dialout group.
	void closePort();
protected:
	int	fid;
};

class Handler{
private:

	Interface* interface;

	bool isBusy;

public:

	Handler();

	~Handler();

	Interface::Status openInterface(const char * field, int baudrate);


protected:

	int baud;
};

class Packet {

private:



	void beginPacket(int iID);//the head/header of the packet
	void endPacket();//the foot of the packet

	void setPacketID(int iID);
	void setPacketInstruction(unsigned char ucInstruction);
	void setPacketAddress(int iAddress);
	void setPacketLowHighByte(int iWord);

	int getPacketLowByte(int iWord);
	int getPacketHighByte(int iWord);


	

public:
	std::vector<unsigned char> packet;//the data packet to be sent/received

	Packet(){
	}
	std::vector<unsigned char>& ping(int iID);
	std::vector<unsigned char>& readByte(int iID, int iAddress);//length read = 1
	std::vector<unsigned char>& readWord(int iID, int iAddress);//length read = 2
	std::vector<unsigned char>& writeByte(int iID, int iAddress, int ivalue);//length written = 1
	std::vector<unsigned char>& writeWord(int iID, int iAddress, int ivalue);//length written = 2
	std::vector<unsigned char>& reset(int iID);

	void write(int iID, int iAddress, int ivalue);
	void viewPacket();

protected:
	bool m_bBusInUse;
};
struct DataRange {

	unsigned short reg;
	unsigned short length;

	DataRange(unsigned short reg = 0, unsigned short length = 0) : reg(reg), length(length){
	}
};
unsigned char getChecksum(const unsigned char * pkucBuffer);


}//namespace Dynamixel
}//namespace EARL

#endif//#ifndef _DYNAMIXEL_H_
