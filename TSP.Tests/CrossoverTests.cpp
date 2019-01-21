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
			auto c1 = Chromosome<int>(0, { 0,8,4,7,3,6,2,5,1,9,0 });
			auto c2 = Chromosome<int>(0, { 0,1,2,3,4,5,6,7,8,9,0 });
			auto expected = std::vector<int>{0,1,4,7,3,6,2,5,8,9,0,};
			auto crossover = PMX<int>();
			auto actual = crossover.CrossoverAtPoints(c1, c2, 3, 7).tour;
			Assert::AreEqual(RandomHelper::ToString(expected).c_str(), RandomHelper::ToString(actual).c_str());
		}
		TEST_METHOD(PMXCrossoverAtPoints2) {
			auto c1 = Chromosome<int>(0, { 0,8,4,7,3,6,2,5,1,9,0 });
			auto c2 = Chromosome<int>(0, { 0,1,2,3,4,5,6,7,8,9,0 });
			auto expected = std::vector<int>{ 0, 8, 7, 4, 3, 6, 2, 5, 1, 9, 0 };
			auto crossover = PMX<int>();
			auto actual = crossover.CrossoverAtPoints(c1, c2, 4, 8).tour;
			Assert::AreEqual(RandomHelper::ToString(expected).c_str(), RandomHelper::ToString(actual).c_str());
		}
		TEST_METHOD(PMXCrossoverAtPointsResearchGateExample) {
			auto c1 = Chromosome<int>(0, {0,9,2,7,5,4,3,6,1,8,0 });
			auto c2 = Chromosome<int>(0, { 0,2,8,3,6,9,5,7,4,1,0});
			auto expected = std::vector<int>{ 0, 2, 8, 7, 5, 4, 6, 3, 9, 1, 0 };
			auto crossover = PMX<int>();
			auto actual = crossover.CrossoverAtPoints(c1, c2, 3, 5).tour;
			Assert::AreEqual(RandomHelper::ToString(expected).c_str(), RandomHelper::ToString(actual).c_str());
		}
		TEST_METHOD(PMXCrossoverAtPointsCSStackExample) {
			auto c1 = Chromosome<int>(0, { 0,1,4,2,8,5,7,3,6,9,0 });
			auto c2 = Chromosome<int>(0, { 0,7,5,3,1,9,8,6,4,2,0});
			auto expected = std::vector<int>{ 0,1,9,2,8,5,7,6,4,3,0 };
			auto crossover = PMX<int>();
			auto actual = crossover.CrossoverAtPoints(c1, c2, 3, 6).tour;
			Assert::AreEqual(RandomHelper::ToString(expected).c_str(), RandomHelper::ToString(actual).c_str());
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
			auto c1 = Chromosome<int>(0, { 0,8,4,7,3,6,2,5,1,9,0});
			auto c2 = Chromosome<int>(0, { 0,1,2,3,4,5,6,7,8,9,0 });
			auto crossover = CX<int>();
			crossover.FillMaps(c1, c2);
			auto expected = Cycle{ 8,1 };
			auto actual = crossover.FindCycle(1, c1, c2);
			Assert::AreEqual(RandomHelper::ToString(expected).c_str(), RandomHelper::ToString(actual).c_str());
		}
		TEST_METHOD(CXFindSecondCycle) {
			auto c1 = Chromosome<int>(0, { 0,8,4,7,3,6,2,5,1,9,0 });
			auto c2 = Chromosome<int>(0, { 0,1,2,3,4,5,6,7,8,9,0 });
			auto crossover = CX<int>();
			crossover.FillMaps(c1, c2);
			auto expected = Cycle{ 4,2,6,5,7,3 };
			auto actual = crossover.FindCycle(2, c1, c2);
			Assert::AreEqual(RandomHelper::ToString(expected).c_str(), RandomHelper::ToString(actual).c_str());
		}
		TEST_METHOD(CXFindAllCycles) {
			auto c1 = Chromosome<int>(0, { 0,8,4,7,3,6,2,5,1,9,0 });
			auto c2 = Chromosome<int>(0, { 0,1,2,3,4,5,6,7,8,9,0 });
			auto crossover = CX<int>();
			crossover.FillMaps(c1, c2);
			auto cycles = crossover.FindAllCycles(c1, c2);
			Assert::AreEqual(RandomHelper::ToString(Cycle{ 8,1 }).c_str(), RandomHelper::ToString(cycles[0]).c_str());
			Assert::AreEqual(RandomHelper::ToString(Cycle{ 4,2,6,5,7,3 }).c_str(), RandomHelper::ToString(cycles[1]).c_str());
			Assert::AreEqual(RandomHelper::ToString(Cycle{ 9}).c_str(), RandomHelper::ToString(cycles[2]).c_str());
		}

		TEST_METHOD(CXTakeFirstParentChunk) {
			auto c1 = Chromosome<int>(0, { 0,8,4,7,3,6,2,5,1,9,0 });
			auto c2 = Chromosome<int>(0, { 0,1,2,3,4,5,6,7,8,9,0 });
			auto crossover = CX<int>();
			crossover.FillMaps(c1, c2);
			auto cycles = crossover.FindAllCycles(c1, c2);
			auto actual = crossover.TakeFirstParentChunk(c1, 1);
			auto expected = std::vector<int>{ 8 };
			Assert::AreEqual(RandomHelper::ToString(expected).c_str(), RandomHelper::ToString(actual).c_str());		
		}
		TEST_METHOD(CXTakeLastFirstParentChunk) {
			auto c1 = Chromosome<int>(0, { 0,8,4,7,3,6,2,5,1,9,0 });
			auto c2 = Chromosome<int>(0, { 0,1,2,3,4,5,6,7,8,9,0 });
			auto crossover = CX<int>();
			crossover.FillMaps(c1, c2);
			auto cycles = crossover.FindAllCycles(c1, c2);
			auto actual = crossover.TakeFirstParentChunk(c1, 9);
			auto expected = std::vector<int>{ 9 };
			Assert::AreEqual(RandomHelper::ToString(expected).c_str(), RandomHelper::ToString(actual).c_str());
		}
		TEST_METHOD(CXTakeSecondParentChunk) {
			auto c1 = Chromosome<int>(0, { 0,8,4,7,3,6,2,5,1,9,0 });
			auto c2 = Chromosome<int>(0, { 0,1,2,3,4,5,6,7,8,9,0 });
			auto crossover = CX<int>();
			crossover.FillMaps(c1, c2);
			auto cycles = crossover.FindAllCycles(c1, c2);
			auto actual = crossover.TakeSecondParentChunk(c2, 2);
			auto expected = std::vector<int>{ 2,3,4,5,6,7 };
			Assert::AreEqual(RandomHelper::ToString(expected).c_str(), RandomHelper::ToString(actual).c_str());
		}
		TEST_METHOD(CXCreateChildTour) {
			auto c1 = Chromosome<int>(0, { 0,8,4,7,3,6,2,5,1,9,0 });
			auto c2 = Chromosome<int>(0, { 0,1,2,3,4,5,6,7,8,9,0 });
			auto crossover = CX<int>();
			crossover.FillMaps(c1, c2);
			auto cycles = crossover.FindAllCycles(c1, c2);
			auto actual = crossover.CreateChildTour(c1, c2);
			auto expected = std::vector<int>{ 0, 8, 2, 3, 4, 5, 6, 7, 1, 9, 0 };
			Assert::AreEqual(RandomHelper::ToString(expected).c_str(), RandomHelper::ToString(actual).c_str());
		}
		TEST_METHOD(CXExecute) {
			auto c1 = Chromosome<int>(0, { 0,8,4,7,3,6,2,5,1,9,0 });
			auto c2 = Chromosome<int>(0, { 0,1,2,3,4,5,6,7,8,9,0 });
			auto crossover = CX<int>();
			auto actual = crossover.Execute(c1, c2).tour;
			auto expected = std::vector<int>{0, 8, 2, 3, 4, 5, 6, 7, 1, 9, 0};
			Assert::AreEqual(RandomHelper::ToString(expected).c_str(), RandomHelper::ToString(actual).c_str());
		}
		TEST_METHOD(CXExecuteNormalIndices) {
			auto c1 = Chromosome<int>(0, { 1,9,5,8,4,7,3,6,2,10,1 });
			auto c2 = Chromosome<int>(0, { 1,2,3,4,5,6,7,8,9,10,1 });
			auto crossover = CX<int>();
			auto actual = crossover.Execute(c1, c2).tour;
			auto expected = std::vector<int>{ 1,9,3,4,5,6,7,8,2,10,1 };
			Assert::AreEqual(RandomHelper::ToString(expected).c_str(), RandomHelper::ToString(actual).c_str());
		}

	};
}