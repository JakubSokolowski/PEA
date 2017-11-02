#pragma once

#include "Solution.h"
#include "GraphRepresentation.h"

namespace TSP
{
	template<typename Cost>
	class PartialSolution :
		public Solution<Cost>
	{
	public:
		PartialSolution();
		PartialSolution(int n, std::vector<uint> &reduced);
		bool operator >(const PartialSolution& other) const;
		virtual ~PartialSolution();

	private:

		int enabled_edges_, disabled_edges_;
		int n = 0;
		int lower_bound_times_two_ = 0;
		std::vector<uint> reduced_;
		std::vector<int8_t> constraints_;		
	};

	template<typename Cost>
	TSP::PartialSolution<Cost>::PartialSolution()
	{
	}

	template<typename Cost>
	TSP::PartialSolution<Cost>::PartialSolution(int n, std::vector<uint>& reduced)
	{
	}

	template<typename Cost>
	bool TSP::PartialSolution<Cost>::operator>(const PartialSolution & other) const
	{
		return false;
	}

	template<typename Cost>
	TSP::PartialSolution<Cost>::~PartialSolution()
	{
	}
}
