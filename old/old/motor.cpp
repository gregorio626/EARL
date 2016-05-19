/*
*
*   Author: EARL Technologies
*
*   Created: May 10, 2016
*
*/

#include "/home/gregorio626/EARL/include/dynamixel/dynamixel.h"
#include <iostream>
#include <string.h>

#ifdef _cplusplus
extern "C" {
#endif


using namespace EARL;
using namespace Dynamixel;

void Motor::allocateRegMemory(size_t size) {
	if (size != numberOfRegisters) {
		// allocate new size
		unsigned char* newRegisters = new unsigned char[size];
		unsigned char* newRegistersDesired = new unsigned char[size];

		// copy previous values
		memcpy(newRegisters, registers,
				size < numberOfRegisters ? size : numberOfRegisters);
		memcpy(newRegistersDesired, registersDesired,
				size < numberOfRegisters ? size : numberOfRegisters);

		// delete old allocations, if needed
		unsigned char* toDelete = registers;
		registers = newRegisters;
		if (toDelete != NULL) {
			delete[] toDelete;
		}

		toDelete = registersDesired;
		registersDesired = newRegistersDesired;
		if (toDelete != NULL) {
			delete[] toDelete;
		}

		numberOfRegisters = size;
	}
}

std::map<int, Register> Motor::reg_map;


#ifdef _cplusplus
}
#endif