#pragma once

#ifndef TSP_PERFORMANCE_TESTS
#define TSP_PERFORMANCE_TESTS


#define NOMINMAX

#include "../stdafx.h"
#include <Windows.h>
#include "../Graphs/AdjacencyMatrix/SymmetricAdjacencyMatrix.h"
#include "../Graphs/GraphDataParser.h"
#include "../Solvers/BranchAndBound/BranchAndBound.h"
#include "../Solvers/TabuSearch/TabuSearch.h"
#include <vector>
#include <string>
#include <random>
#include <time.h>
#include <stdio.h>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <algorithm>

namespace TSP
{
	using std::vector;
	using std::string;	
	using std::cout;
	using std::endl;

	double PCFreq = 0.0;
	__int64 CounterStart = 0;

	// PATHS

	string symmetric_path = "C:\\Users\\jakub\\Documents\\Visual Studio 2017\\Projects\\PEA\\TSP\\Benchmarks\\ProblemData\\TSPLIB\\Symmetric\\";
	string asymmetric_path = "C:\\Users\\jakub\\Documents\\Visual Studio 2017\\Projects\\PEA\\TSP\\Benchmarks\\ProblemData\\TSPLIB\\Asymmetric\\";
	string tabu_result_path = "C:\\Users\\jakub\\Documents\\Visual Studio 2017\\Projects\\PEA\\TSP\\Benchmarks\\Results\\Tabu\\";

	// TIME MEASUREMENT

	extern double time_unit = 1000.0;
	void StartCounter()
	{
		LARGE_INTEGER li;
		if (!QueryPerformanceFrequency(&li))
			std::cout << "QueryPerformanceFrequency failed!\n";

		PCFreq = double(li.QuadPart) / time_unit;

		QueryPerformanceCounter(&li);
		CounterStart = li.QuadPart;
	}
	double GetCounter()
	{
		LARGE_INTEGER li;
		QueryPerformanceCounter(&li);
		return double(li.QuadPart - CounterStart) / PCFreq;
	}	
	string GetTimestamp()
	{
		auto t = std::time(nullptr);
		auto tm = *std::localtime(&t);

		std::ostringstream oss;
		oss << std::put_time(&tm, "%d-%m-%Y %H:%M:%S");
		auto str = oss.str();
		return str;
	}

	// FILE OPERATIONS INSTANCE GENERATION AND STATISTICS

