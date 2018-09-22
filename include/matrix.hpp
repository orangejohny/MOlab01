#include <algorithm>
#include <iterator.hpp>

#ifndef __MATRIX_HPP__
#define __MATRIX_HPP__

template <typename T>
class Matrix {
 private:
  T** po_arr;
  size_t rows;
  size_t columns;

  T** create_matrix();

 public:
  Matrix() : po_arr{nullptr}, rows{0}, columns{0} {};
  Matrix(size_t rows_, size_t columns_);
  Matrix(Matrix<T> const& other);
  Matrix(T** po_arr_, size_t rows_, size_t columns_);
  template <typename InputIt>
  Matrix(InputIt begin, InputIt end, size_t rows_, size_t columns_);
  ~Matrix();

  auto operator=(Matrix<T> const& other) -> void;

  auto swap(Matrix<T>& other) -> void;

  auto begin() -> rIt<T> { return rIt<T>(po_arr, 0, 0, rows, columns); }
  auto end() -> rIt<T> { return rIt<T>(po_arr, rows, 0, rows, columns); }
  auto rbegin() -> rIt<T> {
    return rIt<T>(po_arr, rows - 1, columns - 1, rows, columns);
  }
  auto rend() -> rIt<T> {
    return rIt<T>(po_arr, -1, columns - 1, rows, columns);
  }
};

template <typename T>
template <typename InputIt>
Matrix<T>::Matrix(InputIt begin, InputIt end, size_t rows_, size_t columns_) {
  rows = rows_;
  columns = columns_;
  po_arr = create_matrix();
  std::copy(begin, end, this->begin());
}

template <typename T>
T** Matrix<T>::create_matrix() {
  T** ptr = new T*[rows];
  for (size_t i = 0; i < rows; i++) {
    ptr[i] = new T[columns];
  }
  return ptr;
}

template <typename T>
Matrix<T>::Matrix(T** po_arr_, size_t rows_, size_t columns_) {
  rows = rows_;
  columns = columns_;
  po_arr = create_matrix();
  for (size_t i = 0; i < rows; i++) {
    for (size_t j = 0; j < columns; j++) {
      po_arr[i][j] = po_arr_[i][j];
    }
  }
}

template <typename T>
Matrix<T>::Matrix(size_t rows_, size_t columns_) {
  rows = rows_;
  columns = columns_;
  po_arr = create_matrix();
}

template <typename T>
Matrix<T>::Matrix(Matrix<T> const& other) {
  rows = other.rows;
  columns = other.columns;
  po_arr = create_matrix();
  std::copy(other.begin(), other.end(), begin());
}

template <typename T>
auto Matrix<T>::operator=(Matrix<T> const& other) -> void {
  Matrix<T>{other}.swap(*this);
}

template <typename T>
auto Matrix<T>::swap(Matrix<T>& other) -> void {
  std::swap(po_arr, other.po_arr);
  std::swap(rows, other.rows);
  std::swap(columns, other.columns);
}

template <typename T>
Matrix<T>::~Matrix() {
  for (size_t i = 0; i < rows; i++) {
    delete[] po_arr[i];
  }
  delete[] po_arr;
}

#endif