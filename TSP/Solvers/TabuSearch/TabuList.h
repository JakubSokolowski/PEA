#pragma once

#ifndef TSP_TABU_LIST
#define TSP_TABU_LIST

#include <vector>
#include <deque>
namespace TSP
{
	// Parameters that describe move, such as which vertices to swap
	using MoveParameters = std::vector<int>;
	// The tenure of a move, describes for how many iterations move will be forbidden
	using TabuMove = std::pair<MoveParameters, int>;
	
	class TabuList
	{
	public:
		TabuList();
		TabuList(int max_len, int tenure);
		~TabuList();
		bool IsForbidden(MoveParameters &move);
		void ForbidMove(MoveParameters &move);
		void DecreaseTenure();
		void ResetList();
		uint GetListLenght() { return tabu_list_m.size(); }
	protected:		
		int max_length_m, tabu_tenure_m;
		// The list of all the forbidden moves. Using std::deque for O(1) front deletion
		std::deque<TabuMove> tabu_list_m;
	};

	inline TSP::TabuList::TabuList()
		: max_length_m(50)
		, tabu_tenure_m(15)
		, tabu_list_m(50)
	{}

	inline TSP::TabuList::TabuList(int max_len, int tenure)
		: max_length_m(max_len)
		, tabu_tenure_m(tenure)
		, tabu_list_m(max_len)
	{}

	inline TSP::TabuList::~TabuList()
	{}

	// Checks wether the move parameters match any moves from tabu list
	inline bool TSP::TabuList::IsForbidden(MoveParameters & move)
	{
		for (auto &tabu_move : tabu_list_m)
			if (move == tabu_move.first)
				return true;
		return false;
	}

	// Adds the move to the tabu list, and removes the oldest (first in list)
	// move if the length of the list exceeds max_length_m;
	inline void TSP::TabuList::ForbidMove(MoveParameters & params)
	{
		tabu_list_m.push_back(TabuMove{ params, tabu_tenure_m });
		if (tabu_list_m.size() > max_length_m)
			tabu_list_m.pop_front();
	}

	// Decreses tenur of all the moves in the list by 1.
	// If tenure of a move reaches 0, the move is ereased from the list
	inline void TSP::TabuList::DecreaseTenure()
	{
		for (auto it = tabu_list_m.begin(); it != tabu_list_m.end();)
		{
			it->second--;
			if (it->second == 0)
				it = tabu_list_m.erase(it);
			else
				++it;
		}
	}

	// Deletes all the moves from the list
	inline void TabuList::ResetList()
	{
		tabu_list_m.clear();
	}

	class SwapTabuList
	{
	public:
		SwapTabuList();
		SwapTabuList(int tenure);
		~SwapTabuList();
		bool IsForbidden(MoveParameters &move);
		void ForbidMove(MoveParameters &move);
		void DecreaseTenure();
		void ResetList();
		void CreateList(int vertices_num);

	private:
		int tenure_m;
		std::vector<std::vector<int>> tabu_list_m;

	};

	inline TSP::SwapTabuList::SwapTabuList()
		: tenure_m(15)
	{}
	inline TSP::SwapTabuList::SwapTabuList(int tenure)
		: tenure_m(tenure)
	{}
	inline SwapTabuList::~SwapTabuList()
	{
	}
	inline bool SwapTabuList::IsForbidden(MoveParameters & move)
	{
		return tabu_list_m[move[0]][move[1]] > 0;
	}
	inline void SwapTabuList::ForbidMove(MoveParameters & move)
	{
		tabu_list_m[move[0]][move[1]] += tenure_m;
	}
	inline void SwapTabuList::DecreaseTenure()
	{
		for (auto &row : tabu_list_m)
			for (auto &tenure : row)
				tenure > 0 ? --tenure : tenure;
	}
	inline void SwapTabuList::ResetList()
	{
		for(auto &row : tabu_list_m)
			for (auto &tenure : row)
				tenure = 0;
			
	}
	inline void SwapTabuList::CreateList(int vertices_num)
	{
		tabu_list_m = std::vector<std::vector<int>>(vertices_num, std::vector<int>(vertices_num,0));
	}
}
#endif
