#ifndef __IT_BASE_HPP__
#define __IT_BASE_HPP__

#include <iterator>
#include <utility>

// base class for matrix's iterators
template <typename T>
class It_base : public std::iterator<std::bidirectional_iterator_tag, T> {
 protected:
  T** ptr;         // beginning of matrix
  size_t row;      // current row
  size_t column;   // current column
  size_t rows;     // height
  size_t columns;  // width

 public:
  It_base() : ptr{nullptr}, row{0}, column{0}, rows{0}, columns{0} {};
  It_base(T** ptr_, size_t row_, size_t column_, size_t rows_, size_t columns_)
      : ptr{ptr_},
        row{row_},
        column{column_},
        rows{rows_},
        columns{columns_} {};
  It_base(const It_base<T>& other)
      : ptr{other.ptr},
        row{other.row},
        column{other.column},
        rows{other.rows},
        columns{other.columns} {};
  virtual ~It_base() = 0;

  It_base<T>& operator=(const It_base<T>& other) {
    ptr = other.ptr;
    row = other.row;
    column = other.column;
    rows = other.rows;
    columns = other.columns;
    return (*this);
  }

  template <typename T_>
  friend bool operator==(const It_base<T_>& lhs, const It_base<T_>& rhs) {
    return (lhs.ptr == rhs.ptr && lhs.rows == rhs.rows &&
            lhs.columns == rhs.columns && lhs.row == rhs.row &&
            lhs.column == rhs.column);
  }

  template <typename T_>
  friend bool operator!=(const It_base<T_>& lhs, const It_base<T_>& rhs) {
    return !(lhs == rhs);
  }

  T& operator*() {
    if (row >= rows || column >= columns)
      throw std::out_of_range{"Bad range operator*() !"};
    return ptr[row][column];
  }

  const T& operator*() const {
    if (row >= rows || column >= columns)
      throw std::out_of_range{"Bad range operator*() !"};
    return ptr[row][column];
  }

  T* operator->() const {
    if (row >= rows || column >= columns)
      throw std::out_of_range{"Bad range operator->() !"};
    return &(ptr[row][column]);
  }

  template <typename T_>
  friend void swap(It_base<T_>& lhs, It_base<T_>& rhs) {
    std::swap(lhs.ptr, rhs.ptr);
    std::swap(lhs.row, rhs.row);
    std::swap(lhs.rows, rhs.rows);
    std::swap(lhs.column, rhs.column);
    std::swap(lhs.columns, rhs.columns);
  }

  auto get_index() -> std::pair<size_t, size_t> {
    return std::make_pair(row, column);
  }
};

template <typename T>
It_base<T>::~It_base() {
  ptr = nullptr;
}

#endif
