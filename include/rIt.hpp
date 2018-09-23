#ifndef __RIT_HPP__
#define __RIT_HPP__

#include <It_base.hpp>

// iterator that parses rows
template <typename T>
class rIt : public It_base<T> {
 public:
  using It_base<T>::rows;
  using It_base<T>::columns;
  using It_base<T>::row;
  using It_base<T>::column;

  rIt() : It_base<T>(){};
  rIt(T** ptr_, size_t row_, size_t column_, size_t rows_, size_t columns_)
      : It_base<T>(ptr_, row_, column_, rows_, columns_){};
  rIt(const rIt<T>& other) : It_base<T>(other){};

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

  ~rIt() = default;
};

#endif
