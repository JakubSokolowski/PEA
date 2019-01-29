#pragma once

#ifndef TSP_POPULATION
#define TSP_POPULATION

#include "..\Solution.h"
#include "..\..\Graphs\GraphRepresentation.h"
#include "Chromosome.h"
#include <queue>
#include <deque>
#include <algorithm>
#include <numeric>
#include <random>
#include <set>
#include "RandomHelper.h"

namespace TSP
{
	enum Selection {
		ROULETTE,
		TOURNAMENT
	};
	// Represents the population of avalible chromosomes and handles random generation and selection
	template<typename Cost>
	class Population
	{
	public:

		Population();
		Population(int population_size);
		Population(int population_size, GraphRepresentation<Cost>& representation);
		Population(const Population<Cost> &other);
		Population(const std::vector<Chromosome<Cost>> &chromosomes);
		Population(const Population<Cost> &other, double elite_percentage);
		Population<Cost> operator = (const Population& other);

		~Population();
	
		int GetSize() const { return population_m.size(); }
		int GetMaxSize() const { return max_pop_size_m; }
		bool IsFull() { return GetSize() == GetMaxSize(); }

		void AddChromosome(Chromosome<Cost> &chromosome);

	
		Cost GetAvergeTourCost() const {
			return  Cost(std::accumulate(population_m) / population_m.size());
		}
		Chromosome<Cost> GetMostFit() const;
		std::vector<Chromosome<Cost>> GetMostFitPercentage(double percentage) const;	
		Chromosome<Cost> GetBestChromosomeWithProbability(double probability, std::vector<Chromosome<Cost>> &chromosomes);
		std::vector<Chromosome<Cost>> GetKChromosomes(int k);
		Chromosome<Cost> TournamentSelection(int tournament_size);
		Chromosome<Cost> RouletteSelection();
		std::pair<Chromosome<Cost>, Chromosome<Cost>> GetParents(Selection sel);
		std::vector<Chromosome<Cost>> population_m;
		void UpdateFitness(GraphRepresentation<Cost> &rep);
		void GenerateRoulette();

		bool roulette_generated = false;
		std::vector<double> sorted_weights;
		std::vector<Chromosome<Cost>> sorted_pop;
		double weight_sum = 0.0;

	private:

		int max_pop_size_m;
		int tournament_size = 10;
		
		void GenerateRandomPopulation(GraphRepresentation<Cost> &representation);
		Chromosome<Cost> CreateRandomChromosome(GraphRepresentation<Cost> &representation);
		
	};
	template<typename Cost>
	inline Population<Cost>::Population()
		: max_pop_size_m(10)
	{}
	template<typename Cost>
	inline Population<Cost>::Population(int population_size)
		: max_pop_size_m(population_size)
	{}
	template<typename Cost>
	inline Population<Cost>::Population(int population_size, GraphRepresentation<Cost>& representation)
		: max_pop_size_m(population_size)
	{
		GenerateRandomPopulation(representation);
	}
	template<typename Cost>
	inline Population<Cost>::Population(const Population<Cost>& other)
	{
		population_m = other.population_m;
		max_pop_size_m = other.max_pop_size_m;
		roulette_generated = other.roulette_generated;
		sorted_pop = other.sorted_pop;
		weight_sum = other.weight_sum;
	}
	template<typename Cost>
	inline Population<Cost>::Population(const std::vector<Chromosome<Cost>>& chromosomes)
		: population_m(chromosomes)
		, max_pop_size_m(chromosomes.size())
	{}
	template<typename Cost>
	inline Population<Cost> Population<Cost>::operator=(const Population & other)
	{
		population_m = other.population_m;
		max_pop_size_m = other.max_pop_size_m;
		roulette_generated = other.roulette_generated;
		sorted_pop = other.sorted_pop;
		weight_sum = other.weight_sum;
		return *this;
	}
	template<typename Cost>
	inline Population<Cost>::Population(const Population<Cost>& other, double elite_percentage) {
		auto elite = other.GetMostFitPercentage(elite_percentage);
		for (auto el : elite)
			population_m.push_back(el);
		max_pop_size_m = other.max_pop_size_m;
	}
	template<typename Cost>
	inline Population<Cost>::~Population()
	{
	}
	template<typename Cost>
	inline void Population<Cost>::AddChromosome(Chromosome<Cost>& chromosome)
	{
		if (GetSize() < max_pop_size_m)
			population_m.push_back(chromosome);
	}

