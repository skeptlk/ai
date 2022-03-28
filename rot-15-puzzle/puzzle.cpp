#include "puzzle.hpp"


Puzzle::Puzzle(Eigen::Matrix4i initial)
{
    state = new PuzzleState(initial, nullptr);
}

Puzzle::~Puzzle()
{
}

std::vector<PuzzleState*> Puzzle::get_next_steps()
{
    std::vector<PuzzleState*> vec;

    for (int i = 0; i < 3; i++) { // rows
        for (int j = 0; j < 3; j++) { // cols
            Eigen::Matrix4i m = state->mx, s = state->mx;
            Eigen::Matrix2i block;
            block = m.block<2, 2>(i, j).transpose().colwise().reverse();
            m.block<2, 2>(i, j) = block;
            vec.push_back(new PuzzleState(m, state));
            block = s.block<2, 2>(i, j).colwise().reverse().transpose(); // cw
            s.block<2, 2>(i, j) = block;
            vec.push_back(new PuzzleState(s, state));
        }
    }

    return std::move(vec);
}

