/*
*
*	Author: EARL Technologies
*
*	Created: April 16, 2016
*
*/

#include "dynamixel.h"
#include <map>
#include <string>


#ifndef _HEXAPOD_H_
#define _HEXAPOD_H_

namespace EARL {
using namespace Dynamixel;
	namespace Robot {

		class Hexapod {
		private:

		public:

			void setDynamixelHandler(PortHandler* handler);

			PortHandler* dynamixelHandler;

			std::map<unsigned char, Dynamixel::Motor*> dyn;


			/*Populates the motor list
			'motorCount' === the number of motors used in the robot
			*/

		protected:

		};
		void getError(int errorNumber);
	}
}

#endif
