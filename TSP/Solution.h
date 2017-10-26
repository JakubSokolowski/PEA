#pragma once

#include <vector>

#ifdef TSP_EXPORTS
#define TSP_API __declspec(dllexport)
#else
#define TSP_API __declspec(dllimport)
#endif

namespace TSP
{
	class TSP_API Solution
	{
	public:
		Solution();
		Solution(int distance);
		Solution(int distance, std::vector<int> path);
		~Solution();

		int distance_;
		std::vector<int> path_;

	};
}
