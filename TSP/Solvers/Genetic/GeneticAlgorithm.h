#pragma once

#ifndef TSP_GENETIC_ALGORITHM
#define TSP_GENETIC_ALGORITHM

#include "..\Solver.h"

namespace TSP
{
	struct GeneticParameters
	{
		int population_size;
		int max_generations;
		double mutation_rate;
		double crossover_rate;
		int tournament_size;

		GeneticParameters(int pop_size, int max_gen, int trn_size, double mut_rate, double cross_rate)
			: population_size(pop_size)
			, max_generations(max_gen)
			, tournament_size(trn_size)
			, mutation_rate(mut_rate)
			, crossover_rate(cross_rate)
		{}
	};

	template<class Cost>
	class GeneticAlgorithm
		: public Solver<Cost>
	{
	public:

		Solution<Cost> Solve(GraphRepresentation<Cost>& representation);

	private:

		int max_pop_size_m;
		int max_generations_m;
		double mutation_rate_m;
		double crossover_rate_m;
		int tournament_size_m;


	};

	template<class Cost>
	inline Solution<Cost> GeneticAlgorithm<Cost>::Solve(GraphRepresentation<Cost>& representation) {
		return Solution<Cost>();
	}

}
#endif // !TSP_GENETIC_ALGORITHM



