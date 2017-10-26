#include "stdafx.h"
#include "GraphRepresentation.h"
#include <math.h>

using namespace TSP;

// Returns square matrix of given size, with all values set to INT_MAX
std::vector<std::vector<weight>> GraphRepresentation::AllocateSquareMatrix(uint matrix_size)
{	
	 std::vector<std::vector<weight>> matrix(matrix_size,std::vector<weight>(matrix_size, INT_MAX));
	 return matrix;
}

// Converts the vector of Vertices to representation - computes all the edges and adds them to
// Graph representation. The resulting graph will have 100% density
void GraphRepresentation::ConvertCoordinatesToWeights(std::vector<Vertex> &coordinates)
{
	uint destination_vertex = 1;
	for (uint i = 0; i < coordinates.size() - 1; i++)
	{
		for (uint j = destination_vertex; j < coordinates.size(); j++)
		{
			edges_num_++;
			weight edge_weight = GetEuclideanDistance(coordinates[i], coordinates[j]);
			AddEdge(i, j, edge_weight);
		}
		destination_vertex++;
	}	
}

// Returns the euclidean distance (straight line) beetween two Vertices. The result is casted on
// whatever type weight is (presumably int)
weight GraphRepresentation::GetEuclideanDistance(Vertex source, Vertex destination)
{
	int X = source.x_ - destination.x_;
	int Y = source.y_ - destination.y_;
	return (weight)(sqrt(pow(X, 2) + pow(Y, 2)));
}
