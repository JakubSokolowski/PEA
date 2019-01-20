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
		Population(const std::multiset<Chromosome<Cost>> &chromosomes);
		Population(const Population<Cost> &other, double elite_percentage);
		Population<Cost> operator = (const Population& other);

		~Population();
	
		int GetSize() const { return population_m.size(); }
		int GetMaxSize() const { return max_pop_size_m; }
		bool IsFull() { return GetSize() == GetMaxSize(); }

		void AddChromosome(Chromosome<Cost> &chromosome);

	
		Cost GetAvergeTourCost() const { return  Cost(std::accumulate(population_m.) / population_m.size()) }
		Chromosome<Cost> GetMostFit() const;
		std::vector<Chromosome<Cost>> GetMostFitPercentage(double percentage) const;	
		Chromosome<Cost> GetBestChromosomeWithProbability(double probability, std::multiset<Chromosome<Cost>> &chromosomes);
		std::multiset<Chromosome<Cost>> GetKChromosomes(int k);
		Chromosome<Cost> TournamentSelection(int tournament_size);
		Chromosome<Cost> RouletteSelection();
		std::pair<Chromosome<Cost>, Chromosome<Cost>> GetParents(Selection sel);

	private:

		int max_pop_size_m;
		int tournament_size = 10;
		std::multiset<Chromosome<Cost>> population_m;
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
	inline Population<Cost>::Population(const std::multiset<Chromosome<Cost>>& chromosomes)
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
	inline Population<Cost>::Population(const Population<Cost>& other, double elite_percentage) {
		auto elite = other.GetMostFitPercentage(elite_percentage);
		for (auto el : elite)
			population_m.insert(el);
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
			population_m.insert(chromosome);
	}

	// Returns the iterator to the most fit chromosome - the one with the highest 1 / tour_cost ratio
	template<typename Cost>
	inline Chromosome<Cost> Population<Cost>::GetMostFit() const
	{
		return *population_m.rbegin();
	}
	template<typename Cost>
	inline std::vector<Chromosome<Cost>> Population<Cost>::GetMostFitPercentage(double percentage) const {
		int n = std::round(percentage * population_m.size());
		auto result = std::vector<Chromosome<Cost>>();
		int counter = 0;
		for (auto it = population_m.rbegin(); it != population_m.rend(); ++it) {
			result.push_back(*it);
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
			population_m.insert(CreateRandomChromosome(representation));		
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
	// Returns multiset of K randomly selected chromosomes from population
	template<typename Cost>
	inline std::multiset<Chromosome<Cost>> Population<Cost>::GetKChromosomes(int k)
	{
		std::multiset<Chromosome<Cost>> chromosomes;
		for (int i = 0; i < k; i++)		
			chromosomes.insert(*RandomHelper::RandomElement(population_m.begin(), population_m.end()));
		return chromosomes;
	}
	// Returns the best chromosome with specified probability. Otherwise, returns random chromosome
	template<typename Cost>
	inline Chromosome<Cost> Population<Cost>::GetBestChromosomeWithProbability(double probability, std::multiset<Chromosome<Cost>> &chromosomes)
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
		// calculate the total weight
		double weight_sum = 0;
		for (auto ch : population_m) {
			weight_sum += ch.GetFitness();
		}
		// get a random value
		double value = RandomHelper::GetRandomInRange(0.0, 1.0) * weight_sum;
		// locate the random value based on the weights
		for (auto ch : population_m) {
			value -= ch.GetFitness();
			if (value < 0)
				return ch;
		}
		// when rounding errors occur, we return the last item's index 
		return *population_m.begin();
	}
	template<typename Cost>
	inline std::pair<Chromosome<Cost>, Chromosome<Cost>>  Population<Cost>::GetParents(Selection sel) {
		switch (sel)
		{
		case TSP::ROULETTE:
			auto first_parent = RouletteSelection();
			auto second_parent = RouletteSelection();
			while (first_parent == second_parent)
				second_parent = RouletteSelection();
			return std::make_pair(first_parent, second_parent);
		case TSP::TOURNAMENT:
			auto first_parent = TournamentSelection(10);
			auto second_parent = TournamentSelection(10);
			while (first_parent == second_parent)
				second_parent = TournamentSelection(10);
			return std::make_pair(first_parent, second_parent);
		default:
			break;
		}
		return Chromosome<Cost>();
	}
}
#endif // !TSP_POPULATION



