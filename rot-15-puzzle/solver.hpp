#if !defined(SOLVER)
#define SOLVER

#include <eigen3/Eigen/Dense>
#include "puzzle.hpp"
#include "puzzle_state.hpp"

struct MetaInfo
{
    int openStatesMemory;
    int openStatesCount;
    double totalTimeMs;
    int totalIterations;
};

class Solver
{
protected:
    Eigen::Matrix4i goal;
    Puzzle* puzzle;
    std::string goal_str;
    MetaInfo last_run_meta;

public:
    Solver() {
        goal << 1,  2,  3,  4,
                5,  6,  7,  8,
                9,  10, 11, 12,
                13, 14, 15, 16;
        PuzzleState n(goal, nullptr);
        goal_str = n.to_string();
    }
    ~Solver() {};
    MetaInfo get_meta() {
        return last_run_meta;
    }
    virtual std::vector<Eigen::Matrix4i>* solve(Puzzle* pzl) = 0;
};

#endif // SOLVER
