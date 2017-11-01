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
	TEST_CLASS(GraphDataParserTests)
	{
	public:
		TEST_METHOD(ParseFromCoordinatesFileSymmetricMatrix)
		{
			auto Graph = ParseGraphFile<SymmetricAdjacencyMatrix<int>>("C:\\Users\\jakub\\Documents\\Visual Studio 2017\\Projects\\PEA\\TSP\\Symmetric Instances\\51.txt");
			Assert::AreEqual(uint(51), Graph.GetNumOfVertices());
			Assert::AreEqual(uint(2550), Graph.GetNumOfEdges());		
		}
		TEST_METHOD(ParseFromCoordinatesFileAsymmetricMatrix)
		{
			auto Graph = ParseGraphFile<AsymmetricAdjacencyMatrix<int>>("C:\\Users\\jakub\\Documents\\Visual Studio 2017\\Projects\\PEA\\TSP\\Symmetric Instances\\51.txt");
			Assert::AreEqual(uint(51), Graph.GetNumOfVertices());
			Assert::AreEqual(uint(2550), Graph.GetNumOfEdges());
		}
	};
}