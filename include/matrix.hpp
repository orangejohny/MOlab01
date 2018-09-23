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
 private:
  T** po_arr;
  size_t rows;
  size_t columns;

  void create_matrix(size_t rows_, size_t columns_);

 public:
  Matrix() : po_arr{nullptr}, rows{0}, columns{0} {};
  Matrix(size_t rows_, size_t columns_);
  Matrix(Matrix<T>& other);
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

  auto simplex_method() -> void;
  auto optimization(std::vector<char>& basis, std::vector<char>& free) -> bool;
  auto out(std::vector<char>& basis, std::vector<char>& free) -> void;
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
Matrix<T>::Matrix(Matrix<T>& other) {
  this->create_matrix(other.rows, other.columns);
  std::copy(other.begin(), other.end(), this->begin());
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

template <typename T>
auto Matrix<T>::simplex_method() -> void {}

template <typename T>
auto Matrix<T>::optimization(std::vector<char>& basis, std::vector<char>& free)
    -> bool {
  while (true) {
    // trying to find positive value in F() row
    rIt<T> positive_ptr =
        std::find_if(std::next(row_begin(rows - 1)), row_end(rows - 1),
                     [](T elem) { return elem > 0; });

    if (positive_ptr == row_end(rows - 1)) {
      return true;  // solution is optimal
    }

    size_t pivot_column = positive_ptr.get_index().second;

    if (std::find_if(column_begin(pivot_column), column_end(pivot_column),
                     [](T elem) { return elem > 0; }) ==
        column_end(pivot_column)) {
      return false;  // function is unlimited, no optimal solution
    }

    T min_ratio = std::numeric_limits<T>::max();
    cIt<T> pivot_elem;
    for (auto i = column_begin(0), j = column_begin(pivot_column);
         i != column_end(0); ++i, ++j) {
      auto ratio = (*i) / (*j);
      if (ratio > 0 && ratio < min_ratio) {
        min_ratio = ratio;
        pivot_elem = j;
      }
    }

    auto [r, k] = pivot_elem.get_index();
    Matrix<T> tmp(*this);

    po_arr[r][k] = 1.0 / tmp.po_arr[r][k];  // 1st formula

    for (size_t j = 0; j < columns; ++j) {  // 2nd formula
      if (j != k) {
        po_arr[r][j] = tmp.po_arr[r][j] / tmp.po_arr[r][k];
      }
    }

    for (size_t i = 0; i < rows; ++i) {  // 3rd formula
      if (i != r) {
        po_arr[i][k] = -tmp.po_arr[i][k] / tmp.po_arr[r][k];
      }
    }

    for (size_t i = 0; i < rows; ++i) {  // 4th formula
      for (size_t j = 0; j < columns; ++j) {
        if (i != r && j != k) {
          po_arr[i][j] = tmp.po_arr[i][j] -
                         tmp.po_arr[i][k] * tmp.po_arr[r][j] / tmp.po_arr[r][k];
        }
      }
    }

    std::swap(basis[r], free[k]);
    out(basis, free);
    std::cout << std::endl;
  }
}

template <typename T>
auto Matrix<T>::out(std::vector<char>& basis, std::vector<char>& free) -> void {
  for (auto i : free) {
    std::cout.width(9);
    std::cout << "x" << i;
  }
  std::cout << std::endl;

  for (size_t i = 0; i < rows; ++i) {
    std::cout << "x" << basis[i];
    for (size_t j = 0; j < columns; ++j) {
      std::cout.precision(4);
      std::cout.width(10);
      std::cout << po_arr[i][j];
    }
    std::cout << std::endl;
  }
}

#endif
