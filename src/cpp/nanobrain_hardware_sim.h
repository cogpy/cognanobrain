#ifndef NANOBRAIN_HARDWARE_SIM_H
#define NANOBRAIN_HARDWARE_SIM_H

/**
 * Hardware Architecture Simulation - Chapter 5, Task 5.5
 *
 * Thermal breathing model and microtubule dynamics simulation
 * for biologically-inspired time crystal hardware.
 */

#include "nanobrain_time_crystal.h"
#include <array>
#include <functional>
#include <vector>

// ================================================================
// Hardware Component Types
// ================================================================

enum class HardwareComponent {
  Microtubule,       // Tubulin protein network
  Membrane,          // Cell membrane oscillator
  MitochondriaArray, // Energy production
  SynapticCleft,     // Neurotransmitter dynamics
  AxonHillock,       // Action potential initiation
  DendriticSpine,    // Input integration
  GlialNetwork       // Support cell coupling
};

enum class BreathingMode {
  Constant,    // Steady state
  Sinusoidal,  // Simple oscillation
  Cardiac,     // Heart-rate coupled
  Respiratory, // Breathing-coupled
  Chaotic      // Strange attractor
};

// ================================================================
// Thermal Dynamics
// ================================================================

struct ThermalConfig {
  float base_temperature = 310.15f; // 37°C in Kelvin
  float amplitude = 0.5f;           // Temperature swing
  BreathingMode mode = BreathingMode::Sinusoidal;
  float frequency = 0.1f; // Hz
  float thermal_noise = 0.01f;
};

struct ThermalState {
  float current_temperature;
  float heat_capacity;
  float thermal_conductivity;
  float entropy;
  float free_energy;
};

// ================================================================
// Microtubule Model
// ================================================================

struct TubulinDimer {
  int index;
  float alpha_state; // α-tubulin conformation (0-1)
  float beta_state;  // β-tubulin conformation (0-1)
  float gtp_energy;  // GTP binding energy
  float coherence;   // Quantum coherence level
  std::array<float, 3> position;
};

struct Microtubule {
  std::string id;
  std::vector<TubulinDimer> dimers;
  int protofilaments; // Usually 13
  float length;       // In nanometers
  float bending_rigidity;
  float oscillation_frequency;
  std::vector<int> prime_resonances;
};

struct MicrotubuleMetrics {
  float average_coherence;
  float collective_oscillation;
  float information_capacity;
  int active_dimers;
  float quantum_effects_ratio;
};

// ================================================================
// Hardware Simulation Engine
// ================================================================

class HardwareSimulator {
public:
  HardwareSimulator(TimeCrystalKernel *tc_kernel,
                    const ThermalConfig &thermal_config = {});
  ~HardwareSimulator();

  void initialize();
  void reset();
  bool is_initialized() const { return initialized; }

  // ================================================================
  // Thermal Breathing Model
  // ================================================================

  void set_thermal_config(const ThermalConfig &config);
  ThermalState get_thermal_state() const;

  // Update thermal state by one timestep
  void update_thermal(float dt);

  // Get temperature at current time
  float current_temperature() const;

  // Calculate thermal effects on coherence
  float thermal_decoherence_rate() const;

  // ================================================================
  // Microtubule Dynamics
  // ================================================================

  // Create a microtubule with given length (nm) and protofilament count
  std::string create_microtubule(float length, int protofilaments = 13);

  // Get microtubule by ID
  Microtubule *get_microtubule(const std::string &id);
  const Microtubule *get_microtubule(const std::string &id) const;

  // Remove microtubule
  bool remove_microtubule(const std::string &id);

  // Update microtubule dynamics
  void update_microtubules(float dt);

  // Calculate metrics for a microtubule
  MicrotubuleMetrics calculate_metrics(const std::string &id) const;

  // ================================================================
  // Quantum Effects
  // ================================================================

  // Calculate quantum coherence across all microtubules
  float calculate_network_coherence() const;

  // Simulate Orch-OR collapse event
  bool simulate_orchestrated_reduction(const std::string &mt_id);

  // Get quantum state of tubulin network
  std::vector<float> get_quantum_state() const;

  // ================================================================
  // Time Crystal Integration
  // ================================================================

  // Couple microtubules to time crystal phases
  void couple_to_time_crystal();

  // Transfer coherence from TC to microtubules
  void transfer_coherence(float amount);

  // ================================================================
  // Simulation Control
  // ================================================================

  // Run simulation for given duration (ms)
  void simulate(float duration_ms, float timestep_ms = 0.1f);

  // Get simulation time
  float get_simulation_time() const { return simulation_time; }

  // ================================================================
  // Statistics
  // ================================================================

  size_t microtubule_count() const { return microtubules.size(); }
  size_t total_dimers() const;
  float average_coherence() const;

private:
  TimeCrystalKernel *tc_kernel;
  ThermalConfig thermal_config;
  ThermalState thermal_state;
  bool initialized = false;
  float simulation_time = 0.0f;

  // Microtubule storage
  std::vector<Microtubule> microtubules;
  int mt_counter = 0;

  // Private helpers
  std::string generate_mt_id();
  void update_dimer(TubulinDimer &dimer, float dt);
  float calculate_dimer_energy(const TubulinDimer &dimer) const;
};

// ================================================================
// Orch-OR Theory Model
// ================================================================

struct OrchOREvent {
  int64_t timestamp;
  std::string microtubule_id;
  int num_tubulins_collapsed;
  float coherence_before;
  float coherence_after;
  float information_integrated; // Bits
  bool conscious_moment;        // Φ > threshold?
};

/**
 * Orchestrated Objective Reduction model based on Penrose-Hameroff theory
 */
class OrchORModel {
public:
  OrchORModel(HardwareSimulator *hw_sim);

  void initialize();

  // Calculate time until next conscious moment
  float calculate_collapse_time(int num_tubulins) const;

  // Check if collapse should occur
  bool should_collapse(float superposition_mass) const;

  // Trigger orchestrated reduction
  OrchOREvent orchestrated_reduction(const std::string &mt_id);

  // Get Φ (integrated information) estimate
  float calculate_phi() const;

  // Get history of conscious moments
  std::vector<OrchOREvent> get_event_history() const;

private:
  HardwareSimulator *hw_sim;
  std::vector<OrchOREvent> event_history;

  // Planck scale constants
  static constexpr float PLANCK_TIME_S = 5.39e-44f;
  static constexpr float TUBULIN_MASS_KG = 1.0e-22f;
  static constexpr float PHI_THRESHOLD = 0.5f;
};

#endif // NANOBRAIN_HARDWARE_SIM_H
