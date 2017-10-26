#pragma once
#include "Heuristic.h"

namespace TSP
{
	class TSP_API HeldKarp :
		public Heuristic
	{
	public:
		HeldKarp();
		virtual ~HeldKarp();

		// Inherited via Heuristic
		virtual Solution Solve(GraphRepresentation& representation) override;
	private:
		std::vector<std::vector<TSP::weight>> PrepareStateMatrix(int matrix_size);
		int SolveSubset(GraphRepresentation& representation, int position, int visited, std::vector<std::vector<TSP::weight>>& state);
	};

}

