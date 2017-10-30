#pragma once

#include "GraphRepresentation.h"
#include "Solution.h"

namespace TSP
{
	template<typename Cost>
	class Heuristic
	{
	public:
		Heuristic();
		virtual ~Heuristic();
		
		virtual Solution<Cost> Solve(GraphRepresentation<Cost>& representation) = 0;
	};

	template<typename Cost>
	Heuristic<Cost>::Heuristic()
	{
	}

	template<typename Cost>
	Heuristic<Cost>::~Heuristic()
	{
	}
}

