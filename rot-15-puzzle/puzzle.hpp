#if !defined(PUZZLE)
#define PUZZLE

#include <eigen3/Eigen/Dense>
#include <string>
#include <vector>
#include "puzzle_state.hpp"

class Puzzle
{
private:
    PuzzleState* state;
public:
    Puzzle(Eigen::Matrix4i);
    ~Puzzle();
    std::vector<PuzzleState*> get_next_steps();
    PuzzleState* get_state() { return state; }
    void set_state(PuzzleState* s) { state = s; }

};


#endif // PUZZLE
