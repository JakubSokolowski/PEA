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
		const int max_reps = 5;
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
			avg.WriteToFile(filename + "_" + timer.GetTimestamp());
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

	
	

		std::vector<std::string> sym_instances = { "10","S17","bays29", "eil51", "eil76","pr124","rat195" };
		std::vector<std::string> asym_instances = { "10", "17","ftv33" "ftv50", "ftv70", "kro124" };
		
		TEST_METHOD(BenchMarkSymmetircAll) {
			for (auto instance : sym_instances) {
				auto path = tsplib_symmetric_path + instance + ".txt";
				std::ofstream file(path);
				if (file.good())
					continue;
				auto graph = ParseGraphFile<SymmetricAdjacencyMatrix<int>, int>(path);
				auto params = GeneticParameters(
					100,							// Population size
					50,								// Max Generations
					10,								// Tournament size
					0.1,							// Mutation Rate
					0.8,							// Crossover Rate
					0.1,							// Elitism %
					CrossoverType::ORDER_ONE,
					MutationType::SWAP,
					Selection::ROULETTE);
				auto solver = GeneticAlgorithm<int>(params);
				RunConfig(solver, graph, 1, "time_" + instance);
			}
		}
		TEST_METHOD(BenchMarkSymmetric10) {		
			auto path = tsplib_symmetric_path + "S10.txt";
			auto graph = ParseGraphFile<SymmetricAdjacencyMatrix<int>, int>(path);
			auto params = GeneticParameters(
				10,							// Population size
				10,								// Max Generations
				10,								// Tournament size
				0.1,							// Mutation Rate
				0.8,							// Crossover Rate
				0.1,							// Elitism %
				CrossoverType::ORDER_ONE,
				MutationType::SWAP,
				Selection::ROULETTE);
			auto solver = GeneticAlgorithm<int>(params);
			RunConfig(solver, graph, 1, "time_10.txt");
		}
		TEST_METHOD(BenchmarkSymmetric) {
			auto instances = vector<string>{ "S10","S17","bays29" };
			auto params = GeneticParameters(
				10,							    // Population size
				10,								// Max Generations
				10,								// Tournament size
				0.1,							// Mutation Rate
				0.8,							// Crossover Rate
				0.1,							// Elitism %
				CrossoverType::ORDER_ONE,
				MutationType::SWAP,
				Selection::ROULETTE);
			auto solver = GeneticAlgorithm<int>(params);
			RunSymmetricComparision(solver, "symmetric_comparision", instances, 2);
		}

	};
}