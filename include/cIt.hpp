#ifndef __CIT_HPP__
#define __CIT_HPP__

#include <It_base.hpp>

// iterator that parses columns
template <typename T>
class cIt : public It_base<T> {
 public:
  using It_base<T>::rows;
  using It_base<T>::columns;
  using It_base<T>::row;
  using It_base<T>::column;

  cIt() : It_base<T>(){};
  cIt(T** ptr_, size_t row_, size_t column_, size_t rows_, size_t columns_)
      : It_base<T>(ptr_, row_, column_, rows_, columns_){};
  cIt(const cIt<T>& other) : It_base<T>(other){};

  cIt<T>& operator++() {
    column += (row + 1) / rows;
    row = (row + 1) % rows;
    return *this;
  }

  cIt<T> operator++(int) {
    cIt<T> tmp(*this);
    ++(*this);
    return tmp;
  }

  cIt<T>& operator--() {
    if (row == 0) {
      row = rows - 1;
      --column;
    } else {
      --row;
    }
    return *this;
  }

  cIt<T> operator--(int) {
    cIt<T> tmp(*this);
    --(*this);
    return tmp;
  }

  ~cIt() = default;
};

#endif
