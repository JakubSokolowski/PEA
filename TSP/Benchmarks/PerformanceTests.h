#pragma once

#ifndef TSP_PERFORMANCE_TESTS
#define TSP_PERFORMANCE_TESTS


#define NOMINMAX

#include <Windows.h>
#include "SymmetricAdjacencyMatrix.h"
#include "GraphDataParser.h"
#include "BranchAndBound.h"
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

	double PCFreq = 0.0;
	__int64 CounterStart = 0;


	double time_unit = 1000.0;
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
		//time_t     now = time(0);
		//struct tm  tstruct;
		//char       buf[80];
		//tstruct = *localtime(&now);
		//// Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
		//// for more information about date/time format
		//strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

		//return buf;
		return "D";
	}

	bool FileExists(std::string filename)
	{
		std::ifstream f(filename.c_str());
		return f.good();
	}

	std::vector<std::vector<int>> GenerateAsymmetricMatrix(int size, int max_value)
	{
		std::random_device rd; // obtain a random number from hardware
		std::mt19937 eng(rd()); // seed the generator
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
		std::mt19937 eng(rd()); // seed the generator
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

		string filename = "C:\\Users\\jakub\\Documents\\Visual Studio 2017\\Projects\\PEA\\TSP\\Asymmetric Instances\\"
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
	void WriteSymmetricMatrix(std::vector<std::vector<int>> &vec)
	{

		string filename = "C:\\Users\\jakub\\Documents\\Visual Studio 2017\\Projects\\PEA\\TSP\\Symmetric Instances\\"
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
		auto instances = vector<int>{ 10, 29 };
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

	
	void CalculateStdev(std::vector<double>& v, double &stdev, double &variance)
	{
		double sum = std::accumulate(v.begin(), v.end(), 0.0);
		double mean = sum / v.size();

		std::vector<double> diff(v.size());
		std::transform(v.begin(), v.end(), diff.begin(), [mean](double x) { return x - mean; });
		double sq_sum = std::inner_product(diff.begin(), diff.end(), diff.begin(), 0.0);
		variance = sq_sum / v.size();
		stdev = std::sqrt(sq_sum / v.size());
	}
	

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

}
#endif 

