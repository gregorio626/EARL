/*
*
*   Author: EARL Technologies
*
*   Created: May 01, 2016
*
*/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <linux/serial.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <iostream>
#include <errno.h>
#include "dynamixel.h"

#ifdef _cplusplus
extern "C" {
#endif

using namespace EARL;
using namespace Dynamixel;

Interface::Status USB2AX::openPort(const char * field)
{
	struct termios newtio;

	memset(&newtio, 0, sizeof(newtio));
	closePort();

	if((fid = ::open(field, O_RDWR|O_NOCTTY|O_NONBLOCK)) < 0) {
		fprintf(stderr, "device open error: %s\n", field);
		closePort();
		return DEVICE_OPEN_ERROR;
	}

	newtio.c_cflag		= B1000000|CS8|CLOCAL|CREAD;
	newtio.c_iflag		= IGNPAR;
	newtio.c_oflag		= 0;
	newtio.c_lflag		= 0;
	newtio.c_cc[VTIME]	= 0;	// time-out 값 (TIME * 0.1초) 0 : disable
	newtio.c_cc[VMIN]	= 0;	// MIN 은 read 가 return 되기 위한 최소 문자 개수

	tcflush(fid, TCIFLUSH);
	tcsetattr(fid, TCSANOW, &newtio);
	
	if(fid == -1)
		return DEVICE_OPEN_ERROR;
        
	closePort();
	
	memset(&newtio, 0, sizeof(newtio));
	closePort();
	
	if((fid = open(field, O_RDWR|O_NOCTTY|O_NONBLOCK)) < 0) {
		fprintf(stderr, "device open error: %s\n", field);
		closePort();
		return DEVICE_OPEN_ERROR;
	}

	newtio.c_cflag		= B1000000|CS8|CLOCAL|CREAD;
	newtio.c_iflag		= IGNPAR;
	newtio.c_oflag		= 0;
	newtio.c_lflag		= 0;
	newtio.c_cc[VTIME]	= 0;	// time-out 값 (TIME * 0.1초) 0 : disable
	newtio.c_cc[VMIN]	= 0;	// MIN 은 read 가 return 되기 위한 최소 문자 개수

	tcflush(fid, TCIFLUSH);
	tcsetattr(fid, TCSANOW, &newtio);
	
	std::cout << "EARL::Dynamixel::UBS2AX::openPort(\"" << field << "\"):                NONE" << std::endl;
	return ERR_NONE;

}

void USB2AX::closePort()
{
	if(fid != -1)
		close(fid);
	fid = -1;
}



#ifdef _cplusplus
}
#endif

