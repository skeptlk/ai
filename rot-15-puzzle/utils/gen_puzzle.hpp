#if !defined(GEN_PUZZLE)
#define GEN_PUZZLE

#include <eigen3/Eigen/Dense>
#include <iostream>
#include <random>
#include <algorithm>
#include <iterator>

Eigen::Matrix4i rotate_square(Eigen::Matrix4i &m, int square_index);
Eigen::Matrix4i generate_puzzle(int sol_depth);

#endif // GEN_PUZZLE