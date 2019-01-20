#include "stdafx.h"
#include "CppUnitTest.h"
#include <limits>
#include "..\TSP\Graphs\AdjacencyMatrix\SymmetricAdjacencyMatrix.h"
#include "..\TSP\Graphs\GraphDataParser.h"
#include "..\TSP\Solvers\Genetic\Chromosome.h"
#include "..\TSP\Solvers\Genetic\Crossover\OX.h"
#include "..\TSP\Solvers\Genetic\Crossover\PMX.h"
#include "..\TSP\Solvers\Genetic\Crossover\CX.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace TSP;


namespace TSPTests
{
	TEST_CLASS(CrossoverTests)
	{
		TEST_METHOD(OXCrossoverAtPoints) {
			auto c1 = Chromosome<int>(0,     { 0,8,4,7,3,6,2,5,1,9,0 });
			auto c2 = Chromosome<int>(0,	 { 0,1,2,3,4,5,6,7,8,9,0 });
			auto expected = std::vector<int> { 0,4,7,8,3,6,2,5,1,9,0 };
			auto crossover = OX<int>();
			auto actual = crossover.CrossoverAtPoints(c1, c2, 4, 8).tour;
			Assert::IsTrue(expected == actual);
		}
		TEST_METHOD(OXExecute) {
			auto c1 = Chromosome<int>(0, { 0,8,4,7,3,6,2,5,1,9,0 });
			auto c2 = Chromosome<int>(0, { 0,1,2,3,4,5,6,7,8,9,0 });
			auto crossover = OX<int>();
			auto actual = crossover.CrossoverAtPoints(c1, c2, 3, 7);
			Assert::IsFalse(c1.tour == actual.tour);
			Assert::IsFalse(c2.tour == actual.tour);
			Assert::AreEqual(c1.tour[0], actual.tour[0]);
		}

		
		TEST_METHOD(PMXCrossoverAtPoints) {
			auto c1 = Chromosome<int>(0, { 8,4,7,3,6,2,5,1,9,0 });
			auto c2 = Chromosome<int>(0, { 0,1,2,3,4,5,6,7,8,9 });
			auto expected = std::vector<int>{ 0,7,4,3,6,2,5,1,8,9};
			auto crossover = PMX<int>();
			auto actual = crossover.CrossoverAtPoints(c1, c2, 3, 7).tour;
			Assert::IsTrue(expected == actual);
		}
		TEST_METHOD(PMXCrossoverAtPointsResearchGateExample) {
			auto c1 = Chromosome<int>(0, {9,2,7,5,4,3,6,1,8 });
			auto c2 = Chromosome<int>(0, { 2,8,3,6,9,5,7,4,1});
			auto expected = std::vector<int>{ 2,8,6,5,4,3,7,9,1};
			auto crossover = PMX<int>();
			auto actual = crossover.CrossoverAtPoints(c1, c2, 3, 5).tour;
			Assert::IsTrue(expected == actual);
		}
		TEST_METHOD(PMXCrossoverAtPointsCSStackExample) {
			auto c1 = Chromosome<int>(0, {1,4,2,8,5,7,3,6,9 });
			auto c2 = Chromosome<int>(0, { 7,5,3,1,9,8,6,4,2});
			auto expected = std::vector<int>{ 1,9,6,8,5,7,3,4,2 };
			auto crossover = PMX<int>();
			auto actual = crossover.CrossoverAtPoints(c1, c2, 3, 6).tour;
			Assert::IsTrue(expected == actual);
		}

		TEST_METHOD(PMXExecute) {
			auto c1 = Chromosome<int>(0, { 0,8,4,7,3,6,2,5,1,9,0 });
			auto c2 = Chromosome<int>(0, { 0,1,2,3,4,5,6,7,8,9,0 });
			auto crossover = PMX<int>();
			auto actual = crossover.CrossoverAtPoints(c1, c2, 3, 7);
			Assert::IsFalse(c1.tour == actual.tour);
			Assert::IsFalse(c2.tour == actual.tour);
			Assert::AreEqual(c1.tour[0], actual.tour[0]);
		}

		TEST_METHOD(CXFillMaps) {
			auto c1 = Chromosome<int>(0, { 0,8,4,7,3,6,2,5,1,9,0 });
			auto c2 = Chromosome<int>(0, { 0,1,2,3,4,5,6,7,8,9,0 });
			auto crossover = CX<int>();
			crossover.FillMaps(c1, c2);
			Assert::AreEqual(crossover.c1_index[7], 3);
			Assert::AreEqual(crossover.c2_index[6], 6);
		}

		TEST_METHOD(CXFindFirstCycle) {
			auto c1 = Chromosome<int>(0, { 8,4,7,3,6,2,5,1,9,0});
			auto c2 = Chromosome<int>(0, { 0,1,2,3,4,5,6,7,8,9 });
			auto crossover = CX<int>();
			crossover.FillMaps(c1, c2);
			auto cycle = crossover.FindCycle(0, c1, c2);
			Assert::IsTrue(cycle == Cycle{8,0,9});
		}
		TEST_METHOD(CXFindSecondCycle) {
			auto c1 = Chromosome<int>(0, { 8,4,7,3,6,2,5,1,9,0 });
			auto c2 = Chromosome<int>(0, { 0,1,2,3,4,5,6,7,8,9 });
			auto crossover = CX<int>();
			crossover.FillMaps(c1, c2);
			auto cycle = crossover.FindCycle(1, c1, c2);
			Assert::IsTrue(cycle == Cycle{ 4,1,7,2,5,6 });
		}
		TEST_METHOD(CXFindAllCycles) {
			auto c1 = Chromosome<int>(0, { 8,4,7,3,6,2,5,1,9,0 });
			auto c2 = Chromosome<int>(0, { 0,1,2,3,4,5,6,7,8,9 });
			auto crossover = CX<int>();
			crossover.FillMaps(c1, c2);
			auto cycles = crossover.FindAllCycles(c1, c2);
			Assert::IsTrue(cycles[0] == Cycle{ 8,0,9 });
			Assert::IsTrue(cycles[1] == Cycle{ 4,1,7,2,5,6 });
			Assert::IsTrue(cycles[2] == Cycle{3});
		}

		TEST_METHOD(CXTakeFirstParentChunk) {
			auto c1 = Chromosome<int>(0, { 8,4,7,3,6,2,5,1,9,0 });
			auto c2 = Chromosome<int>(0, { 0,1,2,3,4,5,6,7,8,9 });
			auto crossover = CX<int>();
			crossover.FillMaps(c1, c2);
			auto cycles = crossover.FindAllCycles(c1, c2);
			auto chunk = crossover.TakeFirstParentChunk(c1, 0);
			Assert::IsTrue(chunk == std::vector<int>{ 8 });			
		}
		TEST_METHOD(CXTakeLastFirstParentChunk) {
			auto c1 = Chromosome<int>(0, { 8,4,7,3,6,2,5,1,9,0 });
			auto c2 = Chromosome<int>(0, { 0,1,2,3,4,5,6,7,8,9 });
			auto crossover = CX<int>();
			crossover.FillMaps(c1, c2);
			auto cycles = crossover.FindAllCycles(c1, c2);
			auto chunk = crossover.TakeFirstParentChunk(c1, 8);
			Assert::IsTrue(chunk == std::vector<int>{ 9,0 });
		}
		TEST_METHOD(CXTakeSecondParentChunk) {
			auto c1 = Chromosome<int>(0, { 8,4,7,3,6,2,5,1,9,0 });
			auto c2 = Chromosome<int>(0, { 0,1,2,3,4,5,6,7,8,9 });
			auto crossover = CX<int>();
			crossover.FillMaps(c1, c2);
			auto cycles = crossover.FindAllCycles(c1, c2);
			auto chunk = crossover.TakeSecondParentChunk(c2, 1);
			Assert::IsTrue(chunk == std::vector<int>{ 1,2 });
		}
		TEST_METHOD(CXCreateChildTour) {
			auto c1 = Chromosome<int>(0, { 8,4,7,3,6,2,5,1,9,0 });
			auto c2 = Chromosome<int>(0, { 0,1,2,3,4,5,6,7,8,9 });
			auto crossover = CX<int>();
			crossover.FillMaps(c1, c2);
			auto cycles = crossover.FindAllCycles(c1, c2);
			auto tour = crossover.CreateChildTour(c1, c2);
			Assert::IsTrue(tour == std::vector<int>{8,1,2,3,4,5,6,7,9,0});
		}
		TEST_METHOD(CXExecute) {
			auto c1 = Chromosome<int>(0, { 8,4,7,3,6,2,5,1,9,0 });
			auto c2 = Chromosome<int>(0, { 0,1,2,3,4,5,6,7,8,9 });
			auto crossover = CX<int>();
			auto child = crossover.Execute(c1, c2);
			Assert::IsTrue(child.tour == std::vector<int>{8, 1, 2, 3, 4, 5, 6, 7, 9, 0});
		}

	};
}