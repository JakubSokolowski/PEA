#pragma once

#ifndef MATRIX_H
#define	MATRIX_H

#include "IndexOutOfBoundsException.h"
#include "NegativeDimensionException.h"
#include "GraphRepresentation.h"
#include <iostream>
#include <vector>
#include <limits>

using std::vector;
using std::ostream;

template<typename T>
class Matrix {

private:
	int row;
	int col;
	T emptyVal;
	vector<vector<T> > data;

public:
	Matrix(int nbRows = 0, int nbColumns = 0, T emptyValue = 0) throw(NegativeDimensionException);

	T getEmptyValue() { return emptyVal; }

	int getNbRows() { return row; }
	void addRow(int rowIndex) throw(IndexOutOfBoundsException);
	void removeRow(int rowIndex) throw(IndexOutOfBoundsException);

	int getNbColumns() { return col; }
	void addColumn(int colIndex) throw(IndexOutOfBoundsException);
	void removeColumn(int colIndex) throw(IndexOutOfBoundsException);

	T getValue(int rowIndex, int colIndex) throw(IndexOutOfBoundsException);
	void setValue(int rowIndex, int colIndex, T value) throw(IndexOutOfBoundsException);

	~Matrix();
};


template<typename T>
Matrix<T>::Matrix(int nbRows, int nbColumns, T emptyValue) throw(NegativeDimensionException) {
	if (nbColumns < 0 || nbRows < 0) {
		throw NegativeDimensionException();
	}
	data = vector<vector<T> >(nbRows, std::vector<T>(nbColumns, emptyValue));
	row = nbRows;
	col = nbColumns;
	emptyVal = emptyValue;
}


template<typename T>
Matrix<T>::~Matrix() {

}

template<typename T>
void Matrix<T>::addRow(int rowIndex) throw(IndexOutOfBoundsException) {
	if (rowIndex < 0 or rowIndex > row) {
		throw IndexOutOfBoundsException(rowIndex, 0, row);
	}
	auto it = data.begin() + rowIndex;
	data.insert(it, vector<T>(col, emptyVal));
	row++;
}

template<typename T>
void Matrix<T>::removeRow(int rowIndex) throw(IndexOutOfBoundsException) {
	if (rowIndex < 0 or rowIndex >= row) {
		throw IndexOutOfBoundsException(rowIndex, 0, row - 1);
	}
	typename vector<vector<T> >::iterator it = data.begin() + rowIndex;
	data.erase(it);
	row--;
}

template<typename T>
void Matrix<T>::addColumn(int colIndex) throw(IndexOutOfBoundsException) {
	if (colIndex < 0 or colIndex > col) {
		throw IndexOutOfBoundsException(colIndex, 0, col);
	}
	for (int row = 0; row < row; row++) {
		typename vector<T>::iterator it = data[row].begin() + colIndex;
		data[row].insert(it, emptyVal);
	}
	col++;
}
template<typename T>
void Matrix<T>::removeColumn(int colIndex) throw(IndexOutOfBoundsException) {
	if (colIndex < 0 or colIndex >= col) {
		throw IndexOutOfBoundsException(colIndex, 0, col - 1);
	}
	for (int row = 0; row < row; row++) {
		typename vector<T>::iterator it = data[row].begin() + colIndex;
		data[row].erase(it);
	}
	col--;
}

template<typename T>
T Matrix<T>::getValue(int rowIndex, int colIndex) throw(IndexOutOfBoundsException) {
	if (rowIndex < 0 || rowIndex >= row) {
		throw IndexOutOfBoundsException(rowIndex, 0, row - 1);
	}
	if (colIndex < 0 || colIndex >= col) {
		throw IndexOutOfBoundsException(colIndex, 0, col - 1);;
	}
	return data[rowIndex][colIndex];
}

template<typename T>
void Matrix<T>::setValue(int rowIndex, int colIndex, T value) throw(IndexOutOfBoundsException) {
	if (rowIndex < 0 || rowIndex >= row) {
		throw IndexOutOfBoundsException(rowIndex, 0, row - 1);
	}
	if (colIndex < 0 || colIndex >= col) {
		throw IndexOutOfBoundsException(colIndex, 0, col - 1);
	}
	data[rowIndex][colIndex] = value;
}

#endif	/* MATRIX_H */