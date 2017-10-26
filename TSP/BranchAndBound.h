#pragma once
#include "Heuristic.h"
#include <vector>
#include <bitset>

namespace TSP
{
	class TSP_API BranchAndBound :
		public Heuristic
	{
	public:
		BranchAndBound();
		virtual ~BranchAndBound();		

		// Inherited via Heuristic
		virtual Solution Solve(GraphRepresentation & representation) override;

	private:

		void TSPRec(GraphRepresentation & adj, int level);
		void Prepare(GraphRepresentation & representation);

		std::vector<int> curr_path;
		std::vector<int> final_path;
		int curr_bound;
		int curr_weight;
		int level_;
		int final_res = INT_MAX;
		std::bitset<1000> visited;

		weight FindMinimumEdgeCost(GraphRepresentation &representation, uint vertex);
		weight FindSecondMinimumEdgeCost(GraphRepresentation &representation, uint vertex);

	};


}
