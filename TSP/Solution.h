#pragma once

#ifndef TSP_SOLUTION
#define TSP_SOLUTION

#include <string>
#include <vector>
#include <limits>

namespace TSP
{
	template<typename Cost>
	class Solution
	{
	public:
		Solution();
		Solution(Cost distance);
		Solution(Cost distance, std::vector<int> path);
		~Solution();

		Cost distance_ = std::numeric_limits<Cost>::max();
		std::vector<int> path_;
	};

	template<typename Cost>
	TSP::Solution<Cost>::Solution()
	{
	}

	template<typename Cost>
	TSP::Solution<Cost>::Solution(Cost distance)
	{
		distance_ = distance;
	}

	template<typename Cost>
	TSP::Solution<Cost>::Solution(Cost distance, std::vector<int> path)
	{
		distance_ = distance;
		path_ = path;
	}


	template<typename Cost>
	TSP::Solution<Cost>::~Solution()
	{
	}
}
#endif
