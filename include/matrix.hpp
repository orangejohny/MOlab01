#ifndef __MATRIX_HPP__
#define __MATRIX_HPP__

#include <algorithm>
#include <cIt.hpp>
#include <iostream>
#include <iterator>
#include <limits>
#include <rIt.hpp>
#include <vector>

template <typename T>
class Matrix {
 protected:
  T** po_arr;
  size_t rows;
  size_t columns;

  void create_matrix(size_t rows_, size_t columns_);
  void delete_matrix();

 public:
  typedef rIt<T> const_iterator;
  Matrix() : po_arr{nullptr}, rows{0}, columns{0} {};
  Matrix(size_t rows_, size_t columns_);
  Matrix(const Matrix<T>& other);
  Matrix(Matrix<T>&& other);
  Matrix(T** po_arr_, size_t rows_, size_t columns_);
  template <typename InputIt>
  Matrix(InputIt begin, InputIt end, size_t rows_, size_t columns_);
  ~Matrix();

  auto operator=(Matrix<T> const& other) -> void;
  auto operator[](size_t index) -> T*& { return po_arr[index]; }

  auto swap(Matrix<T>& other) -> void;

  auto begin() -> rIt<T> { return rIt<T>(po_arr, 0, 0, rows, columns); }
  const_iterator cbegin() const {
    return const_iterator(po_arr, 0, 0, rows, columns);
  }

  auto end() -> rIt<T> { return rIt<T>(po_arr, rows, 0, rows, columns); }
  const_iterator cend() const {
    return const_iterator(po_arr, rows, 0, rows, columns);
  }

  auto rbegin() -> rIt<T> {
    return rIt<T>(po_arr, rows - 1, columns - 1, rows, columns);
  }

  auto rend() -> rIt<T> {
    return rIt<T>(po_arr, -1, columns - 1, rows, columns);
  }

  auto row_begin(size_t row) -> rIt<T> {
    return rIt<T>(po_arr, row, 0, rows, columns);
  }

  auto row_end(size_t row) -> rIt<T> {
    return rIt<T>(po_arr, row + 1, 0, rows, columns);
  }

  auto column_begin(size_t column) -> cIt<T> {
    return cIt<T>(po_arr, 0, column, rows, columns);
  }

  auto column_end(size_t column) -> cIt<T> {
    return cIt<T>(po_arr, 0, column + 1, rows, columns);
  }

  auto get_rows() -> size_t { return rows; }
  auto get_columns() -> size_t { return columns; }
};

template <typename T>
template <typename InputIt>
Matrix<T>::Matrix(InputIt begin, InputIt end, size_t rows_, size_t columns_) {
  create_matrix(rows_, columns_);
  std::copy(begin, end, this->begin());
}

template <typename T>
void Matrix<T>::create_matrix(size_t rows_, size_t columns_) {
  rows = rows_;
  columns = columns_;
  po_arr = new T*[rows];
  for (size_t i = 0; i < rows; i++) {
    po_arr[i] = new T[columns];
  }
}

template <typename T>
Matrix<T>::Matrix(T** po_arr_, size_t rows_, size_t columns_) {
  create_matrix(rows_, columns_);
  for (size_t i = 0; i < rows; i++) {
    for (size_t j = 0; j < columns; j++) {
      po_arr[i][j] = po_arr_[i][j];
    }
  }
}

template <typename T>
Matrix<T>::Matrix(size_t rows_, size_t columns_) {
  create_matrix(rows_, columns_);
}

template <typename T>
Matrix<T>::Matrix(const Matrix<T>& other) {
  this->create_matrix(other.rows, other.columns);
  std::copy(other.cbegin(), other.cend(), begin());
}

template <typename T>
Matrix<T>::Matrix(Matrix<T>&& other) {
  po_arr = other.po_arr;
  columns = other.columns;
  rows = other.rows;

  other.po_arr = nullptr;
}

template <typename T>
auto Matrix<T>::operator=(Matrix<T> const& other) -> void {
  Matrix<T>(other).swap(*this);
}

template <typename T>
auto Matrix<T>::swap(Matrix<T>& other) -> void {
  std::swap(po_arr, other.po_arr);
  std::swap(rows, other.rows);
  std::swap(columns, other.columns);
}

template <typename T>
Matrix<T>::~Matrix() {
  delete_matrix();
}

template <typename T>
void Matrix<T>::delete_matrix() {
  if (po_arr == nullptr) return;
  for (size_t i = 0; i < rows; i++) {
    delete[] po_arr[i];
  }
  delete[] po_arr;
}

#endif