	bool FileExists(std::string filename)
	{
		std::ifstream f(filename.c_str());
		return f.good();
	}
	std::vector<std::vector<int>> GenerateAsymmetricMatrix(int size, int max_value)
	{
		std::random_device rd; // obtain a random number from hardware
		std::mt19937 eng(rd()); // seed the generator_m
		std::uniform_int_distribution<> distr(1, max_value); // define the range

		std::vector<std::vector<int>> matrix(size, std::vector<int>(size, INT_MAX));
		for (uint i = 0; i < matrix.size(); i++)
			for (uint j = 0; j < matrix.size(); j++)
			{
				if (i == j)
					continue;
				matrix[i][j] = distr(eng);
			}

		return matrix;
	}
	std::vector<std::vector<int>> GenerateSymmetricMatrix(int size, int max_value)
	{
		std::random_device rd; // obtain a random number from hardware
		std::mt19937 eng(rd()); // seed the generator_m
		std::uniform_int_distribution<> distr(1, max_value); // define the range

		std::vector<std::vector<int>> matrix(size, std::vector<int>(size, INT_MAX));
		for (uint i = 0; i < matrix.size(); i++)
			for (uint j = 0; j < matrix.size(); j++)
			{
				if (i == j)
					continue;
				auto rand = distr(eng);
				matrix[i][j] = rand;
				matrix[j][i] = rand;
			}

		return matrix;
	}
	void WriteAsymmetricMatrix(std::vector<std::vector<int>> &vec)
	{

		string filename = "C:\\Users\\jakub\\Documents\\Visual Studio 2017\\Projects\\PEA\\TSP\\"
			+ std::to_string(vec.size()) + "as.txt";
		if (FileExists(filename))
			return;
		std::ofstream file(filename);

		file << "MATRIX" << "\n";
		for (uint i = 0; i < vec.size(); i++)
		{
			for (uint j = 0; j < vec.size(); j++)
				file << vec[i][j] << " ";
			file << "\n";
		}
		file.close();

	}
	void WriteSymmetricMatrix(std::vector<std::vector<int>> &vec)
	{

		string filename = "C:\\Users\\jakub\\Documents\\Visual Studio 2017\\Projects\\PEA\\TSP\\"
			+ std::to_string(vec.size()) + ".txt";
		if (FileExists(filename))
			return;
		std::ofstream file(filename);

		file << "MATRIX" << "\n";
		for (uint i = 0; i < vec.size(); i++)
		{
			for (uint j = 0; j < vec.size(); j++)
				file << vec[i][j] << " ";
			file << "\n";
		}
		file.close();

	}
	void GenerateProblemData()
	{
		auto instances = vector<int>{ 10 };
		for (auto instance : instances)
		{
			WriteSymmetricMatrix(GenerateSymmetricMatrix(instance, 999));
			WriteAsymmetricMatrix(GenerateAsymmetricMatrix(instance, 999));
		}
	}
	void WriteTestResults(vector<double> &results, vector<std::pair<double,double>> &statistics, string filename, bool symmetric)
	{
		string dir = symmetric ? "Symmetric" : "Asymmetric";
		string filepath = "C:\\Users\\jakub\\Documents\\Visual Studio 2017\\Projects\\PEA\\TSP\\" + dir + " Results\\"
			+ filename + ".txt";
		if (FileExists(filepath))
			return;

		std::ofstream file(filepath);

		for (auto result : results)
		{
			file << result << " ";
		}
		file << "\n";

		for (auto stat : statistics)
		{
			file << stat.first << " " << stat.second << "\n";
		}

		file.close();
	}
	inline void CalculateStdev(std::vector<double>& v, double &stdev, double &variance)
	{
		double sum = std::accumulate(v.begin(), v.end(), 0.0);
		double mean = sum / v.size();

		std::vector<double> diff(v.size());
		std::transform(v.begin(), v.end(), diff.begin(), [mean](double x) { return x - mean; });
		double sq_sum = std::inner_product(diff.begin(), diff.end(), diff.begin(), 0.0);
		variance = sq_sum / v.size();
		stdev = std::sqrt(sq_sum / v.size());
	}
	
	// BRANCH AND BOUND

