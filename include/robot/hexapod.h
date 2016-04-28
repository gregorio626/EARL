/*
*
*	Author: EARL Technologies
*
*	Created: April 16, 2016
*
*/

#ifndef _HEXAPOD_H_
#define _HEXAPOD_H_

namespace EARL
{
	class Hexapod
	{
	public:
		void powerOn(bool debug);//turns the robot on
		bool isPoweredOn();//used to verify that the robot has powered up correctly

		void powerOff();//turns the robot off
		bool isPoweredOff();//used to verify that the robot has been powered down corrcetly

	private:

	protected:

	};
}

#endif