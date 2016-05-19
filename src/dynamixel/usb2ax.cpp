/*
*
*	Author: Greg Roberts
*
*	Created: May 10, 2016
*
*/

#include <stdio.h>    /* Standard input/output definitions */
 #include <string.h>  /* String function definitions */
 #include <unistd.h>  /* UNIX standard function definitions */
 #include <fcntl.h>   /* File control definitions */
 #include <errno.h>   /* Error number definitions */
 #include <termios.h> /* POSIX terminal control definitions */
#include <iostream>   /* Standard input/output for c++ */

#include "/home/gregorio626/EARL/include/dynamixel/dxl.h"

#ifdef _cplusplus
extern "C" {
#endif

using namespace EARL;
using namespace Dynamixel;

bool USB2AX::openPort(const char * portName) {


	struct termios newtio;

	memset(&newtio, 0, sizeof(newtio));

	closePort();

	if((fd = ::open(portName, O_RDWR | O_NOCTTY | O_NONBLOCK)) < 0) {

		std::cerr << "Error opening port: \"" << portName << "\"" << std::endl;
		closePort();
		return 0;
	}

	newtio.c_cflag		= B1000000|CS8|CLOCAL|CREAD;
	newtio.c_iflag		= IGNPAR;
	newtio.c_oflag		= 0;
	newtio.c_lflag		= 0;
	newtio.c_cc[VTIME]	= 0;
	newtio.c_cc[VMIN]	= 0;

	tcflush(fd, TCIFLUSH);
	tcsetattr(fd, TCSANOW, &newtio);

	if(fd == -1) {
		std::cerr << "Device open error: \"" << portName << "\"" << std::endl;
		closePort();
		return 0;
	}

	closePort();
	
	memset(&newtio, 0, sizeof(newtio));
	closePort();
	
	if((fd = ::open(portName, O_RDWR|O_NOCTTY|O_NONBLOCK)) < 0) {
		std::cerr << "Device open error: \"" << portName << "\"" << std::endl;
		closePort();
		return 0;
	}

	newtio.c_cflag		= B1000000|CS8|CLOCAL|CREAD;
	newtio.c_iflag		= IGNPAR;
	newtio.c_oflag		= 0;
	newtio.c_lflag		= 0;
	newtio.c_cc[VTIME]	= 0;	// time-out 값 (TIME * 0.1초) 0 : disable
	newtio.c_cc[VMIN]	= 0;	// MIN 은 read 가 return 되기 위한 최소 문자 개수

	tcflush(fd, TCIFLUSH);//flush the port

    //set attributes to port
    if(tcsetattr(fd, TCSANOW, &newtio) < 0)
    {
        std::cout << "Error setting attributes for \"" << portName << "\"" << std::endl;
        return 0;
    }	
    
	std::cout << "EARL::Dynamixel::UBS2AX::openPort(\"" << portName << "\"):                SUCCESS" << std::endl;

	this->portName = portName;
	bCurrentlyOpen = true;
	this->fd = fd;
	return 1;
}

void USB2AX::closePort() {
	close(fd);
	if(bCurrentlyOpen == true)
		bCurrentlyOpen = false;

}

/*int USB2AX::writeSerialInterface(std::vector<unsigned char> packet) {
	if(isOpen() == false) {
		std::cerr << "Error writing data to the UART transmit: " << -1 << std::endl;
		return -1;//returns the value of SerialInterface::ERR_DISCONNECTED
	}

	int _result = ::write(fd, packet.data(), packet.size());
	tcdrain(fd);
	std::cout << "Successfully wrote " << _result << " bytes." << std::endl;
	return _result;
}

pPacketBuffer is a vector that will store the current data packet being read
lengthMax represents the number of bytes being read from the buffer

int USB2AX::readSerialInterface(std::vector<unsigned char> * pPacketBuffer, size_t lengthMAX) {
	if(isOpen() == false) {
		std::cerr << "Error reading data from the UART receive: " << -1 << std::endl;
		return -1;//returns the value of SerialInterface::ERR_DISCONNECTED
	}

	int numberSent = ERR_PACKET_TOO_SHORT;-3

	If the buffer is empty(if there are no packets waiting to be read...)
	if(pPacketBuffer == NULL) {
		return ERR_PACKET_EMPTY;-5
	}

	usleep(10000);
	unsigned char ucTempBuffer[lengthMAX];
	if(!bCurrentlyOpen) {
		return ERR_DISCONNECTED;the port is not open
	}
	else if(bCurrentlyOpen) {
		numberSent = ::read(fd, ucTempBuffer, lengthMAX);//read data from the file descriptor
		for(int ii = 0; ii < numberSent; ii++) {
			pPacketBuffer->push_back(ucTempBuffer[ii]);
		}
	}

	return numberSent;
}*/

#ifdef _cplusplus
}
#endif
