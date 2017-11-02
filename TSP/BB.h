#pragma once
#include "Heuristic.h"

#include <vector>
#include <deque>
#include <stack>
#include "BBMatrix.h"


using std::vector;
using std::deque;
using std::cout;
using std::endl;

namespace TSP
{
	using Edge = std::pair<uint, uint>;

	template<typename Cost>
	struct Node
	{
		Cost cost_;
		Edge edge_;
		bool included_ = false;
		long parent_ = -1;
	};


	template<typename Cost>
	class BB :
		public Heuristic<Cost>
	{
	public:
		BB();
		BB(GraphRepresentation<Cost> &rep);
		Solution<Cost> Solve(GraphRepresentation<Cost>& representation);
		virtual ~BB();

	private:
		Cost reference_ = std::numeric_limits<Cost>::max();
		Cost infinity_ = std::numeric_limits<Cost>::max();
		deque<Node<Cost>> tree_;
		vector<uint> current_tour_;
		BBMatrix<Cost>  matrix_;

		bool is_optimal_ = false;
		void RemoveSubTour(BBMatrix<Cost> &matrix, uint index, Path &path);	
		vector<uint> OrderPath(int index, int begin);
		void AddLastPath(BBMatrix<Cost> &m);
		void CheckTourCost();
		void FindTour();
	};
	template<typename Cost>
	inline BB<Cost>::BB()
	{
	}
	template<typename Cost>
	inline Solution<Cost> BB<Cost>::Solve(GraphRepresentation<Cost>& representation)
	{
		matrix_ = BBMatrix<Cost>(representation);
		FindTour();
		return Solution<Cost>(reference_);		
	}
	template<typename Cost>
	inline BB<Cost>::~BB()
	{
	}
	template<typename Cost>
	inline void BB<Cost>::RemoveSubTour(BBMatrix<Cost>& matrix, uint index, Path & path)
	{
		CellPosition position;
		int founds = 0;

		vector<Path> paths;

		// Search all the included paths
		// iterate till root
		while (index !=0 ) 
		{
			if (tree_[index].included_ == false)			
				paths.push_back(tree_[index].edge_);
			index = tree_[index].parent_;
		}

		// Compute upper bound (longest subtour)
		deque<uint> subtour = { path.first, path.second };
		bool found = true;
		int h = 0;
		while (found) 
		{
			std::cout << h <<std::endl;
			h++;
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
		h = 0;


		// Research of the segment to delete in the matrix
		for (uint i = 0; i < matrix.row_count_; i++) 
		{
			if (matrix(i, 0) == subtour.back())
			{
				position.first = i;
				founds++;
			}
			if (matrix(0, i) == subtour.front())
			{
				position.second = i;
				founds++;
			}
		}

		// If the segment to delete has been found, then delete it by giving him an infinite cost
		if (founds == 2)
			matrix_.SetCost(position.first, position.second, infinity_);
		
	}
	template<typename Cost>
	inline vector<uint> BB<Cost>::OrderPath(int index, int begin)
	{
		vector<Path> path;
		vector<uint> tour;

		// Retrieval of the path stored in a branch's tree
		while (index != 0) {    // Iterate until we are not arrived at the root
			if (!tree_[index].included_) {     // If it is a node without regret cost
				path.push_back(tree_[index].edge_);   // then we add this segment to the path
			}
			index = tree_[index].parent_;
		}

		// Research of the path segment containing begin
		int pathSize = path.size();
		for (int i = 0; i < pathSize; i++) {
			if (path[i].first == begin) {
				tour.push_back(path[i].first);
				tour.push_back(path[i].second);
				path.erase(path.begin() + i);
			}
		}

		// Ordering of the rest of the tour
		while (tour.size() != pathSize) {
			for (int i = 0; i < path.size(); i++) {
				if (tour.back() == path[i].first) {
					tour.push_back(path[i].second);
					path.erase(path.begin() + i);
				}
			}
		}

		return tour;
	}
	
	template<typename Cost>
	inline void BB<Cost>::AddLastPath(BBMatrix<Cost>& m)
	{
		Node<Cost> normalNode;

		for (int i = 1; i < 3; i++) {
			for (int j = 1; j < 3; j++) {
				if (m(i, j) == 0) {
					normalNode.edge_.first = m(i, 0);
					normalNode.edge_.second = m(0, j);
					normalNode.cost_ = tree_.back().cost_;
					normalNode.parent_ = tree_.size() - 1;
					tree_.push_back(normalNode);
				}
			}
		}
	}
	template<typename Cost>
	inline void BB<Cost>::CheckTourCost()
	{
		int cost_ = 0;
		int size = current_tour_.size();
		for (int i = 0; i < size - 1; i++) {
			cost_ += matrix_(current_tour_[i], current_tour_[i + 1]);
		}
		cost_ += matrix_(current_tour_.back(), current_tour_.front());
		cout << "Cost check " << cost_ << " ";
	}
	template<typename Cost>
	inline void BB<Cost>::FindTour()
	{
		Node<Cost> normalNode;     // node with regret
		Node<Cost> regretNode;     // node without regret
		regretNode.included_ = true;
		CellPosition pos;     // var to store the position of a cell in the matrix
		std::stack<pair<uint, BBMatrix<Cost>>> matrices;    // stack containing the necessary matrix to pursue other branch of the tree
		pair<uint, BBMatrix<Cost>> matrix;  // matrix associated to a node

										// Init of the stack with the initial distances matrix
		matrix.first = 0;
		matrix.second = matrix_;
		matrices.push(matrix);

		while (!matrices.empty()) {     // Iterate till the stack is empty
			uint id = matrices.top().first;
			BBMatrix<Cost> m = matrices.top().second;
			matrices.pop();

			// Reduction of the matrix and computation of the minimum sum (raw + col)
			normalNode.cost_ = matrix_.ReduceMatrix();
			if (id == 0)      // root tree case
				tree_.push_back(normalNode);
			

			/* Until it ends up with a 2x2 matrix (3x3 du to the indexes storage)
			* and until the current node is lower than the reference value */
			while (m.row_count_ > 2 && tree_[id].cost_ < reference_)
			{

#ifdef DEBUG
				if ((tree_.size() - 1) % 1000 == 0) {
					cout << "\r" << tree_.size() - 1 << " nodes ..." << std::flush;
				}
#endif

				// Compute the node with regret
				regretNode.cost_ = tree_[id].cost_ + m.FindHighestPenaltyEdge(normalNode.edge_, pos);
				regretNode.parent_ = id;
				tree_.push_back(regretNode);

				// Storing of the matrix
				if (regretNode.cost_ < reference_) {
					matrix.first = tree_.size() - 1;
					matrix.second = m;
					matrix.second.SetCost(pos.first, pos.second, infinity_);      // Suppression case i, j pour une potentielle recherche ulterieur
					matrices.push(matrix);
				}

				// Deletion raw col
				m.RemoveRow(pos.first);
				m.RemoveColumn(pos.second);

				// Subtour deletion
				RemoveSubTour(m, tree_.size() - 1, normalNode.edge_);

				// Compute the node without regret
				normalNode.cost_ = tree_[id].cost_ + m.ReduceMatrix();
				normalNode.parent_ = id;
				tree_.push_back(normalNode);

				id = tree_.size() - 1;
			}

			// Update of the best tour and the reference value
			if (m.Size() == 2) 
			{
				if (normalNode.cost_ < reference_)
				{
					AddLastPath(m);
					reference_ = normalNode.cost_;
					current_tour_ = OrderPath(tree_.size() - 1, 1);

#ifdef DEBUG
					cout << "\r";
					CheckTourCost();
					cout << "Cost " << reference_;
					cout << " Tour ";
					for (int i = 0; i < current_tour_.size(); i++) {
						cout << current_tour_[i] << " ";
					}
					cout << "Node " << tree_.size() - 1;
					cout << endl;
#endif
				}
			}
		}

		is_optimal_ = true;   // Computing finished, the tour is thus optimal

#ifdef DEBUG
		cout << endl << tree_.size() << " nodes visited" << endl;
#endif
	}
}

