/*
*
*	Author: EARL Technologies
*
*	Created: April 16, 2016
*
*/

#include "/home/gregorio626/EARL/include/dynamixel/dynamixel.h"

#ifndef _HEXAPOD_H_
#define _HEXAPOD_H_

namespace EARL {
	namespace Robot {

		class Hexapod {
		private:

		public:

			std::map<unsigned char, Dynamixel::Motor*> dxl;

			Dynamixel::Handler* dynamixelHandler;

			/*Populates the motor list
			'motorCount' === the number of motors used in the robot
			*/
			void setupHandler(Dynamixel::Handler* pHandlerObj, unsigned int motorCount);
		protected:

		};
	}
}

#endif