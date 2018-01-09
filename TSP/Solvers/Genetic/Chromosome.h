#pragma once

#ifndef TSP_CHROMOSOME
#define TSP_CHROMOSOME

#include "../Solution.h"

namespace TSP
{
	template<typename Cost>
	class Chromosome : Solution<Cost>
	{
	public:
		Chromosome();
		Chromosome(Cost tour_cost);
		Chromosome(Cost tour_cost, std::vector<int> path);
		Chromosome(const Solution<Cost>& solution);
		Chromosome(const Chromosome<Cost> &other);
		Chromosome<Cost> operator = (const Chromosome<Cost> &other);
		~Chromosome();
	
		double GetFitness() const;
		Solution<Cost> GetSolution() const;

		Cost total_cost = std::numeric_limits<Cost>::max();
		std::vector<int> tour;		
	};
	
	template<typename Cost>
	struct ChromosomeCompare
	{
		bool operator()(const Chromosome<Cost>& lhs, const Chromosome<Cost> &rhs)
		{ 
			return lhs.GetFitness() > rhs.GetFitness();
		}
	};

	template<typename Cost>
	inline Chromosome<Cost>::Chromosome()
	{}
	template<typename Cost>
	inline Chromosome<Cost>::Chromosome(Cost tour_cost)
		: total_cost(tour_cost)
	{}
	template<typename Cost>
	inline Chromosome<Cost>::Chromosome(Cost tour_cost, std::vector<int> path)
		: total_cost(tour_cost)
		, tour(path)
	{}
	template<typename Cost>
	inline Chromosome<Cost>::Chromosome(const Solution<Cost>& solution)
		: total_cost(solution.total_cost)
		, tour(solution.tour)
	{}
	template<typename Cost>
	inline Chromosome<Cost>::Chromosome(const Chromosome<Cost>& other)
		: total_cost(other.total_cost)
		, tour(other.tour)
	{}
	template<typename Cost>
	inline Chromosome<Cost> Chromosome<Cost>::operator=(const Chromosome<Cost>& other)
	{
		total_cost = other.total_cost;
		tour = other.tour;
		return *this;
	}	
	template<typename Cost>
	inline Chromosome<Cost>::~Chromosome()
	{}

	template<typename Cost>
	inline double Chromosome<Cost>::GetFitness() const
	{
		return 1 / double(total_cost);
	}
	template<typename Cost>
	inline Solution<Cost> Chromosome<Cost>::GetSolution() const
	{
		return Solution<Cost>(total_cost, tour);
	}

}


#endif // !TSP_CHROMOSOME
