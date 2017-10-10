#pragma once

#include <string>

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

	class TSP_API GraphRepresentation
	{
	public:

		GraphRepresentation() {};
		GraphRepresentation(std::string filename) {};
		virtual ~GraphRepresentation() {};

	    virtual void AddEdge(uint source, uint destination, uint weight) = 0;


		virtual bool IsConnected(uint source, uint destination) = 0;
		virtual weight GetWeight(uint source, uint destination) = 0;
		virtual uint GetNumOfEdges() = 0;
		virtual uint GetNumOfVertices() = 0;		
		virtual std::string GetGraphInfo() = 0;

	protected:

		weight** AllocateSquareMatrix(uint matrix_size);		
	};
}


