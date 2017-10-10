#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\TSP\TSP.h"
#include "..\TSP\AdjacencyMatrix.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace TSP;

namespace TSPTests
{		
	TEST_CLASS(m)
	{
	public:
		
		TEST_METHOD(AllocateSquareMatrixTest)
		{			
			AdjacencyMatrix matrix = AdjacencyMatrix(5);
			Assert::AreEqual(uint(5), matrix.GetNumOfVertices());
			Assert::AreEqual(uint(0), matrix.GetNumOfEdges());
			Assert::AreEqual(-1.0, matrix.GetWeight(0, 0));
			Assert::AreEqual(-1.0, matrix.GetWeight(4, 4));
		}	

	};
}