#include "stdafx.h"
#include "CppUnitTest.h"
#include <limits>
#include "..\TSP\Graphs\AdjacencyMatrix\SymmetricAdjacencyMatrix.h"
#include "..\TSP\Graphs\GraphDataParser.h"
#include "..\TSP\Solvers\Genetic\Chromosome.h"
#include "..\TSP\Solvers\Genetic\Population.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace TSP;


namespace TSPTests
{
	TEST_CLASS(PopulationTests)
	{
		TEST_METHOD(RandomGeneration)
		{
			auto path = std::vector<int>{ 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,1 };
			auto graph = ParseGraphFile<SymmetricAdjacencyMatrix<int>, int>(tsplib_symmetric_path + "17.txt");
			auto population = Population<int>(10, graph);
			Assert::AreEqual(population.GetSize(), 10);
			Assert::AreEqual(population.GetMaxSize(), 10);
			Assert::IsTrue(std::is_permutation(path.begin(), path.end(), population.GetMostFit().tour.begin()));
		}
		TEST_METHOD(RandomSelection)
		{
			int population_size = 10;
			auto population = Population<int>(population_size);		
			for (int i = 0; i < 10; i++)
				population.AddChromosome(Chromosome<int>(i + 1));
			auto selected = population.GetKChromosomes(10);
			Assert::AreEqual(population.GetMostFit().total_cost, 1);		
		}
	};
}