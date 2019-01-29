#pragma once
#include <string>
#include <fstream>
#include <map>
#include <vector>
#include "../TSP/Solvers/Genetic/GeneticAlgorithm.h"
using std::string;
using std::map;
using std::vector;
using namespace TSP;
class GlobalConfig {
public:	
	GlobalConfig();
	~GlobalConfig();
	int GetTourCost(string instance_name);
	GeneticParameters GetParams();
	vector<string> GetSymInstances() { return sym_inst; }
	vector<string> GetAsymInstances() { return asym_inst; }
private:
	string best_cost_path = "C:\\Users\\Jakub\\source\\repos\\JakubSokolowski\\TSP\\TSP\\Benchmarks\\ProblemData\\best_solutions.txt";
	map<string, int> costs;
	vector<string> sym_inst;
	vector<string> asym_inst;
	void ReadCosts();
};

