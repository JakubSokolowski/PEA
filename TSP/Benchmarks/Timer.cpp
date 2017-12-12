#pragma once

#include "Timer.h"

using namespace TSP;

TSP::Timer::Timer()
{
}

TSP::Timer::Timer(TimeUnit unit)
	: time_unit_m(AssignUnit(unit))
{
}

TSP::Timer::~Timer()
{
}

void TSP::Timer::Start()
{
	LARGE_INTEGER li;
	if (!QueryPerformanceFrequency(&li))
		return;

	pc_freq_m = double(li.QuadPart) / time_unit_m;

	QueryPerformanceCounter(&li);
	counter_start_m = li.QuadPart;
}

double TSP::Timer::GetTime()
{
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	return double(li.QuadPart - counter_start_m) / pc_freq_m;
}

double TSP::Timer::AssignUnit(TimeUnit unit)
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
