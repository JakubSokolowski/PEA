#pragma once

#ifndef TSP_TIMER
#define TSP_TIMER
#define NOMINMAX
#include <Windows.h>
#include <time.h>
#include <iomanip>

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
		std::string GetTimestamp();

	private:

		double pc_freq_m = 0.0;
		__int64 counter_start_m = 0;
		double time_unit_m = 1000;
		double AssignUnit(TimeUnit unit);

	};


	inline Timer::Timer()
	{
	}

	inline Timer::Timer(TimeUnit unit)
		: time_unit_m(AssignUnit(unit))
	{}

	inline Timer::~Timer()
	{}

	inline void Timer::Start()
	{
		LARGE_INTEGER li;
		if (!QueryPerformanceFrequency(&li))
			return;

		pc_freq_m = double(li.QuadPart) / time_unit_m;

		QueryPerformanceCounter(&li);
		counter_start_m = li.QuadPart;
	}


	inline double Timer::GetTime()
	{
		LARGE_INTEGER li;
		QueryPerformanceCounter(&li);
		return double(li.QuadPart - counter_start_m) / pc_freq_m;
	}

	inline std::string Timer::GetTimestamp() {
		auto t = std::time(nullptr);
		auto tm = *localtime(&t);
		std::ostringstream oss;
		oss << std::put_time(&tm, "%d-%m-%Y %H-%M-%S");
		return oss.str();
	}

	inline double Timer::AssignUnit(TimeUnit unit)
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

