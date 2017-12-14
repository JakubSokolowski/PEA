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

		std::string tsplib_symmetric_path = "C:\\Users\\jakub\\Documents\\Visual Studio 2017\\Projects\\PEA\\TSP\\Benchmarks\\ProblemData\\TSPLIB\\Symmetric\\";

		TEST_METHOD(ParseFromCoordinatesFileSymmetricMatrix)
		{
			auto Graph = ParseGraphFile<SymmetricAdjacencyMatrix<int>, int>(tsplib_symmetric_path + "berlin52.txt");
			// Check the number of edges and vertices
			Assert::AreEqual(uint(52), Graph.GetNumOfVertices());
			Assert::AreEqual(uint(2652), Graph.GetNumOfEdges());		
			// Assume that graph was read correctly, if the cost of optimal path from tsplib is equal to the optimal solution
			auto path = std::vector<int>{ 1,49,32,45,19,41,8,9,10,43,33,51,11,52,14,13,47,26,27,28,12,25,4,6,15,5,24,48,38,37,40,39,36,35,34,44,46,16,29,50,20,23,30,2,7,42,21,17,3,18,31,22,1 };
			Assert::AreEqual(path.size(), uint(53));
			Assert::AreEqual(Graph.GetTourCost(path), 7542);
		}
		/*TEST_METHOD(ParseFromCoordinatesFileAsymmetricMatrix)
		{
			auto Graph = ParseGraphFile<AsymmetricAdjacencyMatrix<int>, int>("C:\\Users\\jakub\\Documents\\Visual Studio 2017\\Projects\\PEA\\TSP\\Symmetric Instances\\51.txt");
			Assert::AreEqual(uint(51), Graph.GetNumOfVertices());
			Assert::AreEqual(uint(2550), Graph.GetNumOfEdges());
		}*/
	};
}