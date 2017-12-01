#pragma once

#ifndef TSP_TABU_SEARCH
#define TSP_TABU_SEARCH


#include <algorithm>
#include <numeric>
#include "..\Solver.h"
#include "Move.h"

namespace TSP
{
	// Tabu Move consists of MoveParameters and move tenure
	using TabuMove = std::pair<MoveParameters, uint>;
	using TabuList = std::vector<TabuMove>;

	template<class Cost>
	class TabuSearch
		: public Solver<Cost>
	{
	public:
		TabuSearch() {};
		~TabuSearch() {};

		Solution<Cost> Solve(GraphRepresentation<Cost> &representation);
		Solution<Cost> GenerateInitialSolution(GraphRepresentation<Cost> &representation);		

	private:

		Solution<Cost> initial_solution_m;
	};

	

	
	

	template<class Cost>
	inline Solution<Cost> TabuSearch<Cost>::Solve(GraphRepresentation<Cost>& representation)
	{
		return Solution<Cost>();
	}

	template<class Cost>
	inline Solution<Cost> TabuSearch<Cost>::GenerateInitialSolution(GraphRepresentation<Cost>& representation)
	{
		std::vector<Cost> tour(representation.GetNumOfVertices() + 1);
		std::iota(tour.begin(), tour.end(), 1);
		tour.back() = 1;
		std::random_shuffle(std::next(tour.begin(), 1), std::prev(tour.end(), 1));
		return Solution<Cost>(representation.GetTourCost(tour), tour);
	}

	

}



#endif
