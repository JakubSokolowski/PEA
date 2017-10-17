#pragma once

#include "GraphRepresentation.h"
#include "Solution.h"

namespace TSP
{
	class Heuristic
	{
	public:
		Heuristic();
		virtual ~Heuristic();
		virtual Solution Solve(GraphRepresentation *representation) = 0;
	};
}

