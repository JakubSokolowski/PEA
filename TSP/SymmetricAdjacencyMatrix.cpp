#include "stdafx.h"
#include "SymmetricAdjacencyMatrix.h"
#include <iostream>
#include <iomanip>

using namespace TSP;

SymmetricAdjacencyMatrix::SymmetricAdjacencyMatrix()
	: adjacency_matrix_(AllocateSquareMatrix(5))	
{	
	edges_num_ = 0;
}

TSP::SymmetricAdjacencyMatrix::SymmetricAdjacencyMatrix(uint vertices)	
	: adjacency_matrix_(AllocateSquareMatrix(vertices))
{
	edges_num_ = 0;
}

TSP::SymmetricAdjacencyMatrix::SymmetricAdjacencyMatrix(std::string filename)
{

}

TSP::SymmetricAdjacencyMatrix::SymmetricAdjacencyMatrix(std::vector<Vertex> &coordinates)
{
	coordinates_ = coordinates;	
	adjacency_matrix_ = AllocateSquareMatrix(coordinates.size());
	ConvertCoordinatesToWeights(coordinates);
}

TSP::SymmetricAdjacencyMatrix::SymmetricAdjacencyMatrix(std::vector<std::vector<weight>> matrix)
{
	adjacency_matrix_ = matrix;	
}


SymmetricAdjacencyMatrix::~SymmetricAdjacencyMatrix()
{
}

void TSP::SymmetricAdjacencyMatrix::AddEdge(uint source, uint destination, uint weight)
{
	if (!IsConnected(source, destination))
	{
		adjacency_matrix_[source][destination] = weight;
		adjacency_matrix_[destination][source] = weight;
	}		
}

const weight TSP::SymmetricAdjacencyMatrix::GetWeight(uint source, uint destination)
{
	return adjacency_matrix_[source][destination];
}

const uint TSP::SymmetricAdjacencyMatrix::GetNumOfEdges()
{
	return edges_num_;
}

const uint TSP::SymmetricAdjacencyMatrix::GetNumOfVertices()
{
	return adjacency_matrix_.size();
}

const std::string TSP::SymmetricAdjacencyMatrix::GetGraphInfo()
{
	return std::string();
}

const void TSP::SymmetricAdjacencyMatrix::Print()
{
	for (auto row : adjacency_matrix_)
	{
		for (auto vertex : row)
		{
			if (vertex == INT_MAX)
				std::cout << "INF ";
			else
				std::cout << std::setw(3) << vertex << " ";
		}
		std::cout << std::endl;
	}
}

const bool TSP::SymmetricAdjacencyMatrix::IsConnected(uint source, uint destination)
{
	return adjacency_matrix_[source][destination] != INT_MAX;
}
