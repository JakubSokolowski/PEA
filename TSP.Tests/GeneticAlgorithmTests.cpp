#include "stdafx.h"
#include "CppUnitTest.h"
#include <limits>
#include "..\TSP\Graphs\AdjacencyMatrix\SymmetricAdjacencyMatrix.h"
#include "..\TSP\Graphs\GraphDataParser.h"
#include "..\TSP\Solvers\Genetic\Chromosome.h"
#include "..\TSP\Solvers\Genetic\Mutation.h"
#include "..\TSP\Solvers\Genetic\Population.h"
#include "..\TSP\Solvers\Genetic\GeneticAlgorithm.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace TSP;


namespace TSPTests
{
	TEST_CLASS(GeneticAlgorithmTests)
	{
		TEST_METHOD(SolveSymmetric17) {
			auto graph = ParseGraphFile<SymmetricAdjacencyMatrix<int>, int>(tsplib_asymmetric_path + "17.txt");
			auto params = GeneticParameters(10, 100, 5, 0.4, 0.7, 0.3, 
				CrossoverType::PARTIALLY_MAPPED,
				MutationType::SCRAMBLE, 
				Selection::ROULETTE);
			auto solver = GeneticAlgorithm<int>(params);
			auto solution = solver.Solve(graph);
			Logger::WriteMessage(RandomHelper::ToString(solution.tour).c_str() + '\n');
			Logger::WriteMessage(std::to_string(solution.total_cost).c_str());
		}
		TEST_METHOD(SolveSymmetric124) {
			auto graph = ParseGraphFile<SymmetricAdjacencyMatrix<int>, int>(tsplib_asymmetric_path + "pr124.txt");
			auto params = GeneticParameters(10, 2, 5, 0.4, 0.7, 0.3,
				CrossoverType::CYCLE,
				MutationType::SCRAMBLE,
				Selection::ROULETTE);
			auto solver = GeneticAlgorithm<int>(params);
			auto solution = solver.Solve(graph);
			Logger::WriteMessage(RandomHelper::ToString(solution.tour).c_str() + '\n');
			Logger::WriteMessage(std::to_string(solution.total_cost).c_str());
		}
	};
}