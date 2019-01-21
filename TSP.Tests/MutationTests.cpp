#include "stdafx.h"
#include "CppUnitTest.h"
#include <limits>
#include "..\TSP\Graphs\AdjacencyMatrix\SymmetricAdjacencyMatrix.h"
#include "..\TSP\Graphs\GraphDataParser.h"
#include "..\TSP\Solvers\Genetic\Chromosome.h"
#include "..\TSP\Solvers\Genetic\Mutation.h"
#include "..\TSP\Solvers\Genetic\Population.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace TSP;


namespace TSPTests
{
	TEST_CLASS(MutationTests)
	{
		TEST_METHOD(InsertAtIndexFromGreaterThanTo)
		{
			auto chromosome = Chromosome<int>(0, { 1,2,3,4,5,1 });
			Mutation<int>::InsertAtIndex(chromosome,4,1);
			auto expected = std::vector<int>{ 1,5,2,3,4,1 };
			auto actual = chromosome.tour;
			Assert::AreEqual(RandomHelper::ToString(expected).c_str(), RandomHelper::ToString(actual).c_str());
		}		

		TEST_METHOD(InsertAtIndexToGreaterThanFrom) {
			auto chromosome = Chromosome<int>(0, { 1,2,3,4,5,1 });
			Mutation<int>::InsertAtIndex(chromosome, 1, 4);
			auto expected = std::vector<int>{ 1,3,4,5,2,1 };
			auto actual = chromosome.tour;
			Assert::AreEqual(RandomHelper::ToString(expected).c_str(), RandomHelper::ToString(actual).c_str());
		}

		TEST_METHOD(InsertionMutationMultipleRandom) {
			auto chromosome = Chromosome<int>(0, { 1,2,3,4,5,1 });
			auto original = chromosome;
			for (int i = 0; i < 10 ; i++) {
				Mutation<int>::Insertion(chromosome);
				Assert::AreEqual(1, chromosome.tour.front());
				Assert::AreEqual(1, chromosome.tour.back());
				Assert::AreNotEqual(RandomHelper::ToString(original.tour).c_str(), 
					RandomHelper::ToString(chromosome.tour).c_str());
			}
		}

		TEST_METHOD(SwapAtIndicesFromGreaterThanTo) {
			auto chromosome = Chromosome<int>(0, { 1,2,3,4,5,1 });
			Mutation<int>::SwapAtIndices(chromosome, 4, 1);
			auto expected = std::vector<int>{ 1,5,3,4,2,1 };
			auto actual = chromosome.tour;
			Assert::AreEqual(RandomHelper::ToString(expected).c_str(), RandomHelper::ToString(actual).c_str());
		}

		TEST_METHOD(SwapAtIndicesToGreaterThanFrom) {
			auto chromosome = Chromosome<int>(0, { 1,2,3,4,5,1 });
			Mutation<int>::SwapAtIndices(chromosome, 1, 4);
			auto expected = std::vector<int>{ 1,5,3,4,2,1 };
			auto actual = chromosome.tour;
			Assert::AreEqual(RandomHelper::ToString(expected).c_str(), RandomHelper::ToString(actual).c_str());
		}

		TEST_METHOD(SwapMutationMultipleRandom) {
			auto chromosome = Chromosome<int>(0, { 1,2,3,4,5,1 });
			auto original = chromosome;
			for (int i = 0; i < 10; i++) {
				Mutation<int>::Swap(chromosome);
				Assert::AreEqual(1, chromosome.tour.front());
				Assert::AreEqual(1, chromosome.tour.back());
				Assert::AreNotEqual(RandomHelper::ToString(original.tour).c_str(),
					RandomHelper::ToString(chromosome.tour).c_str());
			}
		}

		TEST_METHOD(ScrambleMutationMutipleRandom) {
			srand(time(NULL));
			auto base = Chromosome<int>(0, { 1,2,3,4,5,1 });
			int same_counter = 0, different_counter = 0;
			for (int i = 0; i < 50; i++) {
				auto chromosome = base;
				Mutation<int>::ScrambleBetweenIndices(chromosome,1,4);
				Assert::AreEqual(1, chromosome.tour.front());
				Assert::AreEqual(1, chromosome.tour.back());
				if (base.tour == chromosome.tour)
					same_counter++;
				else
					different_counter++;
			}
			Logger::WriteMessage(std::to_string(different_counter).c_str());
			Assert::IsTrue(different_counter > same_counter);
		}

		
	};
}