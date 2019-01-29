#include "stdafx.h"
#include "CppUnitTest.h"
#include <limits>
#include "..\TSP\Graphs\AdjacencyMatrix\SymmetricAdjacencyMatrix.h"
#include "..\TSP\Graphs\GraphDataParser.h"
#include "..\TSP\Solvers\Genetic\Chromosome.h"
#include "..\TSP\Solvers\Genetic\Mutation.h"
#include "..\TSP\Solvers\Genetic\Population.h"
#include "..\TSP\Solvers\Genetic\GeneticAlgorithm.h"
#include "BestTours.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace TSP;


namespace Benchmarks
{
	TEST_CLASS(TimeBenchmark) {
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

		TEST_METHOD(TimeSymmetric) {
			auto conf = GlobalConfig();
			auto instances = conf.GetSymInstances();
			auto params = conf.GetParams();
		
			auto result_path = "C:\\Users\\Jakub\\source\\repos\\JakubSokolowski\\TSP\\TSP\\Benchmarks\\Results\\Genetic\\time\\time_sym.txt";
			auto conf1 = GeneticParameters(
				10,								// Population size
				10,								// Max Generations
				10,								// Tournament size
				0.1,							// Mutation Rate
				0.9,							// Crossover Rate
				0.05,							// Elitism %
				CrossoverType::PARTIALLY_MAPPED,
				MutationType::SWAP,
				Selection::ROULETTE);
			auto conf2 = GeneticParameters(
				10,								// Population size
				10,								// Max Generations
				10,								// Tournament size
				0.1,							// Mutation Rate
				0.9,							// Crossover Rate
				0.05,							// Elitism %
				CrossoverType::PARTIALLY_MAPPED,
				MutationType::SWAP,
				Selection::ROULETTE);
			std::ofstream stream(result_path);
			stream << "inst " << "conf " << "res " << "acc " << "time " << std::endl;
			bool big = false;
			for (auto inst : instances) {
				if (inst == "eil76") {
					big = true;
				}
				if (!big) {
					auto sol = GeneticAlgorithm<int>(conf1);
					auto graph = ParseGraphFile<SymmetricAdjacencyMatrix<int>, int>(tsplib_symmetric_path + inst + ".txt");
					auto avg = RepeatBenchmark(sol, graph, max_reps);
					stream  << graph.GetNumOfVertices() 
							<< " 1 " 
							<< avg.best_cost <<" " 
							<< avg.best_cost / conf.GetTourCost(inst) << " " 
							<< avg.total_execution_time << std::endl;
				}
				else {
					auto sol = GeneticAlgorithm<int>(conf2);
					auto graph = ParseGraphFile<SymmetricAdjacencyMatrix<int>, int>(tsplib_symmetric_path + inst + ".txt");
					auto avg = RepeatBenchmark(sol, graph, max_reps);
					stream << graph.GetNumOfVertices()
							<< " 2 "
							<< avg.best_cost << " "
							<< avg.best_cost / conf.GetTourCost(inst) << " "
							<< avg.total_execution_time << std::endl;
				}	
			}
			stream.close();
		}
		TEST_METHOD(TimeAsymmetric) {
			auto conf = GlobalConfig();
			auto instances = vector<string>{"kro124", "rgb323"};
			//auto instances = conf.GetAsymInstances();
			auto params = conf.GetParams();

			auto result_path = "C:\\Users\\Jakub\\source\\repos\\JakubSokolowski\\TSP\\TSP\\Benchmarks\\Results\\Genetic\\time\\time_asym.txt";
			auto conf1 = GeneticParameters(
				10,								// Population size
				10,								// Max Generations
				10,								// Tournament size
				0.1,							// Mutation Rate
				0.9,							// Crossover Rate
				0.05,							// Elitism %
				CrossoverType::PARTIALLY_MAPPED,
				MutationType::SWAP,
				Selection::ROULETTE);
			auto conf2 = GeneticParameters(
				1,								// Population size
				1,								// Max Generations
				10,								// Tournament size
				0.1,							// Mutation Rate
				0.9,							// Crossover Rate
				0.05,							// Elitism %
				CrossoverType::PARTIALLY_MAPPED,
				MutationType::SWAP,
				Selection::ROULETTE);
			std::ofstream stream(result_path);
			stream << "inst " << "conf " << "res " << "acc " << "time " << std::endl;
			bool big = false;
			for (auto inst : instances) {
				if (inst == "ftv70") {
					big = true;
				}
				if (!big) {
					auto sol = GeneticAlgorithm<int>(conf1);
					auto graph = ParseGraphFile<SymmetricAdjacencyMatrix<int>, int>(tsplib_asymmetric_path + inst + ".txt");
					auto avg = RepeatBenchmark(sol, graph, max_reps);
					stream << graph.GetNumOfVertices()
						<< " 1 "
						<< avg.best_cost << " "
						<< avg.best_cost / conf.GetTourCost(inst) << " "
						<< avg.total_execution_time << std::endl;
				}
				else {
					auto sol = GeneticAlgorithm<int>(conf2);
					auto graph = ParseGraphFile<SymmetricAdjacencyMatrix<int>, int>(tsplib_asymmetric_path + inst + ".txt");
					auto avg = RepeatBenchmark(sol, graph, max_reps);
					stream << graph.GetNumOfVertices()
						<< " 2 "
						<< avg.best_cost << " "
						<< avg.best_cost / conf.GetTourCost(inst) << " "
						<< avg.total_execution_time << std::endl;
				}
			}
			stream.close();
		}


	};
}