#include "stdafx.h"
#include "CppUnitTest.h"
#include <limits>
#include "..\TSP\Graphs\AdjacencyMatrix\SymmetricAdjacencyMatrix.h"
#include "..\TSP\Graphs\GraphDataParser.h"
#include "..\TSP\Solvers\Genetic\Chromosome.h"
#include "..\TSP\Solvers\Genetic\Population.h"
#include "..\TSP\Solvers\Genetic\Mutation.h";
#include <set>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace TSP;


namespace TSPTests
{
	TEST_CLASS(ChromosomeTests)
	{
		TEST_METHOD(ComparatorTest)
		{
			auto rhs = Chromosome<int>(20);
			auto lhs = Chromosome<int>(10);
			auto comparator = ChromosomeCompare<int>();
			Assert::IsTrue(comparator(lhs,rhs));
		}
		TEST_METHOD(CompareMethods) {
			auto rhs = Chromosome<int>(20);
			auto lhs = Chromosome<int>(10);
			Assert::IsTrue(rhs < lhs);
			Assert::IsFalse(rhs == lhs);
			Assert::IsFalse(rhs > lhs);
		}
		TEST_METHOD(MultisetFind) {
			std::multiset<Chromosome<int>> mset;
			for (int i = 0; i < 5; i++) {
				mset.insert(Chromosome<int>(i));
			}
			Assert::IsTrue(mset.end() != mset.find(Chromosome<int>(1)));
		}

		TEST_METHOD(UpdateFitness) {
			auto graph = ParseGraphFile<SymmetricAdjacencyMatrix<int>, int>(tsplib_asymmetric_path + "17.txt");
			auto pop = Population<int>(1, graph);
			auto chromosome = pop.GetMostFit();
			auto prev_fitness = chromosome.GetFitness();
			Mutation<int>::Swap(chromosome);
			chromosome.UpdateFitness(graph);
			Assert::AreNotEqual(prev_fitness, chromosome.GetFitness());
		}
	};
}