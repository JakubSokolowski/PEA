#include "stdafx.h"
#include "GraphRepresentation.h"
#include <math.h>

using namespace TSP;

weight ** GraphRepresentation::AllocateSquareMatrix(uint matrix_size)
{
	weight **matrix = new weight*[matrix_size];
	for (uint i = 0; i < matrix_size; i++)
	{
		matrix[i] = new weight[matrix_size];
		for (uint j = 0; j < matrix_size; j++)
			matrix[i][j] = -1;
	}		
	return matrix;
}

void GraphRepresentation::ConvertCoordinatesToWeights()
{
	uint destination_vertex = 1;
	for (uint i = 0; i < coordinates_.size() - 1; i++)
	{
		for (uint j = destination_vertex; j < coordinates_.size(); j++)
		{
			edges_num_++;
			weight edge_weight = GetEuclideanDistance(coordinates_[i], coordinates_[j]);
			AddEdge(i, j, edge_weight);
		}
		destination_vertex++;
	}
}

weight GraphRepresentation::GetEuclideanDistance(Vertex source, Vertex destination)
{
	int X = source.x_ - destination.x_;
	int Y = source.y_ - destination.y_;
	return (sqrt(pow(X, 2) + pow(Y, 2)));
}
