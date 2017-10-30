#include "stdafx.h"
#include "HeldKarp.h"

using namespace TSP;

template <typename Cost>
HeldKarp<Cost>::HeldKarp()
{
}

template <typename Cost>
HeldKarp<Cost>::~HeldKarp()
{
}

template <typename Cost>
Solution<Cost> HeldKarp<Cost>::Solve(GraphRepresentation<Cost>& representation)
{
	auto states = PrepareStateMatrix(representation.GetNumOfVertices());
	Cost minimum = SolveSubset(representation, 0, 1, states);
	return Solution<Cost>(minimum);
}
template <typename Cost>
std::vector<std::vector<Cost>> HeldKarp<Cost>::PrepareStateMatrix(int matrix_size)
{
	//std::vector<std::vector<TSP::weight>> state_matrix(matrix_size, std::vector<TSP::weight>((1 << matrix_size) - 1, INT_MAX));

	std::vector<std::vector<Cost>> state_matrix(matrix_size);
	for (auto& neighbors : state_matrix)
		neighbors = std::vector<TSP::Cost>((1 << matrix_size) - 1, INT_MAX);

	return state_matrix;
}

template <typename Cost>
Cost HeldKarp<Cost>::SolveSubset(GraphRepresentation<Cost>& cities, int position, int visited, std::vector<std::vector<Cost>>& state)
{
	if (visited == ((1 << cities.GetNumOfVertices()) - 1))
		return cities.GetWeight(position, 0);

	if (state[position][visited] != INT_MAX)
		return state[position][visited];

	for (int i = 0; i < cities.GetNumOfVertices(); ++i)
	{
		if (i == position || (visited & (1 << i)))
			continue;
		int distance = cities.GetWeight(position,i) + SolveSubset(cities, i, visited | (1 << i), state);
		if (distance < state[position][visited])
			state[position][visited] = distance;
	}

	return state[position][visited];
}

template HeldKarp<int>;
template HeldKarp<unsigned int>;
template HeldKarp<double>;



