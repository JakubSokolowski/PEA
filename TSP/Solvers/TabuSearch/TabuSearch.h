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
#include <typeinfo>
#include "../../Benchmarks/Timer.h"

namespace TSP
{
	


	struct TabuParameters
	{
		// For how many 
		uint tabu_tenure;
		uint tabu_list_length;
		// Num of different neigbourhoods that will be checked for solution
		uint max_iterations;
		// Max num of consecutive iterations where there is no improvement
		// comparing to best global solution
		uint max_no_improve;
		// Max number of initial solutions generated
		uint restart_count;
		// After this time, search stops
		double max_time_s;
	};

	template<class Cost>
	class TabuSearch
		: public Solver<Cost>
	{
	public:
		TabuSearch() { move_m = new Swap<Cost>(); };
		TabuSearch(Move<Cost> *move);
		TabuSearch(TabuParameters params);
		~TabuSearch() { delete move_m; };
		bool TimeLimited = true;
		bool IsSymmetric = true;

		Solution<Cost> Solve(GraphRepresentation<Cost> &representation);
		Solution<Cost> SolveFreq(GraphRepresentation<Cost> &representation);
		Solution<Cost> GenerateInitialSolution(GraphRepresentation<Cost> &representation);	
		Solution<Cost> FindBestNeigbourSolution(Solution<Cost> &solution, GraphRepresentation<Cost> &representation);
		Solution<Cost> FreqFindBestNeigbourSolution(Solution<Cost> &solution, GraphRepresentation<Cost> &representation, int iteration);

	private:
		bool AspirationCritirionReached(int solution_cost) { return solution_cost < best_global_m.total_cost; }
		MoveParameters ConvertToGraphIndices(MoveParameters &param, Solution<Cost> &solution);
		MoveParameters Mirror(MoveParameters &param);
		Solution<Cost> best_global_m;
		Move<Cost> *move_m;
		TabuList tabu_list_m;
		SwapTabuList swap_list_m;
		uint max_iterations_m = 50;
		uint max_iterations_without_improvement_m = 50;
		uint restart_count_m = 3;
		uint penalty_m = 10;
		uint long_term_length = 100;
		uint tenure_m;
		double max_time_s_m = 60; //3 mins;
	};

	
	

	template<class Cost>
	inline TabuSearch<Cost>::TabuSearch(Move<Cost> *move)
		: move_m(move)
	{
		tabu_list_m = TabuList(50, 4);

	}

	template<class Cost>
	inline TabuSearch<Cost>::TabuSearch(TabuParameters params)
		: tabu_list_m(TabuList(params.tabu_list_length, params.tabu_tenure))
		, max_iterations_m(params.max_iterations)
		, max_iterations_without_improvement_m(params.max_no_improve)
		, max_time_s_m(params.max_time_s)
	{
		move_m = new Swap<Cost>();
	}

