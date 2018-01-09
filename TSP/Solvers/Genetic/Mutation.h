#pragma once

#ifndef TSP_MUTATION
#define TSP_MUTATION

#include "..\Solution.h"
#include "..\..\Graphs\GraphRepresentation.h"
#include "Chromosome.h"
#include "RandomHelper.h"
#include <algorithm>

namespace TSP
{
	template<typename Cost>
	class Mutation
	{
	public:
		static void Insertion(Chromosome<Cost> &chromosome, const GraphRepresentation<Cost> &representation);
		static void Swap(Chromosome<Cost> &chromosome, const GraphRepresentation<Cost> &representation);
		static void Scramble(Chromosome<Cost> &chromosome, const GraphRepresentation<Cost> &representation);
	private:		
		Mutation() {}
	};

	template<typename Cost>
	inline void Mutation<Cost>::Insertion(Chromosome<Cost>& chromosome, const GraphRepresentation<Cost>& representation)
	{
		// First and last city in the tour is const - city_id is 1, exclude it from mutation by subtracting 2
		int rnd_source = RandomHelper::GetRandomInRange(1, chromosome.tour.size() - 2);
		int rnd_destination = RandomHelper::GetRandomInRange(1, chromosome.tour.size() - 2);
		auto city = chromosome.tour.at(rnd_source);
		chromosome.tour.erase(chromosome.tour.begin() + rnd_source);
		// To take into account ereased ciry, if the destination index is bigger than source, insert before the destination, otherwise after
		chromosome.tour.insert(chromosome.tour.begin() + rnd_destination + ((rnd_source < rnd_destination) ? 1 : -1), city);
		// Calculate new tour cost
		chromosome.total_cost = representation.GetTourCost(chromosome.tour);
	}
	template<typename Cost>
	inline void Mutation<Cost>::Swap(Chromosome<Cost>& chromosome, const GraphRepresentation<Cost>& representation)
	{
		int rnd_source = RandomHelper::GetRandomInRange(1, chromosome.tour.size() - 2);
		int rnd_destination = RandomHelper::GetRandomInRange(1, chromosome.tour.size() - 2);
		auto temp = chromosome.tour.at(rnd_source);
		chromosome.tour[rnd_source] = chromosome.tour[rnd_destination];
		chromosome.tour[rnd_destination] = temp;
		chromosome.total_cost = representation.GetTourCost(chromosome.tour);
	}
	template<typename Cost>
	inline void Mutation<Cost>::Scramble(Chromosome<Cost>& chromosome, const GraphRepresentation<Cost>& representation)
	{
	
	}

}
#endif



