#pragma once

#ifndef TSP_BBMATRIX
#define TSP_BBMATRIX


#include "SymmetricAdjacencyMatrix.h"
#include <limits>
namespace TSP
{
	using std::vector;
	using std::pair;
	using Path = pair<uint, uint>;
	using CellPosition = pair<uint, uint>;

	// Wrapper Class for AdjacencyMatrix graph representation for use
	template <typename Cost>
	class BBMatrix
	{
	public:
		BBMatrix();
		BBMatrix(SymmetricAdjacencyMatrix<Cost> &matrix);
		BBMatrix(GraphRepresentation<Cost> &matrix);
		BBMatrix(std::vector<std::vector<Cost>> &matrix);
		BBMatrix(const BBMatrix<Cost> &other);
		~BBMatrix();

		Cost FindRowMinimum(uint row_index, int disabled_column_index = -1);
		Cost FindColumnMinimum(uint column_index, int disabled_row_index = -1);
		Cost ReduceRow(uint row_index);
		Cost ReduceColumn(uint column_index);
		Cost ReduceAllRows();
		Cost ReduceAllColumns();
		Cost ReduceMatrix();

		void SetCost(uint source, uint destination, Cost cost_);
		Cost FindHighestPenaltyEdge(Path &path, CellPosition &position);
		void RemoveRow(uint row_index);
		void RemoveColumn(uint column_index);

		void DisableRow(uint row_index);
		void DisableColumn(uint column_index);

		uint Size();
		Cost operator()(uint source, uint destination);
		BBMatrix<Cost> operator = (BBMatrix<Cost> &other);

		uint row_count_ = 0;
		uint column_count_ = 0;
		
		vector<vector<Cost>> matrix_;
	private:
		
		const Cost infinity_ = std::numeric_limits<Cost>::max();		
		const Cost zero_ = Cost{};
		

	};
	template<typename Cost>
	inline BBMatrix<Cost>::BBMatrix()
	{
	}
	template<typename Cost>
	inline BBMatrix<Cost>::BBMatrix(SymmetricAdjacencyMatrix<Cost>& matrix)
	{
		matrix_ = matrix.GetMatrix();
		row_count_ = column_count_ = matrix_.size();
	}
	template<typename Cost>
	inline BBMatrix<Cost>::BBMatrix(GraphRepresentation<Cost>& matrix)
	{
		std::vector<std::vector<Cost>> new_matrix(matrix.GetNumOfVertices(), std::vector<Cost>(matrix.GetNumOfVertices(), std::numeric_limits<Cost>::max()));
		matrix_ = new_matrix;

		for (uint i = 0; i < matrix_.size(); i++)
			for (uint j = 0; j < matrix_.size(); j++)
				matrix_[i][j] = matrix.GetWeight(i, j);
		row_count_ = column_count_ = matrix_.size();
	}
	template<typename Cost>
	inline BBMatrix<Cost>::BBMatrix(std::vector<std::vector<Cost>>& matrix)
	{
		matrix_ = matrix;
		row_count_ = column_count_ = matrix_.size();
	}
	template<typename Cost>
	inline BBMatrix<Cost>::BBMatrix(const BBMatrix<Cost>& other)
	{
		matrix_ = other.matrix_;
		row_count_ = column_count_ = matrix_.size();
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
	inline void BBMatrix<Cost>::SetCost(uint source, uint destination, Cost cost_)
	{
		matrix_[source][destination] = cost_;
	}

	// Finds the highest penalty edge, returns the penalty, cell position and path segment
	template<typename Cost>
	inline Cost BBMatrix<Cost>::FindHighestPenaltyEdge(Path & path, CellPosition & position)
	{
		// TODO : might change to it std::numeric_limits<Cost>::min() if the value not being negative 
		// somehow breaks algorithm
		Cost max{};

		// iterations from 1 ?
		for (int i = 0; i < matrix_.size(); i++)
		{
			for (int j = 0; j < matrix_.size(); j++)
			{
				// Find all the cells with 0 in it 
				// (Cells when the min of row or column used to be
				if (matrix_[i][j] == zero_)
				{
					// Find the min of current row with current column ignored
					// and Find the min of current column with current row ignored
					Cost val = FindRowMinimum(i, j) + FindColumnMinimum(i, j);
					if (max < val)
					{
						max = val;
						// Remember which row and column to disable
						position.first = i;
						position.first = j;

						// i think the cities ids used to be stored in the 0th row and columnd, idk
						path.first = matrix_[i][0];
						path.second = matrix_[0][j];
					}
				}
			}
		}
		return max;
	}
	template<typename Cost>
	inline void BBMatrix<Cost>::RemoveRow(uint row_index)
	{
		auto it = matrix_.begin() + row_index;
		matrix_.erase(it);
		row_count_--;
	}
	template<typename Cost>
	inline void BBMatrix<Cost>::RemoveColumn(uint column_index)
	{
		for (int row = 0; row < matrix_.size(); row++) 
		{
			auto it = matrix_[row].begin() + column_index;
			matrix_[row].erase(it);
		}
		column_count_--;
	}
	template<typename Cost>
	inline void BBMatrix<Cost>::DisableRow(uint row_index)
	{
	}
	template<typename Cost>
	inline void BBMatrix<Cost>::DisableColumn(uint column_index)
	{
	}
	template<typename Cost>
	inline uint BBMatrix<Cost>::Size()
	{
		return matrix_.size();
	}
	template<typename Cost>
	inline Cost BBMatrix<Cost>::operator()(uint source, uint destination)
	{
		return matrix_[source][destination];
	}

	template<typename Cost>
	inline BBMatrix<Cost> BBMatrix<Cost>::operator=(BBMatrix<Cost>& other)
	{
		matrix_ = other.matrix_;
		row_count_ = column_count_ = matrix_.size();
		return *this;
	}
	
}
#endif // !TSP_BBMATRIX
