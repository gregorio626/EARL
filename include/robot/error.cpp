/*
*
*	Author: EARL Technologies
*
*	Created: April 16, 2016
*
*/

/*
http://www.thegeekstuff.com/2010/10/linux-error-codes/
*/

#include "/home/gregorio626/EARL/include/robot/Hexapod.h"
#include <iostream>


using namespace EARL;
using namespace Robot;

void EARL::Robot::getError(int errorNumber) {
	switch (errorNumber) {
		case 0:
			std::cout << "No Errors(0)" << std::endl;
			break;
		case 1:
			std::cout << "Operation not permitted(1)" << std::endl;
			break;
		case 2:
			std::cout << "No such file or directory(2)" << std::endl;
			break;
		case 3:
			std::cout << "No such process(3)" << std::endl;
			break;
		case 4:
			std::cout << "Interrupted system call(4)" << std::endl;
			break;
		case 5:
			std::cout << "I/O error(5)" << std::endl;
			break;
		case 6:
			std::cout << "No such device or address(6)" << std::endl;
			break;
		case 7:
			std::cout << "Argument list too long(7)" << std::endl;
			break;
	}
}
