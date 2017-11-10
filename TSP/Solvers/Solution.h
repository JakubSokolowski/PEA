#pragma once

#ifndef TSP_SOLUTION
#define TSP_SOLUTION

#include <string>
#include <vector>
#include <limits>
#include <iostream>

namespace TSP
{
	template<typename Cost>
	class Solution
	{
	public:
		Solution();
		Solution(Cost distance);
		Solution(Cost distance, std::vector<int> path);
		Solution(const Solution<Cost> & other);

		Solution<Cost> operator = (Solution<Cost> & other);
		void PrintTour();
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
	inline Solution<Cost>::Solution(const Solution<Cost> & other)
	{
		distance_ = other.distance_;
		path_ = other.path_;
	}


	template<typename Cost>
	inline Solution<Cost> Solution<Cost>::operator=(Solution<Cost>& other)
	{
		distance_ = other.distance_;
		path_ = other.path_;
		return *this;
	}

	template<typename Cost>
	inline void Solution<Cost>::PrintTour()
	{
		for (auto node : path_)
		{
			std::cout << node << " ";
		}
		std::cout << std::endl;
	}


	template<typename Cost>
	TSP::Solution<Cost>::~Solution()
	{
	}
}
#endif
