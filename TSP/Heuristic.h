#pragma once

#include "GraphRepresentation.h"
#include "Solution.h"


#ifdef TSP_EXPORTS
#define TSP_API __declspec(dllexport)
#else
#define TSP_API __declspec(dllimport)
#endif

namespace TSP
{
	class TSP_API Heuristic
	{
	public:
		Heuristic();
		virtual ~Heuristic();
		virtual Solution Solve(GraphRepresentation& representation) = 0;
	};
}

