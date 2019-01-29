#include "stdafx.h"
#include "BestTours.h"


GlobalConfig::GlobalConfig() {
	ReadCosts();
}


GlobalConfig::~GlobalConfig() {
}

int GlobalConfig::GetTourCost(string instance_name) {
	return costs[instance_name];
}

TSP::GeneticParameters GlobalConfig::GetParams() {
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
	return params;
}

void GlobalConfig::ReadCosts() {
	std::fstream stream(best_cost_path);
	string name;
	int cost;
	bool symmetric = true;
	while (stream >> name >> cost) {
		costs.insert(std::make_pair(name, cost));
		if (name == "AS10")
			symmetric = false;
		if (symmetric) {
			sym_inst.push_back(name);
		}  else {
			asym_inst.push_back(name);
		}
	}
	stream.close();
}
