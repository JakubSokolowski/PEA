#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\TSP\Graphs\AdjacencyMatrix\SymmetricAdjacencyMatrix.h"
#include "..\TSP\Solvers\TabuSearch\TabuSearch.h"
#include <limits>
#include <algorithm>
#include <random>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace TSP;

using std::vector;
using std::string;

namespace TSPTests
{
	TEST_CLASS(TabuSearchTests)
	{
	public:


		TEST_METHOD(GetTourCost)
		{
			std::vector<std::vector<int>> matrix =
			{
				{ INF, 10,  8,   9,   7 },
				{ 10,  INF, 10,  5,   6 },
				{ 8,   10,  INF, 8,   9 },
				{ 9,   5,   8,   INF, 6 },
				{ 7,   6,   9,   6,  INF }
			};

			auto tour = std::vector<int>{ 1,3,4,2,5,1 };
			auto graph = SymmetricAdjacencyMatrix<int>(matrix);
			auto cost = graph.GetTourCost(tour);
			Assert::AreEqual(34, cost);
	
		}

		TEST_METHOD(GenerateInitialSolutionTest)
		{
			std::vector<std::vector<int>> matrix =
			{
				{ INF, 10,  8,   9,   7 },
				{ 10,  INF, 10,  5,   6 },
				{ 8,   10,  INF, 8,   9 },
				{ 9,   5,   8,   INF, 6 },
				{ 7,   6,   9,   6,  INF }
			};

			auto expected = std::vector<int>{ 1,2,3,4,5,1 };
			auto graph = SymmetricAdjacencyMatrix<int>(matrix);
			auto solver = TabuSearch<int>();
			auto solution = solver.GenerateInitialSolution(graph);
			Assert::AreEqual(1, solution.tour.front());
			Assert::AreEqual(1, solution.tour.front());
			Assert::AreEqual((uint)6, solution.tour.size());
			Assert::AreEqual(true, std::is_permutation(expected.begin(), expected.end(), solution.tour.begin()));
		}
	};
}