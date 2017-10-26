#pragma once
#include "SymmetricAdjacencyMatrix.h"

namespace TSP
{
	class TSP_API AsymmetricAdjacencyMatrix :
		public SymmetricAdjacencyMatrix
	{
	public:
		AsymmetricAdjacencyMatrix();
		AsymmetricAdjacencyMatrix(int size);
		virtual ~AsymmetricAdjacencyMatrix();
		void AddEdge(int source, uint destination, uint weigh);		
	};
}

