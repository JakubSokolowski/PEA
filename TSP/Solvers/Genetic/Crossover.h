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

namespace TSP
{
	template<typename Cost>
	class Population
	{
	public:
		Population();
		Population(int population_size, GraphRepresentation<Cost>& representation);
		Population(const Population<Cost> &other);
		Population<Cost> operator = (const Population& other);

		int GetSize() const { return population_m.size(); }
		Chromosome<Cost> GetMostFit() const;
		Cost GetAvergeTourCost() const { return  Cost(std::accumulate(population_m.) / population_m.size()) }

		~Population();
	private:
		int population_size_m;
		std::deque<Chromosome<Cost>> population_m;
		void GenerateRandomPopulation(GraphRepresentation<Cost> &representation);
		Chromosome<Cost> CreateRandomChromosome(GraphRepresentation<Cost> &representation);
	};
	template<typename Cost>
	inline Population<Cost>::Population()
		: population_size_m(10)
	{}
	template<typename Cost>
	inline Population<Cost>::Population(int population_size, GraphRepresentation<Cost>& representation)
		: population_size_m(population_size)
	{
		GenerateRandomPopulation(representation);
	}
	template<typename Cost>
	inline Population<Cost>::Population(const Population<Cost>& other)
	{
		population_m = other.population_m;
		population_size_m = other.population_size_m;
	}
	template<typename Cost>
	inline Chromosome<Cost> Population<Cost>::GetMostFit() const
	{
		return *std::max_element(population_m.begin(), population_m.end(), ChromosomeCompare<Cost>());
	}
	template<typename Cost>
	inline Population<Cost> Population<Cost>::operator=(const Population & other)
	{
		population_m = other.population_m;
		population_size_m = other.population_size_m;
		return *this;
	}
	template<typename Cost>
	inline Population<Cost>::~Population()
	{
	}
	template<typename Cost>
	inline void Population<Cost>::GenerateRandomPopulation(GraphRepresentation<Cost>& representation)
	{
		for (int i = 0; i < population_size_m; i++)		
			population_m.push_back(CreateRandomChromosome(representation));		
	}
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
}
#endif // !TSP_POPULATION



