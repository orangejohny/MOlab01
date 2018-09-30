#ifndef __SIMPLEX_METHOD_MATRIX_HPP
#define __SIMPLEX_METHOD_MATRIX_HPP

#include <fstream>
#include <iterator>
#include <matrix.hpp>
#include <string>

template <typename T>
class splx_matrix : public Matrix<T> {
 public:
  using Matrix<T>::rows;
  using Matrix<T>::columns;
  using Matrix<T>::po_arr;
  using Matrix<T>::column_begin;
  using Matrix<T>::column_end;
  using Matrix<T>::row_begin;
  using Matrix<T>::row_end;
  using Matrix<T>::begin;
  using Matrix<T>::end;
  using Matrix<T>::create_matrix;
  using Matrix<T>::delete_matrix;

  splx_matrix() : Matrix<T>(), num_of_basis{0}, num_of_free{0} {};
  splx_matrix(size_t rows_, size_t columns_) : Matrix<T>(rows_, columns_){};
  splx_matrix(splx_matrix<T>& other) : Matrix<T>(other){};
  splx_matrix(T** po_arr_, size_t rows_, size_t columns_)
      : Matrix<T>(po_arr_, rows_, columns_){};
  template <typename InputIt>
  splx_matrix(InputIt begin, InputIt end, size_t rows_, size_t columns_)
      : Matrix<T>(begin, end, rows_, columns_){};

  auto load_data(std::string file_A, std::string file_B, std::string file_C)
      -> void;
  auto simplex_method() -> int;
  auto show_answer() -> void;

 private:
  std::vector<std::string> basis;
  std::vector<std::string> free;
  bool is_to_min;
  size_t num_of_free;
  size_t num_of_basis;

  auto form_basis_solution() -> int;
  auto optimization() -> int;
  auto out() -> void;
  auto find_pivot_elem(size_t pivot_column) -> cIt<T>;
  auto jordanic_exception(cIt<T> pivot_elem) -> void;
  auto load_A(std::string file_A) -> Matrix<T>;
  auto load_B(std::string file_B) -> std::vector<T>;
  auto load_C(std::string file_C) -> std::vector<T>;
};

template <typename T>
auto splx_matrix<T>::simplex_method() -> int {
  if (form_basis_solution() == 1) {
    return 1;  // no solution
  }

  if (optimization() == 1) {
    return 2;  // function is unlimited
  }

  return 0;  // solution is founded
}

template <typename T>
auto splx_matrix<T>::optimization() -> int {
  while (true) {
    // trying to find positive value in F() row
    rIt<T> positive_ptr =
        std::find_if(std::next(row_begin(rows - 1)), row_end(rows - 1),
                     [](T elem) { return elem > 0; });

    if (positive_ptr == row_end(rows - 1)) {
      return 0;  // solution is optimal
    }

    size_t pivot_column = positive_ptr.get_index().second;

    if (std::find_if(column_begin(pivot_column), column_end(pivot_column),
                     [](T elem) { return elem > 0; }) ==
        column_end(pivot_column)) {
      return 1;  // function is unlimited, no optimal solution
    }

    auto pivot_elem = find_pivot_elem(pivot_column);
    jordanic_exception(pivot_elem);

    out();
    std::cout << std::endl;
  }
}

template <typename T>
auto splx_matrix<T>::form_basis_solution() -> int {
  while (true) {
    // iterator that points on negative element in column with free elements
    cIt<T> negative_it_free =
        std::find_if(column_begin(0), std::prev(column_end(0)),
                     [](T elem) { return elem < 0; });

    if (negative_it_free == std::prev(column_end(0))) {
      return 0;  // basis solution founded
    }

    size_t negative_row = negative_it_free.get_index().first;

    rIt<T> pivot_column =
        std::find_if(std::next(row_begin(negative_row)), row_end(negative_row),
                     [](T elem) { return elem < 0; });

    if (pivot_column == row_end(negative_row)) {
      return 1;  // no solution
    }

    auto pivot_elem = find_pivot_elem(pivot_column.get_index().second);
    jordanic_exception(pivot_elem);

    out();
    std::cout << std::endl;
  }
}

template <typename T>
auto splx_matrix<T>::find_pivot_elem(size_t pivot_column) -> cIt<T> {
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

  return pivot_elem;
}

template <typename T>
auto splx_matrix<T>::jordanic_exception(cIt<T> pivot_elem) -> void {
  auto [r, k] = pivot_elem.get_index();
  Matrix<T> tmp(*this);

  po_arr[r][k] = 1.0 / tmp[r][k];  // 1st formula

  for (size_t j = 0; j < columns; ++j) {  // 2nd formula
    if (j != k) {
      po_arr[r][j] = tmp[r][j] / tmp[r][k];
    }
  }

  for (size_t i = 0; i < rows; ++i) {  // 3rd formula
    if (i != r) {
      po_arr[i][k] = -tmp[i][k] / tmp[r][k];
    }
  }

  for (size_t i = 0; i < rows; ++i) {  // 4th formula
    for (size_t j = 0; j < columns; ++j) {
      if (i != r && j != k) {
        po_arr[i][j] = tmp[i][j] - tmp[i][k] * tmp[r][j] / tmp[r][k];
      }
    }
  }

  std::swap(basis[r], free[k]);
}

