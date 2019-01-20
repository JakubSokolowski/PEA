#include "stdafx.h"
#include "CppUnitTest.h"
#include <limits>
#include "..\TSP\Graphs\AdjacencyMatrix\SymmetricAdjacencyMatrix.h"
#include "..\TSP\Solvers\Genetic\Chromosome.h"
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
	};
}