	void RunSymmetricTests(int reps)
	{
		cout << "Starting tests for Symmetric instances" << endl;

		auto problem_sizes = vector<int>{ 10, 17, 29 };
		auto time_results = vector<double>();
		auto all_results = vector<double>();
		auto statistics = vector<std::pair<double, double>>();

		GenerateProblemData();

		for (auto instance : problem_sizes)
		{
			std::cout << "Starting tests for size " << instance << std::endl;
			string filepath = "C:\\Users\\jakub\\Documents\\Visual Studio 2017\\Projects\\PEA\\TSP\\Symmetric Instances\\"
				+ std::to_string(instance) + ".txt";
			auto graph = ParseGraphFile<SymmetricAdjacencyMatrix<int>, int>(filepath);
			auto solver = BranchAndBound<int>();

			double time = 0.0;

			for (int i = 0; i < reps; i++)
			{
				StartCounter();
				solver.Solve(graph);
				auto result = GetCounter();
				all_results.push_back(result);
				time += result;
			}

			double stdev, var;
			CalculateStdev(all_results, stdev, var);

			statistics.push_back(std::pair<double, double>{stdev, var});
			all_results.clear();

			time = time / reps;
			time_results.push_back(time);
			std::cout << "End of tests for size " << instance << std::endl;
		}
		WriteTestResults(time_results, statistics, "Symmetric" + std::to_string(reps), true);
	}
	void RunAsymmetricTests(int reps)
	{
		cout << "Starting tests for Asymmetric instances" << endl;

		auto problem_sizes = vector<int>{ 10, 17, 33};
		auto time_results = vector<double>();
		auto all_results = vector<double>();
		auto statistics = vector<std::pair<double, double>>();
		GenerateProblemData();

		for (auto instance : problem_sizes)
		{
			std::cout << "Starting tests for size " << instance << std::endl;
			string filepath = "C:\\Users\\jakub\\Documents\\Visual Studio 2017\\Projects\\PEA\\TSP\\Asymmetric Instances\\"
				+ std::to_string(instance) + ".txt";
			auto graph = ParseGraphFile<SymmetricAdjacencyMatrix<int>, int>(filepath);
			auto solver = BranchAndBound<int>();

			double time = 0.0;

			for (int i = 0; i < reps; i++)
			{
				StartCounter();
				solver.Solve(graph);
				auto result = GetCounter();
				all_results.push_back(result);
				time += result;
			}

			double stdev, var;
			CalculateStdev(all_results, stdev, var);

			statistics.push_back(std::pair<double, double>{stdev, var});
			all_results.clear();
			time = time / reps;
			time_results.push_back(time);
			std::cout << "End of tests for size " << instance << std::endl;
		}
		WriteTestResults(time_results, statistics, "Asymmetric" + std::to_string(reps), false);
	}
	void RunTests(int reps)
	{
		RunSymmetricTests(reps);
		RunAsymmetricTests(reps);
	}
	
	
	// TABU SEARCH

	/*
		------------INSTANCES------------

		SYMMETRIC			ASYMMETRIC
		
		S10.txt		1140		
		S17.txt		2085?
		eil51.txt	426
		eil76.txt	538
		pr124.txt	
	*/

	vector<string> sym_instances = { "S10.txt","S17.txt","bays29.txt","eil51.txt","eil76.txt", "pr124.txt" };
	vector<int> sym_optimal = { 1140,2085,2020,426,538,59030 };
	vector<string> asym_instances = { "AS10.txt","AS17.txt","ftv33.txt","ft53.txt","ftv70.txt", "kro124.txt" };
	//vector<string> asym_instances = { "AS10.txt","AS17.txt"};
	vector<int> asym_optimal = { 10,139,1285,6905,1950,36230 };
	Timer timer{ TimeUnit::miliseconds };

	TabuParameters GetTestParams(int size)
	{
		TabuParameters params;
		params.max_time_s = 15 * 60;
		params.max_no_improve = 3 * size;
		params.max_iterations = 6 * size;
		params.tabu_list_length = 3 * size;
		params.restart_count = 2;
		return params;
	}

	// TIME ESTIMATION

	void AsymTimeEstimation()
	{
		string filepath = tabu_result_path + "asym_estimation.txt";	
		std::ofstream file(filepath, std::ios_base::app);		
		file << endl << GetTimestamp() << endl;
		file << "Instance,Cost,Time" << endl;
		std::cout << "Estimation test start!:  " << endl;

		for (auto instance : asym_instances)
		{
			cout << "Starting for graph " + instance << endl;
			string graph_path = asymmetric_path + instance;
			auto graph = ParseGraphFile<SymmetricAdjacencyMatrix<int>, int>(graph_path);

			auto params = GetTestParams(graph.GetNumOfVertices());
			auto solver = TabuSearch<int>(params);
			solver.TimeLimited = true;
			solver.IsSymmetric = false;		
			timer.Start();
			auto result = solver.Solve(graph);
			double time = timer.GetTime();

			cout << "Instance " + instance + " time: " << time << " seconds" << endl;
			file << instance << "," << result.total_cost << "," << time << endl;
		}
		file << GetTimestamp() << endl;
		file.close();

		cout << "Estimation tests for asymmetric instances finished! " << endl;
	}
	void SymTimeEstimation()
	{
		string filepath = tabu_result_path + "sym_estimation.txt";
		std::ofstream file(filepath, std::ios_base::app);
		file << endl << GetTimestamp() << endl;
		file << "Instance,Cost,Time" << endl;
		cout << "Estimation test start!:  " << endl;

		for (auto instance : sym_instances)
		{
			cout << "Starting for graph " + instance << endl;
			string graph_path = symmetric_path + instance;
			auto graph = ParseGraphFile<SymmetricAdjacencyMatrix<int>, int>(graph_path);

			cout << "Vertices: " << graph.GetNumOfVertices() << " Edges: " << graph.GetNumOfEdges() << endl;

			auto params = GetTestParams(graph.GetNumOfVertices());
			auto solver = TabuSearch<int>(params);
			solver.TimeLimited = true;
			solver.IsSymmetric = true;

			timer.Start();
			auto result = solver.Solve(graph);
			double time = timer.GetTime();

			cout << "Instance " + instance + " time: " << time << " seconds" << endl;
			file << instance << "," << result.total_cost << "," << time << endl;
		}
		file << GetTimestamp() << endl;
		file.close();
		cout << "Estimation tests for symmetric instances finished! " << endl;
	}
	void TimeEstimation()
	{
		SymTimeEstimation();
		AsymTimeEstimation();
	}

