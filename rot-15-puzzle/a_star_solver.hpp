#include <iostream>
#include <map>
#include <set>
#include <unordered_map>
#include <chrono>
#include "solver.hpp"
#include "puzzle.hpp"
#include "costed_puzzle_state.hpp"

typedef CostedPuzzleState State;

class AStarSolver : public Solver
{
private:
    std::multimap<int, State *> open_costs;
    std::unordered_map<long long, State *> closed;

    void push_open_state(State *prev, PuzzleState *state);
    std::pair<int, State*> pop_open_state_with_cost();
    void update_cost_for_state(State *parent, State *node, int new_steps);

    /* Number of  */
    int heuristic1(State *state);
    int heuristic2(State *state);
public:
    std::unordered_map<long long, State *> open;
    AStarSolver();
    ~AStarSolver();
    int usedHeuristic = 1;
    std::vector<Eigen::Matrix4i> *solve(Puzzle *pzl);
};
