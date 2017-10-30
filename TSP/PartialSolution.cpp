#include "stdafx.h"
#include "PartialSolution.h"

using namespace TSP;

template<typename Cost>
TSP::PartialSolution<Cost>::PartialSolution()
{
}

template<typename Cost>
TSP::PartialSolution<Cost>::PartialSolution(int n, std::vector<uint>& reduced)
{
}

template<typename Cost>
bool TSP::PartialSolution<Cost>::operator>(const PartialSolution & other) const
{
	return false;
}

template<typename Cost>
TSP::PartialSolution<Cost>::~PartialSolution()
{
}
