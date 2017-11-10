#pragma once

#ifndef LITTLE_MATRIX_H
#define	LITTLE_MATRIX_H

#include <iostream>
#include <vector>
#include <limits>
#include "GraphRepresentation.h"

namespace TSP
{
	using std::vector;
	
	using Edge = std::pair<uint, uint>; // The edge in graph - for example (2,4) is the edge between vertex 2  and 4
	using Position = std::pair<uint, uint>;
	template<class Cost> class LittleMatrix;

	template<class Cost> class LittleMatrix 
	{
	public:
		LittleMatrix(uint nbRows = 0, uint nbColumns = 0, Cost emptyValue = 0);
		LittleMatrix(vector<vector<Cost>> &matrix);
		LittleMatrix(GraphRepresentation<Cost> &graph);
		LittleMatrix(const LittleMatrix<Cost> &other);
		void AssingIndices();

		Cost getEmptyValue() { return infinity_m; }

		Cost FindRowMinimum(uint row);
		Cost FindRowMinimum(uint row, uint disabled_column);
		Cost FindColumnMinimum(uint column);
		Cost FindColumnMinimum(uint column, uint disabled_row);

		Cost ReduceRow(uint row);
		Cost ReduceColumn(uint column);
		Cost ReduceAllRows();
		Cost ReduceAllColumns();
		Cost ReduceMatrix();

		Cost FindHighestPenaltyEdge(Edge &edge, Position &position);

		void AddRow(uint row_index);
		void RemoveRow(uint row_index);
	
		void AddColumn(uint column_index);
		void RemoveColumn(uint column_index);

		uint GetRowCount() { return row_count_m; }
		uint GetColumnCount() { return column_count_m; }

		Cost GetCost(uint row_index, uint column_index);
		void SetCost(uint row_index, uint column_index, Cost value);

		Cost operator() (uint row, uint column);

		~LittleMatrix();

	private:

		uint row_count_m;
		uint column_count_m;
		Cost infinity_m ;
		vector<vector<Cost>> data_m;
	};

}

using namespace TSP;


template <class Cost>
inline  TSP::LittleMatrix<Cost>::LittleMatrix(uint row_count, uint column_count, Cost empty_value)
	: data_m(vector<vector<Cost> >(row_count, std::vector<Cost>(column_count, empty_value)))
	, row_count_m(row_count)
	, column_count_m(column_count)
	, infinity_m(empty_value)
{}

template<class Cost>
inline  TSP::LittleMatrix<Cost>::LittleMatrix(vector<vector<Cost>>& matrix)
	: data_m(matrix)
	, row_count_m(matrix.size())
	, column_count_m(matrix.size())
	, infinity_m(matrix[0][0])
{}

template<class Cost>
inline TSP::LittleMatrix<Cost>::LittleMatrix(GraphRepresentation<Cost>& graph)
{
	row_count_m = column_count_m = graph.GetNumOfVertices();	
	infinity_m = graph.GetWeight(0, 0);
	data_m = vector<vector<Cost> >(row_count_m, std::vector<Cost>(column_count_m, infinity_m));
	for (uint i = 0; i < row_count_m; i++)
		for (uint j = 0; j < row_count_m; j++)
			SetCost(i, j, graph.GetWeight(i, j));
}

template<class Cost>
inline TSP::LittleMatrix<Cost>::LittleMatrix(const LittleMatrix<Cost>& other)
	: row_count_m(other.row_count_m)
	, column_count_m(other. column_count_m)
	, data_m(other.data_m)
	, infinity_m(other.infinity_m)
{}


template<class Cost>
inline Cost TSP::LittleMatrix<Cost>::FindRowMinimum(uint row)
{	
	Cost min = infinity_m;
	for (uint column = 1; column < column_count_m; column++)
	{
		Cost current_cost = GetCost(row, column);
		if (current_cost != infinity_m && current_cost < min)
			min = current_cost;	
	}
	return min;
}
template<class Cost>
inline Cost TSP::LittleMatrix<Cost>::FindRowMinimum(uint row, uint disabled_column)
{
	Cost min = infinity_m;
	for (uint column = 1; column < column_count_m; column++)
	{
		Cost current_cost = GetCost(row, column);
		if (current_cost != infinity_m && current_cost < min && column != disabled_column)
			min = current_cost;
	}
	return min;
}
template<class Cost>
inline Cost TSP::LittleMatrix<Cost>::FindColumnMinimum(uint column)
{
	Cost min = infinity_m;
	for (uint row = 1; row < row_count_m; row++)
	{
		Cost current_cost = GetCost(row, column);
		if (current_cost != infinity_m && current_cost < min)
			min = current_cost;
	}
	return min;
}
template<class Cost>
inline Cost TSP::LittleMatrix<Cost>::FindColumnMinimum(uint column, uint disabled_row)
{
	Cost min = infinity_m;
	for (uint row = 1; row < row_count_m; row++)
	{
		Cost current_cost = GetCost(row, column);
		if (current_cost != infinity_m && current_cost < min && row != disabled_row)
			min = current_cost;
	}
	return min;
}


