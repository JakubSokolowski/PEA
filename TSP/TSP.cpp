// TSP.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "AsymmetricAdjacencyMatrix.h"
#include "SymmetricAdjacencyMatrix.h"
#include "GraphDataParser.h"
#include "LittleMatrix.h"
#include "BranchAndBound.h"
#include "PerformanceTests.h"
#include <fstream>
#include <conio.h>
using namespace TSP;

using std::cout;
using std::endl;
using std::string;

void StartMenu()
{
	Solution<int> result;
	double time = 0.0;
	uint visited_nodes = 0;
	char option = '1';
	do
	{
		cout << "\n--Menu--" << endl;
		cout << "Travelling salesman problem using Branch and Bound method" << endl;
		cout << "1. Solve symmetric instance" << endl;
		cout << "2. Solve asymmetric instance" << endl;
		cout << "3. Show last resul" << endl;
		cout << "O. Exit" << endl;

		option = _getche();
	
		string filepath;
		switch (option)
		{
			case '1':
			{
				cout <<"\nEnter the relative filepath: " << endl;
				std::getline(std::cin, filepath);
				if (FileExists(filepath))
				{
					auto graph = ParseGraphFile<SymmetricAdjacencyMatrix<int>, int>(filepath);
					auto solver = BranchAndBound<int>();				
					StartCounter();
					result = solver.Solve(graph);
					time = GetCounter();
					visited_nodes = solver.VisitedNodesCount();
					cout << "\nTime of solving: " << time << " ms. Visited " << visited_nodes << " nodes." << endl;
					cout << "The optimal tour: " << endl;
					result.PrintTour();
					cout << "The total tour distance: " << result.distance_;
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
					result = solver.Solve(graph);
					time = GetCounter();
					visited_nodes = solver.VisitedNodesCount();
					cout << "\nTime of solving: " << time << " ms. Visited " << visited_nodes << " nodes." << endl;
					cout << "The optimal tour: " << endl;
					result.PrintTour();
					cout << "The total tour distance: " << result.distance_ << endl;
				}
			}break;
			case '3':
			{	
				cout << "\nTime of solving: " << time << " ms. Visited " << visited_nodes << " nodes." << endl;
				cout << "The optimal tour: " << endl;
				result.PrintTour();
				cout << "The total tour distance: " << result.distance_ << endl;
				
			}break;
			case '0':
				break;
			default:
				break;
		}
	} while (option != 0);
}

int main()
{
	StartMenu();
	//RunTests(100);
}