	// Returns the iterator to the most fit chromosome - the one with the highest 1 / tour_cost ratio
	template<typename Cost>
	inline Chromosome<Cost> Population<Cost>::GetMostFit() const
	{
		return *std::min_element(population_m.begin(), population_m.end(), ChromosomeCompare<Cost>());
	}
	template<typename Cost>
	inline std::vector<Chromosome<Cost>> Population<Cost>::GetMostFitPercentage(double percentage) const {
		int n = std::round(percentage * population_m.size());
		auto result = std::vector<Chromosome<Cost>>();
		int counter = 0;
		auto pop = population_m;
		std::sort(pop.begin(), pop.end(), ChromosomeCompare<Cost>());
		for (auto ch : pop) {
			result.push_back(ch);
			counter++;
			if (counter == n)
				break;
		}
		return result;
	}
	template<typename Cost>
	inline void Population<Cost>::GenerateRandomPopulation(GraphRepresentation<Cost>& representation)
	{
		for (int i = 0; i < max_pop_size_m; i++)
			population_m.push_back(CreateRandomChromosome(representation));		
	}
	// Creates random chromosome for given graph. In tsp chromosome is combination of graph tour and the cost of this tour
	template<typename Cost>
	inline Chromosome<Cost> Population<Cost>::CreateRandomChromosome(GraphRepresentation<Cost>& representation)
	{
		std::vector<Cost> tour(representation.GetNumOfVertices() + 1);
		std::iota(tour.begin(), tour.end(), 1);
		tour.back() = 1;
		std::random_shuffle(std::next(tour.begin(), 1), std::prev(tour.end(), 1));
		return Chromosome<Cost>(representation.GetTourCost(tour), tour);
	}
	// Returns vector of K randomly selected chromosomes from population
	template<typename Cost>
	inline std::vector<Chromosome<Cost>> Population<Cost>::GetKChromosomes(int k)
	{
		std::vector<Chromosome<Cost>> chromosomes;
		for (int i = 0; i < k; i++)		
			chromosomes.push_back(*RandomHelper::RandomElement(population_m.begin(), population_m.end()));
		return chromosomes;
	}
	// Returns the best chromosome with specified probability. Otherwise, returns random chromosome
	template<typename Cost>
	inline Chromosome<Cost> Population<Cost>::GetBestChromosomeWithProbability(double probability, std::vector<Chromosome<Cost>> &chromosomes)
	{
		return RandomHelper::EventOccured(probability) ? GetMostFit() : *RandomHelper::RandomElement(chromosomes.begin(), chromosomes.end());
	}
	template<typename Cost>
	inline Chromosome<Cost> Population<Cost>::TournamentSelection(int tournament_size)
	{
		return GetBestChromosomeWithProbability(0.9, GetKChromosomes(tournament_size));
	}
	template<typename Cost>
	inline Chromosome<Cost> Population<Cost>::RouletteSelection() {
		if (!roulette_generated) {
			GenerateRoulette();
			roulette_generated = true;
		}
		double value = RandomHelper::GetRandomInRange(0.0, 1.0) * weight_sum;
		auto it = std::lower_bound(sorted_weights.begin(), sorted_weights.end(), value);
		int index = it - sorted_weights.begin();
		return sorted_pop[index];
	}
	template<typename Cost>
	inline std::pair<Chromosome<Cost>, Chromosome<Cost>>  Population<Cost>::GetParents(Selection sel) {
		switch (sel)
		{
			case TSP::ROULETTE: {
				auto first_parent = RouletteSelection();
				auto second_parent = RouletteSelection();
				return std::make_pair(first_parent, second_parent);
			}
			
			case TSP::TOURNAMENT: {
				auto first_parent = TournamentSelection(10);
				auto second_parent = TournamentSelection(10);
				while (first_parent == second_parent)
					second_parent = TournamentSelection(10);
				return std::make_pair(first_parent, second_parent);
			}	
		}
	}
	template<typename Cost>
	inline void Population<Cost>::UpdateFitness(GraphRepresentation<Cost>& rep) {
		for (auto &it : population_m) {
			it.UpdateFitness(rep);
		}
		GenerateRoulette();
	}
	template<typename Cost>
	inline void Population<Cost>::GenerateRoulette() {
		sorted_pop = population_m;
		sorted_weights.clear();
		std::sort(sorted_pop.begin(), sorted_pop.end(), ChromosomeCompare<Cost>());
		// calculate the total weight
		weight_sum = 0.0;
		for (auto ch : sorted_pop) {
			weight_sum += ch.GetFitness();
			sorted_weights.push_back(weight_sum);
		}
	}
}
#endif // !TSP_POPULATION



