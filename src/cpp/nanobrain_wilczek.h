#ifndef NANOBRAIN_WILCZEK_H
#define NANOBRAIN_WILCZEK_H

/**
 * WilczekTimeCrystal - Chapter 2, Task 2.4
 *
 * Implementation of Wilczek's discrete time translation symmetry breaking
 * time crystal for comparison with NanoBrain's continuous time crystal
 * approach.
 *
 * Reference: Wilczek, F. (2012). "Quantum Time Crystals"
 */

#include "nanobrain_kernel.h"
#include "nanobrain_time_crystal.h"
#include <array>
#include <map>
#include <memory>
#include <string>
#include <vector>

// ================================================================
// Wilczek Time Crystal Configuration
// ================================================================

struct WilczekConfig {
  int num_spins = 100;               // Number of spins in the system
  float driving_frequency = 1.0f;    // External driving frequency
  float interaction_strength = 0.5f; // Spin-spin interaction
  float disorder_strength = 0.1f;    // Disorder in the system
  int driving_period = 10;           // Period of discrete time crystal
  bool many_body_localized = true;   // MBL protection
};

// ================================================================
// Spin State
// ================================================================

/**
 * Individual spin state in the time crystal
 */
struct WilczekSpin {
  int index;
  float z_component; // σ_z expectation
  float x_component; // σ_x expectation
  float coupling;    // Coupling to neighbors
  float local_field; // Local magnetic field
};

/**
 * Time crystal phase
 */
enum class WilczekPhase {
  Normal,       // Normal phase (no time crystal)
  DiscreteTC,   // Discrete time crystal phase
  ContinuousTC, // Approximate continuous (prethermal)
  Melted        // Melted (thermalized)
};

// ================================================================
// Comparison Metrics
// ================================================================

/**
 * Comparison between NanoBrain TC and Wilczek TC
 */
struct TimeCrystalComparison {
  float nanobrain_coherence;
  float wilczek_coherence;
  float nanobrain_period;
  float wilczek_period;
  float correlation;             // Correlation between approaches
  float phase_alignment;         // Phase alignment measure
  std::string dominant_approach; // "NanoBrain" or "Wilczek"
};

/**
 * Benchmark result
 */
struct TimeCrystalBenchmark {
  int cycles_tested;
  float nanobrain_stability;
  float wilczek_stability;
  float nanobrain_compute_time_ms;
  float wilczek_compute_time_ms;
  float coherence_decay_rate_nb;
  float coherence_decay_rate_w;
  std::string recommendation;
};

// ================================================================
// Wilczek Time Crystal Class
// ================================================================

/**
 * WilczekTimeCrystal - Discrete time translation symmetry breaking
 *
 * Implements a periodically driven many-body system exhibiting
 * discrete time crystal behavior (period doubling under driving).
 */
class WilczekTimeCrystal {
public:
  WilczekTimeCrystal(const WilczekConfig &config = {});
  ~WilczekTimeCrystal();

  // ================================================================
  // Initialization
  // ================================================================

  void initialize();
  void reset();
  bool is_initialized() const { return initialized; }

  // ================================================================
  // Time Evolution
  // ================================================================

  // Evolve system by one driving period
  void evolve_period();

  // Evolve for N periods
  void evolve(int periods);

  // Get current phase
  WilczekPhase current_phase() const;

  // Get phase name
  std::string phase_name() const;

  // ================================================================
  // Measurements
  // ================================================================

  // Measure magnetization
  float measure_magnetization() const;

  // Measure spin-spin correlations
  float measure_correlation(int i, int j) const;

  // Measure total coherence (order parameter)
  float measure_coherence() const;

  // Get oscillation period
  float measure_period() const;

  // Check for time crystal signature (period doubling)
  bool has_time_crystal_signature() const;

  // ================================================================
  // Comparison with NanoBrain TC
  // ================================================================

  // Compare with NanoBrain time crystal
  TimeCrystalComparison
  compare_with_nanobrain(const TimeCrystalKernel &nanobrain_tc) const;

  // Run benchmark comparison
  TimeCrystalBenchmark run_benchmark(TimeCrystalKernel &nanobrain_tc,
                                     int test_cycles = 100);

  // ================================================================
  // Spin Access
  // ================================================================

  // Get spin state
  const WilczekSpin *get_spin(int index) const;

  // Get all spin z-components
  std::vector<float> get_spin_configuration() const;

  // Set initial spin configuration
  void set_spin_configuration(const std::vector<float> &config);

  // ================================================================
  // Configuration
  // ================================================================

  const WilczekConfig &get_config() const { return config; }
  void update_config(const WilczekConfig &new_config);

  // Statistics
  int get_period_count() const { return period_count; }
  float get_total_time() const { return total_time; }

private:
  WilczekConfig config;
  bool initialized = false;

  // Spin system
  std::vector<WilczekSpin> spins;

  // State history for period detection
  std::vector<std::vector<float>> magnetization_history;
  int history_size = 50;

  // Counters
  int period_count = 0;
  float total_time = 0.0f;

  // Private methods
  void apply_floquet_drive();
  void apply_interaction();
  void apply_disorder();
  float compute_hamiltonian() const;
};

// ================================================================
// Utility Functions
// ================================================================

/**
 * Create default Wilczek time crystal with optimal parameters
 */
WilczekTimeCrystal create_default_wilczek_tc();

/**
 * Quick comparison between two approaches
 */
TimeCrystalComparison quick_compare(const TimeCrystalKernel &nanobrain,
                                    const WilczekTimeCrystal &wilczek);

#endif // NANOBRAIN_WILCZEK_H
