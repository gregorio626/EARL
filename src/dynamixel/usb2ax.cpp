/*
*
*   Author: EARL Technologies
*
*   Created: April 18, 2016
*
*/

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <linux/serial.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <errno.h>
#include "dynamixel.h"

#ifdef _cplusplus
extern "C" {
#endif

using namespace EARL;
using namespace Dynamixel;


	Interface::StatusError USB2AX::openPort(const char * devPort)
	{
		char Port[20];
		strcpy(Port, devPort);
		float baudrate = baud;

		struct termios newtio;

		memset(&newtio, 0, sizeof(newtio));
		closePort();//make sure it is not open

		if((_fd = ::open(devPort, O_RDWR|O_NOCTTY|O_NONBLOCK)) < 0)
		{
			std::cerr << "device open error: " << devPort << std::endl;
			closePort();
			return ERR_DEVICE_OPEN;
		}

		newtio.c_cflag 		= B1000000|CS8|CLOCAL|CREAD;
		newtio.c_iflag 		= IGNPAR;
		newtio.c_oflag 		= 0;
		newtio.c_lflag		= 0;
		newtio.c_cc[VTIME] 	= 0;
		newtio.c_cc[VMIN] 	= 0;

		tcflush(_fd, TCIFLUSH);
		tcsetattr(_fd, TCSANOW, &newtio);

		if(_fd == -1)
			return ERR_UNKNOWN;

		closePort();

		byteTransferTime = (float)((1000.0f / baudrate) * 12.0f);

		memset(&newtio, 0, sizeof(newtio));
		closePort();

		if((_fd = open(devPort, O_RDWR|O_NOCTTY|O_NONBLOCK)) < 0)
		{
			std::cerr << "device open error: " << devPort << std::endl;
			closePort();
			return ERR_DEVICE_OPEN;
		}

		newtio.c_cflag 		= B1000000|CS8|CLOCAL|CREAD;
		newtio.c_iflag 		= IGNPAR;
		newtio.c_oflag 		= 0;
		newtio.c_lflag		= 0;
		newtio.c_cc[VTIME] 	= 0;	// time-out 값 (TIME * 0.1초) 0 : disable
		newtio.c_cc[VMIN] 	= 0;	// MIN 은 read 가 return 되기 위한 최소 문자 개수

		tcflush(_fd, TCIFLUSH);
		tcsetattr(_fd, TCSANOW, &newtio);


		return ERR_NONE;
	}

	int USB2AX::closePort()
	{
		if(_fd != -1)
		{
			close(_fd);
			_fd = -1;
		}
		return ERR_NONE;
	}

	




#ifdef _cplusplus
}
#endif
