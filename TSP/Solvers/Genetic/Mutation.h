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
	enum MutationType {
		INSERTION,
		SWAP,
		SCRAMBLE
	};
	template<typename Cost>
	class Mutation
	{
	public:
		static void Insertion(Chromosome<Cost> &chromosome);
		static void InsertAtIndex(Chromosome<Cost> &chromosome, int from, int to);
		static void Swap(Chromosome<Cost> &chromosome);
		static void SwapAtIndices(Chromosome<Cost> &chromosome, int from, int to);
		static void Scramble(Chromosome<Cost> &chromosome);
		static void ScrambleBetweenIndices(Chromosome<Cost> &chromosome, int from, int to);
		static void Mutate(Chromosome<Cost> &chromosome, MutationType type);
	private:		
		Mutation() {}
	};

	template<typename Cost>
	inline void Mutation<Cost>::InsertAtIndex(Chromosome<Cost>& chromosome, int from, int to) {
		auto city = chromosome.tour.at(from);
		chromosome.tour.erase(chromosome.tour.begin() + from);
		// To take into account ereased ciry, if the destination index is bigger than source, insert before the destination, otherwise after
		chromosome.tour.insert(chromosome.tour.begin() + to, city);
	}
	template<typename Cost>
	inline void Mutation<Cost>::Insertion(Chromosome<Cost>& chromosome)
	{
		// First and last city in the tour is const - city_id is 1, exclude it from mutation by subtracting 2
		auto rnd_pair = RandomHelper::GetRandomPair(1, chromosome.tour.size() -2);
		InsertAtIndex(chromosome, rnd_pair[0], rnd_pair[1]);
	}
	template<typename Cost>
	inline void Mutation<Cost>::Swap(Chromosome<Cost>& chromosome)
	{
		int rnd_source = RandomHelper::GetRandomInRange(1, chromosome.tour.size() - 2);
		int rnd_destination = RandomHelper::GetRandomInRange(1, chromosome.tour.size() - 2);
		SwapAtIndices(chromosome, rnd_source, rnd_destination);
	}
	template<typename Cost>
	inline void Mutation<Cost>::SwapAtIndices(Chromosome<Cost>& chromosome, int from, int to) {
		auto temp = chromosome.tour.at(from);
		chromosome.tour[from] = chromosome.tour[to];
		chromosome.tour[to] = temp;
	}
	template<typename Cost>
	inline void Mutation<Cost>::Scramble(Chromosome<Cost>& chromosome)	{
		auto rnd_pair = RandomHelper::GetRandomPair(1, chromosome.tour.size() - 2);
		ScrambleBetweenIndices(chromosome, rnd_pair[0], rnd_pair[1]);
	}

	template<typename Cost>
	inline void Mutation<Cost>::ScrambleBetweenIndices(Chromosome<Cost>& chromosome, int from, int to) {
		RandomHelper::ShuffleFragment(chromosome.tour, from, to);
	}

	

	template<typename Cost>
	inline void Mutation<Cost>::Mutate(Chromosome<Cost>& chromosome, MutationType type) {
		switch (type) {
			case MutationType::INSERTION: {
				Insertion(chromosome);
			}
			case MutationType::SWAP: {
				Swap(chromosome);
			}
			case MutationType::SCRAMBLE: {
				Scramble(chromosome);
			}
		}
	}

}
#endif