	// STATIC TENURE TABU TESTS

	vector<int> static_tenures = { 5,15,25,50 };

	void StaticTenureSymmetricTest(int reps)	
	{	
		for (auto instance : sym_instances)
		{
			cout << "Starting static tenure tests for symmetric graph from file " << instance << endl;
			string filepath = tabu_result_path + "static_tenure_" + instance;
			string graph_path = symmetric_path + instance;

			std::ofstream file(filepath, std::ios_base::app);
			file << endl << GetTimestamp() << endl;
			file << "Tenure,Mean Time,Mean Result" << endl;

			auto time_results = vector<double>();
			auto results = vector<int>();
			auto graph = ParseGraphFile<SymmetricAdjacencyMatrix<int>, int>(graph_path);
			auto params = GetTestParams(graph.GetNumOfVertices());

			for (auto tenure : static_tenures)
			{
				params.tabu_tenure = tenure;
				cout << "\tStarting tests for tenure " << tenure << endl;
				for (int i = 0; i < reps; i++)
				{
					auto solver = TabuSearch<int>(params);
					solver.TimeLimited = true;
					solver.IsSymmetric = true;
					timer.Start();
					auto result = solver.Solve(graph);
					time_results.push_back(timer.GetTime());
					results.push_back(result.total_cost);
				}
				double mean_time = std::accumulate(time_results.begin(), time_results.end(), 0.0) / time_results.size();
				double mean_result = std::accumulate(results.begin(), results.end(), 0.0) / results.size();
				file << tenure << "," << mean_time << "," << mean_result << endl;
			}
			file << GetTimestamp() << endl;
			file.close();
		}
		cout << " Static tenure tests for symmetric instances finished! " << endl;
	}
	void StaticTenureAsymmetricTest(int reps)
	{
		for (auto instance : asym_instances)
		{
			cout << "Starting static tenure tests for asymmetric graph from file " << instance << endl;
			string filepath = tabu_result_path + "static_tenure_" + instance;
			string graph_path = asymmetric_path + instance;

			std::ofstream file(filepath, std::ios_base::app);
			file << endl << GetTimestamp() << endl;
			file << "Tenure,Mean Time,Mean Result" << endl;

			auto time_results = vector<double>();
			auto results = vector<int>();
			auto graph = ParseGraphFile<SymmetricAdjacencyMatrix<int>, int>(graph_path);
			auto params = GetTestParams(graph.GetNumOfVertices());

			for (auto tenure : static_tenures)
			{
				params.tabu_tenure = tenure;
				cout << "\tStarting tests for tenure " << tenure << endl;
				for (int i = 0; i < reps; i++)
				{
					auto solver = TabuSearch<int>(params);
					solver.TimeLimited = true;
					solver.IsSymmetric = false;
					timer.Start();
					auto result = solver.Solve(graph);
					time_results.push_back(timer.GetTime());
					results.push_back(result.total_cost);
				}
				double mean_time = std::accumulate(time_results.begin(), time_results.end(), 0.0) / time_results.size();
				double mean_result = std::accumulate(results.begin(), results.end(), 0.0) / results.size();
				file << tenure << "," << mean_time << "," << mean_result << endl;
			}
			file << GetTimestamp() << endl;
			file.close();
		}
		cout << " Static tenure tests for asymmetric instances finished! " << endl;
	}
	void StaticTenureTests(int reps)
	{
		StaticTenureAsymmetricTest(reps);
		StaticTenureSymmetricTest(reps);
		
	}

