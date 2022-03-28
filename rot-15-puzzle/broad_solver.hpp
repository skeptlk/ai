#if !defined(BROAD_SOLVER)
#define BROAD_SOLVER

#include <eigen3/Eigen/Dense>
#include <iostream>
#include <vector>
#include <queue>
#include <chrono>
#include <unordered_map>
#include "solver.hpp"
#include "puzzle.hpp"
#include "puzzle_state.hpp"


class BroadSolver: public Solver
{
private:
   std::queue<PuzzleState*> open;
   std::unordered_map<long long, PuzzleState*> open_map;
   std::unordered_map<long long, PuzzleState*> closed;

   void push_open_state(PuzzleState *state);
   PuzzleState* pop_open_state();
public:
    BroadSolver();
    ~BroadSolver();
    std::vector<Eigen::Matrix4i>* solve(Puzzle* pzl);
};


#endif // BROAD_SOLVER
