#if !defined(COSTED_PUZZLE_STATE)
#define COSTED_PUZZLE_STATE

#include <eigen3/Eigen/Dense>
#include <sstream>
#include "puzzle_state.hpp"

struct CostedPuzzleState: PuzzleState
{
    CostedPuzzleState(Eigen::Matrix4i m, PuzzleState *p): PuzzleState(m, p) {}
    CostedPuzzleState(PuzzleState* ps, int num, int heu): 
        PuzzleState(ps->mx, ps->prev), n(num), h(heu) {}

    int n; // number of steps
    int h; // heuristics
};


#endif // COSTED_PUZZLE_STATE
