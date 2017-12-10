#pragma once

#ifndef TSP_TABU_SEARCH
#define TSP_TABU_SEARCH


#include <algorithm>
#include <numeric>
#include "..\Solver.h"
#include "TabuList.h"
#include "Move.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

namespace TSP
{
	const uint MAX_ITERATIONS = 500;	
	const uint MAX_ITERATIONS_WITHOUT_IMPROVEMENT = 50;

	template<class Cost>
	class TabuSearch
		: public Solver<Cost>
	{
	public:
		TabuSearch() { move_m = new Swap<Cost>(); };
		TabuSearch(Move<Cost> *move);
		~TabuSearch() { delete move_m; };

		Solution<Cost> Solve(GraphRepresentation<Cost> &representation);
		Solution<Cost> GenerateInitialSolution(GraphRepresentation<Cost> &representation);	
		Solution<Cost> FindBestNeigbourSolution(Solution<Cost> &solution, GraphRepresentation<Cost> &representation);
	private:
		bool AspirationCritirionReached(uint solution_cost) { return solution_cost < best_global_m.total_cost; }
		MoveParameters ConvertToGraphIndices(MoveParameters &param, Solution<Cost> &solution);
		Solution<Cost> best_global_m;
		Move<Cost> *move_m;
		TabuList tabu_list_m;		
	};

	
	

	template<class Cost>
	inline TabuSearch<Cost>::TabuSearch(Move<Cost> *move)
		: move_m(move)
	{
		tabu_list_m = TabuList(50, 4);
	}

	template<class Cost>
	inline Solution<Cost> TabuSearch<Cost>::Solve(GraphRepresentation<Cost>& representation)
	{
		best_global_m = GenerateInitialSolution(representation);
		Solution<Cost> best_local, current_solution{ std::numeric_limits<Cost>::max() ,std::vector<int>{} };

		int reset_count = 3;

		for (int loop = 0; loop < reset_count; loop++)
		{
			tabu_list_m.ResetList();
			if (!loop)
				best_local = best_global_m;
			else
				best_local = GenerateInitialSolution(representation);
			current_solution = best_local;

			int iterations_without_improvement = 0;
			for (uint i = 0; i < MAX_ITERATIONS; i++)
			{
				current_solution = FindBestNeigbourSolution(current_solution, representation);
				if (current_solution.total_cost < best_local.total_cost)
				{
					best_local = current_solution;
					iterations_without_improvement = 0;
					if (best_local.total_cost < best_global_m.total_cost)
						best_global_m = best_local;
				}
				else
				{
					iterations_without_improvement++;
					// Critical Event
					if (iterations_without_improvement > MAX_ITERATIONS_WITHOUT_IMPROVEMENT)
						break;
				}					
			}
		}		
		return best_global_m;
	}

	template<class Cost>
	inline Solution<Cost> TabuSearch<Cost>::GenerateInitialSolution(GraphRepresentation<Cost>& representation)
	{
		srand(time(NULL));
		std::vector<Cost> tour(representation.GetNumOfVertices() + 1);
		std::iota(tour.begin(), tour.end(), 1);
		tour.back() = 1;
		std::random_shuffle(std::next(tour.begin(), 1), std::prev(tour.end(), 1));
		return Solution<Cost>(representation.GetTourCost(tour), tour);
	}

	template<class Cost>
	inline Solution<Cost> TabuSearch<Cost>::FindBestNeigbourSolution(Solution<Cost> &solution, GraphRepresentation<Cost>& representation)
	{
		Solution<Cost> best_neighbour_solution = solution;
		auto tour_size = best_neighbour_solution.tour.size();

		MoveParameters tabu_params{ 0,0 };

		for (int i = 1; i < tour_size - 1; i++)
		{
			for (int j = (i+1); j < tour_size - 1; j++)
			{				
				// Create new neighbour solution
				MoveParameters params{ i, j };
				Solution<Cost> new_solution = move_m->Execute(best_neighbour_solution, params, representation);
				if (AspirationCritirionReached(new_solution.total_cost) || ((new_solution.total_cost < best_neighbour_solution.total_cost) && !tabu_list_m.IsForbidden(params)))
				{
					tabu_params = params;
					best_neighbour_solution = new_solution;
				}
			}
		}
		
		tabu_list_m.DecreaseTenure();
		tabu_list_m.ForbidMove(ConvertToGraphIndices(tabu_params, best_neighbour_solution));
	
		return best_neighbour_solution;
	}
	template<class Cost>
	inline MoveParameters TabuSearch<Cost>::ConvertToGraphIndices(MoveParameters & param, Solution<Cost>& solution)
	{
		return MoveParameters{solution.tour[param[0]], solution.tour[param[1]]};
	}
}



#endif
