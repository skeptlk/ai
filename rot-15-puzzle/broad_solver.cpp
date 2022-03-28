#include "broad_solver.hpp"

BroadSolver::BroadSolver(): Solver() {}

BroadSolver::~BroadSolver() {}

std::vector<Eigen::Matrix4i>* BroadSolver::solve(Puzzle* pzl)
{
    auto t1 = std::chrono::high_resolution_clock::now();
    long iterations = 0;
    std::vector<Eigen::Matrix4i> *solution = nullptr;

    puzzle = pzl;
    PuzzleState *prev = nullptr;

    push_open_state(pzl->get_state());

    while (!open.empty())
    {
        auto x = pop_open_state();
        
        if (x->to_string() == goal_str) {
            // trace back solution
            solution = new std::vector<Eigen::Matrix4i>();
            auto node = x;
            while (node != nullptr) {
                solution->push_back(node->mx);
                node = node->prev;
            }
            std::reverse(solution->begin(), solution->end());
            break;
        }

        closed[x->hash()] = x;

        if (closed.size() % 1000 == 0)
            std::cout 
                << closed.size() / 1000 << "K closed; " 
                << open.size() / 1000 << "K open \n";

        puzzle->set_state(x);

        auto steps = puzzle->get_next_steps();

        for (auto step: steps) {
            if (
                open_map.find(step->hash()) == open_map.end() && 
                closed.find(step->hash()) == closed.end()
            ) {
                push_open_state(step);
            }
        }

        iterations++;
    }
    
    auto t2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> dur = t2 - t1;
    last_run_meta.totalTimeMs = dur.count();
    last_run_meta.totalIterations = iterations;
    last_run_meta.openStatesCount = open.size();
    last_run_meta.openStatesMemory 
        = sizeof(open) + sizeof(open_map) + open.size() * (sizeof *open_map.begin()->second);
    
    return solution;
}

PuzzleState* BroadSolver::pop_open_state() 
{
    auto x = open.front();
    open.pop();
    open_map.erase(x->hash());
    return x;
}

void BroadSolver::push_open_state(PuzzleState *state) 
{
    open.push(state);
    open_map[state->hash()] = state;
}
