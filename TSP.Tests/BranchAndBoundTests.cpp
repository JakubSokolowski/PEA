#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\TSP\SymmetricAdjacencyMatrix.h"
#include "..\TSP\GraphDataParser.h"
#include "..\TSP\HeldKarp.h"
#include "..\TSP\BranchAndBound.h"
#include <limits>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace TSP;


namespace TSPTests
{
	TEST_CLASS(BranchAndBoundTests)
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

		TEST_METHOD(BranchAndBoundSymmetric5Int)
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

			BranchAndBound<int> solver = BranchAndBound<int>();
			auto Graph = SymmetricAdjacencyMatrix<int>(matrix);
			auto result = solver.Solve(Graph);
			Assert::AreEqual(34, result.distance_);

			// 2nd instance

			auto Graph2 = SymmetricAdjacencyMatrix<int>(4);
			Graph2.AddEdge(0, 1, 20);
			Graph2.AddEdge(0, 2, 42);
			Graph2.AddEdge(0, 3, 35);
			Graph2.AddEdge(3, 2, 12);
			Graph2.AddEdge(1, 3, 34);
			Graph2.AddEdge(1, 2, 30);
			
			auto solver2 = BranchAndBound<int>();
			auto result2 = solver2.Solve(Graph2);
			Assert::AreEqual(97, result2.distance_);
		}
		TEST_METHOD(BranchAndBoundSymmetric5IDouble)
		{

			// 1st instance
			double D_INF = std::numeric_limits<double>::max();

			std::vector<std::vector<double>> matrix =
			{
				{ D_INF, 10,  8,   9,   7 },
				{ 10,  D_INF, 10,  5,   6 },
				{ 8,   10,  D_INF, 8,   9 },
				{ 9,   5,   8,   D_INF, 6 },
				{ 7,   6,   9,   6,  D_INF }
			};

			BranchAndBound<double> solver = BranchAndBound<double>();
			auto Graph = SymmetricAdjacencyMatrix<double>(matrix);
			auto result = solver.Solve(Graph);
			Assert::AreEqual(34.0, result.distance_);

			// 2nd instance

			auto Graph2 = SymmetricAdjacencyMatrix<double>(4);
			Graph2.AddEdge(0, 1, 20);
			Graph2.AddEdge(0, 2, 42);
			Graph2.AddEdge(0, 3, 35);
			Graph2.AddEdge(3, 2, 12);
			Graph2.AddEdge(1, 3, 34);
			Graph2.AddEdge(1, 2, 30);

			auto solver2 = BranchAndBound<double>();
			auto result2 = solver2.Solve(Graph2);
			Assert::AreEqual(97.0, result2.distance_);
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