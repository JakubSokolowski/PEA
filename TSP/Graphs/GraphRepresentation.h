#pragma once

#ifndef TSP_GRAPH_REPRESENTATION
#define TSP_GRAPH_REPRESENTATION

#define NINT(a) ((a) >= 0.0 ? (int)((a)+0.5) : (int)((a)-0.5))

#include <string>
#include <vector>
#include <math.h>
#include <limits>


namespace TSP
{
	using uint = unsigned int;
	using density = double;
	
	
	struct Vertex
	{
		uint x_;
		uint y_;
		uint id_;
	};

	template<typename Cost>
	class  GraphRepresentation
	{		

	public:

		GraphRepresentation() {};
		GraphRepresentation(std::string filename) {};
		GraphRepresentation(std::vector<Vertex> &coordinates) {};		
		virtual ~GraphRepresentation() {};

	    virtual void AddEdge(uint source, uint destination, Cost cost_) = 0;

		virtual bool IsConnected(uint source, uint destination) const = 0;
		virtual Cost GetWeight(uint source, uint destination) const = 0;
		virtual uint GetNumOfEdges() const = 0;
		virtual uint GetNumOfVertices() const = 0;		
		virtual std::string GetGraphInfo() const = 0;
		virtual void Print() const = 0;
		virtual Cost GetTourCost(const std::vector<Cost> &tour) const;

	protected:

		std::vector<Vertex> coordinates_;		
		uint edges_num_ = 0;
		std::vector<std::vector<Cost>> AllocateSquareMatrix(uint matrix_size);
		void ConvertCoordinatesToWeights(std::vector<Vertex> &coordinates);
		Cost GetEuclideanDistance(Vertex source, Vertex destination);
	};

	// Returns square matrix of given size, with all values set to INT_MAX
	template <typename Cost>
	std::vector<std::vector<Cost>> GraphRepresentation<Cost>::AllocateSquareMatrix(uint matrix_size)
	{
		return std::vector<std::vector<Cost>>(matrix_size, std::vector<Cost>(matrix_size, std::numeric_limits<Cost>::max()));
	}

	// Converts the vector of Vertices to representation - computes all the edges and adds them to
	// Graph representation. The resulting graph will have 100% density
	template <typename Cost>
	void GraphRepresentation<Cost>::ConvertCoordinatesToWeights(std::vector<Vertex> &coordinates)
	{
		uint destination_vertex = 1;
		for (uint i = 0; i < coordinates.size() - 1; i++)
		{
			for (uint j = destination_vertex; j < coordinates.size(); j++)
			{
				edges_num_++;
				Cost edge_weight = GetEuclideanDistance(coordinates[i], coordinates[j]);
				AddEdge(i, j, edge_weight);
			}
			destination_vertex++;
		}

	}

	// Returns the euclidean total_cost (straight line) beetween two Vertices. The result is casted on
	// whatever type weight is (presumably int)
	template <typename Cost>
	Cost GraphRepresentation<Cost>::GetEuclideanDistance(Vertex source, Vertex destination)
	{
		int X = source.x_ - destination.x_;
		int Y = source.y_ - destination.y_;
		return NINT(sqrt(pow(X, 2) + pow(Y, 2)));
	}

	
	template<typename Cost>
	inline Cost GraphRepresentation<Cost>::GetTourCost(const std::vector<Cost>& tour) const
	{
		Cost result{};
		for (uint i = 0; i < GetNumOfVertices(); ++i)
			result += GetWeight(tour[i] - 1, tour[i + 1] - 1);
		return result;
	}
}

#endif