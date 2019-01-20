#pragma once

#ifndef TSP_OX
#define TSP_OX

#include "Crossover.h"

namespace TSP
{	
	template<typename Cost>
	class OX : public Crossover<Cost> {
	public:
		Chromosome<Cost> Execute(Chromosome<Cost> &c1, Chromosome<Cost> &c2);
		Chromosome<Cost> CrossoverAtPoints(Chromosome<Cost> &c1, Chromosome<Cost> &c2, int from, int to);
	};

	template<typename Cost>
	inline Chromosome<Cost> OX<Cost>::Execute(Chromosome<Cost> &c1, Chromosome<Cost> &c2) {
		auto len = c1.tour.size();
		auto max = std::numeric_limits<Cost>::max();
		auto pair = RandomHelper::GetRandomPair(size_t(1), len - 2);
		return CrossoverAtPoints(c1, c2, pair[0], pair[1]);
	}
	template<typename Cost>
	inline Chromosome<Cost> OX<Cost>::CrossoverAtPoints(Chromosome<Cost> &c1, Chromosome<Cost> &c2, int from, int to) {
		auto len = c2.tour.size();
		auto max = std::numeric_limits<Cost>::max();
		auto child_tour = std::vector<int>(len, std::numeric_limits<int>::max());
		auto inserted = std::map<int, bool>();
		for (int i = from; i <= to; i++) {
			child_tour[i] = c1.tour[i];
			inserted[c1.tour[i]] = true;
		}
		uint child_index = 0;
		for (uint i = 0; i < len; i++) {
			if (inserted[c2.tour[i]])
				continue;
			while (child_tour[child_index] != max)
				child_index++;
			child_tour[child_index] = c2.tour[i];
			child_index++;
		}
		return Chromosome<Cost>(max, child_tour);
	}
}
#endif



