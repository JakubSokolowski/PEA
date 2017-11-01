#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\TSP\SymmetricAdjacencyMatrix.h"
#include "..\TSP\BBMatrix.h"
#include <limits>

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

		TEST_METHOD(BBMatrixGetRowMin)
		{
			auto matrix = BBMatrix<int>(matrix_);
			Assert::AreEqual(7, matrix.FindRowMinimum(0));
			Assert::AreEqual(8, matrix.FindRowMinimum(0, 4));		
		}	
		TEST_METHOD(BBMatrixReduceRow)
		{
			auto matrix = BBMatrix<int>(matrix_);

			std::vector<std::vector<int>> reduced_matrix =
			{
				{ INF, 3,  1,   2,   0 },
				{ 10,  INF, 10,  5,   6 },
				{ 8,   10,  INF, 8,   9 },
				{ 9,   5,   8,   INF, 6 },
				{ 7,   6,   9,   6,  INF }
			};
			matrix.ReduceRow(0);

			for(int i = 0 ; i < 5; i++)
				for (int j = 0; i < 5; i++)
					Assert::AreEqual(reduced_matrix[i][j], matrix.matrix_[i][j]);
			
		}
		TEST_METHOD(BBMatrixReduceAllRows)
		{
			auto matrix = BBMatrix<int>(matrix_);

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
					Assert::AreEqual(reduced_matrix[i][j], matrix.matrix_[i][j]);
			Assert::AreEqual(31, result);
		}

		TEST_METHOD(BBMatrixGetColumnMin)
		{
			auto matrix = BBMatrix<int>(matrix_);
			Assert::AreEqual(matrix.FindColumnMinimum(1), 5);
			Assert::AreEqual(matrix.FindColumnMinimum(1, 3), 6);
		}
		TEST_METHOD(BBMatrixReduceColumn)
		{
			auto matrix = BBMatrix<int>(matrix_);

			std::vector<std::vector<int>> reduced_matrix =
			{
				{ INF, 10,  8,   9,   7 },
				{ 3,  INF, 10,  5,   6 },
				{ 1,   10,  INF, 8,   9 },
				{ 2,   5,   8,   INF, 6 },
				{ 0,   6,   9,   6,  INF }
			};
			matrix.ReduceColumn(0);

			for (int i = 0; i < 5; i++)
				for (int j = 0; i < 5; i++)
					Assert::AreEqual(reduced_matrix[i][j], matrix.matrix_[i][j]);

		}
		TEST_METHOD(BBMatrixReduceAllColumns)
		{
			auto matrix = BBMatrix<int>(matrix_);

			std::vector<std::vector<int>> reduced_matrix =
			{
				{ INF, 5,  0,   4,   1 },
				{ 3,  INF, 2,   0,   0 },
				{ 1,   5,  INF, 3,   2 },
				{ 2,   0,   0,  INF, 0 },
				{ 0,   1,   1,   1,  INF }
			};

			matrix.ReduceAllColumns();			
			for (int i = 0; i < 5; i++)
				for (int j = 0; i < 5; i++)
					Assert::AreEqual(reduced_matrix[i][j], matrix.matrix_[i][j]);
		}

		TEST_METHOD(BBMatrixReduceMatrix)
		{
			auto matrix = BBMatrix<int>(matrix_);

			std::vector<std::vector<int>> reduced_matrix =
			{
				{ INF,   3,  0,  2,   0 },
				{ 5,   INF,  4,  0,   1 },
				{ 0,   2,  INF, 0,   1 },
				{ 4,   0,   2,   INF, 1 },
				{ 1,   0,   2,   1,  INF }
			};

			auto result = matrix.ReduceMatrix();
			for (int i = 0; i < 5; i++)
				for (int j = 0; i < 5; i++)
					Assert::AreEqual(reduced_matrix[i][j], matrix.matrix_[i][j]);
			Assert::AreEqual(32, result);
		}
	};
}