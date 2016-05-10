#include <iostream>
#include <errno.h>
#include "dynamixel.h"
#include "Hexapod.h"
#include <vector>

#ifdef _cplusplus
extern "C" {
#endif
using namespace EARL::Robot;
using namespace EARL::Dynamixel;


int main(int argc, char * argv[])
{
	int baudrate = 1000000;
	unsigned int motorCount = 18;

	Hexapod * hexapod;
	hexapod = new Hexapod;
	Handler * mainHandler = new Handler;
	mainHandler->openInterface("/dev/ttyACM0", baudrate);
	hexapod->setupHandler(mainHandler, motorCount);

	std::cout << mainHandler->getDynamixels();




	std::cout << "Errno: " << errno << std::endl << std::endl;


	return 0;
}

#ifdef _cplusplus
}
#endif