	// PROPORTIONAL TENURE TABU TESTS


	vector<int> proportional_tenures = { 8,4,6,2 };

	void ProportionalTenureSymmetricTests(int reps)
	{
		for (auto instance : sym_instances)
		{
			cout << "Starting proportional tenure tests for symmetric graph from file " << instance << endl;
			string filepath = tabu_result_path + "proportional_tenure_" + instance;
			string graph_path = symmetric_path + instance;

			std::ofstream file(filepath, std::ios_base::app);
			file << endl << GetTimestamp() << endl;
			file << "Tenure,Mean Time,Mean Result" <<endl;

			auto time_results = vector<double>();
			auto results = vector<int>();
			auto graph = ParseGraphFile<SymmetricAdjacencyMatrix<int>, int>(graph_path);
			auto params = GetTestParams(graph.GetNumOfVertices());

			for (auto tenure : proportional_tenures)
			{
				params.tabu_tenure = (int)(graph.GetNumOfVertices() / tenure);
				cout << "\tStarting tests for tenure equal to size/ " << tenure << endl;
				for (int i = 0; i < reps; i++)
				{
					auto solver = TabuSearch<int>(params);
					solver.TimeLimited = true;
					solver.IsSymmetric = true;
					timer.Start();
					auto result = solver.Solve(graph);
					time_results.push_back(timer.GetTime());
					results.push_back(result.total_cost);
				}
				double mean_time = std::accumulate(time_results.begin(), time_results.end(), 0.0) / time_results.size();
				double mean_result = std::accumulate(results.begin(), results.end(), 0.0) / results.size();
				file << tenure << "," << mean_time << "," << mean_result << endl;
			}
			file << GetTimestamp() << endl;
			file.close();
		}
		cout << " Proportional tenure tests for symmetric instances finished! " << endl;
	}
	void ProportionalTenureAsymmetricTests(int reps)
	{
		for (auto instance : asym_instances)
		{
			cout << "Starting proportional tenure tests for asymmetric graph from file " << instance << endl;
			string filepath = tabu_result_path + "proportional_tenure_" + instance;
			string graph_path = asymmetric_path + instance;

			std::ofstream file(filepath, std::ios_base::app);
			file << endl << GetTimestamp() << endl;
			file << "Tenure,Mean Time,Mean Result" << endl;

			auto time_results = vector<double>();
			auto results = vector<int>();
			auto graph = ParseGraphFile<SymmetricAdjacencyMatrix<int>, int>(graph_path);
			auto params = GetTestParams(graph.GetNumOfVertices());

			for (auto tenure : proportional_tenures)
			{
				params.tabu_tenure = (int)(graph.GetNumOfVertices() / tenure);
				cout << "\tStarting tests for tenure equal to size/ " << tenure << endl;
				for (int i = 0; i < reps; i++)
				{
					auto solver = TabuSearch<int>(params);
					solver.TimeLimited = true;
					solver.IsSymmetric = true;
					timer.Start();
					auto result = solver.Solve(graph);
					time_results.push_back(timer.GetTime());
					results.push_back(result.total_cost);
				}
				double mean_time = std::accumulate(time_results.begin(), time_results.end(), 0.0) / time_results.size();
				double mean_result = std::accumulate(results.begin(), results.end(), 0.0) / results.size();
				file << tenure << "," << mean_time << "," << mean_result << endl;
			}
			file << GetTimestamp() << endl;
			file.close();
		}
		cout << " Proportional tenure tests for asymmetric instances finished! " << endl;
	}
	void ProportionalTenureTests(int reps)
	{
		ProportionalTenureAsymmetricTests(reps);
		ProportionalTenureSymmetricTests(reps);
	}

