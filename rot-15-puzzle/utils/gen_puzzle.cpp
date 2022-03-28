#include "gen_puzzle.hpp"

Eigen::Matrix4i generate_puzzle(int sol_depth)
{
  Eigen::Matrix4i puzzle;
  puzzle << 1,  2,  3,  4,
            5,  6,  7,  8,
            9,  10, 11, 12,
            13, 14, 15, 16;

  std::vector<int> vec;
  for (int i = 0; i < 1000; i++)
    vec.push_back(i % 9);

  std::random_device rd; 
  auto rng = std::default_random_engine { rd() };
  std::shuffle(std::begin(vec), std::end(vec), rng);

  for (int i = 0; i < sol_depth; i++) {
    std::cout << vec[i] << "\n";
    rotate_square(puzzle, vec[i]);
  }

  return puzzle;
  // std::sort( vec.begin(), vec.end() );
  // vec.erase( std::unique( vec.begin(), vec.end() ), vec.end() );
}

Eigen::Matrix4i rotate_square(Eigen::Matrix4i &m, int square_index)
{
  int i = square_index % 3;
  int j = square_index / 3;
  Eigen::Matrix2i block;
  block = m.block<2, 2>(i, j).transpose().colwise().reverse();
  m.block<2, 2>(i, j) = block;
  return m;
}

// int main(int argc, char** argv)
// {
//   if (argc < 2) {
//     std::cerr << "Provide depth!\n";
//     return 1;
//   }

//   const int depth = std::atoi(argv[1]);

//   std::cout << generate_puzzle(depth);

//   return 0;
// }
