#include <iterator>

#ifndef __ITERATOR_HPP__
#define __ITERATOR_HPP__

template <typename T>
class rIt : public std::iterator<std::bidirectional_iterator_tag, T> {
 private:
  T** ptr;               // beginning of matrix
  size_t row;            // current row
  size_t column;         // current column
  const size_t rows;     // height
  const size_t columns;  // width

 public:
  rIt() : ptr{nullptr}, row{0}, column{0}, rows{0}, columns{0} {};
  rIt(T** ptr_, size_t row_, size_t column_, size_t rows_, size_t columns_)
      : ptr{ptr_},
        row{row_},
        column{column_},
        rows{rows_},
        columns{columns_} {};
  rIt(const rIt<T>& other)
      : ptr{other.ptr},
        row{other.row},
        column{other.column},
        rows{other.rows},
        columns{other.columns} {};

  rIt<T>& operator=(const rIt<T>& other) {
    ptr = other.ptr;
    row = other.row;
    column = other.column;
    rows = other.rows;
    columns = other.columns;
  }

  template <typename T_>
  friend bool operator==(const rIt<T_>& lhs, const rIt<T_>& rhs) {
    return (lhs.ptr == rhs.ptr && lhs.rows == rhs.rows &&
            lhs.columns == rhs.columns && lhs.row == rhs.row &&
            lhs.column == rhs.column);
  }

  template <typename T_>
  friend bool operator!=(const rIt<T_>& lhs, const rIt<T_>& rhs) {
    return !(lhs == rhs);
  }

  T& operator*() const {
    if (row >= rows || column >= columns)
      throw std::out_of_range{"Bad range operator*() !"};
    return ptr[row][column];
  }

  T* operator->() const {
    if (row >= rows || column >= columns)
      throw std::out_of_range{"Bad range operator->() !"};
    return &(ptr[row][column]);
  }

  rIt<T>& operator++() {
    row += (column + 1) / columns;
    column = (column + 1) % columns;
    return *this;
  }

  rIt<T> operator++(int) {
    rIt<T> tmp(*this);
    ++(*this);
    return tmp;
  }

  rIt<T>& operator--() {
    if (column == 0) {
      column = columns - 1;
      --row;
    } else {
      --column;
    }
    return *this;
  }

  rIt<T> operator--(int) {
    rIt<T> tmp(*this);
    --(*this);
    return tmp;
  }

  template <typename T_>
  friend void swap(rIt<T_>& lhs, rIt<T_>& rhs) {
    std::swap(lhs.ptr, rhs.ptr);
    std::swap(lhs.row, rhs.row);
    std::swap(lhs.rows, rhs.rows);
    std::swap(lhs.column, rhs.column);
    std::swap(lhs.columns, rhs.columns);
  }

  /*rIt<T>& operator[](size_t num) {
    row = num / rows;
    column = num % columns;
    return *this;
  }*/
};

#endif