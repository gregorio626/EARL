/*
*
*	Author: EARL Technologies
*
*	Created: May 05, 2016
*
*/


#include "dynamixel.h"

#include <math.h>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include <typeinfo>
#include <map>

using namespace EARL::Robot;
using namespace EARL::Dynamixel;

void Hexapod::setDynamixelHandler(PortHandler * handler) {
	dynamixelHandler = handler;
	if (dynamixelHandler == NULL) {
		return;
	}

	for (unsigned int i = 0; i < 18; i++)  // initialize motor handling
			{
		dynamixelHandler->forceAddDynamixel((i + 1), MODEL_AX18A);
		// dyn[i]->uploadAll();    // Have all current motor information
	}

	// dyn = new std::vector<Motor*>;
	dyn = dynamixelHandler->getDynamixels();

	for (std::map<unsigned char, Motor*>::iterator it = dyn.begin();
			it != dyn.end(); it++) {
		dynamixelHandler->pushInstruction(
				new Instruction(Instruction::READ, DataRange(0, 2),
						it->second));
	}
	while (dynamixelHandler->busy())
		;
	for (std::map<unsigned char, Motor*>::iterator it = dyn.begin();
			it != dyn.end(); it++) {
		dynamixelHandler->pushInstruction(
				new Instruction(Instruction::READ,
						DataRange(2, it->second->getNumberOfRegisters() - 2),
						it->second));
	}
	while (dynamixelHandler->busy() == true)
		;
}
