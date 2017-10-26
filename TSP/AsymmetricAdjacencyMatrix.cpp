#include "stdafx.h"
#include "AsymmetricAdjacencyMatrix.h"

using namespace TSP;


AsymmetricAdjacencyMatrix::AsymmetricAdjacencyMatrix()
{

}

TSP::AsymmetricAdjacencyMatrix::AsymmetricAdjacencyMatrix(int size) : SymmetricAdjacencyMatrix(size)
{
}


AsymmetricAdjacencyMatrix::~AsymmetricAdjacencyMatrix()
{
}

void TSP::AsymmetricAdjacencyMatrix::AddEdge(int source, uint destination, uint weight)
{
	if(!IsConnected(source,destination))
		adjacency_matrix_[source][destination] = weight;
	edges_num_++;
}
