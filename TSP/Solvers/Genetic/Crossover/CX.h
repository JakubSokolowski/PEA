#pragma once

#ifndef TSP_CX
#define TSP_CX

#include "Crossover.h"

namespace TSP
{	
	using Cycle = std::vector<int>;
	template<typename Cost>
	class CX : public Crossover<Cost> {
	public:
		Chromosome<Cost> Execute(Chromosome<Cost> &c1, Chromosome<Cost> &c2);
		void FillMaps(Chromosome<Cost> &c1, Chromosome<Cost> &c2);
		Cycle FindCycle(uint index, Chromosome<Cost> &c1, Chromosome<Cost> &c2);	
		std::vector<Cycle> FindAllCycles(Chromosome<Cost>&c1, Chromosome<Cost> &c2);
		std::map<int, int> c1_index, c2_index, cycles, c1_cycle, c2_cycle;
		uint cycle_counter = 0;
		void Reset();
		bool IsPartOfCycle(int allel);
		std::vector<int> CreateChildTour(Chromosome<Cost> &c1, Chromosome<Cost> &c2);
		std::vector<int> TakeFirstParentChunk(Chromosome<Cost> &c1, int index);
		std::vector<int> TakeSecondParentChunk(Chromosome<Cost> &c2, int index);
	};

	template<typename Cost>
	inline Chromosome<Cost> CX<Cost>::Execute(Chromosome<Cost> &c1, Chromosome<Cost> &c2) {	
		Reset();
		FillMaps(c1, c2);
		FindAllCycles(c1, c2);
		auto max = std::numeric_limits<int>::max();
		auto tour = CreateChildTour(c1, c2);
		return Chromosome<Cost>(max, tour);
	}

	template<typename Cost>
	inline void CX<Cost>::FillMaps(Chromosome<Cost>& c1, Chromosome<Cost>& c2) {
		for (uint i = 0; i < c1.tour.size(); i++) {
			c1_index[c1.tour[i]] = i;
			c2_index[c2.tour[i]] = i;
			cycles[i] = false;
			c1_cycle[i] = -1;
			c2_cycle[i] = -1;
		}
	}

	template<typename Cost>
	inline Cycle CX<Cost>::FindCycle(uint index, Chromosome<Cost>& c1, Chromosome<Cost>& c2) {
		auto cycle = Cycle();
		cycle.push_back(c1.tour[index]);
		int c1_allel = cycle[0], c2_allel = 0;
		while (true) {
			c2_allel = c2.tour[index];
			if (c2_allel == cycle[0])
				break;
			cycle.push_back(c2_allel);
			index = c1_index[c2_allel];
		}
		for (auto allel : cycle) {
			cycles[allel] = true;
			c1_cycle[allel] = cycle_counter;
			c2_cycle[allel] = cycle_counter;
		}
			
		cycle_counter++;
		return cycle;
	}

	template<typename Cost>
	inline std::vector<Cycle> CX<Cost>::FindAllCycles(Chromosome<Cost>& c1, Chromosome<Cost>& c2) {
		std::vector<Cycle> cycles;
		for (uint i = 0; i < c1.tour.size(); i++) {
			if (!IsPartOfCycle(c1.tour[i])) {
				cycles.push_back(FindCycle(i, c1, c2));
			}
		}
		return cycles;
	}

	template<typename Cost>
	inline void CX<Cost>::Reset() {
		cycle_counter = 0;
		c1_index.clear();
		c2_index.clear();
		c1_cycle.clear();
		c2_cycle.clear();
		cycles.clear();
	}

	template<typename Cost>
	inline bool CX<Cost>::IsPartOfCycle(int allel) {
		return cycles[allel];
	}

	template<typename Cost>
	inline std::vector<int> CX<Cost>::CreateChildTour(Chromosome<Cost>& c1, Chromosome<Cost>& c2) {
		std::vector<int> result;
		bool first_parent_is_source = true;
		int index = 0;
		while (true) {
			if (index == c1.tour.size())
				break;
			std::vector<int> chunk;
			if (first_parent_is_source)
				chunk = TakeFirstParentChunk(c1, index);
			else
				chunk = TakeSecondParentChunk(c2, index);
			first_parent_is_source = !first_parent_is_source;
			index += chunk.size();
			result.insert(result.end(), chunk.begin(), chunk.end());
		}
		return result;
	}

	template<typename Cost>
	inline std::vector<int> CX<Cost>::TakeFirstParentChunk(Chromosome<Cost> &c1, int index) {
		std::vector<int> result;
		auto cycle_id = c1_cycle[c1.tour[index]];
		while (true) {
			if ( index == c1.tour.size() || c1_cycle[c1.tour[index]] != cycle_id)
				break;
			result.push_back(c1.tour[index]);
			index++;
		}
		return result;
	}

	template<typename Cost>
	inline std::vector<int> CX<Cost>::TakeSecondParentChunk(Chromosome<Cost>& c2, int index) {
		std::vector<int> result;
		auto cycle_id = c2_cycle[c2.tour[index]];
		while (true) {
			if (index == c2.tour.size() || c2_cycle[c2.tour[index]] != cycle_id)
				break;
			result.push_back(c2.tour[index]);
			index++;
		}
		return result;
	}


	
}
#endif



