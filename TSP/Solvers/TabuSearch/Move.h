#pragma once

#ifndef TSP_MOVE
#define TSP_MOVE

#include "..\Solver.h"
#include "..\..\Graphs\GraphRepresentation.h"

namespace TSP
{
	using MoveParameters = std::vector<uint>;

	// The method of moving from one solution to another
	template<typename Cost>
	class Move
	{
	public:
		Move() {};
		~Move() {};
		virtual void Execute(Solution<Cost> &solution, const MoveParameters &parameters, const GraphRepresentation<Cost> &representation) = 0;
	};


	template<typename Cost>
	class Swap
		: public Move<Cost>
	{
		Swap() {};
		~Swap() {};
		void Execute(Solution<int> &solution, const MoveParameters &parameters, const GraphRepresentation<Cost> &representation);
	};


	// Change the solution by swapping 2 vertives in tour
	template<typename Cost>
	inline void Swap<Cost>::Execute(Solution<int>& solution, const MoveParameters &parameters, const GraphRepresentation<Cost>& representation)
	{
		auto temp = solution.tour[parameters[0]];
		solution.tour[0] = solution.tour[parameters[1]];
		solution.tour[1] = temp;		
		solution.total_cost = representation.GetTourCost(solution.tour);
	}

}



#endif
