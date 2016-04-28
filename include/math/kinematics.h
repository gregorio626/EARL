/*
*
*	Author: Gregory Roberts
*
*	Created: April 15, 2016
*
*/

#ifndef _KINEMATICS_H_
#define _KINEMATICS_H_

#include "/home/gregorio626/EARL/framework/include/robot/leg.h"

namespace EARL
{
	class Kinematics : public Leg
	{
	public:
		bool * IK_Solve(float * LegData, float * GoalPos);//solves the inverse kinematics, returns an array of joint angles
		bool FK_Solve(float * LegData, float * Angles);//solves the forward kinematics, returns the calculated XYZ coord's of the end effector
		enum
		{
			KINEMATICS_OUT_OF_RANGE		= ,
			KINEMATICS_INVALID_PARAM	= ,
			KINEMATICS_GREATER_THAN_MAX = 
		}KinematicsStatus;
	private:

	protected:

	};
}


#endif