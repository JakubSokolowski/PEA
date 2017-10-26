#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\TSP\SymmetricAdjacencyMatrix.h"
#include "..\TSP\GraphDataParser.h"
#include "..\TSP\HeldKarp.h"
#include "..\TSP\BranchAndBound.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace TSP;

namespace TSPTests
{
	TEST_CLASS(BranchAndBoundTest)
	{
	public:

		// Warning!!! Might take a substantial amount of time
		/*TEST_METHOD(BranchAndBoundSymmetric51Test)		
		{
			std::string filepath = "";
			auto Graph = ParseGraphFile<SymmetricAdjacencyMatrix>("C:\\Users\\jakub\\Documents\\Visual Studio 2017\\Projects\\PEA\\TSP\\Symmetric Instances\\51.txt");	

			BranchAndBound solver = BranchAndBound();
			auto result = solver.Solve(Graph);
			auto path = std::vector<int>{ 0, 1, 3, 2, 0 };
			Assert::AreEqual(80, result.distance_);
		}*/

		TEST_METHOD(BranchAndBoundSymmetric5Tests)
		{

			// 1st instance

			std::vector<std::vector<int>> matrix =
			{
				{ INF, 10,  8,   9,   7 },
				{ 10,  INF, 10,  5,   6 },
				{ 8,   10,  INF, 8,   9 },
				{ 9,   5,   8,   INF, 6 },
				{ 7,   6,   9,   6,  INF }
			};

			BranchAndBound solver = BranchAndBound();
			auto Graph = SymmetricAdjacencyMatrix(matrix);
			auto result = solver.Solve(Graph);
			Assert::AreEqual(34, result.distance_);

			// 2nd instance

			auto Graph2 = SymmetricAdjacencyMatrix(4);
			Graph2.AddEdge(0, 1, 20);
			Graph2.AddEdge(0, 2, 42);
			Graph2.AddEdge(0, 3, 35);
			Graph2.AddEdge(3, 2, 12);
			Graph2.AddEdge(1, 3, 34);
			Graph2.AddEdge(1, 2, 30);
			
			BranchAndBound solver2 = BranchAndBound();
			auto result2 = solver2.Solve(Graph2);
			Assert::AreEqual(97, result2.distance_);
		}

		// Helper Comparision Methods
		std::string VecToStr(std::vector<int> vec)
		{
			std::string str;
			for (auto it : vec)
				str += std::to_string(it);
			return str;
		}

	};
}