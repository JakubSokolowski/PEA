#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\TSP\Graphs\AdjacencyMatrix\AsymmetricAdjacencyMatrix.h"
#include "..\TSP\Graphs\AdjacencyMatrix\SymmetricAdjacencyMatrix.h"
#include "..\TSP\Graphs\GraphDataParser.h"
#include "..\TSP\Solvers\BranchAndBound\BranchAndBound.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace TSP;

namespace TSPTests
{
	TEST_CLASS(GraphDataParserTests)
	{
	public:
		/*TEST_METHOD(ParseFromCoordinatesFileSymmetricMatrix)
		{
			auto Graph = ParseGraphFile<SymmetricAdjacencyMatrix<int>, int>("C:\\Users\\jakub\\Documents\\Visual Studio 2017\\Projects\\PEA\\TSP\\Symmetric Instances\\51.txt");
			Assert::AreEqual(uint(51), Graph.GetNumOfVertices());
			Assert::AreEqual(uint(2550), Graph.GetNumOfEdges());		
		}
		TEST_METHOD(ParseFromCoordinatesFileAsymmetricMatrix)
		{
			auto Graph = ParseGraphFile<AsymmetricAdjacencyMatrix<int>, int>("C:\\Users\\jakub\\Documents\\Visual Studio 2017\\Projects\\PEA\\TSP\\Symmetric Instances\\51.txt");
			Assert::AreEqual(uint(51), Graph.GetNumOfVertices());
			Assert::AreEqual(uint(2550), Graph.GetNumOfEdges());
		}*/
	};
}