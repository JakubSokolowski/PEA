#pragma once


#ifndef TSP_SYMMETRIC_ADJACENCY_MATRIX
#define TSP_SYMMETRIC_ADJACENCY_MATRIX

#include <iomanip>
#include <iostream>
#include "..\GraphRepresentation.h"

#define INF INT_MAX

namespace TSP
{	

	template<typename Cost>
	class SymmetricAdjacencyMatrix :
		public GraphRepresentation<Cost>
	{
	public:
		SymmetricAdjacencyMatrix();
		SymmetricAdjacencyMatrix(uint vertices_num);
		SymmetricAdjacencyMatrix(std::string filename);
		SymmetricAdjacencyMatrix(std::vector<Vertex> &coordinates);
		SymmetricAdjacencyMatrix(std::vector<std::vector<Cost>> matrix);
		virtual ~SymmetricAdjacencyMatrix();

		
		virtual void AddEdge(uint source, uint destination, Cost cost_) override;

		Cost operator()(uint source, uint destination);		

		virtual bool IsConnected(uint source, uint destination) const override;
		virtual Cost GetWeight(uint source, uint destination) const override;
		virtual uint GetNumOfEdges() const override;
		virtual uint GetNumOfVertices() const override;
		virtual std::string GetGraphInfo()const override;
		virtual void Print() const override;

		std::vector<std::vector<Cost>> GetMatrix();

	protected:
		std::vector<std::vector<Cost>> adjacency_matrix_;
	
	};

	template<typename Cost>
	SymmetricAdjacencyMatrix<Cost>::SymmetricAdjacencyMatrix()
		: adjacency_matrix_(AllocateSquareMatrix(5))
	{
		edges_num_ = 0;
	}

	template<typename Cost>
	SymmetricAdjacencyMatrix<Cost>::SymmetricAdjacencyMatrix(uint vertices)
		: adjacency_matrix_(AllocateSquareMatrix(vertices))
	{
		edges_num_ = 0;
	}

	template<typename Cost>
	SymmetricAdjacencyMatrix<Cost>::SymmetricAdjacencyMatrix(std::string filename)
	{

	}

	template<typename Cost>
	TSP::SymmetricAdjacencyMatrix<Cost>::SymmetricAdjacencyMatrix(std::vector<Vertex> &coordinates)
	{
		coordinates_ = coordinates;
		adjacency_matrix_ = AllocateSquareMatrix(coordinates.size());
		ConvertCoordinatesToWeights(coordinates);
	}

	template<typename Cost>
	TSP::SymmetricAdjacencyMatrix<Cost>::SymmetricAdjacencyMatrix(std::vector<std::vector<Cost>> matrix)
	{
		adjacency_matrix_ = matrix;
	}

	template<typename Cost>
	SymmetricAdjacencyMatrix<Cost>::~SymmetricAdjacencyMatrix()
	{
	}

	template<typename Cost>
	void TSP::SymmetricAdjacencyMatrix<Cost>::AddEdge(uint source, uint destination, Cost weight)
	{		
		if (!IsConnected(source, destination))
		{
			adjacency_matrix_[source][destination] = weight;
			adjacency_matrix_[destination][source] = weight;
			edges_num_ += 1;
		}
	}

	template<typename Cost>
	Cost TSP::SymmetricAdjacencyMatrix<Cost>::operator()(uint source, uint destination)
	{
		return adjacency_matrix_[source][destination];
	}

	template<typename Cost>
	Cost TSP::SymmetricAdjacencyMatrix<Cost>::GetWeight(uint source, uint destination) const
	{
		return adjacency_matrix_[source][destination];
	}

	template<typename Cost>
	uint TSP::SymmetricAdjacencyMatrix<Cost>::GetNumOfEdges() const
	{
		return edges_num_;
	}

	template<typename Cost>
	uint TSP::SymmetricAdjacencyMatrix<Cost>::GetNumOfVertices() const
	{
		return adjacency_matrix_.size();
	}

	template<typename Cost>
	std::string TSP::SymmetricAdjacencyMatrix<Cost>::GetGraphInfo() const
	{
		return std::string();
	}

	template<typename Cost>
	void TSP::SymmetricAdjacencyMatrix<Cost>::Print() const
	{
		for (auto row : adjacency_matrix_)
		{
			for (auto vertex : row)
			{
				if (vertex == std::numeric_limits<Cost>::max())
					std::cout << "INF ";
				else
					std::cout << std::setw(3) << vertex << " ";
			}
			std::cout << std::endl;
		}
	}

	template<typename Cost>
	inline std::vector<std::vector<Cost>> SymmetricAdjacencyMatrix<Cost>::GetMatrix()
	{
		return adjacency_matrix_;
	}

	template<typename Cost>
	bool TSP::SymmetricAdjacencyMatrix<Cost>::IsConnected(uint source, uint destination) const
	{
		return adjacency_matrix_[source][destination] != std::numeric_limits<Cost>::max();
	}
}
#endif // !TSP_SYMMETRIC_ADJACENCY_MATRIX
