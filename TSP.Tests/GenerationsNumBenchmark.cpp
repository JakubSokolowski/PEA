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
	TEST_CLASS(GenerationNumBenchmark) {
		const int max_reps = 1;
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


		TEST_METHOD(BenchmarkGeneration50Medium) {
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
			RunConfig(solver, graph, max_reps, "generation\\generation_50_medium");
		}
		TEST_METHOD(BenchmarkGeneration100Medium) {
			auto graph = ParseGraphFile<SymmetricAdjacencyMatrix<int>, int>(tsplib_symmetric_path + "eil51.txt");
			auto params = GeneticParameters(
				50,								// Population size
				100,							// Max Generations
				10,								// Tournament size
				0.1,							// Mutation Rate
				0.9,							// Crossover Rate
				0.1,							// Elitism %
				CrossoverType::PARTIALLY_MAPPED,
				MutationType::SWAP,
				Selection::ROULETTE);
			auto solver = GeneticAlgorithm<int>(params);
			RunConfig(solver, graph, max_reps, "generation\\generation_100_medium");
		}
		TEST_METHOD(BenchmarkGeneration150Medium) {
			auto graph = ParseGraphFile<SymmetricAdjacencyMatrix<int>, int>(tsplib_symmetric_path + "eil51.txt");
			auto params = GeneticParameters(
				50,								// Population size
				150,							// Max Generations
				10,								// Tournament size
				0.1,							// Mutation Rate
				0.9,							// Crossover Rate
				0.1,							// Elitism %
				CrossoverType::PARTIALLY_MAPPED,
				MutationType::SWAP,
				Selection::ROULETTE);
			auto solver = GeneticAlgorithm<int>(params);
			RunConfig(solver, graph, max_reps, "generation\\generation_150_medium");
		}
		TEST_METHOD(BenchmarkGeneration200Medium) {
			auto graph = ParseGraphFile<SymmetricAdjacencyMatrix<int>, int>(tsplib_symmetric_path + "eil51.txt");
			auto params = GeneticParameters(
				50,							// Population size
				200,							// Max Generations
				10,								// Tournament size
				0.1,							// Mutation Rate
				0.9,							// Crossover Rate
				0.1,							// Elitism %
				CrossoverType::PARTIALLY_MAPPED,
				MutationType::SWAP,
				Selection::ROULETTE);
			auto solver = GeneticAlgorithm<int>(params);
			RunConfig(solver, graph, max_reps, "generation\\generation_200_medium");
		}

		TEST_METHOD(BenchmarkGeneration50Big) {
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
			RunConfig(solver, graph, max_reps, "generation\\generation_50_big");
		}
		TEST_METHOD(BenchmarknGeneration100Big) {
			auto graph = ParseGraphFile<SymmetricAdjacencyMatrix<int>, int>(tsplib_symmetric_path + "rat195.txt");
			auto params = GeneticParameters(
				50,								// Population size
				100,							// Max Generations
				10,								// Tournament size
				0.1,							// Mutation Rate
				0.9,							// Crossover Rate
				0.1,							// Elitism %
				CrossoverType::PARTIALLY_MAPPED,
				MutationType::SWAP,
				Selection::ROULETTE);
			auto solver = GeneticAlgorithm<int>(params);
			RunConfig(solver, graph, max_reps, "generation\\generation_100_big");
		}
		TEST_METHOD(BenchmarkGeneration150Big) {
			auto graph = ParseGraphFile<SymmetricAdjacencyMatrix<int>, int>(tsplib_symmetric_path + "rat195.txt");
			auto params = GeneticParameters(
				50,								// Population size
				150,							// Max Generations
				10,								// Tournament size
				0.1,							// Mutation Rate
				0.9,							// Crossover Rate
				0.1,							// Elitism %
				CrossoverType::PARTIALLY_MAPPED,
				MutationType::SWAP,
				Selection::ROULETTE);
			auto solver = GeneticAlgorithm<int>(params);
			RunConfig(solver, graph, max_reps, "generation\\generation_150_big");
		}
		TEST_METHOD(BenchmarkGeneration200Big) {
			auto graph = ParseGraphFile<SymmetricAdjacencyMatrix<int>, int>(tsplib_symmetric_path + "rat195.txt");
			auto params = GeneticParameters(
				50,								// Population size
				200,							// Max Generations
				10,								// Tournament size
				0.1,							// Mutation Rate
				0.9,							// Crossover Rate
				0.1,							// Elitism %
				CrossoverType::PARTIALLY_MAPPED,
				MutationType::SWAP,
				Selection::ROULETTE);
			auto solver = GeneticAlgorithm<int>(params);
			RunConfig(solver, graph, max_reps, "generation\\generation_200_big");
		}


	};
}