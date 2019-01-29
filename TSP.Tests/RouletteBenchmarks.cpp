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
	TEST_CLASS(SelectionBenchmarks) {
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
		TEST_METHOD(BenchmarkRouletteMedium) {
			auto graph = ParseGraphFile<SymmetricAdjacencyMatrix<int>, int>(tsplib_symmetric_path + "eil51.txt");
			auto params = GeneticParameters(
				25,								// Population size
				50,								// Max Generations
				10,								// Tournament size
				0.1,							// Mutation Rate
				0.9,							// Crossover Rate
				0.3,							// Elitism %
				CrossoverType::PARTIALLY_MAPPED,
				MutationType::INSERTION,
				Selection::ROULETTE);
			auto solver = GeneticAlgorithm<int>(params);
			RunConfig(solver, graph, max_reps, "selection\\roulette_medium");
		}
		TEST_METHOD(BenchmarkRouletteBig) {
			auto graph = ParseGraphFile<SymmetricAdjacencyMatrix<int>, int>(tsplib_symmetric_path + "rat195.txt");
			auto params = GeneticParameters(
				25,								// Population size
				50,								// Max Generations
				10,								// Tournament size
				0.1,							// Mutation Rate
				0.9,							// Crossover Rate
				0.3,							// Elitism %
				CrossoverType::PARTIALLY_MAPPED,
				MutationType::INSERTION,
				Selection::ROULETTE);
			auto solver = GeneticAlgorithm<int>(params);
			RunConfig(solver, graph, max_reps, "selection\\roulette_big");
		}

		TEST_METHOD(BenchmarkTournament50Medium) {
			srand(time(0));
			auto graph1 = ParseGraphFile<SymmetricAdjacencyMatrix<int>, int>(tsplib_symmetric_path + "eil51.txt");
			auto params = GeneticParameters(
				25,								// Population size
				50,								// Max Generations
				13,								// Tournament size
				0.1,							// Mutation Rate
				0.9,							// Crossover Rate
				0.3,							// Elitism %
				CrossoverType::PARTIALLY_MAPPED,
				MutationType::INSERTION,
				Selection::TOURNAMENT);
			auto solver = GeneticAlgorithm<int>(params);
			RunConfig(solver, graph1, max_reps, "selection\\tournament_50_medium");
		}
		TEST_METHOD(BenchmarkTournament50Big) {
			srand(time(0));
			auto graph1 = ParseGraphFile<SymmetricAdjacencyMatrix<int>, int>(tsplib_symmetric_path + "rat195.txt");
			auto params = GeneticParameters(
				25,								// Population size
				50,								// Max Generations
				13,								// Tournament size
				0.1,							// Mutation Rate
				0.9,							// Crossover Rate
				0.3,							// Elitism %
				CrossoverType::PARTIALLY_MAPPED,
				MutationType::INSERTION,
				Selection::TOURNAMENT);
			auto solver = GeneticAlgorithm<int>(params);
			RunConfig(solver, graph1, max_reps, "selection\\tournament_50_big");
		}
		TEST_METHOD(BenchmarkTournament33Medium) {
			srand(time(0));
			auto graph = ParseGraphFile<SymmetricAdjacencyMatrix<int>, int>(tsplib_symmetric_path + "eil51.txt");
			auto params = GeneticParameters(
				25,								// Population size
				50,								// Max Generations
				8,								// Tournament size
				0.1,							// Mutation Rate
				0.9,							// Crossover Rate
				0.3,							// Elitism %
				CrossoverType::PARTIALLY_MAPPED,
				MutationType::INSERTION,
				Selection::TOURNAMENT);
			auto solver = GeneticAlgorithm<int>(params);
			RunConfig(solver, graph, max_reps, "selection\\tournament_33_medium");
		}
		TEST_METHOD(BenchmarkTournament33Big) {
			srand(time(0));
			auto graph = ParseGraphFile<SymmetricAdjacencyMatrix<int>, int>(tsplib_symmetric_path + "rat195.txt");
			auto params = GeneticParameters(
				25,								// Population size
				50,								// Max Generations
				8,								// Tournament size
				0.1,							// Mutation Rate
				0.9,							// Crossover Rate
				0.3,							// Elitism %
				CrossoverType::PARTIALLY_MAPPED,
				MutationType::INSERTION,
				Selection::TOURNAMENT);
			auto solver = GeneticAlgorithm<int>(params);
			RunConfig(solver, graph, max_reps, "selection\\tournament_33_big");
		}
		TEST_METHOD(BenchmarkTournament25Medium) {
			srand(time(0));
			auto graph = ParseGraphFile<SymmetricAdjacencyMatrix<int>, int>(tsplib_symmetric_path + "eil51.txt");
			auto params = GeneticParameters(
				25,								// Population size
				50,								// Max Generations
				6,								// Tournament size
				0.1,							// Mutation Rate
				0.9,							// Crossover Rate
				0.3,							// Elitism %
				CrossoverType::PARTIALLY_MAPPED,
				MutationType::INSERTION,
				Selection::TOURNAMENT);
			auto solver = GeneticAlgorithm<int>(params);
			RunConfig(solver, graph, max_reps, "selection\\tournament_25_medium");
		}
		TEST_METHOD(BenchmarkTournament25Big) {
			srand(time(0));
			auto graph = ParseGraphFile<SymmetricAdjacencyMatrix<int>, int>(tsplib_symmetric_path + "rat195.txt");
			auto params = GeneticParameters(
				25,								// Population size
				50,								// Max Generations
				6,								// Tournament size
				0.1,							// Mutation Rate
				0.9,							// Crossover Rate
				0.3,							// Elitism %
				CrossoverType::PARTIALLY_MAPPED,
				MutationType::INSERTION,
				Selection::TOURNAMENT);
			auto solver = GeneticAlgorithm<int>(params);
			RunConfig(solver, graph, max_reps, "selection\\tournament_25_big");
		}

	};
}