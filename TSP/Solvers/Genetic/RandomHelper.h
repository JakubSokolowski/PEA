#pragma once

#ifndef TSP_RANDOM_HELPER
#define TSP_RANDOM_HELPER

#include <random>
#include <iterator>

namespace TSP
{	

	class RandomHelper
	{
	public:
		static int GetRandomInRange(int from, int to);
		static bool EventOccured(double probability);
		

		template <typename I>
		static I RandomElement(I begin, I end);
	private:
		static std::mt19937& Generator() { static std::mt19937 gen; return gen; }
		static std::random_device rnd_m;
		static std::mt19937 generator_m;
		RandomHelper() {}
	};

	template<typename I>
	static inline I RandomHelper::RandomElement(I begin, I end)
	{
		std::uniform_int_distribution<> distr(0, std::distance(begin, end) - 1);
		std::advance(begin, distr(Generator()));
		return begin;
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
}




#endif 


