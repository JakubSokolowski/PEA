#pragma once

#ifndef TSP_GENETIC_ALGORITHM
#define TSP_GENETIC_ALGORITHM

#include "..\Solver.h"
#include "Population.h"
#include "Crossover\Crossover.h"
#include "Crossover\OX.h"
#include "Crossover\PMX.h"
#include "Crossover\CX.h"
#include "Mutation.h"
#include "Windows.h"
#include "../../Benchmarks/Timer.h"

namespace TSP
{
	using std::vector;
	using std::pair;
	using std::string;

	enum CrossoverType {
		ORDER_ONE,
		PARTIALLY_MAPPED,
		CYCLE
	};
	
	struct GeneticParameters
	{
		int population_size;
		int max_generations;
		double mutation_rate;
		double crossover_rate;
		int tournament_size;
		double elitism_rate;
		CrossoverType cros;
		MutationType mut;
		Selection sel;

		GeneticParameters() {}

		GeneticParameters(int pop_size, int max_gen, int trn_size, double mut_rate, double cross_rate, double elite_rate, CrossoverType c, MutationType m, Selection s)
			: population_size(pop_size)
			, max_generations(max_gen)
			, tournament_size(trn_size)
			, mutation_rate(mut_rate)
			, crossover_rate(cross_rate)
			, elitism_rate(elite_rate)
			, cros(c)
			, mut(m)
			, sel(s)
		{}
		void WriteToStream(std::ofstream &stream) {
			stream << "GENETIC PARAMETERS:\n";
			stream << "Population Size: \t" << population_size << '\n';
			stream << "Max Generations: \t" << max_generations << '\n';
			stream << "Crossover Type : \t" << ToString(cros) << '\n';
			stream << "Crossover Rate : \t" << crossover_rate << '\n';
			stream << "Mutation Type  : \t" << ToString(mut) << '\n';
			stream << "Mutation Rate  : \t" << mutation_rate << '\n';
			stream << "Elitism Rate   : \t" << elitism_rate << '\n';
			stream << "Selection Type : \t" << ToString(sel, tournament_size) << '\n';
		}
		static string ToString(CrossoverType type) {
			switch (type)
			{
			case TSP::ORDER_ONE:
				return "OX";
			case TSP::PARTIALLY_MAPPED:
				return "PMX";
			case TSP::CYCLE:
				return "CX";
			default:
				break;
			}
		}
		static string ToString(MutationType type) {
			switch (type)
			{
			case TSP::INSERTION:
				return "Insertion";
			case TSP::SWAP:
				return "Swap";
			case TSP::SCRAMBLE:
				return "Scramble";
			default:
				break;
			}
		}
		static string ToString(Selection type, int size) {
			switch (type)
			{
			case TSP::ROULETTE:
				return "Roulette";
			case TSP::TOURNAMENT:
				return "Tournament, " + std::to_string(size);
			default:
				break;
			}
		}
	};
	
	struct Benchmark {
		vector<int> most_fit_tours = vector<int>();
		double total_execution_time = 0.0;
		int instance_size = 0;
		int best_cost = 0;
		GeneticParameters params;

		Benchmark() {}

		void WriteToStream(std::ofstream &stream, bool head = false, bool time = false) {
			if(head)
				params.WriteToStream(stream);
			if (time) {
				stream << "Total Time:\tAvg gen time: \n";
				stream << total_execution_time << "\t" << total_execution_time / most_fit_tours.size() << "\n";
			}
		
			stream << "gen\tcost\n";
			for (int i = 0; i < most_fit_tours.size(); i++) {
				stream << i + 1 << "\t" << most_fit_tours[i] << "\n";
			}
			stream << std::endl;
		
		}
	
		void WriteToFile(string filename, bool head = false, bool time = false) {
			string filepath = "C:\\Users\\Jakub\\source\\repos\\JakubSokolowski\\TSP\\TSP\\Benchmarks\\Results\\Genetic\\" + filename + ".txt";
			std::ofstream file(filepath);
			WriteToStream(file, head, time);
			file.close();
		}
		void WriteSummaryToStream(std::ofstream &stream) {
			stream << total_execution_time << " " << best_cost << "\n";
		}
		void WriteSummaryToFile(string filename) {
			string filepath = "C:\\Users\\Jakub\\source\\repos\\JakubSokolowski\\TSP\\TSP\\Benchmarks\\Results\\Genetic\\" + filename + ".txt";
			std::ofstream file(filepath);
			if (file.good()) {
				file.close();
				file.open(filepath, std::ios_base::app);
			}
			WriteSummaryToStream(file);
			file.close();
		}
		static Benchmark Average(vector<Benchmark> &vec) {
			Benchmark result;
			double total_time = 0.0;
			double total_cost = 0.0;
			
			int generation_num = vec[0].params.max_generations;
			auto tours_num = vec[0].most_fit_tours.size();
			vector<int> tour_average(tours_num);
			int counter = 0;
			for (auto &summary : vec) {
				total_time += summary.total_execution_time;
				total_cost += summary.best_cost;
				int gen_counter = 0;
				for (auto &cost : summary.most_fit_tours) {
					tour_average[gen_counter] += cost;
					++gen_counter;
				}
				++counter;
			}
			for (auto &cost : tour_average) {
				cost /= vec.size();
			}

			auto avg_time = total_time / vec.size();
			auto avg_cost = total_cost / vec.size();

			result.most_fit_tours = tour_average;
			result.params = vec[0].params;
			result.total_execution_time = avg_time;
			result.best_cost = avg_cost;

			return result;
		}
	};

