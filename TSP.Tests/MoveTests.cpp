#include "stdafx.h"
#include "CppUnitTest.h"
#include <limits>
#include <algorithm>
#include <random>
#include "..\TSP\Graphs\AdjacencyMatrix\SymmetricAdjacencyMatrix.h"
#include "..\TSP\Solvers\TabuSearch\TabuList.h"
#include "..\TSP\Solvers\TabuSearch\Move.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace TSP;

using std::vector;
using std::string;

namespace TSPTests
{
	TEST_CLASS(MoveTests)
	{
	public:

		TEST_METHOD(EvaluateMoveTest)
		{
			std::vector<std::vector<int>> matrix =
			{
				{ INF, 10,  8,   9,   7 },
				{ 10,  INF, 10,  5,   6 },
				{ 8,   10,  INF, 8,   9 },
				{ 9,   5,   8,   INF, 6 },
				{ 7,   6,   9,   6,  INF }
			};

			auto path  = std::vector<int>{ 1,3,4,2,5,1 };
			auto path_cost = 34;
			auto solution = Solution<int>{ path_cost, path };
			auto swap_params = MoveParameters{ 1,4 };
			auto graph = SymmetricAdjacencyMatrix<int>(matrix);
			auto move = new Swap<int>();
			auto expected = 36;
			auto actual = move->EvaluateMove(solution, swap_params, graph);
			Assert::AreEqual(expected, actual);
		}
		TEST_METHOD(ExecuteSwapTest)
		{
			std::vector<std::vector<int>> matrix =
			{
				{ INF, 10,  8,   9,   7 },
				{ 10,  INF, 10,  5,   6 },
				{ 8,   10,  INF, 8,   9 },
				{ 9,   5,   8,   INF, 6 },
				{ 7,   6,   9,   6,  INF }
			};

			auto path = std::vector<int>{ 1,3,4,2,5,1 };
			auto path_cost = 34;
			auto solution = Solution<int>{ path_cost, path };
			auto swapped_path = std::vector<int>{ 1,5,4,2,3,1 };
			auto swapped_path_cost = 36;		
		
			auto swap_params = MoveParameters{ 1,4 };
			auto graph = SymmetricAdjacencyMatrix<int>(matrix);
			auto move = new Swap<int>();
			auto expected = move->Execute(solution, swap_params, graph);
			Assert::AreEqual(36, expected.total_cost);
			for (uint i = 0; i < expected.tour.size(); i++)
				Assert::AreEqual(swapped_path[i], expected.tour[i]);
		}

		
	};
}