/*
*
*	Author: Gregory Roberts
*
*	Created: April 7, 2016
*
*/

#ifndef _LEG_H_
#define _LEG_H_

#include <iostream>
#include <math.h>
#include <vector>
#include "/home/gregorio626/EARL/framework/include/math/MathDefinition.h"
#include "/home/gregorio626/EARL/framework/include/dynamixel/dynamixel.h"

namespace EARL
{
	class Leg
	{
	public:


		enum
		{
			CoxaLength 			=  26,//from coxa motor to femur motor
			FemurLength 		= 104,//from femur motor to tibia motor
			TibiaLength 		= 235//from tibia motor to end-effector
		};

		std::vector<int> length(3);//coxa, femur, tibia






	};
}

#endif
