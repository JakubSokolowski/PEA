// TSP.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Graphs\AdjacencyMatrix\AsymmetricAdjacencyMatrix.h"
#include "Graphs\AdjacencyMatrix\SymmetricAdjacencyMatrix.h"
#include "Graphs\GraphDataParser.h"
#include "Solvers\BranchAndBound\LittleMatrix.h"
#include "Solvers\BranchAndBound\BranchAndBound.h"
#include "Benchmarks\PerformanceTests.h"
#include "Solvers\Solution.h"
#include <fstream>
#include <sstream>
#include <conio.h>
using namespace TSP;

using std::cout;
using std::endl;
using std::string;




void PrintParameters(TabuParameters p)
{
	cout << endl << "Tenure: " << p.tabu_tenure << endl;
	cout << "Tabu List Length: " << p.tabu_list_length << endl;
	cout << "Max Iterations: " << p.max_iterations << endl;
	cout << "Max non improving iterations: " << p.max_no_improve << endl;
	cout << "Max time [s]: " << p.max_time_s << endl;
	cout << "Number of restarts: " << p.restart_count << endl;
}

TabuParameters ReadFromString(std::string str)
{
	TabuParameters params;
	std::cout << "Enter parameters separate by space." << endl;
	cout << "The order is as follows: tenure, tabu list length, max iterations"<<endl
		<<" max non improving iterations, max time [s], number of restarts" << endl;
	std::istringstream is(str);
	is >> params.tabu_tenure >> params.tabu_list_length >> params.max_iterations >> params.max_no_improve >> params.max_time_s >> params.restart_count;
	return params;
}

