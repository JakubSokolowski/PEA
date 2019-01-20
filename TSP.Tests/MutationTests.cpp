#include "stdafx.h"
#include "CppUnitTest.h"
#include <limits>
#include "..\TSP\Graphs\AdjacencyMatrix\SymmetricAdjacencyMatrix.h"
#include "..\TSP\Graphs\GraphDataParser.h"
#include "..\TSP\Solvers\Genetic\Chromosome.h"
#include "..\TSP\Solvers\Genetic\Mutation.h"
#include "..\TSP\Solvers\Genetic\Population.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace TSP;


namespace TSPTests
{
	TEST_CLASS(MutationTests)
	{
		TEST_METHOD(InsertionMutation)
		{
			auto graph = ParseGraphFile<SymmetricAdjacencyMatrix<int>, int>( tsplib_symmetric_path + "17.txt");
			auto population = Population<int>(1, graph);
			auto chromosome = population.GetMostFit();
			auto mutated = chromosome;
			Mutation<int>::Insertion(mutated, graph);
			Assert::IsTrue(std::is_permutation(chromosome.tour.begin(), chromosome.tour.end(), mutated.tour.begin()));
			Assert::AreNotEqual(chromosome.total_cost, mutated.total_cost);
		}
		TEST_METHOD(SwapMutation)
		{
			auto graph = ParseGraphFile<SymmetricAdjacencyMatrix<int>, int>(tsplib_symmetric_path + "17.txt");
			auto population = Population<int>(1, graph);
			auto chromosome = population.GetMostFit();
			auto mutated = chromosome;
			Mutation<int>::Swap(mutated, graph);
			Assert::IsTrue(std::is_permutation(chromosome.tour.begin(), chromosome.tour.end(), mutated.tour.begin()));
			Assert::AreNotEqual(chromosome.total_cost, mutated.total_cost);
		}
	};
}