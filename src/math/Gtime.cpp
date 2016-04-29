/*
*
*   Author: EARL Technologies
*
*   Created: April 28, 2016
*
*/

#include "/home/gregorio626/EARL/include/math/Gtime.h"
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>

#ifdef _cplusplus
extern "C"
{
#endif

	using namespace EARL;
	using namespace Math;

void Gtime::init()
{
	favg  =30;
	fnum = 0;
	gettimeofday(&tv, NULL);						//Gets the current time
	timecycle = tv.tv_sec + tv.tv_usec*1e-6;		//Computes the current time(in seconds)
	usleep(10000);
}



#ifdef _cplusplus
}
#endif