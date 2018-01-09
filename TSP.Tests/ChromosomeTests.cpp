#include "stdafx.h"
#include "CppUnitTest.h"
#include <limits>
#include "..\TSP\Graphs\AdjacencyMatrix\SymmetricAdjacencyMatrix.h"
#include "..\TSP\Solvers\Genetic\Chromosome.h"

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
	};
}