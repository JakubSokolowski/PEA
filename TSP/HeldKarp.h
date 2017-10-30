#pragma once
#include "Heuristic.h"

namespace TSP
{
	template <typename Cost>
	class HeldKarp :
		public Heuristic<Cost>
	{
	public:	
		HeldKarp();
		virtual ~HeldKarp();

		// Inherited via Heuristic
		virtual Solution<Cost> Solve(GraphRepresentation<Cost>& representation) override;
	private:
		std::vector<std::vector<Cost>> PrepareStateMatrix(int matrix_size);
		Cost SolveSubset(GraphRepresentation<Cost>& representation, int position, int visited, std::vector<std::vector<Cost>>& state);
	};

}

