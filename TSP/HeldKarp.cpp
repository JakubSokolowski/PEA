#include "stdafx.h"
#include "HeldKarp.h"

using namespace TSP;

HeldKarp::HeldKarp()
{
}


HeldKarp::~HeldKarp()
{
}

Solution TSP::HeldKarp::Solve(GraphRepresentation& representation)
{
	auto states = PrepareStateMatrix(representation.GetNumOfVertices());
	int minimum = SolveSubset(representation, 0, 1, states);
	return Solution(minimum);
}

std::vector<std::vector<TSP::weight>> TSP::HeldKarp::PrepareStateMatrix(int matrix_size)
{
	//std::vector<std::vector<TSP::weight>> state_matrix(matrix_size, std::vector<TSP::weight>((1 << matrix_size) - 1, INT_MAX));

	std::vector<std::vector<TSP::weight>> state_matrix(matrix_size);
	for (auto& neighbors : state_matrix)
		neighbors = std::vector<TSP::weight>((1 << matrix_size) - 1, INT_MAX);

	return state_matrix;
}

int TSP::HeldKarp::SolveSubset(GraphRepresentation& cities, int position, int visited, std::vector<std::vector<TSP::weight>>& state)
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


