#if !defined(PUZZLE_STATE)
#define PUZZLE_STATE

#include <eigen3/Eigen/Dense>
#include <sstream>

struct PuzzleState
{
    PuzzleState(Eigen::Matrix4i m, PuzzleState *p): mx(m), prev(p) {}
    Eigen::Matrix4i mx;
    PuzzleState *prev;

    operator std::string() const {
        return to_string();
    }

    std::string to_string() const {
        std::stringstream ss;
        ss << mx;
        return ss.str();
    }

    long long hash() const {
        long long hash = 0;
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                hash = 17 * hash + mx(i, j);
        return hash;
    }
};


#endif // PUZZLE_STATE