	template<class Cost>
	class GeneticAlgorithm
		: public Solver<Cost>
	{
	public:
		GeneticAlgorithm(GeneticParameters params);

		Solution<Cost> Solve(GraphRepresentation<Cost>& representation);
		void Evolve(Population<Cost> &pop, GraphRepresentation<Cost> &rep);
		Chromosome<Cost> Crossover(Chromosome<Cost> &c1, Chromosome<Cost> &c2);
		void Mutate(Population<Cost>& pop, GraphRepresentation<Cost>& rep, MutationType type);
		void WriteSummaryToFile(string filename);
		Benchmark GetSummary() {
			return summary_m;
		}
		GeneticParameters params_m;
	private:
		Timer timer = Timer(TimeUnit::seconds);
		Benchmark summary_m;
	
	};

	template<class Cost>
	inline GeneticAlgorithm<Cost>::GeneticAlgorithm(GeneticParameters params) {
		params_m = params;
		summary_m.params = params;
	}

	template<class Cost>
	inline Solution<Cost> GeneticAlgorithm<Cost>::Solve(GraphRepresentation<Cost>& representation) {
		timer.Start();
		summary_m = Benchmark();
		summary_m.instance_size = representation.GetNumOfVertices();
		summary_m.params = params_m;
		auto current_generation = Population<Cost>(params_m.population_size, representation);
		auto next_generation = Population<Cost>();
		for (int i = 0; i < params_m.max_generations; i++) {
			next_generation = Population<Cost>(current_generation, params_m.elitism_rate);
			while (!next_generation.IsFull()) {
				auto parents = current_generation.GetParents(params_m.sel);
				Chromosome<Cost> offspring = Crossover(parents.first, parents.second);
				next_generation.AddChromosome(offspring);
			}		
			Mutate(current_generation, representation, params_m.mut);
			next_generation.UpdateFitness(representation);
			current_generation = next_generation;			
			summary_m.most_fit_tours.push_back(current_generation.GetMostFit().total_cost);
		}
		summary_m.total_execution_time = timer.GetTime();
		summary_m.best_cost = current_generation.GetMostFit().total_cost;
		return current_generation.GetMostFit().GetSolution();
	}



	template<class Cost>
	inline void GeneticAlgorithm<Cost>::Evolve(Population<Cost>& pop, GraphRepresentation<Cost>& rep) {
		while (!pop.IsFull()) {
			auto first_parent = pop.TournamentSelection();
		}
	}
	template<class Cost>
	inline void GeneticAlgorithm<Cost>::Mutate(Population<Cost>& pop, GraphRepresentation<Cost>& rep, MutationType type) {
		for (auto &ch : pop.population_m) {
			if (RandomHelper::EventOccured(params_m.mutation_rate))
				Mutation<int>::Mutate(ch, type);
		}
	}
	template<class Cost>
	inline Chromosome<Cost> GeneticAlgorithm<Cost>::Crossover(Chromosome<Cost>& c1, Chromosome<Cost>& c2) {
		switch (params_m.cros) {
			case CrossoverType::ORDER_ONE: {
				auto cros = OX<int>();
				return cros.Execute(c1, c2);
			}
			case CrossoverType::PARTIALLY_MAPPED: {
				auto cros = PMX<int>();
				return cros.Execute(c1, c2);
			}
			case CrossoverType::CYCLE: {
				auto cros = CX<int>();
				return cros.Execute(c1, c2);
			}
		}
	}
	template<class Cost>
	inline void GeneticAlgorithm<Cost>::WriteSummaryToFile(string filename) {
		string filepath = genetic_result_path + filename + ".txt";
		std::ofstream file(filepath);
		summary_m.WriteToStream(file);
		file.close();
	}
}
#endif // !TSP_GENETIC_ALGORITHM