	void FinalSymmetricTests(int reps)
	{
		for (auto instance : sym_instances)
		{
			cout << "Starting final tests for symmetric graph from file " << instance << endl;
			string filepath = tabu_result_path + "final_" + instance;
			string graph_path = symmetric_path + instance;

			std::ofstream file(filepath, std::ios_base::app);
			file << endl << GetTimestamp() << endl;
			file << "Mean Time,Mean Result" << endl;

			auto time_results = vector<double>();
			auto results = vector<int>();
			auto graph = ParseGraphFile<SymmetricAdjacencyMatrix<int>, int>(graph_path);
			auto params = GetTestParams(graph.GetNumOfVertices());

			params.tabu_tenure = 25;

			for (int i = 0; i < reps; i++)
			{
				auto solver = TabuSearch<int>(params);
				solver.TimeLimited = true;
				solver.IsSymmetric = true;
				timer.Start();
				auto result = solver.Solve(graph);
				time_results.push_back(timer.GetTime());
				results.push_back(result.total_cost);
			}
			double mean_time = std::accumulate(time_results.begin(), time_results.end(), 0.0) / time_results.size();
			double mean_result = std::accumulate(results.begin(), results.end(), 0.0) / results.size();
			file << "," << mean_time << "," << mean_result << endl;
			file << GetTimestamp() << endl;
			file.close();
		}
		cout << " Final tests finished " << endl;
	}
	void FinalAsymmetricTests(int reps)
	{
		for (auto instance : asym_instances)
		{
			cout << "Starting final tests for asymmetric graph from file " << instance << endl;
			string filepath = tabu_result_path + "final_" + instance;
			string graph_path = asymmetric_path + instance;

			std::ofstream file(filepath, std::ios_base::app);
			file << endl << GetTimestamp() << endl;
			file << "Mean Time,Mean Result" << endl;

			auto time_results = vector<double>();
			auto results = vector<int>();
			auto graph = ParseGraphFile<SymmetricAdjacencyMatrix<int>, int>(graph_path);
			auto params = GetTestParams(graph.GetNumOfVertices());

			params.tabu_tenure = 25;

			for (int i = 0; i < reps; i++)
			{
				auto solver = TabuSearch<int>(params);
				solver.TimeLimited = true;
				solver.IsSymmetric = false;
				timer.Start();
				auto result = solver.Solve(graph);
				time_results.push_back(timer.GetTime());
				results.push_back(result.total_cost);
			}
			double mean_time = std::accumulate(time_results.begin(), time_results.end(), 0.0) / time_results.size();
			double mean_result = std::accumulate(results.begin(), results.end(), 0.0) / results.size();
			file << "," << mean_time << "," << mean_result << endl;
			file << GetTimestamp() << endl;
			file.close();
		}
		cout << " Final tests finished " << endl;
	}

	void FinalTests(int reps)
	{
		//FinalSymmetricTests( reps);
		FinalAsymmetricTests(reps);
	}

