#include "stdafx.h"
#include "CppUnitTest.h"
#include <limits>
#include "..\TSP\Graphs\AdjacencyMatrix\SymmetricAdjacencyMatrix.h"
#include "..\TSP\Solvers\Genetic\RandomHelper.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace TSP;


namespace TSPTests
{
	TEST_CLASS(RandomHelperTests) {
		template<typename T>
		bool IsEqual(std::vector<T> const &v1, std::vector<T> const &v2) {
			return (v1.size() == v2.size() &&
				std::equal(v1.begin(), v1.end(), v2.begin()));
		}

		TEST_METHOD(GetRandomPairTest) {
			auto pair = RandomHelper::GetRandomPair(0, 10);
			Assert::IsTrue(pair[0] < pair[1]);
		}

		TEST_METHOD(ShuffleFragmentTest) {
			srand(time(0));
			auto vec = std::vector<int>{ 1, 2, 3, 4, 5, 6, 7, 8 };
			auto vec2 = vec;
			RandomHelper::ShuffleFragment(vec, 1, 6);
			Assert::AreEqual(1, vec.front());
			Assert::AreEqual(8, vec.back());
			Assert::IsFalse(IsEqual(vec, vec2));
		}

		TEST_METHOD(ShuffleRandomFragmentTest) {
			srand(time(0));
			auto vec = std::vector<int>{ 1, 2, 3, 4, 5, 6, 7, 8,9,10,11,12,13,14,15,16 };
			auto vec2 = vec;
			RandomHelper::ShuffleRandomFragment(vec);
			Assert::AreEqual(1, vec.front());
			Assert::AreEqual(16, vec.back());
			Assert::IsFalse(IsEqual(vec,vec2));
		}
	};
}