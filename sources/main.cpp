#include <iostream>
#include <matrix.hpp>
#include <vector>

int main() {
  std::vector<int> vec{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
  Matrix<int> matrix(vec.begin(), vec.end(), 3, 4);

  std::cout << std::endl;
}