#pragma once

#ifndef TSP_BBMATRIX
#define TSP_BBMATRIX


#include "SymmetricAdjacencyMatrix.h"
#include <limits>
namespace TSP
{
	using std::vector;


	// Wrapper Class for AdjacencyMatrix graph representation for use
	template <typename Cost>
	class BBMatrix
	{
	public:
		BBMatrix();
		BBMatrix(SymmetricAdjacencyMatrix<Cost> &matrix);
		BBMatrix(std::vector<std::vector<Cost>> &matrix);
		~BBMatrix();

		Cost FindRowMinimum(uint row_index, int disabled_column_index = -1);
		Cost FindColumnMinimum(uint column_index, int disabled_row_index = -1);
		Cost ReduceRow(uint row_index);
		Cost ReduceColumn(uint column_index);
		Cost ReduceAllRows();
		Cost ReduceAllColumns();
		Cost ReduceMatrix();
		void DisableRow(uint row_index);
		void DisableColumn(uint column_index);

		
		vector<vector<Cost>> matrix_;
	private:
		
		Cost infinity_ = std::numeric_limits<Cost>::max();		

	};
	template<typename Cost>
	inline BBMatrix<Cost>::BBMatrix()
	{
	}
	template<typename Cost>
	inline BBMatrix<Cost>::BBMatrix(SymmetricAdjacencyMatrix<Cost>& matrix)
	{
		matrix_ = matrix.GetMatrix();
	}
	template<typename Cost>
	inline BBMatrix<Cost>::BBMatrix(std::vector<std::vector<Cost>>& matrix)
	{
		matrix_ = matrix;
	}
	template<typename Cost>
	inline BBMatrix<Cost>::~BBMatrix()
	{
	}

	template<typename Cost>
	inline Cost BBMatrix<Cost>::FindRowMinimum(uint row_index, int disabled_column_index)
	{
		Cost min = infinity_;
		// When checking the minimum value of row, we need to skip the disabled column
		for (uint i = 0; i < matrix_.size(); i++)
		{
			if (i != disabled_column_index)
				if (matrix_[row_index][i] < min)
					min = matrix_[row_index][i];
		}
		return min;
	}
	template<typename Cost>
	inline Cost BBMatrix<Cost>::FindColumnMinimum(uint column_index, int disabled_row_index)
	{
		Cost min = infinity_;	
		// When checking the minimum value of column, we need to skip the disabled row
		for (uint i = 0; i < matrix_.size(); i++)
		{
			if (i != disabled_row_index)
				if (matrix_[i][column_index] < min)
					min = matrix_[i][column_index];
		}
		return min;
	}

	// Substracts row minimum from all the values in given row.
	// Returns the subtracted minimum Cost
	template<typename Cost>
	inline Cost BBMatrix<Cost>::ReduceRow(uint row_index)
	{
		Cost minimum = FindRowMinimum(row_index);
		if (minimum == Cost())
			return minimum;
		for (uint i = 0; i < matrix_.size(); i++)					
			if (matrix_[row_index][i] != infinity_)	
				matrix_[row_index][i] = matrix_[row_index][i] - minimum;
		return minimum;
	}

	// Substracts row minimum from all the values in given column.
	// Returns the subtracted minimum Cost
	template<typename Cost>
	inline Cost BBMatrix<Cost>::ReduceColumn(uint column_index)
	{
		Cost minimum = FindColumnMinimum(column_index);

		// If the minimum is equal to value of zero in Cost, return
		if (minimum == Cost())
			return minimum;
		for (uint i = 0; i < matrix_.size(); i++)
			if (matrix_[i][column_index] != infinity_)
				matrix_[i][column_index] = matrix_[i][column_index] - minimum;
		return minimum;
	}

	template<typename Cost>
	inline Cost BBMatrix<Cost>::ReduceAllRows()
	{
		Cost total = Cost();
		for (uint i = 0; i < matrix_.size(); i++)
			total += ReduceRow(i);
		return total;
	}

	template<typename Cost>
	inline Cost BBMatrix<Cost>::ReduceAllColumns()
	{
		Cost total = Cost();
		for (uint i = 0; i < matrix_.size(); i++)
			total += ReduceColumn(i);
		return total;
	}

	// Reduced the matrix by reducing all the rows and columns
	// Returns the sum of subtracted minima
	template<typename Cost>
	inline Cost BBMatrix<Cost>::ReduceMatrix()
	{
		return ReduceAllRows() + ReduceAllColumns();
	}
	template<typename Cost>
	inline void BBMatrix<Cost>::DisableRow(uint row_index)
	{
	}
	template<typename Cost>
	inline void BBMatrix<Cost>::DisableColumn(uint column_index)
	{
	}
}
#endif // !TSP_BBMATRIX
