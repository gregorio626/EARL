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
typedef enum{
	MODEL_AX12_W = 44,
	MODEL_AX12 = 12,
	MODEL_AX18_A = 18
}Model;


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

	std::map<unsigned char, Motor*> dynamixelList;

	Interface* interface;

	bool isBusy;

public:

	Handler();

	~Handler();

	Interface::Status openInterface(const char * field, int baudrate);

	void forceAddDynamixel(unsigned int ID, Model model = MODEL_AX18_A);

	std::map<unsigned char, Motor*> getDynamixels();

protected:

	int baud;
};