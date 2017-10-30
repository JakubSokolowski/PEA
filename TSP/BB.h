#pragma once
#include "Heuristic.h"

namespace TSP
{
	template<typename Cost>
	class BB :
		public Heuristic<Cost>
	{
	public:
		BB();
		virtual ~BB();
	};
	template<typename Cost>
	inline BB<Cost>::BB()
	{
	}
	template<typename Cost>
	inline BB<Cost>::~BB()
	{
	}
}

