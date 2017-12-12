#pragma once

#ifndef TSP_TIMER
#define TSP_TIMER

#include <Windows.h>

namespace TSP
{	
	enum TimeUnit
	{
		microseconds,
		miliseconds,
		seconds,
	};

	class Timer
	{
	public:

		Timer();
		Timer(TimeUnit unit);
		~Timer();
		void Start();
		double GetTime();

	private:

		double pc_freq_m = 0.0;
		__int64 counter_start_m = 0;
		double time_unit_m = 1000;
		double AssignUnit(TimeUnit unit);

	};

}
#endif 