template <typename T>
auto splx_matrix<T>::out() -> void {
  std::cout << "  ";
  for (auto i : free) {
    std::cout.width(10);
    std::cout << i;
  }
  std::cout << std::endl;

  for (size_t i = 0; i < rows; ++i) {
    std::cout << basis[i];
    for (size_t j = 0; j < columns; ++j) {
      std::cout.precision(4);
      std::cout.width(10);
      std::cout << po_arr[i][j];
    }
    std::cout << std::endl;
  }
}

template <typename T>
auto splx_matrix<T>::load_data(std::string file_A, std::string file_B,
                               std::string file_C) -> void {
  std::vector<T> vec_c;
  std::vector<T> vec_b;
  Matrix<T> matrix_A;
  try {
    vec_c = load_C(file_C);
    vec_b = load_B(file_B);
    matrix_A = load_A(file_A);
  } catch (std::ios::failure& e) {
    throw e;
  }

  delete_matrix();  // avoid memory leak
  create_matrix(num_of_basis + 1,
                num_of_free + 1);  // make space for simplex table

  std::copy(vec_b.begin(), vec_b.end(), column_begin(0));
  std::copy(vec_c.begin(), vec_c.end(), row_begin(rows - 1));

  for (auto i = 0; i < matrix_A.get_rows(); ++i) {
    for (auto j = 0; j < matrix_A.get_columns(); ++j) {
      po_arr[i][j + 1] = matrix_A[i][j];
    }
  }

  free.push_back("Si0");
  for (size_t i = 1; i <= num_of_free; ++i) {
    free.push_back("x" + std::to_string(i));
  }

  for (size_t i = num_of_free + 1; i <= num_of_free + num_of_basis; ++i) {
    basis.push_back("x" + std::to_string(i));
  }
  basis.push_back(" F");
}

// file with C should contain the world "max" or "min" int the end
template <typename T>
auto splx_matrix<T>::load_C(std::string file_C) -> std::vector<T> {
  std::ifstream fin_C(file_C);
  if (!fin_C.good()) {
    throw std::ios::failure("Can't read from file C!");
  }

  std::vector<T> vec_c{0};
  while (true) {
    T val;
    fin_C >> val;
    if (fin_C.good()) {
      vec_c.push_back(val);
    } else {
      break;
    }
  }

  fin_C.clear();

  std::string s;
  if (fin_C >> s; s == "min") {
    is_to_min = true;
    std::transform(vec_c.begin(), vec_c.end(), vec_c.begin(),
                   [](T v) { return v * (-1); });
  } else {
    is_to_min = false;
  }

  num_of_free = vec_c.size() - 1;
  fin_C.close();
  return std::move(vec_c);
}

// less then <= b
template <typename T>
auto splx_matrix<T>::load_B(std::string file_B) -> std::vector<T> {
  std::ifstream fin_B(file_B);
  if (!fin_B.good()) {
    throw std::ios::failure("Can't read from file B!");
  }

  std::vector<T> vec_b;
  std::copy(std::istream_iterator<T>(fin_B), std::istream_iterator<T>{},
            std::back_inserter(vec_b));

  num_of_basis = vec_b.size();
  fin_B.close();
  return std::move(vec_b);
}

template <typename T>
auto splx_matrix<T>::load_A(std::string file_A) -> Matrix<T> {
  std::ifstream fin_A(file_A);
  if (!fin_A.good()) {
    throw std::ios::failure("Can't read from file A!");
  }

  Matrix<T> matrix(std::istream_iterator<T>(fin_A), std::istream_iterator<T>{},
                   this->num_of_basis, this->num_of_free);
  // if not "less then" we should modify particular rows

  fin_A.close();
  return std::move(matrix);
}

template <typename T>
auto splx_matrix<T>::show_answer() -> void {
  std::cout << "Answer is:\nFree variables ";
  for (auto i = std::next(free.begin()); i != free.end(); ++i) {
    std::cout << *i << "=";
  }
  std::cout << "0.\nBasis variables and F:\n";

  for (size_t i = 0; i < rows; ++i) {
    std::cout << basis[i];
    std::cout.precision(4);
    std::cout.width(10);
    if (i == rows - 1 && !is_to_min) {
      std::cout << po_arr[i][0] * -1;
    } else {
      std::cout << po_arr[i][0];
    }
    std::cout << std::endl;
  }
}

#endif
