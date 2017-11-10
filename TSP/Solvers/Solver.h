#pragma once

#include "GraphRepresentation.h"
#include "Solution.h"

namespace TSP
{
	template<typename Cost>
	class Solver
	{
	public:
		Solver();
		virtual ~Solver();		
		virtual Solution<Cost> Solve(GraphRepresentation<Cost>& representation) = 0;
	};
	template<typename Cost>
	inline Solver<Cost>::Solver()
	{
	}
	template<typename Cost>
	inline Solver<Cost>::~Solver()
	{
	}
}

