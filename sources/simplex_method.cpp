#include <simplex_method_matrix.hpp>
#include <string>

int main(int argc, char* argv[]) {
  if (argc != 4) {
    std::cout << "Run program with arguments: ./simplex_method "
                 "<paath_to_file_with_A> <path_to_file_with_B> "
                 "<path_to_file_with_C>\n ";
    return 0;
  }

  splx_matrix<double> matrix;
  try {
    matrix.load_data(std::string(argv[1]), std::string(argv[2]),
                     std::string(argv[3]));
  } catch (std::ios::failure& e) {
    std::cout << e.what() << std::endl;
    return 1;
  }


  std::cout << "Canonsical simplex table:\n";
  matrix.out();
  std::cout << std::endl;
  if (auto c = matrix.simplex_method(); c == 0) {
    std::cout << "Found the most optimal solution!\n";
    matrix.show_answer();
  } else if (c == 2) {
    std::cout << "Function is unlimited!\n";
  } else {
    std::cout << "No solution!\n";
  }

  return 0;
}
