
#include <eigen3/Eigen/Dense>
#include <iostream>
#include <random>
#include <algorithm>
#include <iterator>
#include <vector>
#include "puzzle.hpp"
#include "puzzle_state.hpp"
#include "broad_solver.hpp"
#include "a_star_solver.hpp"

void random_puzzle(Eigen::Matrix4i & m) 
{
    std::vector<int> vec;
    for (size_t i = 1; i <= 16; i++) {
        vec.push_back(i);
    }

    std::random_device rd; 
    auto rng = std::default_random_engine { rd() };
    std::shuffle(std::begin(vec), std::end(vec), rng);
    
    for (int i = 0; i < 16; i++) {
        m(i / 4, i % 4) = vec[i];
    }
}

int main(int argc, char** argv)
{
    Eigen::Matrix4i m;
    // m << 1,  2,  7,  3,
    //      5,  6,  4, 12,
    //      10, 14, 8, 11,
    //      9, 13, 15, 16;

    // m << 8,  2, 15,  4,
    //     3, 14,  7, 16,
    //     12,  5,  1,  6,
    //     10, 11, 13,  9;


    AStarSolver solver;
    random_puzzle(m);
    std::cout << m << "\n\n";
    Puzzle p(m);
    auto solution = solver.solve(&p);

    for (auto s: *solution) {
        std::cout << "\nStep: \n" << s;
    }

}