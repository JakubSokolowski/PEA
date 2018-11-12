#pragma once
#include "Timer.h"
#include <vector>
#include <string>
using std::vector;
using std::string;

enum PathType {
	TSPLIB_SYM,
	TSPLIB_ASYM,
	RANDOM_SYM,
	RANDOM_ASYM,
};
namespace TSP {
	class TestRunner {
	public:
		TestRunner();
		~TestRunner();
	private:
		Timer timer;
		

		void GenerateProblemData(const vector<int> &sizes) const;

	};



	TestRunner::TestRunner() {
	}


	TestRunner::~TestRunner() {
	}

	inline void TestRunner::GenerateProblemData(const vector<int>& sizes) const {
		for (auto size : sizes) {

		}
	}

}
