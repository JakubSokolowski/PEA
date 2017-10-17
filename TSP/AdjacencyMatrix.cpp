#include "stdafx.h"
#include "AdjacencyMatrix.h"

using namespace TSP;

AdjacencyMatrix::AdjacencyMatrix()	
	: adjacency_matrix_(AllocateSquareMatrix(5))
{
	vertices_num_ = 5;
	edges_num_ = 0;
}

TSP::AdjacencyMatrix::AdjacencyMatrix(uint vertices)	
	: adjacency_matrix_(AllocateSquareMatrix(vertices))
{
	vertices_num_ = vertices;
	edges_num_ = 0;
}

TSP::AdjacencyMatrix::AdjacencyMatrix(std::string filename)
{
}

TSP::AdjacencyMatrix::AdjacencyMatrix(std::vector<Vertex> &coordinates)
{
	coordinates_ = coordinates;
	vertices_num_ = coordinates_.size();
	adjacency_matrix_ = AllocateSquareMatrix(coordinates.size());
	ConvertCoordinatesToWeights();
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
