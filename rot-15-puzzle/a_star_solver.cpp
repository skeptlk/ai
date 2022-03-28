#include "a_star_solver.hpp"

AStarSolver::AStarSolver(): Solver() {}

AStarSolver::~AStarSolver() {}

std::vector<Eigen::Matrix4i>* AStarSolver::solve(Puzzle* pzl)
{
    auto t1 = std::chrono::high_resolution_clock::now();
    long iterations = 0;

    puzzle = pzl;
    std::vector<Eigen::Matrix4i> *solution = nullptr;
    State *prev = nullptr;

    push_open_state(prev, pzl->get_state());

    while (!open.empty())
    {
        auto pair = pop_open_state_with_cost();
        int cost = pair.first;
        State* x = pair.second;
        
        if (x->to_string() == goal_str) {
            // std::cout << "solution!\n"; 
            // trace back solution
            solution = new std::vector<Eigen::Matrix4i>();
            PuzzleState* node = x;
            while (node != nullptr) {
                solution->push_back(node->mx);
                node = node->prev;
            }
            std::reverse(solution->begin(), solution->end());
            break;
        }

        closed[x->hash()] = x;

        if (closed.size() > 80000) {
            return nullptr;
        }

        if (closed.size() % 1000 == 0)
            std::cout 
                << closed.size() / 1000 << "K closed; " 
                << open.size() / 1000 << "K open \n";

        puzzle->set_state(x);

        auto steps = puzzle->get_next_steps();

        for (auto step: steps) {
            if (
                open.find(step->hash()) == open.end() && 
                closed.find(step->hash()) == closed.end()
            ) {
                push_open_state(x, step);
            } else if (closed.find(step->hash()) == closed.end()) {
                update_cost_for_state(x, open.find(step->hash())->second, x->n + 1);
            }
        }

        iterations++;
    }

    auto t2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> dur = t2 - t1;
    last_run_meta.totalTimeMs = dur.count();
    last_run_meta.totalIterations = iterations;
    last_run_meta.openStatesCount = open.size();
    last_run_meta.openStatesMemory = (sizeof open) + open.size() * (sizeof *open.begin()->second) + (sizeof open_costs);
    
    return solution;
}

std::pair<int, State*> AStarSolver::pop_open_state_with_cost() 
{
    auto x = std::make_pair(
        open_costs.begin()->first, 
        open_costs.begin()->second
    );
    open_costs.erase(open_costs.begin());
    open.erase(x.second->hash());
    return x;
}

void AStarSolver::update_cost_for_state(State *parent, State *node, int new_steps)
{
    // std::cout << "Update! " << new_steps << "\n";
    auto range = open_costs.equal_range(node->h + node->n);
    for (auto it = range.first; it != range.second; it++) {
        if (it->second->hash() == node->hash()) {
            it = open_costs.erase(it);
            break;
        }
    }

    node->n = new_steps;
    node->prev = parent;
    open_costs.insert({ node->h + node->n, node });
}

void AStarSolver::push_open_state(State *prev, PuzzleState *state) 
{
    auto costedState = new CostedPuzzleState(state, prev ? prev->n + 1 : 0, 0);
    if (usedHeuristic == 1) {
        costedState->h = heuristic1(costedState);
    } else {
        costedState->h = heuristic2(costedState);
    }
    open_costs.insert(std::make_pair(costedState->h + costedState->n, costedState));
    open.insert(std::make_pair(costedState->hash(), costedState));
}


/* Number of  */
int AStarSolver::heuristic1(State *state)
{
    auto mx = state->mx;
    int count = 0;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            count += (i * 4 + j + 1) != mx(i, j);
    return count * 10;
}

int AStarSolver::heuristic2(State *state)
{
    auto m = state->mx;
    // manhattan distance
    int value = 0;
    for (int i = 0; i < 3; i++) { // rows
        for (int j = 0; j < 3; j++) { // cols
            int goal_i = (m(i, j) - 1) / 4;
            int goal_j = (m(i, j) - 1) % 4;
            value += abs(i - goal_i) + abs(j - goal_j);
        }
    }

    return 30 * value;
}
