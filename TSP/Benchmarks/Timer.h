#pragma once

#ifndef TSP_TIMER
#define TSP_TIMER

#define NOMINMAX
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


	inline TSP::Timer::Timer()
	{
	}

	inline TSP::Timer::Timer(TimeUnit unit)
		: time_unit_m(AssignUnit(unit))
	{
	}

	inline TSP::Timer::~Timer()
	{
	}

	inline void TSP::Timer::Start()
	{
		LARGE_INTEGER li;
		if (!QueryPerformanceFrequency(&li))
			return;

		pc_freq_m = double(li.QuadPart) / time_unit_m;

		QueryPerformanceCounter(&li);
		counter_start_m = li.QuadPart;
	}

	inline double TSP::Timer::GetTime()
	{
		LARGE_INTEGER li;
		QueryPerformanceCounter(&li);
		return double(li.QuadPart - counter_start_m) / pc_freq_m;
	}

	inline double TSP::Timer::AssignUnit(TimeUnit unit)
	{
		double time_unit = 0.0;
		switch (unit)
		{
		case TimeUnit::microseconds:
			time_unit = 1000000.0;
			break;
		case TimeUnit::miliseconds:
			time_unit = 1000.0;
			break;
		case TimeUnit::seconds:
			time_unit = 1.0;
			break;
		}
		return time_unit;
	}


}
#endif 