	void SymmetricTabuTenureProportionalTests(int reps)
	{
		auto tenures = vector<int>{ 2,4,6,8 };
		auto instances = vector<string>{ "S29.txt","eil51.txt","eil101.txt","rat195.txt" };
		// 10 tests per tenure
		TabuParameters params;
		params.max_time_s = 60 * 15;
		params.max_no_improve = 100;
		params.max_iterations = 500;
		params.restart_count = 2;
		Timer timer{ TimeUnit::miliseconds };

		for (int file_id = 0; file_id< instances.size(); file_id++)
		{

			std::cout << "Starting tests for file " << instances[file_id] << std::endl;
			string filepath = tabu_result_path + "Proportional_tenure_" + instances[file_id];
			string graph_path = symmetric_path + instances[file_id];

			std::ofstream file(filepath, std::ios_base::app);

			auto time_results = vector<double>();
			auto results = vector<int>();

			auto graph = ParseGraphFile<SymmetricAdjacencyMatrix<int>, int>(graph_path);
			for (int id = 0; id < tenures.size(); id++)
			{
				std::cout << "\tStarting tests for tenure " << tenures[id] << std::endl;
				for (int i = 0; i < reps; i++)
				{
					params.tabu_tenure = (int)( graph.GetNumOfVertices() / tenures[id]);
					params.tabu_list_length = graph.GetNumOfVertices() * 3;
					params.max_iterations = graph.GetNumOfVertices() * 4;
					params.max_no_improve = graph.GetNumOfVertices();
					auto solver = TabuSearch<int>(params);
					solver.TimeLimited = true;
					timer.Start();
					auto result = solver.Solve(graph);
					time_results.push_back(timer.GetTime());
					results.push_back(result.total_cost);
				}

				double mean_time = std::accumulate(time_results.begin(), time_results.end(), 0.0) / time_results.size();
				double mean_result = std::accumulate(results.begin(), results.end(), 0.0) / results.size();
				file << tenures[id] << " " << mean_time << " " << mean_result << std::endl;
			}
			file.close();
		}
		std::cout << " Proportional tenure tests finished! " << endl;
	}
	void SymmetricTabuIterationsProportionalTests(int reps)
	{
		auto max_iter_multiplier = vector<int>{ 5,10,20};
		auto max_no_improve_multiplier = vector<int>{ 2,4,8 };
		auto instances = vector<string>{ "S29.txt","eil51.txt","eil101.txt","rat195.txt"};

		TabuParameters params;
		params.max_time_s = 60 * 15;
		params.tabu_list_length = 50;
		params.max_no_improve = 100;
		params.max_iterations - 500;
		params.restart_count = 2;


		Timer timer{ TimeUnit::miliseconds };

		for (int file_id = 0; file_id< instances.size(); file_id++)
		{

			std::cout << "Starting tests for file " << instances[file_id] << std::endl;
			string filepath = tabu_result_path + "Proportional_tenure_" + instances[file_id];
			string graph_path = symmetric_path + instances[file_id];

			std::ofstream file(filepath, std::ios_base::app);

			auto time_results = vector<double>();
			auto results = vector<int>();

			auto graph = ParseGraphFile<SymmetricAdjacencyMatrix<int>, int>(graph_path);
			for (int id = 0; id < max_iter_multiplier.size(); id++)
			{
				std::cout << "\tStarting tests for tenure " << max_iter_multiplier[id] << std::endl;
				for (int i = 0; i < reps; i++)
				{
					params.tabu_tenure = 15;
					params.max_iterations = graph.GetNumOfVertices() * max_iter_multiplier[id];
					params.max_no_improve = graph.GetNumOfVertices() * max_no_improve_multiplier[id];
					params.tabu_list_length = graph.GetNumOfVertices() * 3;
					auto solver = TabuSearch<int>(params);
					solver.TimeLimited = true;
					timer.Start();
					auto result = solver.Solve(graph);
					time_results.push_back(timer.GetTime());
					results.push_back(result.total_cost);
				}

				double mean_time = std::accumulate(time_results.begin(), time_results.end(), 0.0) / time_results.size();
				double mean_result = std::accumulate(results.begin(), results.end(), 0.0) / results.size();
				file << max_iter_multiplier[id] << " " << mean_time << " " << mean_result << std::endl;
			}
			file.close();
		}
		std::cout << " Proportional iterations tests finished! " << endl;
	}

	// PROPORTIONAL TABU TESTS

	void RunTabuSymmetric(int reps)
	{

	}
	void RunTabuAsymmetric(int reps)
	{

	}

	void RunTabuTests(int reps)
	{
		RunTabuSymmetric(reps);
		RunTabuAsymmetric(reps);
	}

}
#endif 

