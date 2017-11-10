#pragma once

#ifndef TSP_BRANCH_AND_BOUND
#define TSP_BRANCH_AND_BOUND

#include "Solvers/BranchAndBound/LittleMatrix.h"
#include "Solvers/Solver.h"
#include <iostream>
#include <limits>
#include <deque>
#include <stack>
#include <utility>


using std::stack;
using std::deque;
using std::pair;
using std::cout;
using std::endl;

namespace TSP
{
	// Node structure
	template<class Cost>
	struct Node 
	{
		long parent_key = -1;			// parent id node (in tree)
		Cost lower_bound;               // path cost till this node
		Edge path;					    // path segment (example : 0 1)
		bool included = false;          // included or excluded path segment		
	};

	template<class Cost>
	class BranchAndBound
		: public Solver<Cost>
	{
	public:

		BranchAndBound(LittleMatrix<Cost> &m);
		BranchAndBound();
		Solution<Cost> Solve(GraphRepresentation<Cost>& representation);

		Cost FindRowMinimum(LittleMatrix<Cost> &m, int row, int ignored_col = -1);
		Cost FindColumnMinimum(LittleMatrix<Cost> &m, int col, int ignored_row = -1);
		Cost ReduceRow(LittleMatrix<Cost> &m, int row);
		Cost ReduceColumn(LittleMatrix<Cost> &m, int col);
		Cost ReduceMatrix(LittleMatrix<Cost> &m);
		Cost FindBranchingPositionAndRegret(LittleMatrix<Cost> &m, Edge &path, Position &pos);
		void RemoveSubtour(LittleMatrix<Cost> &m, int index, Edge &path);
		uint VisitedNodesCount() { return tree_m.size(); }

		void AddIndices(LittleMatrix<Cost> &m);
		vector<int> RetriveNewTourFromTree(int index, int begin);
		void UpdateCurrentBestSolution(LittleMatrix<Cost> &m);
		void AssignMatrix(LittleMatrix<Cost> &m);


		
		void FindTour();
		vector<int> GetLastTour() { return current_best_tour_m; }   
		int GetLastCost() { return current_smallest_cost_m; }          
		bool IsOptimal() { return optimal; }              
		

	private:
		Cost infinity;                                     
		LittleMatrix<Cost> initial_matrix_m;                      
		Cost current_smallest_cost_m = std::numeric_limits<Cost>::max(); 
		deque<Node<Cost>> tree_m;                         
		vector<int> current_best_tour_m;                           
		bool optimal = 0;                               
	};
}


// Return the minimum of a row in a matrix
template<class Cost> Cost BranchAndBound<Cost>::FindRowMinimum(LittleMatrix<Cost> &m, int row, int ignored_col)
{
	int nbCol = m.GetColumnCount();
	Cost min = infinity;
	for (int i = 1; i < nbCol; i++)
	{
		Cost currentValue = m.GetCost(row, i);
		if (currentValue != infinity && i != ignored_col)		
			min = (min < currentValue ? min : currentValue);		
	}
	return min;
};

// Return the minimum of a column in a matrix
template<class Cost> Cost BranchAndBound<Cost>::FindColumnMinimum(LittleMatrix<Cost> &m, int col, int ignoredRow) 
{
	int nbRow = m.GetRowCount();
	Cost min = infinity;
	for (int i = 1; i < nbRow; i++) {
		Cost currentValue = m.GetCost(i, col);
		if (currentValue != infinity && i != ignoredRow) {
			min = (min < currentValue ? min : currentValue);
		}
	}
	return min;
};

// Reduce a row of the matrix
template<class Cost> Cost BranchAndBound<Cost>::ReduceRow(LittleMatrix<Cost> &m, int row) {
	int nbCol = m.GetColumnCount();
	Cost min = FindRowMinimum(m, row);
	for (int i = 1; i < nbCol; i++) {
		if (m.GetCost(row, i) != infinity) {
			m.SetCost(row, i, m.GetCost(row, i) - min);
		}
	}
	return min;
};

// Reduce a column of the matrix
template<class Cost> Cost BranchAndBound<Cost>::ReduceColumn(LittleMatrix<Cost> &m, int col) {
	int nbRow = m.GetRowCount();
	Cost min = FindColumnMinimum(m, col);
	for (int i = 1; i < nbRow; i++) {
		if (m.GetCost(i, col) != infinity) {
			m.SetCost(i, col, m.GetCost(i, col) - min);
		}
	}
	return min;
};

/*
* Reduce the matrix and return the sum of
* the subtracted cost on each row and each columns
*/
template<class Cost> Cost BranchAndBound<Cost>::ReduceMatrix(LittleMatrix<Cost> &m) {
	int nbRow = m.GetRowCount();
	Cost minRowTotal = 0;
	for (int i = 1; i < nbRow; i++) {
		minRowTotal += ReduceRow(m, i);
	}

	int nbCol = m.GetColumnCount();
	Cost minColTotal = 0;
	for (int i = 1; i < nbCol; i++) {
		minColTotal += ReduceColumn(m, i);
	}

	return minRowTotal + minColTotal;
};

