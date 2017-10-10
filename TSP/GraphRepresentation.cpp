#include "stdafx.h"
#include "GraphRepresentation.h"

using namespace TSP;

weight ** TSP::GraphRepresentation::AllocateSquareMatrix(uint matrix_size)
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

