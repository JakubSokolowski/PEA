#pragma once

#ifndef TSP_PMX
#define TSP_PMX

#include "Crossover.h"

namespace TSP
{	
	template<typename Cost>
	class PMX : public Crossover<Cost> {
	public:
		Chromosome<Cost> Execute(Chromosome<Cost> &c1, Chromosome<Cost> &c2);
		Chromosome<Cost> CrossoverAtPoints(Chromosome<Cost> &c1, Chromosome<Cost> &c2, int from, int to);
		int MapAllel(int allel);
	private:
		std::map<int, int> allel_map = std::map<int, int>();
	};	
	template<typename Cost>
	inline Chromosome<Cost> PMX<Cost>::Execute(Chromosome<Cost>& c1, Chromosome<Cost>& c2) {
		auto len = c1.tour.size() - 1;
		auto max = std::numeric_limits<Cost>::max();
		auto pair = RandomHelper::GetRandomPair(size_t(1), len - 2);
		return CrossoverAtPoints(c1, c2, pair[0], pair[1]);
	}
	template<typename Cost>
	inline Chromosome<Cost> PMX<Cost>::CrossoverAtPoints(Chromosome<Cost>& c1, Chromosome<Cost>& c2, int from, int to) {
		allel_map.clear();
		auto len = c2.tour.size();
		auto max = std::numeric_limits<Cost>::max();
		auto child_tour = std::vector<Cost>(len, Cost{});
		auto inserted = std::map<int, bool>();
		// Copy a random swath of consecutive allels from c1 to child
		for (int i = from; i <= to; i++) {
			child_tour[i] = c1.tour[i];
			inserted[c2.tour[i]] = true;
		}
		// Create c1:c2 swath mapping 
		for (int i = from; i <= to; i++) {
			allel_map[c1.tour[i]] = c2.tour[i];
		}

		uint child_index = 1;
		for (uint i = 1; i < len - 1; i++) {
			auto allel_to_map = c2.tour[i];
			if (inserted[allel_to_map])
				continue;
			while (child_tour[child_index] != Cost{})
				child_index++;
			auto mapped_allel = MapAllel(allel_to_map);
			child_tour[child_index] = MapAllel(c2.tour[i]);
			child_index++;
		}
		child_tour[0] = c1.tour[0];
		child_tour[len - 1] = c1.tour[len - 1];
		return Chromosome<Cost>(max, child_tour);
	}
	template<typename Cost>
	inline int PMX<Cost>::MapAllel(int allel) {
		while (true) {
			if (allel_map.find(allel) == allel_map.end())
				break;
			allel = allel_map[allel];
		}
		return allel;
	}
}
#endif