/*
* Return the path segment and the cell position
* in the matrix containing the maximal regret
*/
template<class Cost> Cost BranchAndBound<Cost>::FindBranchingPositionAndRegret(LittleMatrix<Cost> &m, Edge &path, Position &pos) {
	int size = m.GetRowCount();
	int max = -1;
	for (int i = 1; i < size; i++) {
		for (int j = 1; j < size; j++) 
		{
			// Find where there was minimum
			if (m.GetCost(i, j) == 0)
			{
				// Find the max Cost of all the minima in matrix 
				// Disable rows and column
				Cost val = FindRowMinimum(m, i, j) + FindColumnMinimum(m, j, i);
				if (max < val || max < 0) {
					max = val;
					// Remember position in current matrix
					pos.first = i;
					pos.second = j;

					/*
					The position is not necessarily in matrix is not equal to the edge

					NORMAL									DISABLED

					0 1 2 3 4	Disable i and col (2,3) ->   0 1 2 4
					0  - 2 8 9 3 								0  - 2 8 3
					1  4 - 5 6 3 <- the (1,3) pos is 6			1  4 - 5 3  <- now the (1,3) position in matrix is 5
					2  1 3 - 6 2	the edge (1,2) is 6			3  3 4 5 7	   but the edge coresponding to that pos
					3  3 4 5 - 7								4  2 9 2 -	   is not (1,3) but (1,2)
					4  2 9 2 3 -

					The easiest way of tracking edges is to extend the matrix by one i and j
					and keep the indices in there. That way, when reducing the matrix, we can still
					keep track of the edges. The 0 i and 0 j are not taken into account
					when searching for i and col minima and reducing the matrix
					*/

					// The actual index of edge i position is stored in in first i, same for j
					path.first = m.GetCost(i, 0);
					path.second = m.GetCost(0, j);
				}
			}
		}
	}
	return max;
};

using namespace TSP;

template<class Cost> void BranchAndBound<Cost>::RemoveSubtour(LittleMatrix<Cost> &m, int index, Edge &path) {
	int size = m.GetRowCount();
	Position pos;
	int founds = 0;
	vector<pair<int, int> > paths;

	// Research of all the included path
	while (index) 
	{ // Iterate until we are not arrived at the root
		if (!tree_m[index].included)
			paths.push_back(tree_m[index].path);		
		index = tree_m[index].parent_key;
	}

	// Research of the longest subtour
	deque<uint> subtour = { path.first, path.second };
	bool found = true;
	while (found)
	{
		found = false;
		for (const auto &segment : paths)
		{
			// Check that "segment" go ahead in a subtour
			if (segment.second == subtour.front())
			{
				subtour.push_front(segment.second);
				subtour.push_front(segment.first);
				found = true;
				break;
			}
			// Check that "segment" go behind in a subtour
			else if (segment.first == subtour.back()) 
			{
				subtour.push_back(segment.first);
				subtour.push_back(segment.second);
				found = true;
				break;
			}
		}
	}

	// Research of the segment to delete in the matrix
	for (int i = 1; i < size; i++)
	{
		if (m.GetCost(i, 0) == subtour.back())
		{
			pos.first = i;
			founds++;
		}
		if (m.GetCost(0, i) == subtour.front())
		{
			pos.second = i;
			founds++;
		}
	}

	// If the segment to delete has been found, then delete it by giving him an infinite cost
	if (founds == 2) 
		m.SetCost(pos.first, pos.second, infinity);	
}



// Add the city's number in the matrix
template<class Cost> void BranchAndBound<Cost>::AddIndices(LittleMatrix<Cost> &m) {
	m.AddRow(0);
	m.AddColumn(0);
	int size = m.GetRowCount();

	for (int i = 0; i < size; i++)
	{
		m.SetCost(0, i, i);
		m.SetCost(i, 0, i);
	}
}


// Travelling from 3x3 node to the root of the tree does not give ordered path
// Find all the edges in path, order them so 
template<class Cost> vector<int> BranchAndBound<Cost>::RetriveNewTourFromTree(int index, int beginning) {
	vector<Edge> path;
	vector<int> tour;

	// Retrieval of the path stored in a branch's tree
	while (index)
	{    // Iterate until we are not arrived at the root
		if (!tree_m[index].included)				// If it is a node without regret cost
			path.push_back(tree_m[index].path);   // then we add this segment to the path		
		index = tree_m[index].parent_key;
	}

	// Search for the edge that contains beginning
	int path_size = path.size();
	for (uint i = 0; i < path.size(); i++) 
	{
		if (path[i].first == beginning) 
		{
			tour.push_back(path[i].first);
			tour.push_back(path[i].second);
			path.erase(path.begin() + i);
		}
	}

	//  Order the rest of the tour
	while (tour.size() != path_size) 
	{
		for (uint i = 0; i < path.size(); i++)
		{
			if (tour.back() == path[i].first) 
			{
				tour.push_back(path[i].second);
				path.erase(path.begin() + i);
			}
		}
	}
	return tour;
}




