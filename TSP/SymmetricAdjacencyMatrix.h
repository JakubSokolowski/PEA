#pragma once
#include "GraphRepresentation.h"

#ifdef TSP_EXPORTS
#define TSP_API __declspec(dllexport)
#else
#define TSP_API __declspec(dllimport)
#endif
#define INF INT_MAX


namespace TSP
{
	using weight = int;

	class TSP_API SymmetricAdjacencyMatrix :
		public GraphRepresentation
	{
	public:
		SymmetricAdjacencyMatrix();
		SymmetricAdjacencyMatrix(uint vertices_num);
		SymmetricAdjacencyMatrix(std::string filename);
		SymmetricAdjacencyMatrix(std::vector<Vertex> &coordinates);
		SymmetricAdjacencyMatrix(std::vector<std::vector<weight>> matrix);
		virtual ~SymmetricAdjacencyMatrix();

		
		virtual void AddEdge(uint source, uint destination, uint weight) override;


		virtual const bool IsConnected(uint source, uint destination) override;
		virtual const weight GetWeight(uint source, uint destination) override;
		virtual const uint GetNumOfEdges() override;
		virtual const uint GetNumOfVertices() override;
		virtual const std::string GetGraphInfo() override;
		virtual const void Print() override;

	protected:
		std::vector<std::vector<weight>> adjacency_matrix_;		
	

	};
}
