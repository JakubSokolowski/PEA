#include "stdafx.h"
#include "Solution.h"

using namespace TSP;

Solution::Solution()
{
}

TSP::Solution::Solution(int distance)
{
	distance_ = distance;
}

TSP::Solution::Solution(int distance, std::vector<int> path)
{
	distance_ = distance;
	path_ = path;
}


Solution::~Solution()
{
}
