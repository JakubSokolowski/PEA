#pragma once

#ifndef TSP_MOVE
#define TSP_MOVE

#include "..\Solver.h"
#include "..\..\Graphs\GraphRepresentation.h"
#include "TabuList.h"

namespace TSP
{

	// The method of moving from one solution to another
	template<typename Cost>
	class Move
	{
	public:
		Move() {};
		~Move() {};
		virtual Solution<Cost> Execute(Solution<Cost> &solution, MoveParameters &parameters,  GraphRepresentation<Cost> &representation) = 0;
		virtual Cost EvaluateMove(Solution<Cost> &solution, MoveParameters &parameters, GraphRepresentation<Cost> &representation) = 0;
	};


	template<typename Cost>
	class Swap
		: public Move<Cost>
	{
	public:
		Swap() {};
		~Swap() {};
	    virtual Solution<Cost> Execute(Solution<Cost> &solution, MoveParameters &parameters, GraphRepresentation<Cost> &representation) override;
		virtual Cost EvaluateMove(Solution<Cost> &solution, MoveParameters &parameters, GraphRepresentation<Cost> &representation) override;

	};


	// Swaps the vertives described by parameters.
	// In this case, parameters describe the indices of vertices relative to
	// their position in tour, and not their indices in graph
	template<typename Cost>
	inline Solution<Cost> Swap<Cost>::Execute(Solution<Cost>& solution, MoveParameters & parameters, GraphRepresentation<Cost>& representation)
	{
		//Cost total_cost = EvaluateMove(solution, parameters, representation);
		auto tour = solution.tour;
		auto temp = tour[parameters[0]];
		tour[parameters[0]] = tour[parameters[1]];
		tour[parameters[1]] = temp;
		Cost total_cost = representation.GetTourCost(tour);		
		return Solution<Cost>(total_cost, tour);
	}

	// Calculates total_cost of new solution in constant time
	template<typename Cost>
	inline Cost Swap<Cost>::EvaluateMove(Solution<Cost>& solution, MoveParameters & params, GraphRepresentation<Cost>& representation)
	{
		Cost new_cost = solution.total_cost;
		// Subtract the cost of edges that were removed due to the swap
		// In tour : 1-2-3-4-5-6-1 if we're to swap vertices 2 and 5, to get the new cost 
		// of tour : 1-5-3-4-2-6-1 we need to subtract the cost of edges 1-2,2-3 and 4-5 and 5-6 from the old tour
		// and then add cost of edges 1-5,5-3,4-2,6-1
		// In the case above, the MoveParameters would be (1,4)
		// Need to remove (vertex_before - tour[params[0]-1], tour[parms[0]]), (vertex_before - tour[params[0]-1], tour[parms[0]]) same for params[1]
		// As the indices in graph representation are numered from 0, subtract 1 from the above results

		// Get which vertices this is really about
		int left, left_prev, left_next, right, right_prev, right_next;

		left = solution.tour[params[0]] - 1;
		left_prev = solution.tour[params[0] - 1] - 1;
		left_next = solution.tour[params[0] + 1] - 1;

		right = solution.tour[params[1]] - 1;
		right_prev = solution.tour[params[1] - 1] - 1;
		right_next = solution.tour[params[1] + 1] - 1;

		new_cost -= representation.GetWeight(left_prev, left);
		new_cost -= representation.GetWeight(left, left_next);
		new_cost -= representation.GetWeight(right_prev, right);
		new_cost -= representation.GetWeight(right, right_next);

		new_cost += representation.GetWeight(left_prev, right);
		new_cost += representation.GetWeight(right, left_next);
		new_cost += representation.GetWeight(right_prev, left);
		new_cost += representation.GetWeight(left, right_next);		

		return new_cost;
	}

}



#endif
