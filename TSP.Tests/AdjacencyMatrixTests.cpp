#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\TSP\AdjacencyMatrix.h"
#include "..\TSP\GraphDataParser.h"


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

		TEST_METHOD(ParseCoordinates)
		{			
			std::string filepath = "";
			auto Graph = ParseGraphFile<AdjacencyMatrix>("C:\\Users\\jakub\\Documents\\Visual Studio 2017\\Projects\\PEA\\TSP\\Symmetric Instances\\51.txt");
			Assert::AreEqual(uint(51), Graph->GetNumOfVertices());		
			Assert::AreEqual(uint(1275), Graph->GetNumOfEdges());
					
		}

	};
}