// Finds the optimal tour using Little's algorithm
template<class Cost>
inline void TSP::BranchAndBound<Cost>::FindTour()
{
	// Subset with highest penalty, branching to the right
	// This is the node where the rows and columns will be crossed out
	Node<Cost> right_node;
	Node<Cost> left_node;
	left_node.included = true;

	Position pos;
	stack<pair<int, LittleMatrix<Cost>>> matrices;
	// Matrix associated to node
	pair<int, LittleMatrix<Cost>> matrix;   

	// Push initial matrix on stack
	matrix.first = 0;
	matrix.second = initial_matrix_m;

	matrices.push(matrix);

	while (!matrices.empty())
	{
		int id = matrices.top().first;
		LittleMatrix<Cost> m = matrices.top().second;
		matrices.pop();

		// Find the lower bound of matrix, by reducing it
		right_node.lower_bound = ReduceMatrix(m);
		if (id == 0)
			tree_m.push_back(right_node);

		// If matrix is 3x3 time to trace back the path - 2x2 costs and 1 row and column of indices
		while (m.GetRowCount() > 3 && tree_m[id].lower_bound < current_smallest_cost_m)
		{
			// Calculate new lower bound for left node
			left_node.lower_bound = tree_m[id].lower_bound + FindBranchingPositionAndRegret(m, right_node.path, pos);
			left_node.parent_key = id;
			tree_m.push_back(left_node);

			// Branching to the left
			if (left_node.lower_bound < current_smallest_cost_m)
			{
				matrix.first = tree_m.size() - 1;
				matrix.second = m;
				matrix.second.SetCost(pos.first, pos.second, infinity);
				matrices.push(matrix);
			}

			// Branching to the right

			// Remove rows, columns, and subtours
			m.RemoveRow(pos.first);
			m.RemoveColumn(pos.second);
			RemoveSubtour(m, tree_m.size() - 1, right_node.path);

			// Calculate new lower bound - parent lower bound + sum of reduction
			right_node.lower_bound = tree_m[id].lower_bound + ReduceMatrix(m);
			right_node.parent_key = id;
			tree_m.push_back(right_node);

			id = tree_m.size() - 1;
		}

		// Update the best tour and the current_smallest_cost_m value
		if (m.GetRowCount() == 3)
		{
			if (right_node.lower_bound < current_smallest_cost_m)
			{
				UpdateCurrentBestSolution(m);
				current_smallest_cost_m = right_node.lower_bound;
				current_best_tour_m = RetriveNewTourFromTree(tree_m.size() - 1, 1);
			}
		}
	}
	optimal = true;
}
template<class Cost>
inline Solution<Cost> TSP::BranchAndBound<Cost>::Solve(GraphRepresentation<Cost>& representation)
{
	auto matrix = LittleMatrix<Cost>(representation);
	AssignMatrix(matrix);
	FindTour();
	// Add the start vertex to the tour
	current_best_tour_m.push_back(1);
	return Solution<Cost>(current_smallest_cost_m, current_best_tour_m);
}

// Add 2 last edges of tour in 2x2 matrix
template<class Cost> void BranchAndBound<Cost>::UpdateCurrentBestSolution(LittleMatrix<Cost> &m)
{
	Node<Cost> node;

	for (uint i = 1; i < 3; i++)
	{
		for (uint j = 1; j < 3; j++) 
		{
			if (m.GetCost(i, j) == 0) 
			{
				node.path.first = m.GetCost(i, 0);
				node.path.second = m.GetCost(0, j);
				node.lower_bound = tree_m.back().lower_bound;
				node.parent_key = tree_m.size() - 1;
				tree_m.push_back(node);
			}
		}
	}
}

template<class Cost> BranchAndBound<Cost>::BranchAndBound(LittleMatrix<Cost> &m)
{
	AssignMatrix(m);
}

template<class Cost>
inline TSP::BranchAndBound<Cost>::BranchAndBound()
{

}

template<class Cost>
inline void TSP::BranchAndBound<Cost>::AssignMatrix(LittleMatrix<Cost>& m)
{
	infinity = m.getEmptyValue();     // Retrieval of the emptyValue, that we consider as infinity
	AddIndices(m);      // Addition of the indexes in the matrix
	initial_matrix_m = m;
}

#endif 