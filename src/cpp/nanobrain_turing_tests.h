#ifndef NANOBRAIN_TURING_TESTS_H
#define NANOBRAIN_TURING_TESTS_H

/**
 * Turing Failure Scenarios - Chapter 5, Task 5.4
 *
 * Port of 10 Turing-failure scenarios demonstrating problems
 * that Turing machines cannot solve but fractal machines can.
 */

#include "nanobrain_time_crystal.h"
#include <functional>
#include <string>
#include <vector>

// ================================================================
// Turing Test Scenarios
// ================================================================

enum class TuringScenario {
  HaltingProblem,       // Self-referential halting
  GoldbachConjecture,   // Number theory verification
  CollatzConjecture,    // Convergence prediction
  BusyBeaver,           // Maximum productivity
  DiophantineEquations, // Integer equation solving
  WangTiles,            // Tile arrangement decision
  PostCorrespondence,   // String matching problem
  MatrixMortality,      // Matrix product decidability
  WordProblem,          // Group theory equivalence
  RiceTheorem           // Program property decision
};

// ================================================================
// Test Result Structures
// ================================================================

struct TuringTestResult {
  TuringScenario scenario;
  std::string scenario_name;
  bool turing_can_solve;
  bool fractal_can_solve;
  float fractal_confidence;
  std::string solution_approach;
  int64_t compute_time_us;
  std::string notes;
};

struct TuringTestSuite {
  std::vector<TuringTestResult> results;
  int turing_successes;
  int fractal_successes;
  float overall_improvement;
};

struct FractalSolution {
  bool found;
  std::vector<float> solution_vector;
  float confidence;
  int iterations;
  std::string method;
};

// ================================================================
// Turing Test Engine
// ================================================================

class TuringTestEngine {
public:
  TuringTestEngine(TimeCrystalKernel *tc_kernel);
  ~TuringTestEngine();

  void initialize();
  void reset();

  // ================================================================
  // Run Individual Scenarios
  // ================================================================

  TuringTestResult test_scenario(TuringScenario scenario);

  TuringTestResult test_halting_problem(std::function<bool()> program,
                                        int max_steps = 1000);

  TuringTestResult test_goldbach(int even_number);

  TuringTestResult test_collatz(int64_t starting_number);

  TuringTestResult test_busy_beaver(int num_states);

  TuringTestResult test_diophantine(const std::vector<int> &coefficients,
                                    int target);

  TuringTestResult test_wang_tiles(const std::vector<std::vector<int>> &tiles,
                                   int grid_size);

  TuringTestResult
  test_post_correspondence(const std::vector<std::string> &top,
                           const std::vector<std::string> &bottom);

  TuringTestResult test_matrix_mortality(
      const std::vector<std::vector<std::vector<float>>> &matrices);

  TuringTestResult test_word_problem(
      const std::string &word1, const std::string &word2,
      const std::vector<std::pair<std::string, std::string>> &rules);

  TuringTestResult
  test_rice_theorem(std::function<bool(std::function<int(int)>)> property);

  // ================================================================
  // Full Test Suite
  // ================================================================

  TuringTestSuite run_all_tests();

  std::string generate_report(const TuringTestSuite &suite) const;

  // ================================================================
  // Fractal Machine Solutions
  // ================================================================

  FractalSolution
  fractal_solve_halting(const std::vector<float> &program_encoding);

  FractalSolution
  fractal_solve_convergence(const std::vector<float> &sequence_start);

  FractalSolution
  fractal_solve_search(const std::vector<float> &target_pattern,
                       const std::vector<std::vector<float>> &search_space);

private:
  TimeCrystalKernel *tc_kernel;
  bool initialized = false;

  // Helper functions
  std::string scenario_to_string(TuringScenario scenario) const;
  bool is_prime(int n) const;
  std::vector<int> collatz_sequence(int64_t n, int max_steps) const;
};

#endif // NANOBRAIN_TURING_TESTS_H
