#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\TSP\Graphs\AdjacencyMatrix\SymmetricAdjacencyMatrix.h"
#include "..\TSP\Graphs\AdjacencyMatrix\AsymmetricAdjacencyMatrix.h"
#include "..\TSP\Solvers\TabuSearch\TabuSearch.h"
#include "..\TSP\Graphs\GraphDataParser.h"
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

		bool IsAccurate(int optimal, int actual, double accuracy)
		{
			return(double(optimal) / double(actual) > accuracy);
		}


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

		TEST_METHOD(TabuSearchSym5)
		{
			std::vector<std::vector<int>> matrix =
			{ 
				{ 0, 1, 3, 4, 5 },
				{ 1, 0, 1, 4, 8 },
				{ 3, 1, 0, 5, 1 },
				{ 4, 4, 5, 0, 2 },
				{ 5, 8, 1, 2, 0 } 
			};

			auto expected = std::vector<int>{ 1, 2, 3, 4, 5, 1 };
			auto graph = SymmetricAdjacencyMatrix<int>(matrix);
			auto solver = TabuSearch<int>();
			auto move = new Swap<int>();
			auto actual = solver.Solve(graph);
			std::wstring message = L"Optimal: 9, actual: " + std::to_wstring(actual.total_cost);
			Assert::AreEqual(true, IsAccurate(9, actual.total_cost, 0.8), message.c_str());		
			Assert::AreEqual(true, std::is_permutation(expected.begin(), expected.end(), actual.tour.begin()));
		}

		TEST_METHOD(TabuSearchSym17)
		{
			auto path = std::vector<int>{ 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,1 };
			auto graph = ParseGraphFile<SymmetricAdjacencyMatrix<int>, int>("C:\\Users\\jakub\\Documents\\Visual Studio 2017\\Projects\\PEA\\TSP\\Benchmarks\\ProblemData\\TSPLIB\\Symmetric\\S17.txt");
			auto solver = TabuSearch<int>();
			auto move = new Swap<int>();
			auto actual = solver.Solve(graph);
			Assert::AreEqual(true, std::is_permutation(path.begin(), path.end(), actual.tour.begin()));
			Assert::AreEqual(1, actual.tour.front());
			Assert::AreEqual(1, actual.tour.back());
			std::wstring message = L"Optimal: 2085, actual: " +  std::to_wstring(actual.total_cost);
			Assert::AreEqual(true, IsAccurate(2085, actual.total_cost, 0.95),message.c_str());
		}

	};
}