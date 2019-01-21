#pragma once

#ifndef TSP_GENETIC_ALGORITHM
#define TSP_GENETIC_ALGORITHM

#include "..\Solver.h"
#include "Population.h"
#include "Crossover\Crossover.h"
#include "Crossover\OX.h"
#include "Crossover\PMX.h"
#include "Crossover\CX.h"
#include "Mutation.h"
#include "CppUnitTest.h"

namespace TSP
{
	enum CrossoverType {
		ORDER_ONE,
		PARTIALLY_MAPPED,
		CYCLE
	};
	struct GeneticParameters
	{
		int population_size;
		int max_generations;
		double mutation_rate;
		double crossover_rate;
		int tournament_size;
		double elitism_rate;
		CrossoverType cros;
		MutationType mut;
		Selection sel;

		GeneticParameters() {}

		GeneticParameters(int pop_size, int max_gen, int trn_size, double mut_rate, double cross_rate, double elite_rate, CrossoverType c, MutationType m, Selection s)
			: population_size(pop_size)
			, max_generations(max_gen)
			, tournament_size(trn_size)
			, mutation_rate(mut_rate)
			, crossover_rate(cross_rate)
			, elitism_rate(elite_rate)
			, cros(c)
			, mut(m)
			, sel(s)
		{}
	};

	template<class Cost>
	class GeneticAlgorithm
		: public Solver<Cost>
	{
	public:
		GeneticAlgorithm(GeneticParameters params);
		Solution<Cost> Solve(GraphRepresentation<Cost>& representation);
		void Evolve(Population<Cost> &pop, GraphRepresentation<Cost> &rep);
		void Mutate(Population<Cost>& pop, GraphRepresentation<Cost>& rep, MutationType type);
		Chromosome<Cost> Crossover(Chromosome<Cost> &c1, Chromosome<Cost> &c2);

	private:
		GeneticParameters params_m;
		int max_pop_size_m;
		int max_generations_m;
		double mutation_rate_m;
		double crossover_rate_m;
		double elite_percentage_m;
		int tournament_size_m;
		Selection sel_m = Selection::ROULETTE;
		CrossoverType cros_m = CrossoverType::PARTIALLY_MAPPED;
		MutationType mut_m = MutationType::SWAP;
	};

	template<class Cost>
	inline GeneticAlgorithm<Cost>::GeneticAlgorithm(GeneticParameters params) {
		params_m = params;
	}

	template<class Cost>
	inline Solution<Cost> GeneticAlgorithm<Cost>::Solve(GraphRepresentation<Cost>& representation) {
		auto current_generation = Population<Cost>(params_m.population_size, representation);
		auto next_generation = Population<Cost>();
		for (int i = 0; i < params_m.max_generations; i++) {
			next_generation = Population<Cost>(current_generation, params_m.elitism_rate);
			while (!next_generation.IsFull()) {
				auto parents = current_generation.GetParents(params_m.sel);
				Chromosome<Cost> offspring = Crossover(parents.first, parents.second);
				next_generation.AddChromosome(offspring);
			}		
			Mutate(current_generation, representation, params_m.mut);
			next_generation.UpdateFitness(representation);
			current_generation = next_generation;			
			Logger::WriteMessage(std::to_string(current_generation.GetMostFit().total_cost).c_str());
			Logger::WriteMessage("\n");
		}
		return current_generation.GetMostFit().GetSolution();
	}



	template<class Cost>
	inline void GeneticAlgorithm<Cost>::Evolve(Population<Cost>& pop, GraphRepresentation<Cost>& rep) {
		while (!pop.IsFull()) {
			auto first_parent = pop.TournamentSelection();
		}
	}
	template<class Cost>
	inline void GeneticAlgorithm<Cost>::Mutate(Population<Cost>& pop, GraphRepresentation<Cost>& rep, MutationType type) {
		for (auto &ch : pop.population_m) {
			if (RandomHelper::EventOccured(params_m.mutation_rate))
				Mutation<int>::Mutate(ch, type);
		}
	}
	template<class Cost>
	inline Chromosome<Cost> GeneticAlgorithm<Cost>::Crossover(Chromosome<Cost>& c1, Chromosome<Cost>& c2) {
		switch (params_m.cros) {
			case CrossoverType::ORDER_ONE: {
				auto cros = OX<int>();
				return cros.Execute(c1, c2);
			}
			case CrossoverType::PARTIALLY_MAPPED: {
				auto cros = PMX<int>();
				return cros.Execute(c1, c2);
			}
			case CrossoverType::CYCLE: {
				auto cros = CX<int>();
				return cros.Execute(c1, c2);
			}
		}
	}
}
#endif // !TSP_GENETIC_ALGORITHM



