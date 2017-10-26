#include "stdafx.h"
#include "BranchAndBound.h"

using namespace TSP;

BranchAndBound::BranchAndBound()
{
}


BranchAndBound::~BranchAndBound()
{
}

Solution TSP::BranchAndBound::Solve(GraphRepresentation & adj)
{
	curr_weight = 0;
	level_ = 1;

	// Calculate initial lower bound for the root node
	// using the formula 1/2 * (sum of first min +
	// second min) for all edges.
	// Also initialize the curr_path and visited array
	int curr_bound = 0;
	curr_path = std::vector<int>(adj.GetNumOfVertices() + 1, -1);
	//memset(visited, 0, sizeof(curr_path));
	visited.reset();

	// Compute initial bound
	for (int i = 0; i<adj.GetNumOfVertices(); i++)
		curr_bound += (FindMinimumEdgeCost(adj, i) +
			FindSecondMinimumEdgeCost(adj, i));

	// Rounding off the lower bound to an integer
	curr_bound = (curr_bound & 1) ? curr_bound / 2 + 1 :
		curr_bound / 2;

	// We start at vertex 1 so the first vertex
	// in curr_path[] is 0
	visited[0] = true;
	curr_path[0] = 0;

	// Call to TSPRec for curr_weight equal to
	// 0 and level 1
	TSPRec(adj, 1);

	return Solution(final_res, final_path);
}

void TSP::BranchAndBound::Prepare(GraphRepresentation & adj)
{

	curr_weight = 0;
	level_ = 1;

	// Calculate initial lower bound for the root node
	// using the formula 1/2 * (sum of first min +
	// second min) for all edges.
	// Also initialize the curr_path and visited array
	int curr_bound = 0;
	curr_path = std::vector<int>(adj.GetNumOfVertices() + 1, -1);
	//memset(visited, 0, sizeof(curr_path));
	visited.reset();

	// Compute initial bound
	for (int i = 0; i<adj.GetNumOfVertices(); i++)
		curr_bound += (FindMinimumEdgeCost(adj, i) +
			FindSecondMinimumEdgeCost(adj, i));

	// Rounding off the lower bound to an integer
	curr_bound = (curr_bound & 1) ? curr_bound / 2 + 1 :
		curr_bound / 2;

	// We start at vertex 1 so the first vertex
	// in curr_path[] is 0
	visited[0] = true;
	curr_path[0] = 0;

	// Call to TSPRec for curr_weight equal to
	// 0 and level 1
	TSPRec(adj, 1);
}

void TSP::BranchAndBound::TSPRec(GraphRepresentation & adj, int level)
{

	int size = adj.GetNumOfVertices();
	// base case is when we have reached level N which
	// means we have covered all the nodes once
	if (level == size)
	{
		// check if there is an edge from last vertex in
		// path back to the first vertex
		if (adj.GetWeight(curr_path[level-1],curr_path[0]) != 0)
		{
			// curr_res has the total weight of the
			// solution we got
			weight curr_res = curr_weight + adj.GetWeight(curr_path[level - 1], curr_path[0]);

			// Update final result and final path if
			// current result is better.
			if (curr_res < final_res)
			{
				final_path = curr_path;
				final_res = curr_res;
			}
		}
		return;
	}

	// for any other level iterate for all vertices to
	// build the search space tree recursively
	for (int i = 0; i< size; i++)
	{
		// Consider next vertex if it is not same (diagonal
		// entry in adjacency matrix and not visited
		// already)
		int test = adj.GetWeight(curr_path[level - 1], i);
		if (adj.GetWeight(curr_path[level - 1], i) != 0 && visited[i] == false)
		{
			int temp = curr_bound;		
			curr_weight += adj.GetWeight(curr_path[level - 1], i);

			// different computation of curr_bound for
			// level 2 from the other levels
			if (level == 1)
				curr_bound -= ((FindMinimumEdgeCost(adj, curr_path[level - 1]) +
					FindMinimumEdgeCost(adj, i)) / 2);
			else
				curr_bound -= ((FindSecondMinimumEdgeCost(adj, curr_path[level - 1]) +
					FindMinimumEdgeCost(adj, i)) / 2);

			// curr_bound + curr_weight is the actual lower bound
			// for the node that we have arrived on
			// If current lower bound < final_res, we need to explore
			// the node further
			if (curr_bound + curr_weight < final_res)
			{
				curr_path[level] = i;
				visited[i] = true;

				// call TSPRec for the next level			
				TSPRec(adj, level + 1);
			}

			// Else we have to prune the node by resetting
			// all changes to curr_weight and curr_bound			
			curr_weight -= adj.GetWeight(curr_path[level - 1], i);
			curr_bound = temp;

			// Also reset the visited array
			visited.reset();			
			for (int j = 0; j <= level - 1; j++)
				visited[curr_path[j]] = true;
		}
	}
}


// Finds minimum edge cost that has end at the vertex
weight TSP::BranchAndBound::FindMinimumEdgeCost(GraphRepresentation & representation, uint vertex)
{
	int min = INT_MAX;
	for (int i = 0; i < representation.GetNumOfVertices(); i++)
		if (representation.GetWeight(vertex, i) < min && vertex != i)
			min = representation.GetWeight(vertex, i);
	return min;
}

weight TSP::BranchAndBound::FindSecondMinimumEdgeCost(GraphRepresentation & representation, uint i)
{
	weight first = INT_MAX, second = INT_MAX;
	for (int j = 0; j<representation.GetNumOfVertices(); j++)
	{
		if (i == j)
			continue;
		weight current_weight = representation.GetWeight(i, j);
		if (current_weight <= first)
		{
			second = first;
			first = current_weight;
		}
		else if (current_weight <= second && current_weight != first)
			second = current_weight;
	}
	return second;
}
