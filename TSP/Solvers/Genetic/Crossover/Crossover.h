#pragma once

#ifndef TSP_CROSSOVER
#define TSP_CROSSOVER

#include "../Chromosome.h"
#include "../RandomHelper.h"
#include <map>

namespace TSP
{
	template<typename Cost>
	class Crossover {
	public:
		Crossover();
		virtual ~Crossover();
		virtual Chromosome<Cost> Execute(Chromosome<Cost> &c1, Chromosome<Cost> &c2) = 0;
	};
	template<typename Cost>
	inline Crossover<Cost>::Crossover() {
	}

	template<typename Cost>
	inline Crossover<Cost>::~Crossover() {
	}
}
#endif



