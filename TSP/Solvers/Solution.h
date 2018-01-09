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
		Solution(Cost total_cost);
		Solution(Cost total_cost, std::vector<int> path);
		Solution(const Solution<Cost> & other);

		Solution<Cost> operator = (Solution<Cost> & other);
		void PrintTour();
		~Solution();

		Cost total_cost = std::numeric_limits<Cost>::max();
		std::vector<int> tour;
	};

	template<typename Cost>
	TSP::Solution<Cost>::Solution()
	{}
	template<typename Cost>
	TSP::Solution<Cost>::Solution(Cost distance)
	{
		total_cost = distance;
	}
	template<typename Cost>
	TSP::Solution<Cost>::Solution(Cost distance, std::vector<int> path)
	{
		total_cost = distance;
		tour = path;
	}

	template<typename Cost>
	inline Solution<Cost>::Solution(const Solution<Cost> & other)
	{
		total_cost = other.total_cost;
		tour = other.tour;
	}


	template<typename Cost>
	inline Solution<Cost> Solution<Cost>::operator=(Solution<Cost>& other)
	{
		total_cost = other.total_cost;
		tour = other.tour;
		return *this;
	}

	template<typename Cost>
	inline void Solution<Cost>::PrintTour()
	{
		for (auto node : tour)		
			std::cout << node << " ";		
		std::cout << std::endl;
	}


	template<typename Cost>
	TSP::Solution<Cost>::~Solution()
	{
	}
}
#endif
