#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\TSP\SymmetricAdjacencyMatrix.h"
#include "..\TSP\AsymmetricAdjacencyMatrix.h"
#include "..\TSP\GraphDataParser.h"
#include "..\TSP\HeldKarp.h"
#include "..\TSP\BranchAndBound.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace TSP;

namespace TSPTests
{		
	TEST_CLASS(AdjacencyMatrixTests)
	{
	public:
		
		TEST_METHOD(AllocateSquareMatrix)
		{			
			SymmetricAdjacencyMatrix matrix = SymmetricAdjacencyMatrix(5);
			Assert::AreEqual(uint(5), matrix.GetNumOfVertices());
			Assert::AreEqual(uint(0), matrix.GetNumOfEdges());
			Assert::AreEqual(INT_MAX, matrix.GetWeight(0, 0));
			Assert::AreEqual(INT_MAX, matrix.GetWeight(4, 4));
		}		

		TEST_METHOD(SymmetricMatrixConstructionWithAddEdge)
		{
			auto Graph = SymmetricAdjacencyMatrix(4);
			Graph.AddEdge(0, 1, 20);
			Graph.AddEdge(0, 2, 42);
			Graph.AddEdge(0, 3, 35);
			Graph.AddEdge(3, 2, 12);
			Graph.AddEdge(1, 3, 34);
			Graph.AddEdge(1, 2, 30);
			
			Assert::AreEqual(uint(4), Graph.GetNumOfVertices());
			Assert::AreEqual(uint(6), Graph.GetNumOfEdges());
			Assert::AreEqual(INT_MAX, Graph.GetWeight(0, 0));
			Assert::AreEqual(INT_MAX, Graph.GetWeight(3, 3));
			Assert::AreEqual(35, Graph.GetWeight(0, 3));
			Assert::AreEqual(35, Graph.GetWeight(3, 0));
		}

		TEST_METHOD(AsymmetricMatrixConstructionWithAddEdge)
		{
			auto Graph = AsymmetricAdjacencyMatrix(4);
			Graph.AddEdge(0, 1, 20);
			Graph.AddEdge(0, 2, 42);
			Graph.AddEdge(0, 3, 35);
			Graph.AddEdge(3, 2, 12);
			Graph.AddEdge(1, 3, 34);
			Graph.AddEdge(1, 2, 30);

			Assert::AreEqual(uint(4), Graph.GetNumOfVertices());
			Assert::AreEqual(uint(6), Graph.GetNumOfEdges());
			Assert::AreEqual(INT_MAX, Graph.GetWeight(0, 0));
			Assert::AreEqual(INT_MAX, Graph.GetWeight(3, 3));
			Assert::AreEqual(35, Graph.GetWeight(0, 3));
			Assert::AreEqual(INT_MAX, Graph.GetWeight(3, 0));
		}
		
	};
}