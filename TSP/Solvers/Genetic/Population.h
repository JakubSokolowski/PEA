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
#include "RandomHelper.h"

namespace TSP
{
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
		Population<Cost> operator = (const Population& other);
		~Population();

		void AddChromosome(Chromosome<Cost> &chromosome);

		int GetSize() const { return population_m.size(); }
		int GetMaxSize() const { return max_pop_size_m; }
		Chromosome<Cost> GetMostFit() const;
		Cost GetAvergeTourCost() const { return  Cost(std::accumulate(population_m.) / population_m.size()) }
		Chromosome<Cost> GetBestChromosomeWithProbability(double probability, std::vector<Chromosome<Cost>> &chromosomes);
		std::vector<Chromosome<Cost>> GetKChromosomes(int k);
		Chromosome<Cost> TournamentSelection(int tournament_size);

	private:

		int max_pop_size_m;
		std::vector<Chromosome<Cost>> population_m;
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
		return *this;
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
	inline void Population<Cost>::GenerateRandomPopulation(GraphRepresentation<Cost>& representation)
	{
		for (int i = 0; i < max_pop_size_m; i++)		
			population_m.push_back(CreateRandomChromosome(representation));		
	}
	// Creates random chromosome for given graph. In tsp chromosome is combination of graph tour and the cost of this tour
	template<typename Cost>
	inline Chromosome<Cost> Population<Cost>::CreateRandomChromosome(GraphRepresentation<Cost>& representation)
	{
		srand(time(NULL));
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
		return RandomHelper::EventOccured(probability) ? *RandomHelper::RandomElement(chromosomes.begin(), chromosomes.end()) : GetMostFit();
	}
	template<typename Cost>
	inline Chromosome<Cost> Population<Cost>::TournamentSelection(int tournament_size)
	{
		return GetBestChromosomeWithProbability(0.9, GetKChromosomes(tournament_size));
	}
}
#endif // !TSP_POPULATION



