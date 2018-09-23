#include <fstream>
#include <iostream>
#include <iterator>
#include <matrix.hpp>
#include <vector>

int main() {
  std::ifstream fin("./data/SimplexTable.txt");

  if (!fin.good()) {
    std::cout << "Can't read from file!\n";
    return 1;
  }

  std::istream_iterator<double> iit(fin);
  std::istream_iterator<double> eof;

  Matrix<double> matrix(iit, eof, 4, 4);

  std::vector<char> basis{'4', '5', '6', 'F'};
  std::vector<char> free_{'S', '1', '2', '3'};

  matrix.optimization(basis, free_);

  //matrix.out();

  return 0;
}
