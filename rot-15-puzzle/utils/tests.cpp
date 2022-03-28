#include <eigen3/Eigen/Dense>
#include <iostream>
#include <random>
#include <algorithm>
#include <iterator>
#include <fstream>
#include <vector>
#include <map>
#include "gen_puzzle.hpp"
#include "../a_star_solver.hpp"
#include "../broad_solver.hpp"

#define ROUNDS_N 20

void dump_solution_to_file(
  const std::vector<Eigen::Matrix4i> *solution, 
  std::string run, 
  int depth, 
  int index
)
{
  if (solution == nullptr) return;
  std::string filename = "./results/dump" + run + "_" + std::to_string(depth) + "_" + std::to_string(index);
  std::ofstream file(filename);

  for (auto s: *solution) {
    file << "\n\n" << s;
  }
}

int main(int argc, char **argv)
{
  std::string result_filename = "result.csv";

  // syntax: ./test result.csv
  if (argc >= 2)
  {
    result_filename = argv[1];
  }

  std::ofstream csv(result_filename);

  if (!csv.is_open())
  {
    std::cerr << "Unable to open file " << result_filename << std::endl;
    return 1;
  }

  csv << "depth, "
      << "time (ms), "
      << "iterations, "
      << "open states, "
      << "open states memory (bytes)\n";

  const std::vector<int> depth_arr = {
      1, 2, 3, 4, 5,
      6, 7, 8, 9, 10,
      11, 12, 13, 14,
      15, 16, 17, 18, 19, 20};

  std::map<int, std::vector<Eigen::Matrix4i>> states;
  // generate random states
  for (const int depth : depth_arr)
  {
    std::vector<Eigen::Matrix4i> v;
    for (int i = 0; i < ROUNDS_N; i++)
      v.push_back(generate_puzzle(depth));
    states[depth] = v;
  }

  // csv << "Broad search:\n";
  // std::cout << "Broad search:\n";

  // for (const int depth : depth_arr) {
  //   if (depth == 6) break;
  //   int i = 1;
  //   for (auto state : states[depth]) {
  //     auto puzzle = new Puzzle(state);
  //     auto solver = new BroadSolver();
  //     auto answer = solver->solve(puzzle);

  //     MetaInfo meta = solver->get_meta();

  //     std::cout << "Depth " << depth << " solved\n";

  //     csv << depth << ", "
  //         << meta.totalTimeMs << ", "
  //         << meta.totalIterations << ", "
  //         << meta.openStatesCount << ", "
  //         << meta.openStatesMemory << "\n";

  //     dump_solution_to_file(answer, "bsearch", depth, i);

  //     delete solver;
  //     delete puzzle;
  //     i++;
  //   }
  // }


  csv << "A* heuristic 1:\n";
  std::cout << "A* heuristic 1:\n";

  for (const int depth : depth_arr) {   
    int i = 0; 
    for (auto state : states[depth]) {
      auto puzzle = new Puzzle(state);
      auto solver = new AStarSolver();
      solver->usedHeuristic = 1;
      auto answer = solver->solve(puzzle);

      MetaInfo meta = solver->get_meta();

      std::cout << "Depth " << depth << " solved\n";

      csv << depth << ", "
          << meta.totalTimeMs << ", "
          << meta.totalIterations << ", "
          << meta.openStatesCount << ", "
          << meta.openStatesMemory << "\n";

      dump_solution_to_file(answer, "astar1", depth, i);

      delete solver;
      delete puzzle;
      i++;
    }
  }

  csv << "A* heuristic 2:\n";
  std::cout << "A* heuristic 2:\n";

  for (const int depth : depth_arr) {
    if (depth == 8) break;
    int i = 0;
    for (auto state : states[depth]) {
      auto puzzle = new Puzzle(state);
      auto solver = new AStarSolver();
      solver->usedHeuristic = 2;
      auto answer = solver->solve(puzzle);

      MetaInfo meta = solver->get_meta();

      std::cout << "Depth " << depth << " solved\n";

      csv << depth << ", " 
          << meta.totalTimeMs << ", "
          << meta.totalIterations << ", "
          << meta.openStatesCount << ", "
          << meta.openStatesMemory << "\n";

      dump_solution_to_file(answer, "astar2", depth, i);

      delete solver;
      delete puzzle;
      i++;
    }
  }

  csv.close();

  return 0;
}
