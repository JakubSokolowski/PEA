#pragma once

#ifndef TSP_ASYMMETRIC_ADJACENCY_MATRIX
#define TSP_ASYMMETRIC_ADJACENCY_MATRIX

#include "SymmetricAdjacencyMatrix.h"

namespace TSP
{
	template<typename Cost>
	class AsymmetricAdjacencyMatrix :
		public SymmetricAdjacencyMatrix<Cost>
	{
	public:
		AsymmetricAdjacencyMatrix();
		AsymmetricAdjacencyMatrix(int size);
		AsymmetricAdjacencyMatrix(std::vector<Vertex> &coordinates);
		AsymmetricAdjacencyMatrix(std::vector<std::vector<Cost>> &matrix);
		virtual ~AsymmetricAdjacencyMatrix();
		void AddEdge(int source, uint destination, Cost weigh);		
	};

	template<typename Cost>
	AsymmetricAdjacencyMatrix<Cost>::AsymmetricAdjacencyMatrix()
	{

	}
	template<typename Cost>
	TSP::AsymmetricAdjacencyMatrix<Cost>::AsymmetricAdjacencyMatrix(int size) : SymmetricAdjacencyMatrix(size)
	{

	}

	template<typename Cost>
	inline AsymmetricAdjacencyMatrix<Cost>::AsymmetricAdjacencyMatrix(std::vector<Vertex>& coordinates)	 
		: SymmetricAdjacencyMatrix(coordinates)
	{
	}

	template<typename Cost>
	inline AsymmetricAdjacencyMatrix<Cost>::AsymmetricAdjacencyMatrix(std::vector<std::vector<Cost>>& matrix)
		: SymmetricAdjacencyMatrix(matrix)
	{
	}

	template<typename Cost>
	AsymmetricAdjacencyMatrix<Cost>::~AsymmetricAdjacencyMatrix()
	{
	}
	template<typename Cost>
	void TSP::AsymmetricAdjacencyMatrix<Cost>::AddEdge(int source, uint destination, Cost weight)
	{
		if (!IsConnected(source, destination))
		{
			adjacency_matrix_[source][destination] = weight;
			edges_num_++;
		}		
	}
}

#endif // !TSP_ASYMMETRIC_ADJACENCY_MATRIX
