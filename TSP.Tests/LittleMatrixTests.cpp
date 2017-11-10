#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\TSP\SymmetricAdjacencyMatrix.h"
#include <limits>
#include "..\TSP\BranchAndBound.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace TSP;


namespace TSPTests
{
	TEST_CLASS(BranchAndBoundMatrixTests)
	{
	public:

		std::vector<std::vector<int>> matrix_ =
		{
			{ INF, 10,  8,   9,   7 },
			{ 10,  INF, 10,  5,   6 },
			{ 8,   10,  INF, 8,   9 },
			{ 9,   5,   8,   INF, 6 },
			{ 7,   6,   9,   6,  INF }
		};

		// Matrix Reduction

		TEST_METHOD(LittleMatrixGetRowMin)
		{
			auto matrix = LittleMatrix<int>(matrix_);
			auto matrix2 = LittleMatrix<int>(matrix_);
			matrix.AssingIndices();

			auto expected_normal = 7;
			auto expected_disabled = 8;

			auto matrix_normal_result = matrix.FindRowMinimum(1);
			auto matrix_disabled_result = matrix.FindRowMinimum(1,5);

			auto little = BranchAndBound<int>(matrix2);

			auto alg_normal_result = little.FindRowMinimum(matrix, 1);
			auto alg_disabled_result = little.FindRowMinimum(matrix, 1, 5);

			Assert::AreEqual(expected_normal, matrix_normal_result);
			Assert::AreEqual(expected_disabled, matrix_disabled_result);

			Assert::AreEqual(expected_normal, alg_normal_result);
			Assert::AreEqual(expected_disabled, alg_disabled_result);	

		}	
		TEST_METHOD(LittleMatrixReduceRow)
		{
			auto matrix = LittleMatrix<int>(matrix_);
			auto matrix2 = LittleMatrix<int>(matrix_);
			auto little = BranchAndBound<int>(matrix2);

			std::vector<std::vector<int>> reduced_matrix =
			{
				{ INF, 3,  1,   2,   0 },
				{ 10,  INF, 10,  5,   6 },
				{ 8,   10,  INF, 8,   9 },
				{ 9,   5,   8,   INF, 6 },
				{ 7,   6,   9,   6,  INF }
			};

			matrix.ReduceRow(0);
			little.ReduceRow(matrix2, 1);

			for(int i = 0 ; i < 5; i++)
				for (int j = 0; i < 5; i++)
				{
					Assert::AreEqual(reduced_matrix[i][j], matrix(i,j));
					Assert::AreEqual(reduced_matrix[i][j], matrix2(i + 1 , j + 1));
				}	
		}
		TEST_METHOD(LittleMatrixReduceAllRows)
		{
			auto matrix = LittleMatrix<int>(matrix_);
			matrix.AssingIndices();

			std::vector<std::vector<int>> reduced_matrix =
			{
				{ INF,   3,  1,  2,   0 },
				{ 5,   INF,  5,  0,   1 },
				{ 0,   2,  INF,  0,   1 },
				{ 4,   0,   3,   INF, 1 },
				{ 1,   0,   3,   1,  INF }
			};

			auto result = matrix.ReduceAllRows();		
			for (int i = 0; i < 5; i++)
				for (int j = 0; i < 5; i++)
					Assert::AreEqual(reduced_matrix[i][j], matrix(i + 1, j + 1));
			Assert::AreEqual(31, result);
		}

		TEST_METHOD(LittleMatrixGetColumnMin)
		{			
			auto matrix = LittleMatrix<int>(matrix_);
			auto matrix2 = LittleMatrix<int>(matrix_);
			matrix.AssingIndices();

			auto expected_normal = 5;
			auto expected_disabled = 6;

			auto matrix_normal_result = matrix.FindColumnMinimum(2);
			auto matrix_disabled_result = matrix.FindColumnMinimum(2, 4);

			auto little = BranchAndBound<int>(matrix2);

			auto alg_normal_result = little.FindColumnMinimum(matrix, 2);
			auto alg_disabled_result = little.FindColumnMinimum(matrix, 2, 4);

			Assert::AreEqual(expected_normal, matrix_normal_result);
			Assert::AreEqual(expected_disabled, matrix_disabled_result);

			Assert::AreEqual(expected_normal, alg_normal_result);
			Assert::AreEqual(expected_disabled, alg_disabled_result);
		}
		TEST_METHOD(LittleMatrixReduceColumn)
		{
			auto matrix = LittleMatrix<int>(matrix_);
			auto matrix2 = LittleMatrix<int>(matrix_);
			auto little = BranchAndBound<int>(matrix2);

			std::vector<std::vector<int>> reduced_matrix =
			{
				{ INF, 10,  8,   9,   7 },
				{ 3,  INF, 10,  5,   6 },
				{ 1,   10,  INF, 8,   9 },
				{ 2,   5,   8,   INF, 6 },
				{ 0,   6,   9,   6,  INF }
			};
			matrix.AssingIndices();
			matrix.ReduceColumn(1);
			little.ReduceColumn(matrix2, 1);

			for (int i = 0; i < 5; i++)
				for (int j = 0; i < 5; i++)
				{
					Assert::AreEqual(reduced_matrix[i][j], matrix(i + 1, j + 1));
					Assert::AreEqual(reduced_matrix[i][j], matrix2(i + 1, j + 1));
				}				

		}
		TEST_METHOD(LittleMatrixReduceAllColumns)
		{			
			auto matrix = LittleMatrix<int>(matrix_);
			matrix.AssingIndices();

			std::vector<std::vector<int>> reduced_matrix =
			{
			{ INF, 5,  0,   4,   1 },
			{ 3,  INF, 2,   0,   0 },
			{ 1,   5,  INF, 3,   2 },
			{ 2,   0,   0,  INF, 0 },
			{ 0,   1,   1,   1,  INF }
			};

			auto result = matrix.ReduceAllColumns();
			for (int i = 0; i < 5; i++)
				for (int j = 0; i < 5; i++)
					Assert::AreEqual(reduced_matrix[i][j], matrix(i +1 , j + 1));
			Assert::AreEqual(31, result);				
		}

		TEST_METHOD(LittleMatrixReduceMatrix)
		{
			auto matrix = LittleMatrix<int>(matrix_);
			matrix.AssingIndices();
			auto matrix2 = LittleMatrix<int>(matrix_);
			auto little = BranchAndBound<int>(matrix2);

			std::vector<std::vector<int>> reduced_matrix =
			{
				{ INF,   3,  0,  2,   0 },
				{ 5,   INF,  4,  0,   1 },
				{ 0,   2,  INF, 0,   1 },
				{ 4,   0,   2,   INF, 1 },
				{ 1,   0,   2,   1,  INF }
			};

			auto result = matrix.ReduceMatrix();
			auto result_alg = little.ReduceMatrix(matrix2);

			for (uint i = 0; i <  reduced_matrix.size(); i++)
				for (uint j = 0; i <  reduced_matrix.size(); i++)
				{
					Assert::AreEqual(reduced_matrix[i][j], matrix(i + 1, j + 1));
					Assert::AreEqual(reduced_matrix[i][j], matrix2(i + 1, j + 1));
				}					
			Assert::AreEqual(32, result);
			Assert::AreEqual(32, result_alg);
		}

		// Finding highest penalty edge

		TEST_METHOD(LittleMatrixFindHighestPenaltyEdge)
		{
			auto matrix = LittleMatrix<int>(matrix_);
			matrix.AssingIndices();
			auto matrix2 = LittleMatrix<int>(matrix_);
			auto little = BranchAndBound<int>(matrix2);			

			// Preparation : need to reduce the matrix first

			matrix.ReduceMatrix();
			little.ReduceMatrix(matrix2);		

			auto edge1 = Edge{};
			auto edge2 = Edge{};
			auto pos1 = Position();
			auto pos2 = Position();

			auto result = matrix.FindHighestPenaltyEdge(edge1,pos1);		

			

			auto result_alg = little.FindBranchingPositionAndRegret(matrix2, edge2, pos2);
			
			Assert::AreEqual(2, result_alg);
			Assert::AreEqual(2, result);

			Assert::AreEqual(edge1.first, edge2.first);
			Assert::AreEqual(edge1.second, edge2.second);	
			Assert::AreEqual(pos1.first, pos2.first);
			Assert::AreEqual(pos1.second, pos2.second);
			
		}
	};
}