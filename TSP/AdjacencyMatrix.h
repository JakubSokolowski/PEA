#pragma once
#include "GraphRepresentation.h"

#ifdef TSP_EXPORTS
#define TSP_API __declspec(dllexport)
#else
#define TSP_API __declspec(dllimport)
#endif


namespace TSP
{
	using weight = double;

	class TSP_API AdjacencyMatrix :
		public GraphRepresentation
	{
	public:
		AdjacencyMatrix();
		AdjacencyMatrix(uint vertices_num);
		AdjacencyMatrix(std::string filename);
		AdjacencyMatrix(std::vector<Vertex> &coordinates);
		virtual ~AdjacencyMatrix();

		
		virtual void AddEdge(uint source, uint destination, uint weight) override;


		virtual bool IsConnected(uint source, uint destination) override;
		virtual weight GetWeight(uint source, uint destination) override;
		virtual uint GetNumOfEdges() override;
		virtual uint GetNumOfVertices() override;
		virtual std::string GetGraphInfo() override;

		

	private:
		weight **adjacency_matrix_;
	

		



	};
}
