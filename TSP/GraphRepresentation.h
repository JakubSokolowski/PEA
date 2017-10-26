#pragma once

#include <string>
#include <vector>

#ifdef TSP_EXPORTS
#define TSP_API __declspec(dllexport)
#else
#define TSP_API __declspec(dllimport)
#endif

namespace TSP
{
	using uint = unsigned int;
	using weight = int;
	using density = double;

	struct TSP_API Vertex
	{
		uint x_;
		uint y_;
		uint id_;
	};

	class TSP_API GraphRepresentation
	{
	public:

		GraphRepresentation() {};
		GraphRepresentation(std::string filename) {};
		GraphRepresentation(std::vector<Vertex> &coordinates) {};		
		virtual ~GraphRepresentation() {};

	    virtual void AddEdge(uint source, uint destination, uint weight) = 0;

		virtual const bool IsConnected(uint source, uint destination) = 0;
		virtual const weight GetWeight(uint source, uint destination) = 0;
		virtual const uint GetNumOfEdges() = 0;
		virtual const uint GetNumOfVertices() = 0;		
		virtual const std::string GetGraphInfo() = 0;
		virtual const void Print() = 0;

	protected:

		std::vector<Vertex> coordinates_;		
		uint edges_num_ = 0;
		std::vector<std::vector<weight>> AllocateSquareMatrix(uint matrix_size);
		void ConvertCoordinatesToWeights(std::vector<Vertex> &coordinates);
		weight GetEuclideanDistance(Vertex source, Vertex destination);
	};
}


