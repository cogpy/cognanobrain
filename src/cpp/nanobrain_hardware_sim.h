#ifndef NANOBRAIN_HARDWARE_SIM_H
#define NANOBRAIN_HARDWARE_SIM_H

/**
 * NanoBrain Hardware Architecture Simulator
 *
 * Chapter 5: Universal Time Crystal & Big Data
 *
 * Simulates bio-inspired hardware for time crystal computing:
 * - Thermal breathing model for oscillatory computation
 * - Microtubule dynamics for distributed processing
 * - Resonance-based information transfer
 *
 * This provides a computational model that mimics the
 * physical substrates theorized to support consciousness.
 */

#include "nanobrain_kernel.h"
#include "nanobrain_time_crystal.h"
#include <array>
#include <map>
#include <string>
#include <vector>

// Physical constants
constexpr float BODY_TEMPERATURE = 310.0f;     // Kelvin (37°C)
constexpr float TUBULIN_DIAMETER = 25.0f;      // Nanometers
constexpr int PROTOFILAMENTS_PER_TUBULE = 13;  // Standard MT structure
constexpr float BREATHING_PERIOD_MS = 1000.0f; // 1 Hz breathing cycle

/**
 * Hardware simulator configuration
 */
struct HardwareSimConfig {
  float base_temperature = BODY_TEMPERATURE;
  float temperature_variance = 2.0f;   // ±2K thermal noise
  int microtubule_count = 100;         // Number of MTs to simulate
  int tubulin_per_tubule = 100;        // Dimers per MT
  float coherence_threshold = 0.5f;    // Min coherence for processing
  float breathing_phase_offset = 0.0f; // Initial breathing phase
  bool enable_thermal_noise = true;    // Add thermal fluctuations
  bool enable_quantum_effects = true;  // Simulate quantum coherence
};

/**
 * Thermal state of the system
 */
struct ThermalState {
  float temperature;          // Current temperature (K)
  float breathing_phase;      // Breathing cycle phase [0, 2π]
  float energy_level;         // Metabolic energy available
  float entropy;              // System entropy
  float thermal_conductivity; // Heat transfer rate
  bool in_coherent_regime;    // Temperature allows coherence
};

/**
 * Single tubulin dimer state
 */
struct TubulinState {
  int index;           // Position along MT
  float alpha_state;   // α-tubulin conformational state
  float beta_state;    // β-tubulin conformational state
  float dipole_moment; // Electric dipole
  bool gtp_bound;      // GTP vs GDP bound state
};

/**
 * Microtubule state with tubulin array
 */
struct MicrotubuleDynamics {
  std::string id;
  std::vector<TubulinState> tubulin_states;   // Tubulin dimer array
  float coherence;                            // MT-wide coherence
  float resonance_frequency;                  // Collective oscillation
  std::array<float, 13> protofilament_phases; // Phase per protofilament
  float collective_dipole;                    // Net dipole moment
  bool polymerizing;                          // Growing or shrinking
};

/**
 * Resonance network between MTs
 */
struct ResonanceLink {
  std::string source_mt;
  std::string target_mt;
  float coupling_strength;
  float phase_difference;
  bool synchronized;
};

/**
 * Hardware metrics
 */
struct HardwareMetrics {
  float average_temperature;
  float average_mt_coherence;
  float total_dipole_moment;
  int synchronized_mt_pairs;
  float processing_capacity; // Estimated ops/second
  float energy_consumption;  // Arbitrary units
};

/**
 * Hardware Architecture Simulator
 *
 * Models bio-inspired computing substrate for NanoBrain.
 */
class HardwareSimulator {
public:
  HardwareSimulator(NanoBrainKernel *kernel, const HardwareSimConfig &config);
  ~HardwareSimulator();

  // ================================================================
  // Initialization
  // ================================================================

  /**
   * Initialize the hardware simulation
   */
  void initialize();

  /**
   * Shutdown simulation
   */
  void shutdown();

  /**
   * Check if simulation is running
   */
  bool is_active() const { return active; }

  // ================================================================
  // Thermal Breathing Model
  // ================================================================

