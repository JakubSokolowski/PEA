#include "stdafx.h"
#include "CppUnitTest.h"
#include <limits>
#include "..\TSP\Graphs\AdjacencyMatrix\SymmetricAdjacencyMatrix.h"
#include "..\TSP\Graphs\GraphDataParser.h"
#include "..\TSP\Solvers\Genetic\Chromosome.h"
#include "..\TSP\Solvers\Genetic\Population.h"
#include "..\TSP\Solvers\Genetic\Mutation.h"

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
			for (int i = 0; i < 3; i++)
				population.AddChromosome(Chromosome<int>(i + 1));
			auto selected = population.GetKChromosomes(3);
			Assert::AreEqual(population.GetMostFit().total_cost, 1);		
		}
		TEST_METHOD(GetMostFitPercentage) {
			int population_size = 10;
			auto population = Population<int>(population_size);
			for (int i = 0; i < 10; i++)
				population.AddChromosome(Chromosome<int>(i + 1));
			auto selected = population.GetMostFitPercentage(0.3);
			Assert::AreEqual(1, selected[0].total_cost);
			Assert::AreEqual(2, selected[1].total_cost);
			Assert::AreEqual(3, selected[2].total_cost);
		}
		TEST_METHOD(ElitePercentageConstructor) {
			int population_size = 10;
			auto population = Population<int>(population_size);
			for (int i = 0; i < 10; i++)
				population.AddChromosome(Chromosome<int>(i + 1));
			auto new_population = Population<int>(population, 0.3);
			Assert::AreEqual(new_population.GetSize(), 3);
			Assert::AreEqual(new_population.GetMaxSize(), 10);
		}
		TEST_METHOD(RouletteSelection) {
			srand(10);
			int population_size = 10;
			auto population = Population<int>(population_size);
			auto population_average = 0.0;
			for (int i = 0; i < 10; i++) {
				auto ch = Chromosome<int>(i + 1);
				population.AddChromosome(ch);
				population_average += ch.total_cost;
			}
			population_average /= 10;
			auto selected = std::vector < Chromosome<int>>();
			auto average_cost = 0.0;
			for (int i = 0; i < 10; i++) {
				selected.push_back(population.RouletteSelection());
				average_cost += selected.back().total_cost;
			}
			average_cost /= 10;
			Logger::WriteMessage(std::to_string(average_cost).c_str());
			Logger::WriteMessage(std::to_string(population_average).c_str());
			Assert::IsTrue(population_average > average_cost);	
		}
		TEST_METHOD(TournamentSelection) {
			srand(time(0));
			int population_size = 10;
			auto population = Population<int>(population_size);
			for (int i = 0; i < 10; i++)
				population.AddChromosome(Chromosome<int>(i + 1));
			auto selected = std::vector < Chromosome<int>>();
			auto average_cost = 0;
			for (int i = 0; i < 10; i++) {
				selected.push_back(population.TournamentSelection(2));
				average_cost += selected.back().total_cost;
			}
			average_cost /= 10;
			Assert::IsTrue(average_cost < 4.0);
		}

		TEST_METHOD(GetParents) {
			auto graph = ParseGraphFile<SymmetricAdjacencyMatrix<int>, int>(tsplib_symmetric_path + "17.txt");
			auto population = Population<int>(10, graph);
			auto parents = population.GetParents(ROULETTE);
			Assert::IsFalse(parents.first == parents.second);
		}
		TEST_METHOD(MutateAndUpdate) {
			auto graph = ParseGraphFile<SymmetricAdjacencyMatrix<int>, int>(tsplib_symmetric_path + "17.txt");
			auto population = Population<int>(10, graph);
			for (auto &ch : population.population_m) {
				Mutation<int>::Scramble(ch);
			}
			population.UpdateFitness(graph);
		}

	};
}