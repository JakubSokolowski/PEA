#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\TSP\Graphs\AdjacencyMatrix\SymmetricAdjacencyMatrix.h"
#include "..\TSP\Graphs\AdjacencyMatrix\AsymmetricAdjacencyMatrix.h"
#include "..\TSP\Graphs\GraphDataParser.h"
#include <limits>
#include "..\TSP\Solvers\BruteForce\BruteForce.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace TSP;

namespace TSPTests {
	TEST_CLASS(BruteForceTests) {
	public:
		TEST_METHOD(BruteForceSymmetric5) {
			std::vector<std::vector<int>> matrix =
			{
				{ INF, 10,  8,   9,   7 },
				{ 10,  INF, 10,  5,   6 },
				{ 8,   10,  INF, 8,   9 },
				{ 9,   5,   8,   INF, 6 },
				{ 7,   6,   9,   6,  INF }
			};

			auto graph = SymmetricAdjacencyMatrix<int>(matrix);
			auto solver = BruteForce<int>();
			auto result = solver.Solve(graph);
			Assert::AreEqual(34, result.total_cost);
			auto expected_path = std::vector<int>{ 1,3,4,2,5,1 };
			auto actual_path = result.tour;
			for (uint i = 0; i < actual_path.size(); i++)
				Assert::AreEqual(expected_path[i], actual_path[i]);
		}

		TEST_METHOD(BruteForceAsymmetic10Int) {
			auto Graph = ParseGraphFile<AsymmetricAdjacencyMatrix<int>, int>(tsplib_asymmetric_path + "10.txt");
			BruteForce<int> solver = BruteForce<int>();
			auto result = solver.Solve(Graph);
			Assert::AreEqual(1454, result.total_cost);
		}
	};
}