	template<class Cost>
	inline Solution<Cost> TabuSearch<Cost>::Solve(GraphRepresentation<Cost>& representation)
	{
		best_global_m = GenerateInitialSolution(representation);
		Solution<Cost> best_local, current_solution{ std::numeric_limits<Cost>::max() ,std::vector<int>{} };
		//swap_list_m.PrepareList(representation.GetNumOfVertices() + 1);
		Timer timer{ TimeUnit::seconds };
		double time = 0.0;
		timer.Start();
		// O(restart)
		for (uint loop = 0; loop < restart_count_m; loop++)
		{
			if (TimeLimited && time > max_time_s_m)
				break;

			// O(n) / O(n2)
			tabu_list_m.ResetList();
			if (!loop)
				best_local = best_global_m;
			else
				best_local = GenerateInitialSolution(representation);
			current_solution = best_local;

			
			// O(max_iter)
			uint iterations_without_improvement = 0;
			for (uint i = 0; i < max_iterations_m; i++)
			{				
				time = timer.GetTime();
				if (TimeLimited && time > max_time_s_m)
					break;
				// O(N3)
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
					if (iterations_without_improvement > max_iterations_without_improvement_m)
						break;
				}					
			}
		}		
		return best_global_m;
	}

	template<class Cost>
	inline Solution<Cost> TabuSearch<Cost>::SolveFreq(GraphRepresentation<Cost>& representation)
	{
		best_global_m = GenerateInitialSolution(representation);
		Solution<Cost> best_local, current_solution{ std::numeric_limits<Cost>::max() ,std::vector<int>{} };
		swap_list_m.PrepareList(representation.GetNumOfVertices() + 1);
		Timer timer{ TimeUnit::seconds };
		double time = 0.0;
		timer.Start();
		// O(restart)
		for (uint loop = 0; loop < restart_count_m; loop++)
		{
			if (TimeLimited && time > max_time_s_m)
				break;

			// O(n) / O(n2)
			tabu_list_m.ResetList();
			if (!loop)
				best_local = best_global_m;
			else
				best_local = GenerateInitialSolution(representation);
			current_solution = best_local;


			// O(max_iter)
			int iterations_without_improvement = 0;
			for (uint i = 0; i < max_iterations_m; i++)
			{
				time = timer.GetTime();
				if (TimeLimited && time > max_time_s_m)
					break;
				// O(N3)
				current_solution = FreqFindBestNeigbourSolution(current_solution, representation,i);
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
					if (iterations_without_improvement > max_iterations_without_improvement_m)
						break;
				}
			}
		}
		return best_global_m;
	}

	template<class Cost>
	inline Solution<Cost> TabuSearch<Cost>::GenerateInitialSolution(GraphRepresentation<Cost>& representation)
	{
		srand(time((uint)NULL));
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

		// O(n)
		for (int i = 1; i < tour_size - 1; i++)
		{
			// O(n)
			for (int j = (i+1); j < tour_size - 1; j++)
			{				
				// Create new neighbour solution
				MoveParameters params{ i, j };
				// O(1)
				Solution<Cost> new_solution = move_m->Execute(best_neighbour_solution, params, representation);
				// O(tabu_tenure)
				if (AspirationCritirionReached(new_solution.total_cost) || ((new_solution.total_cost < best_neighbour_solution.total_cost) && !tabu_list_m.IsForbidden(params)))
				{
					tabu_params = params;
					best_neighbour_solution = new_solution;
					
				}
			}
		}
		
		// O(n2*tabu_len)
		
		tabu_list_m.DecreaseTenure();
		tabu_list_m.ForbidMove(ConvertToGraphIndices(tabu_params, best_neighbour_solution));
		if (IsSymmetric)
			tabu_list_m.ForbidMove(ConvertToGraphIndices(Mirror(tabu_params), best_neighbour_solution));
		//tabu_list_m.ForbidMove(ConvertToGraphIndices(tabu_params, best_neighbour_solution));
	
		return best_neighbour_solution;
	}

	template<class Cost>
	inline Solution<Cost> TabuSearch<Cost>::FreqFindBestNeigbourSolution(Solution<Cost>& solution, GraphRepresentation<Cost>& representation, int iteration)
	{
		Solution<Cost> best_neighbour_solution = solution;
		auto tour_size = best_neighbour_solution.tour.size();

		MoveParameters tabu_params{ 0,0 };

		// O(n)
		for (int i = 1; i < tour_size - 1; i++)
		{
			// O(n)
			for (int j = (i + 1); j < tour_size - 1; j++)
			{
				// Create new neighbour solution
				MoveParameters params{ i, j };
				// O(1)
				Solution<Cost> new_solution = move_m->Execute(best_neighbour_solution, params, representation);
				double penalty_score = new_solution.total_cost + long_term_length * swap_list_m.GetFrequency(params);
				// O(tabu_tenure)
				if (AspirationCritirionReached(new_solution.total_cost) || ((penalty_score < best_neighbour_solution.total_cost) && swap_list_m.GetTenure(params) <= iteration))
				{
					tabu_params = params;
					best_neighbour_solution = new_solution;
					swap_list_m.ForbidMove(ConvertToGraphIndices(tabu_params, best_neighbour_solution),tenure_m + iteration);
					if (IsSymmetric)
						swap_list_m.ForbidMove(ConvertToGraphIndices(Mirror(tabu_params), best_neighbour_solution),tenure_m + iteration);
				}
				if (swap_list_m.GetFrequency(params) > 0 && iteration > long_term_length)
					swap_list_m.DecreaseFrequency(params);

			}
		}

		// O(n2*tabu_len)

		swap_list_m.IncreaseFrequency(tabu_params,2);
		//tabu_list_m.ForbidMove(ConvertToGraphIndices(tabu_params, best_neighbour_solution));

		return best_neighbour_solution;
	}
	
	
	template<class Cost>
	inline MoveParameters TabuSearch<Cost>::ConvertToGraphIndices(MoveParameters & param, Solution<Cost>& solution)
	{
		return MoveParameters{solution.tour[param[0]], solution.tour[param[1]]};
	}

	template<class Cost>
	inline MoveParameters TabuSearch<Cost>::Mirror(MoveParameters & param)
	{
		return MoveParameters{ param[1],param[0] };
	}

}



#endif