void StartMenu()
{

	TabuParameters default_params;

	default_params.max_time_s = 5 * 60;
	default_params.tabu_list_length = 300;
	default_params.max_iterations = 1000;
	default_params.max_no_improve = 200;
	default_params.restart_count = 2;
	default_params.tabu_tenure = 25;


	Solution<int> result_bb, result_bf;
	Solution<int> result_ts;
	double time_bb = 0.0;
	double time_bf = 0.0;
	double time_ts;
	uint visited_nodes = 0;
	char option = '1';
	do
	{
		cout << "\n--Menu--" << endl;
		cout << "Solving travelling salesman problem" << endl;
		cout << "1. BruteForce" << endl;
		cout << "2. BranchAndBound" << endl;
		cout << "O. Exit" << endl;

		option = _getche();

		TabuParameters params;
		params.max_time_s = 5 * 60;
		params.tabu_list_length = 300;
		params.max_iterations = 600;
		params.max_no_improve = 300;
		params.restart_count = 2;
		params.tabu_tenure = 25;

		string filepath;
		switch (option)
		{
		case '1':
		{
			do
			{
				cout << "\n--Menu--" << endl;
				cout << "Travelling salesman problem using BruteForce method" << endl;
				cout << "1. Symmetric" << endl;
				cout << "2. Asymetric" << endl;
				cout << "3. Show last result" << endl;
				cout << "O. Exit" << endl;

				option = _getche();

				string filepath;
				switch (option)
				{
				case '1':
				{
					cout << "\nEnter the relative filepath: " << endl;
					std::getline(std::cin, filepath);
					if (FileExists(filepath))
					{
						auto graph = ParseGraphFile<SymmetricAdjacencyMatrix<int>, int>(filepath);
						auto solver = BruteForce<int>();
						StartCounter();
						result_bf = solver.Solve(graph);
						time_bf = GetCounter();
						cout << "\nTime of solving: " << time_bf << endl;
						cout << "The optimal tour: " << endl;
						result_bf.PrintTour();
						cout << "The total tour distance: " << result_bf.total_cost;
					}
				}break;

				case '2':
				{
					cout << "\nEnter the relative filepath: \n" << endl;
					std::getline(std::cin, filepath);
					if (FileExists(filepath))
					{
						auto graph = ParseGraphFile<AsymmetricAdjacencyMatrix<int>, int>(filepath);
						auto solver = BruteForce<int>();
						StartCounter();
						result_bf = solver.Solve(graph);
						time_bf = GetCounter();
						cout << "\nTime of solving: " << time_bf << endl;
						cout << "The optimal tour: " << endl;
						result_bf.PrintTour();
						cout << "The total tour distance: " << result_bf.total_cost << endl;
					}
				}break;
				case '3':
				{
					cout << "\nTime of solving: " << time_bf << endl;
					cout << "The optimal tour: " << endl;
					result_bf.PrintTour();
					cout << "The total tour distance: " << result_bf.total_cost << endl;

				}break;
				case '0':
					break;
				default:
					break;
				}
			} while (option != '0');
		}break;
		case '2':
		{
			do
			{
				cout << "\n--Menu--" << endl;
				cout << "Travelling salesman problem using Branch and Bound method" << endl;
				cout << "1. Symmetric" << endl;
				cout << "2. Asymetric" << endl;
				cout << "3. Show last result" << endl;
				cout << "O. Exit" << endl;

				option = _getche();

				string filepath;
				switch (option)
				{
				case '1':
				{
					cout << "\nEnter the relative filepath: " << endl;
					std::getline(std::cin, filepath);
					if (FileExists(filepath))
					{
						auto graph = ParseGraphFile<SymmetricAdjacencyMatrix<int>, int>(filepath);
						auto solver = BranchAndBound<int>();
						StartCounter();
						result_bb = solver.Solve(graph);
						time_bb = GetCounter();
						visited_nodes = solver.VisitedNodesCount();
						cout << "\nTime of solving: " << time_bb << " ms. Visited " << visited_nodes << " nodes." << endl;
						cout << "The optimal tour: " << endl;
						result_bb.PrintTour();
						cout << "The total tour distance: " << result_bb.total_cost;
					}
				}break;

				case '2':
				{
					cout << "\nEnter the relative filepath: \n" << endl;
					std::getline(std::cin, filepath);
					if (FileExists(filepath))
					{
						auto graph = ParseGraphFile<AsymmetricAdjacencyMatrix<int>, int>(filepath);
						auto solver = BranchAndBound<int>();
						StartCounter();
						result_bb = solver.Solve(graph);
						time_bb = GetCounter();
						visited_nodes = solver.VisitedNodesCount();
						cout << "\nTime of solving: " << time_bb << " ms. Visited " << visited_nodes << " nodes." << endl;
						cout << "The optimal tour: " << endl;
						result_bb.PrintTour();
						cout << "The total tour distance: " << result_bb.total_cost << endl;
					}
				}break;
				case '3':
				{
					cout << "\nTime of solving: " << time_bb << " ms. Visited " << visited_nodes << " nodes." << endl;
					cout << "The optimal tour: " << endl;
					result_bb.PrintTour();
					cout << "The total tour distance: " << result_bb.total_cost << endl;

				}break;
				case '0':
					break;
				default:
					break;
				}
			} while (option != '0');
		}break;

		case 'X':
		{
			do
			{
				cout << "\n--Menu--" << endl;
				cout << "Travelling salesman problem using Tabu Search method" << endl;
				cout << "1. Symmetric" << endl;
				cout << "2. Asymmetric" << endl;
				cout << "3. Enter Parameters" << endl;
				cout << "4. Show last result" << endl;
				cout << "5. Show parameters" << endl;
				cout << "O. Exit" << endl;

				option = _getche();

				string filepath;
				switch (option)
				{
				case '1':
				{
					cout << "\nEnter the relative filepath: " << endl;
					std::getline(std::cin, filepath);
					if (FileExists(filepath))
					{
						auto graph = ParseGraphFile<SymmetricAdjacencyMatrix<int>, int>(filepath);
						auto solver = TabuSearch<int>(params);
						solver.IsSymmetric = true;
						solver.TimeLimited = true;
						PrintParameters(params);
						StartCounter();
						result_ts = solver.Solve(graph);
						time_ts = GetCounter();
						cout << "\nTime of solving: " << time_ts << " [ms]" << endl;
						cout << "The tour: " << endl;
						result_ts.PrintTour();
						cout << "The total tour distance: " << result_ts.total_cost;
					}
				}break;

				case '2':
				{
					cout << "\nEnter the relative filepath: \n" << endl;
					std::getline(std::cin, filepath);
					if (FileExists(filepath))
					{
						auto graph = ParseGraphFile<SymmetricAdjacencyMatrix<int>, int>(filepath);
						auto solver = TabuSearch<int>(params);
						solver.IsSymmetric = false;
						solver.TimeLimited = true;
						PrintParameters(params);
						StartCounter();
						result_ts = solver.Solve(graph);
						time_ts = GetCounter();
						cout << "\nTime of solving: " << time_ts << " [ms]" << endl;
						cout << "The tour: " << endl;
						result_ts.PrintTour();
						cout << "The total tour distance: " << result_ts.total_cost << endl;
					}
				}break;
				case '3':
				{					
					cout << endl << "Enter parameters separate by space." << endl;
					cout << "The order is as follows: tenure, tabu list length, max iterations" << endl
						<< "max non improving iterations, max time [s], number of restarts" << endl;
					string str;
					std::getline(std::cin, str);
					std::istringstream is(str);
					is >> params.tabu_tenure >> params.tabu_list_length >> params.max_iterations >> params.max_no_improve >> params.max_time_s >> params.restart_count;

				}break;
				case '4':
				{
					cout << "\nTime of solving: " << time_ts << " [ms]" << endl;
					cout << "The tour: " << endl;
					result_ts.PrintTour();
					cout << "The total tour distance: " << result_ts.total_cost << endl;

				}break;
				case '5':
				{
					PrintParameters(params);
				}break;
				default:
					break;
				}
			} while (option != '0');
		}
		break;
		default:
			break;
		}
	} while (option != '0');
}

int main()
{
	//StartMenu();
	//GenerateProblemData();
	std::vector<int> sym{ 5,6,7,8,9,10 };
	std::vector<int> asym{ 5,7,10 };
	//RunBruteForceTests(100, sym, sym);
	//SolveProblems();
	RunBNBTests(10);
	//RunBruteForce(100);
	//TimeEstimation();
	//StaticTenuresTest();
	//SymmetricTabuTenureTests(5);
	//SymmetricTabuTenureProportionalTests(5);
	//SymmetricTabuTenureProportionalTests(5);
	//ProportionalTenureTests(5);
	//GenerateProblemData();
	//(1);
	//ProportionalTenureAsymmetricTests(5);
	//StaticTenureAsymmetricTest(5);
	//FinalTests(5);
	//std::cout << " Fin !" << std::endl;
	std::cin.get();
}

