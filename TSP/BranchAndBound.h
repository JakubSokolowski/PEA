#pragma once

#ifndef TSP_BRANCH_AND_BOUND
#define TSP_BRANCH_AND_BOUND


#include "Heuristic.h"
#include <vector>
#include <bitset>

namespace TSP
{
	template <typename Cost>
	class BranchAndBound :
		public Heuristic<Cost>
	{
	public:
		BranchAndBound();
		virtual ~BranchAndBound();		

		// Inherited via Heuristic
		virtual Solution<Cost> Solve(GraphRepresentation<Cost> & representation) override;

	private:

		void TSPRec(GraphRepresentation<Cost> & adj, int level);
		void Prepare(GraphRepresentation<Cost> & representation);

		std::vector<int> curr_path;
		std::vector<int> final_path;
		Cost curr_bound = Cost();
		Cost curr_weight = Cost();
		int level_;
		Cost final_res = std::numeric_limits<Cost>::max();
		std::bitset<1000> visited;

		Cost FindMinimumEdgeCost(GraphRepresentation<Cost> &representation, uint vertex);
		Cost FindSecondMinimumEdgeCost(GraphRepresentation<Cost> &representation, uint vertex);

	};

	template <typename Cost>
	BranchAndBound<Cost>::BranchAndBound()
	{
	}

	template <typename Cost>
	BranchAndBound<Cost>::~BranchAndBound()
	{
	}

	template <typename Cost>
	Solution<Cost> BranchAndBound<Cost>::Solve(GraphRepresentation<Cost> & adj)
	{
		curr_weight = 0;
		level_ = 1;

		// Calculate initial lower bound for the root node
		// using the formula 1/2 * (sum of first min +
		// second min) for all edges.
		// Also initialize the curr_path and visited array
		Cost curr_bound = 0;
		curr_path = std::vector<int>(adj.GetNumOfVertices() + 1, -1);
		//memset(visited, 0, sizeof(curr_path));
		visited.reset();

		// Compute initial bound
		for (int i = 0; i<adj.GetNumOfVertices(); i++)
			curr_bound += (FindMinimumEdgeCost(adj, i) +
				FindSecondMinimumEdgeCost(adj, i));

		// Rounding off the lower bound to an integer
		curr_bound = (curr_bound & 1) ? curr_bound / 2 + 1 : curr_bound / 2;

		// We start at vertex 1 so the first vertex
		// in curr_path[] is 0
		visited[0] = true;
		curr_path[0] = 0;

		// Call to TSPRec for curr_weight equal to
		// 0 and level 1
		TSPRec(adj, 1);
		return Solution<Cost>(final_res, final_path);		
	}


	template <typename Cost>
	void TSP::BranchAndBound<Cost>::Prepare(GraphRepresentation<Cost> & adj)
	{


	}


	template <typename Cost>
	void TSP::BranchAndBound<Cost>::TSPRec(GraphRepresentation<Cost> & adj, int level)
	{

		int size = adj.GetNumOfVertices();
		// base case is when we have reached level N which
		// means we have covered all the nodes once
		if (level == size)
		{
			// check if there is an edge from last vertex in
			// path back to the first vertex		
			if (adj.GetWeight(curr_path[level - 1], curr_path[0]) != 0)
			{
				// curr_res has the total weight of the
				// solution we got
				Cost curr_res = curr_weight + adj.GetWeight(curr_path[level - 1], curr_path[0]);

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
			auto test = adj.GetWeight(curr_path[level - 1], i);
			if (adj.GetWeight(curr_path[level - 1], i) != 0 && visited[i] == false)
			{
				Cost temp = curr_bound;
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

	template <typename Cost>
	Cost TSP::BranchAndBound<Cost>::FindMinimumEdgeCost(GraphRepresentation<Cost> & representation, uint vertex)
	{
		Cost min = std::numeric_limits<Cost>::max();
		for (int i = 0; i < representation.GetNumOfVertices(); i++)
			if (representation.GetWeight(vertex, i) < min && vertex != i)
				min = representation.GetWeight(vertex, i);
		return min;
	}


	template <typename Cost>
	Cost TSP::BranchAndBound<Cost>::FindSecondMinimumEdgeCost(GraphRepresentation<Cost> & representation, uint i)
	{
		Cost first = std::numeric_limits<Cost>::max(), second = std::numeric_limits<Cost>::max();
		for (int j = 0; j<representation.GetNumOfVertices(); j++)
		{
			if (i == j)
				continue;
			Cost current_weight = representation.GetWeight(i, j);
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

}
#endif // !TSP_BRANCH_AND_BOUND
