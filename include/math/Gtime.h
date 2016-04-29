/*
*
*   Author: EARL Technologies
*
*   Created: April 28, 2016
*
*/

#ifndef _GTIME_H_
#define _GTIME_H_

#include <time.h>
#include <sys/time.h>
#include <math.h>
#include <unistd.h>

namespace EARL{
	
	namespace Math{
		class Gtime
		{
		private:
			struct timeval tv;					//this handles the time.
			double timecycle, oldtimecycle;		//used to calculate the difference in time
		public:
			void init();						//initialize
			void update();						//called once every loop cycle


		protected:

		};
	}
}


#endif
