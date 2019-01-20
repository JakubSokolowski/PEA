#pragma once

#ifndef TSP_RANDOM_HELPER
#define TSP_RANDOM_HELPER

#include <random>
#include <iterator>
#include <vector>

namespace TSP
{	

	class RandomHelper
	{
	public:
		static int GetRandomInRange(int from, int to);
		static bool EventOccured(double probability);
		static std::vector<int> GetRandomPair(int from, int to);
		

		template <typename I>
		static I RandomElement(I begin, I end);

		template <typename I>
		static void ShuffleRandomFragment(std::vector<I> &vec);

		template <typename I>
		static void ShuffleFragment(std::vector<I> &vec, int from, int to);
	private:
		static std::mt19937& Generator() { static std::mt19937 gen; return gen; }
		RandomHelper() {}
	};

	template<typename I>
	static inline I RandomHelper::RandomElement(I begin, I end)
	{
		std::uniform_int_distribution<> distr(0, std::distance(begin, end) - 1);
		std::advance(begin, distr(Generator()));
		return begin;
	}

	template<typename I>
	inline void RandomHelper::ShuffleRandomFragment(std::vector<I>& vec) {
		auto len = vec.size();
		if (len > 1) {
			auto from = GetRandomInRange(1, len - 2);
			auto to = GetRandomInRange(1, len - 2);
			ShuffleFragment(vec, std::min(from, to), std::max(from, to));
		}		
	}

	template<typename I>
	inline void RandomHelper::ShuffleFragment(std::vector<I>& vec, int from, int to) {
		std::random_shuffle(vec.begin() + from, vec.begin() + to);
	}


	inline int RandomHelper::GetRandomInRange(int from, int to)
	{
		std::uniform_int_distribution<int> distr(from, to);
		return distr(Generator());
	}
	inline bool RandomHelper::EventOccured(double probability)
	{		
		std::bernoulli_distribution distr(probability);
		return distr(Generator());		
	}
	inline std::vector<int> RandomHelper::GetRandomPair(int from, int to) {
		int first = GetRandomInRange(from, to);
		int second = GetRandomInRange(from, to);
		while (first == second)
			second = GetRandomInRange(from, to);
		return std::vector<int>{std::min(first, second), std::max(first, second)};
	}
}




#endif 


