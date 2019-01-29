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


namespace Benchmarks
{
	TEST_CLASS(PopulationSizeBenchmark) {
		const int max_reps = 10;
		Timer timer = Timer(TimeUnit::miliseconds);
		Benchmark RepeatBenchmark(GeneticAlgorithm<int> &sol, GraphRepresentation<int> &rep, int reps) {
			auto benchmarks = std::vector<Benchmark>();
			for (int k = 0; k < reps; k++) {
				sol.Solve(rep);
				benchmarks.push_back(sol.GetSummary());
			}
			return Benchmark::Average(benchmarks);
		}
		void RunConfig(GeneticAlgorithm<int> &sol, GraphRepresentation<int> &rep, int reps, std::string filename) {
			auto avg = RepeatBenchmark(sol, rep, reps);
			avg.WriteToFile(filename);
		}

		void RunSymmetricComparision(GeneticAlgorithm<int> &sol, std::string name, vector<string> instances, int reps) {
			std::string filepath = genetic_result_path + name + ".txt";
			std::ofstream file(filepath);
			for (auto instance : instances) {
				auto graph = ParseGraphFile<SymmetricAdjacencyMatrix<int>, int>(tsplib_symmetric_path + instance + ".txt");
				auto avg = RepeatBenchmark(sol, graph, reps);
				avg.WriteSummaryToStream(file);
			}
			file.close();
		}

		TEST_METHOD_INITIALIZE(Seed) {
			srand(time(0));
		}

		TEST_METHOD(BenchmarkPopulation25Medium) {
			auto graph = ParseGraphFile<SymmetricAdjacencyMatrix<int>, int>(tsplib_symmetric_path + "eil51.txt");
			auto params = GeneticParameters(
				25,								// Population size
				50,								// Max Generations
				10,								// Tournament size
				0.1,							// Mutation Rate
				0.9,							// Crossover Rate
				0.1,							// Elitism %
				CrossoverType::PARTIALLY_MAPPED,
				MutationType::SWAP,
				Selection::ROULETTE);
			auto solver = GeneticAlgorithm<int>(params);
			RunConfig(solver, graph, max_reps, "population\\population_25_medium");
		}
		TEST_METHOD(BenchmarkPopulation50Medium) {
			auto graph = ParseGraphFile<SymmetricAdjacencyMatrix<int>, int>(tsplib_symmetric_path + "eil51.txt");
			auto params = GeneticParameters(
				50,								// Population size
				50,								// Max Generations
				10,								// Tournament size
				0.1,							// Mutation Rate
				0.9,							// Crossover Rate
				0.1,							// Elitism %
				CrossoverType::PARTIALLY_MAPPED,
				MutationType::SWAP,
				Selection::ROULETTE);
			auto solver = GeneticAlgorithm<int>(params);
			RunConfig(solver, graph, max_reps, "population\\population_50_medium");
		}
		TEST_METHOD(BenchmarkPopulation100Medium) {
			auto graph = ParseGraphFile<SymmetricAdjacencyMatrix<int>, int>(tsplib_symmetric_path + "eil51.txt");
			auto params = GeneticParameters(
				100,							// Population size
				50,							    // Max Generations
				10,								// Tournament size
				0.1,							// Mutation Rate
				0.9,							// Crossover Rate
				0.1,							// Elitism %
				CrossoverType::PARTIALLY_MAPPED,
				MutationType::SWAP,
				Selection::ROULETTE);
			auto solver = GeneticAlgorithm<int>(params);
			RunConfig(solver, graph, max_reps, "population\\population_100_medium");
		}
		TEST_METHOD(BenchmarkPopulation200Medium) {
			auto graph = ParseGraphFile<SymmetricAdjacencyMatrix<int>, int>(tsplib_symmetric_path + "eil51.txt");
			auto params = GeneticParameters(
				200,							// Population size
				50,								// Max Generations
				10,								// Tournament size
				0.1,							// Mutation Rate
				0.9,							// Crossover Rate
				0.1,							// Elitism %
				CrossoverType::PARTIALLY_MAPPED,
				MutationType::SWAP,
				Selection::ROULETTE);
			auto solver = GeneticAlgorithm<int>(params);
			RunConfig(solver, graph, max_reps, "population\\population_200_medium");
		}

		TEST_METHOD(BenchmarkPopulation25Big) {
			auto graph = ParseGraphFile<SymmetricAdjacencyMatrix<int>, int>(tsplib_symmetric_path + "rat195.txt");
			auto params = GeneticParameters(
				25,								// Population size
				50,								// Max Generations
				10,								// Tournament size
				0.1,							// Mutation Rate
				0.9,							// Crossover Rate
				0.1,							// Elitism %
				CrossoverType::PARTIALLY_MAPPED,
				MutationType::SWAP,
				Selection::ROULETTE);
			auto solver = GeneticAlgorithm<int>(params);
			RunConfig(solver, graph, max_reps, "population\\population_25_big");
		}
		TEST_METHOD(BenchmarkPopulation50Big) {
			auto graph = ParseGraphFile<SymmetricAdjacencyMatrix<int>, int>(tsplib_symmetric_path + "rat195.txt");
			auto params = GeneticParameters(
				50,								// Population size
				50,								// Max Generations
				10,								// Tournament size
				0.1,							// Mutation Rate
				0.9,							// Crossover Rate
				0.1,							// Elitism %
				CrossoverType::PARTIALLY_MAPPED,
				MutationType::SWAP,
				Selection::ROULETTE);
			auto solver = GeneticAlgorithm<int>(params);
			RunConfig(solver, graph, max_reps, "population\\population_50_big");
		}
		TEST_METHOD(BenchmarkPopulation100Big) {
			auto graph = ParseGraphFile<SymmetricAdjacencyMatrix<int>, int>(tsplib_symmetric_path + "rat195.txt");
			auto params = GeneticParameters(
				100,							// Population size
				50,							    // Max Generations
				10,								// Tournament size
				0.1,							// Mutation Rate
				0.9,							// Crossover Rate
				0.1,							// Elitism %
				CrossoverType::PARTIALLY_MAPPED,
				MutationType::SWAP,
				Selection::ROULETTE);
			auto solver = GeneticAlgorithm<int>(params);
			RunConfig(solver, graph, max_reps, "population\\population_100_big");
		}
		TEST_METHOD(BenchmarkPopulation200Big) {
			auto graph = ParseGraphFile<SymmetricAdjacencyMatrix<int>, int>(tsplib_symmetric_path + "rat195.txt");
			auto params = GeneticParameters(
				200,							// Population size
				50,								// Max Generations
				10,								// Tournament size
				0.1,							// Mutation Rate
				0.9,							// Crossover Rate
				0.1,							// Elitism %
				CrossoverType::PARTIALLY_MAPPED,
				MutationType::SWAP,
				Selection::ROULETTE);
			auto solver = GeneticAlgorithm<int>(params);
			RunConfig(solver, graph, max_reps, "population\\population_200_big");
		}


	};
}