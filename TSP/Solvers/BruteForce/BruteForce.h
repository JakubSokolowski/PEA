#pragma once
#ifndef TSP_BRUTEFORCE
#define TSP_BRUTEFORCE
#include "../Solver.h"
#include <algorithm>
#include <numeric>
namespace TSP {
	template<class Cost>
	class BruteForce : public Solver<Cost> {
	public:
		Solution<Cost> Solve(GraphRepresentation<Cost> &representation);
	private:
		Solution<Cost> GenerateInitialSoltion(GraphRepresentation<Cost> &representation);

	};
	template<class Cost>
	inline Solution<Cost> BruteForce<Cost>::Solve(GraphRepresentation<Cost>& representation) {
		auto initial = GenerateInitialSoltion(representation);
		auto best_solution = initial;
		auto current_permutation = initial.tour;
		while (std::next_permutation(current_permutation.begin() + 1, current_permutation.end() - 1)) {
			auto current_cost = representation.GetTourCost(current_permutation);
			if (current_cost < best_solution.total_cost) {
				best_solution = Solution<Cost>(current_cost, current_permutation);
			}
		}
		return best_solution;
	}
	template<class Cost>
	inline Solution<Cost> BruteForce<Cost>::GenerateInitialSoltion(GraphRepresentation<Cost>& representation) {
		std::vector<Cost> tour(representation.GetNumOfVertices());
		std::iota(tour.begin(), tour.end(), 1);
		tour.push_back(tour[0]);
		auto cost = representation.GetTourCost(tour);
		return Solution<Cost>(cost, tour);
	}
}

#endif // !TSP_BRUTEFORCE