  /**
   * Update thermal breathing cycle
   * @param delta_ms Time step in milliseconds
   */
  void update_thermal_breathing(float delta_ms);

  /**
   * Get current thermal state
   */
  ThermalState get_thermal_state() const { return thermal_state; }

  /**
   * Set target temperature
   */
  void set_temperature(float temp_kelvin);

  /**
   * Add thermal perturbation (external heat)
   */
  void add_thermal_perturbation(float delta_temp);

  /**
   * Check if system is in coherent thermal regime
   */
  bool in_coherent_regime() const;

  // ================================================================
  // Microtubule Dynamics
  // ================================================================

  /**
   * Create a new microtubule
   * @param tubulin_count Number of tubulin dimers
   * @return Microtubule ID
   */
  std::string create_microtubule(int tubulin_count = 100);

  /**
   * Get microtubule state
   */
  const MicrotubuleDynamics *get_microtubule(const std::string &id) const;

  /**
   * Update all microtubule dynamics
   * @param delta_ms Time step in milliseconds
   */
  void update_microtubule_dynamics(float delta_ms);

  /**
   * Get all microtubule IDs
   */
  std::vector<std::string> get_all_microtubule_ids() const;

  /**
   * Calculate MT coherence
   */
  float calculate_mt_coherence(const MicrotubuleDynamics &mt);

  /**
   * Trigger polymerization/depolymerization
   */
  void set_polymerization_state(const std::string &mt_id, bool polymerizing);

  // ================================================================
  // Resonance Network
  // ================================================================

  /**
   * Create resonance link between MTs
   */
  void create_resonance_link(const std::string &source_id,
                             const std::string &target_id,
                             float coupling_strength);

  /**
   * Update resonance network
   */
  void update_resonance_network(float delta_ms);

  /**
   * Find synchronized MT clusters
   */
  std::vector<std::vector<std::string>> find_synchronized_clusters();

  /**
   * Get resonance links for MT
   */
  std::vector<ResonanceLink>
  get_resonance_links(const std::string &mt_id) const;

  // ================================================================
  // Information Processing
  // ================================================================

  /**
   * Inject pattern into MT network
   * @param pattern Pattern tensor to inject
   * @param mt_ids Target microtubules
   */
  void inject_pattern(NanoBrainTensor *pattern,
                      const std::vector<std::string> &mt_ids);

  /**
   * Read collective state from MT network
   * @return State tensor
   */
  NanoBrainTensor *read_collective_state();

  /**
   * Perform distributed computation step
   */
  void process_step();

  // ================================================================
  // Full Simulation Cycle
  // ================================================================

  /**
   * Run one complete simulation cycle
   * @param delta_ms Time step in milliseconds
   */
  void run_cycle(float delta_ms);

  /**
   * Run multiple cycles
   */
  void run_cycles(int count, float delta_ms = 1.0f);

  // ================================================================
  // Metrics
  // ================================================================

  /**
   * Get hardware metrics
   */
  HardwareMetrics get_metrics() const;

  /**
   * Get cycle count
   */
  size_t get_cycle_count() const { return cycle_count; }

  /**
   * Get configuration
   */
  const HardwareSimConfig &get_config() const { return config; }

private:
  NanoBrainKernel *kernel;
  HardwareSimConfig config;

  // State
  bool active = false;
  size_t cycle_count = 0;
  int mt_counter = 0;

  ThermalState thermal_state;
  std::map<std::string, MicrotubuleDynamics> microtubules;
  std::vector<ResonanceLink> resonance_links;

  // Private helpers
  std::string generate_mt_id();
  TubulinState create_tubulin(int index);
  void update_tubulin(TubulinState &tubulin, float delta_ms);
  void update_protofilament_phases(MicrotubuleDynamics &mt, float delta_ms);
  float compute_collective_dipole(const MicrotubuleDynamics &mt);
  void apply_thermal_noise(MicrotubuleDynamics &mt);
  void apply_quantum_effects(MicrotubuleDynamics &mt);
};

#endif // NANOBRAIN_HARDWARE_SIM_H