// Substracts the min of given i from every lower_bound in the i. Returns the subtracted min
template<class Cost>
inline Cost TSP::LittleMatrix<Cost>::ReduceRow(uint row)
{	
	Cost min = FindRowMinimum(row);
	for (uint column = 1; column < column_count_m; column++) 
	{
		Cost lower_bound = GetCost(row, column);
		if (lower_bound != infinity_m) 
			SetCost(row, column, lower_bound - min);		
	}
	return min;
}
// Substracts the min of given j from every lower_bound in the j. Returns the subtracted min
template<class Cost>
inline Cost TSP::LittleMatrix<Cost>::ReduceColumn(uint column)
{
	Cost min = FindColumnMinimum(column);
	for (uint row = 1; row < row_count_m; row++)
	{
		Cost lower_bound = GetCost(row, column);
		if (lower_bound != infinity_m)
			SetCost(row, column, lower_bound - min);
	}
	return min;
}
template<class Cost>
inline Cost TSP::LittleMatrix<Cost>::ReduceAllRows()
{
	Cost sum_of_row_minima{};
	for (uint row = 1; row < row_count_m; row++)
		sum_of_row_minima += ReduceRow(row);
	return sum_of_row_minima;
}
template<class Cost>
inline Cost TSP::LittleMatrix<Cost>::ReduceAllColumns()
{
	Cost sum_of_column_minima{};
	for (uint column = 1; column < column_count_m; column++)
		sum_of_column_minima += ReduceColumn(column);
	return sum_of_column_minima;
}

// Reduced the matrix subtracting all the i and j minima from 
// their coresponding rows and columns, then returns 
// the sum of this minima - the lower bound
template<class Cost>
inline Cost TSP::LittleMatrix<Cost>::ReduceMatrix()
{
	return ReduceAllRows() + ReduceAllColumns();
}

// Finds the highest penalty edge in matrix, and returns its penalty
template<class Cost>
inline Cost TSP::LittleMatrix<Cost>::FindHighestPenaltyEdge(Edge &edge, Position &position)
{
	uint size = row_count_m;
	Cost max = -1;

	for (uint i = 1; i < size; i++) 
	{
		for (uint j = 1; j < size; j++)
		{
			
			if (GetCost(i, j) == 0)
			{
						
				Cost val = FindRowMinimum(i, j) + FindColumnMinimum(j, i);
				if (max < val || max < 0)
				{
					max = val;
		
					position.first = i;
					position.second = j;
					
					edge.first = GetCost(i, 0);
					edge.second = GetCost(j, 0);
				}
			}
		}
	}
	return max;
}


template <class Cost>  TSP::LittleMatrix<Cost>::~LittleMatrix() 
{}



template<class Cost>
inline void TSP::LittleMatrix<Cost>::AssingIndices()
{
	AddRow(0);
	AddColumn(0);

	for (uint i = 0; i < row_count_m; i++) 
	{
		SetCost(0, i, i);
		SetCost(i, 0, i);
	}
}

template<class Cost>
inline void TSP::LittleMatrix<Cost>::AddRow(uint rowIndex)
{
	auto it = data_m.begin() + rowIndex;
	data_m.insert(it, vector<Cost>(column_count_m, infinity_m));
	row_count_m++;	
}

template <class Cost> void  TSP::LittleMatrix<Cost>::RemoveRow(uint rowIndex)
{
	
	auto it = data_m.begin() + rowIndex;
	data_m.erase(it);
	row_count_m--;
}

template <class Cost> void  TSP::LittleMatrix<Cost>::AddColumn(uint col_index)
{
	
	for (uint row = 0; row < row_count_m; row++)
	{
		auto it = data_m[row].begin() + col_index;
		data_m[row].insert(it, infinity_m);
	}
	column_count_m++;
}

template <class Cost> void  TSP::LittleMatrix<Cost>::RemoveColumn(uint col_index)
{	
	for (uint row = 0; row < row_count_m; row++)
	{
		auto it = data_m[row].begin() + col_index;
		data_m[row].erase(it);
	}
	column_count_m--;
}

template <class Cost> Cost  TSP::LittleMatrix<Cost>::GetCost(uint rowIndex, uint col_index)
{
	return data_m[rowIndex][col_index];
}

template <class Cost> 
void LittleMatrix<Cost>::SetCost(uint row_index, uint col_index, Cost value)
{
	data_m[row_index][col_index] = value;
}

template<class Cost>
inline Cost TSP::LittleMatrix<Cost>::operator()(uint row, uint column)
{
	return GetCost(row,column);
}

#endif	