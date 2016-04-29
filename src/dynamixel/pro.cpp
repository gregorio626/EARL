/*
*
*   Author: EARL Technologies
*
*   Created: April 18, 2016
*
*/
#include "dynamixel.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fcntl.h>
#include <termios.h>//terminal input/output
#include <linux/serial.h>//serial
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <string.h>//used to work with strings
#include <errno.h>

#ifdef _cplusplus
extern "C" {
#endif

	using namespace EARL;
	using namespace Dynamixel;

	Interface::StatusError Pro::openPort(const char * devPort)
	{
		struct termios newtio;
		struct serial_struct serinfo;
		
		float baudrate = baud;

		closePort();
		std::cerr << std::endl;
		std::cerr << "open " << devPort;

		if((_fd = ::open(devPort, O_RDWR | O_NOCTTY | O_NONBLOCK)) < 0)
		{
			std::cerr << "device open error: " << devPort << std::endl;
			closePort();
			return ERR_PORT_DISCONNECTED;
		}
		
		std::cerr << "success! " << std::endl;

		// You must set 38400bps!
		memset(&newtio, 0, sizeof(newtio));
		newtio.c_cflag 			= B38400 | CS8 | CLOCAL | CREAD;
		newtio.c_iflag 			= IGNPAR;
		newtio.c_oflag 			= 0;
		newtio.c_lflag 			= 0;
		newtio.c_cc[VTIME]		= 0;
		newtio.c_cc[VMIN]		= 0;

		tcsetattr(_fd, TCSANOW, &newtio);

		std::cout << "Set " << baudrate << " bps ";

		// Set non-standard baudrate
		if(ioctl(_fd, TIOCGSERIAL, &serinfo) < 0)
			return ERR_PORT_DISCONNECTED;

		serinfo.flags &= ~ASYNC_SPD_MASK;
		serinfo.flags |= ASYNC_SPD_CUST;
		serinfo.custom_divisor = serinfo.baud_base / baudrate;

		if (ioctl(_fd, TIOCSSERIAL, &serinfo) < 0)
		{
			std::cout << "failed." << std::endl;
			return ERR_PORT_DISCONNECTED;
		}

		std::cout << "success!" << std::endl;

		tcflush(_fd, TCIFLUSH);

		byteTransferTime = (1000.0 / baudrate) * 12.0;

		return ERR_NONE;
	}

	int Pro::closePort()
	{
		if(_fd != -1)
		{
			close(_fd);
		}
		_fd = -1;
		return ERR_NONE;
	}
	


#ifdef _cplusplus
}
#endif
