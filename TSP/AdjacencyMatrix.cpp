#include "stdafx.h"
#include "AdjacencyMatrix.h"

using namespace TSP;

AdjacencyMatrix::AdjacencyMatrix()
	: vertices_num_(5)
	, edges_num_(0)
	, adjacency_matrix_(AllocateSquareMatrix(5))
{}

TSP::AdjacencyMatrix::AdjacencyMatrix(uint vertices)
	: vertices_num_(vertices)
	, edges_num_(0)
	, adjacency_matrix_(AllocateSquareMatrix(vertices))
{}

TSP::AdjacencyMatrix::AdjacencyMatrix(std::string filename)
{
}


AdjacencyMatrix::~AdjacencyMatrix()
{
}

void TSP::AdjacencyMatrix::AddEdge(uint source, uint destination, uint weight)
{
	if (!IsConnected(source, destination))
		adjacency_matrix_[source][destination] = 0;
}

weight TSP::AdjacencyMatrix::GetWeight(uint source, uint destination)
{
	return adjacency_matrix_[source][destination];
}

uint TSP::AdjacencyMatrix::GetNumOfEdges()
{
	return edges_num_;
}

uint TSP::AdjacencyMatrix::GetNumOfVertices()
{
	return vertices_num_;
}

std::string TSP::AdjacencyMatrix::GetGraphInfo()
{
	return std::string();
}

bool TSP::AdjacencyMatrix::IsConnected(uint source, uint destination)
{
	return adjacency_matrix_[source][destination] >= 0;
}
