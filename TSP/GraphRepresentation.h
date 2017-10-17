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
	using weight = double;
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


		virtual bool IsConnected(uint source, uint destination) = 0;
		virtual weight GetWeight(uint source, uint destination) = 0;
		virtual uint GetNumOfEdges() = 0;
		virtual uint GetNumOfVertices() = 0;		
		virtual std::string GetGraphInfo() = 0;

	protected:
		std::vector<Vertex> coordinates_;
		uint vertices_num_ = 0;
		uint edges_num_ = 0;
		weight** AllocateSquareMatrix(uint matrix_size);
		void ConvertCoordinatesToWeights();
		weight GetEuclideanDistance(Vertex source, Vertex destination);
	};
}


