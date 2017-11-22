
#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\TSP\Graphs\AdjacencyMatrix\SymmetricAdjacencyMatrix.h"
#include "..\TSP\Graphs\GraphDataParser.h"
#include "..\TSP\Solvers\BranchAndBound\BranchAndBound.h"
#include "..\TSP\Benchmarks\PerformanceTests.h"
#include <limits>
#include <algorithm>
#include <random>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace TSP;

using std::vector;
using std::string;

namespace TSPTests
{
	TEST_CLASS(BranchAndBoundTests)
	{
	public:		
		

		TEST_METHOD(LittleSymmetric5)
		{			
			std::vector<std::vector<int>> matrix =
			{
				{ INF, 10,  8,   9,   7 },
				{ 10,  INF, 10,  5,   6 },
				{ 8,   10,  INF, 8,   9 },
				{ 9,   5,   8,   INF, 6 },
				{ 7,   6,   9,   6,  INF }
			};

			auto graph = SymmetricAdjacencyMatrix<int>(matrix);
			auto solver = BranchAndBound<int>();
			auto result = solver.Solve(graph);
			Assert::AreEqual(34, result.distance_);
			auto expected_path = std::vector<int>{ 1,3,4,2,5,1 };
			auto actual_path = result.path_;
			for (uint i = 0; i < actual_path.size(); i++)
				Assert::AreEqual(expected_path[i], actual_path[i]);
		}

		
		/*TEST_METHOD(BranchAndBoundSymmetric51Test)
		{
			std::string filepath = "";
			auto Graph = ParseGraphFile<SymmetricAdjacencyMatrix<int>, int>("C:\\Users\\jakub\\Documents\\Visual Studio 2017\\Projects\\PEA\\TSP\\Symmetric Instances\\26.txt");
			auto solver = BranchAndBound<int>();
			auto result = solver.Solve(Graph);
			Assert::AreEqual(937, result.distance_);
		}
*/
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

			auto expected_path = std::vector<int>{ 1,3,4,2,5,1 };
			auto actual_path = result.path_;
			for (uint i = 0; i < actual_path.size(); i++)
				Assert::AreEqual(expected_path[i], actual_path[i]);

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
		//TEST_METHOD(BranchAndBoundSymmetric5Double)
		//{

		//	// 1st instance
		//	auto res = std::numeric_limits<double>::max();
		//	double D_INF = std::numeric_limits<double>::max();

		//	std::vector<std::vector<double>> matrix =
		//	{
		//		{ D_INF, 10,  8,   9,   7 },
		//		{ 10,  D_INF, 10,  5,   6 },
		//		{ 8,   10,  D_INF, 8,   9 },
		//		{ 9,   5,   8,   D_INF, 6 },
		//		{ 7,   6,   9,   6,  D_INF }
		//	};

		//	BranchAndBound<double> solver = BranchAndBound<double>();
		//	auto Graph = SymmetricAdjacencyMatrix<double>(matrix);
		//	auto result = solver.Solve(Graph);
		//	Assert::AreEqual(34.0, result.distance_);

		//	// 2nd instance

		//	auto Graph2 = SymmetricAdjacencyMatrix<double>(4);
		//	Graph2.AddEdge(0, 1, 20);
		//	Graph2.AddEdge(0, 2, 42);
		//	Graph2.AddEdge(0, 3, 35);
		//	Graph2.AddEdge(3, 2, 12);
		//	Graph2.AddEdge(1, 3, 34);
		//	Graph2.AddEdge(1, 2, 30);

		//	auto solver2 = BranchAndBound<double>();
		//	auto result2 = solver2.Solve(Graph2);
		//	Assert::AreEqual(97.0, result2.distance_);
		//}

		TEST_METHOD(BranchAndBoundAsymmetic3Int)
		{
			std::vector<std::vector<int>> matrix =
			{
				{ INF, 2,  4 },
				{ 5,   INF, 4},
				{ 7,   2,  INF},
			};

			BranchAndBound<int> solver = BranchAndBound<int>();
			auto Graph = SymmetricAdjacencyMatrix<int>(matrix);
			auto result = solver.Solve(Graph);
			Assert::AreEqual(11, result.distance_);


			auto expected_path = std::vector<int>{ 1,3,2,1 };
			auto actual_path = result.path_;
			for (uint i = 0; i < actual_path.size(); i++)
				Assert::AreEqual(expected_path[i], actual_path[i]);
		}

		TEST_METHOD(BranchAndBoundOriginalLittlePaper)
		{
			std::vector<std::vector<int>> matrix =
			{
				{ INF, 27,  43,   16,   30, 26 },
				{ 6,  INF, 16,  1,   30, 25 },
				{ 20,   13,  INF, 35,   5, 0 },
				{ 21,   16,   25,   INF, 18, 18 },
				{ 12,   46,   27,   48,  INF, 5 },
				{ 23,   5,   5,   9,5,  INF }
			};

			BranchAndBound<int> solver = BranchAndBound<int>();
			auto Graph = SymmetricAdjacencyMatrix<int>(matrix);
			auto result = solver.Solve(Graph);
			Assert::AreEqual(62, result.distance_);			
		}


		TEST_METHOD(BranchAndBoundAsymmetic10Int)
		{	
			auto Graph = ParseGraphFile<SymmetricAdjacencyMatrix<int>, int>("C:\\Users\\jakub\\Documents\\Visual Studio 2017\\Projects\\PEA\\TSP\\Asymmetric Instances\\10.txt");
			BranchAndBound<int> solver = BranchAndBound<int>();
			auto result = solver.Solve(Graph);
			Assert::AreEqual(10, result.distance_);		
		}
		TEST_METHOD(BranchAndBoundAsymmetic17Int)
		{
			WriteAsymmetricMatrix(GenerateAsymmetricMatrix(17, 100));

			auto Graph = ParseGraphFile<SymmetricAdjacencyMatrix<int>, int>("C:\\Users\\jakub\\Documents\\Visual Studio 2017\\Projects\\PEA\\TSP\\Asymmetric Instances\\17.txt");
			BranchAndBound<int> solver = BranchAndBound<int>();
			auto result = solver.Solve(Graph);
			Assert::AreEqual(139, result.distance_);
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