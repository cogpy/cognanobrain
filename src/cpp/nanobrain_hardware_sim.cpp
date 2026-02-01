#include "nanobrain_hardware_sim.h"
#include <algorithm>
#include <chrono>
#include <cmath>
#include <random>

// ================================================================
// HardwareSimulator Implementation
// ================================================================

HardwareSimulator::HardwareSimulator(TimeCrystalKernel *tc,
                                     const ThermalConfig &config)
    : tc_kernel(tc), thermal_config(config), initialized(false),
      simulation_time(0.0f), mt_counter(0) {
  thermal_state.current_temperature = thermal_config.base_temperature;
  thermal_state.heat_capacity = 4186.0f;     // J/(kg·K) like water
  thermal_state.thermal_conductivity = 0.6f; // W/(m·K)
  thermal_state.entropy = 0.0f;
  thermal_state.free_energy = 0.0f;
}

HardwareSimulator::~HardwareSimulator() = default;

void HardwareSimulator::initialize() {
  microtubules.clear();
  simulation_time = 0.0f;
  mt_counter = 0;
  thermal_state.current_temperature = thermal_config.base_temperature;
  initialized = true;
}

void HardwareSimulator::reset() {
  microtubules.clear();
  simulation_time = 0.0f;
  initialized = false;
}

std::string HardwareSimulator::generate_mt_id() {
  return "mt_" + std::to_string(++mt_counter);
}

// ================================================================
// Thermal Breathing Model
// ================================================================

void HardwareSimulator::set_thermal_config(const ThermalConfig &config) {
  thermal_config = config;
}

ThermalState HardwareSimulator::get_thermal_state() const {
  return thermal_state;
}

float HardwareSimulator::current_temperature() const {
  return thermal_state.current_temperature;
}

void HardwareSimulator::update_thermal(float dt) {
  static std::random_device rd;
  static std::mt19937 gen(rd());
  std::normal_distribution<float> noise(0.0f, thermal_config.thermal_noise);

  float phase = simulation_time * thermal_config.frequency * 2.0f * M_PI;
  float base = thermal_config.base_temperature;
  float amp = thermal_config.amplitude;

  switch (thermal_config.mode) {
  case BreathingMode::Constant:
    thermal_state.current_temperature = base;
    break;

  case BreathingMode::Sinusoidal:
    thermal_state.current_temperature = base + amp * std::sin(phase);
    break;

  case BreathingMode::Cardiac: {
    // Heart rate ~1 Hz with sharp peaks
    float cardiac =
        std::exp(-0.5f * std::pow(std::fmod(phase, 2.0f * M_PI), 2));
    thermal_state.current_temperature = base + amp * cardiac;
  } break;

  case BreathingMode::Respiratory: {
    // Breathing ~0.2 Hz with asymmetric rise/fall
    float breath = (std::sin(phase) + 0.3f * std::sin(2 * phase)) / 1.3f;
    thermal_state.current_temperature = base + amp * breath;
  } break;

  case BreathingMode::Chaotic: {
    // Simple Lorenz attractor approximation
    static float x = 1.0f, y = 1.0f, z = 1.0f;
    float sigma = 10.0f, rho = 28.0f, beta = 8.0f / 3.0f;
    float dx = sigma * (y - x) * dt;
    float dy = (x * (rho - z) - y) * dt;
    float dz = (x * y - beta * z) * dt;
    x += dx;
    y += dy;
    z += dz;
    thermal_state.current_temperature = base + amp * z / 30.0f;
  } break;
  }

  // Add noise
  thermal_state.current_temperature += noise(gen);

  // Update thermodynamic quantities
  float T = thermal_state.current_temperature;
  thermal_state.entropy = thermal_state.heat_capacity * std::log(T / 273.15f);
  thermal_state.free_energy = thermal_state.heat_capacity * (T - 310.15f);
}

float HardwareSimulator::thermal_decoherence_rate() const {
  // Decoherence increases with temperature
  float T = thermal_state.current_temperature;
  float T0 = 310.15f; // Reference temperature
  return std::exp((T - T0) / 10.0f) - 1.0f;
}

// ================================================================
// Microtubule Dynamics
// ================================================================

std::string HardwareSimulator::create_microtubule(float length,
                                                  int protofilaments) {
  Microtubule mt;
  mt.id = generate_mt_id();
  mt.protofilaments = protofilaments;
  mt.length = length;
  mt.bending_rigidity = 2.2e-23f;    // J·m
  mt.oscillation_frequency = 8.0e6f; // ~8 MHz

  // Create dimers (8nm per dimer)
  int num_dimers = static_cast<int>(length / 8.0f) * protofilaments;
  mt.dimers.reserve(num_dimers);

  for (int i = 0; i < num_dimers; i++) {
    TubulinDimer dimer;
    dimer.index = i;
    dimer.alpha_state = 0.5f;
    dimer.beta_state = 0.5f;
    dimer.gtp_energy = 1.0f;
    dimer.coherence = 0.8f;

    // Position along cylinder
    int row = i / protofilaments;
    int col = i % protofilaments;
    float angle = 2.0f * M_PI * col / protofilaments;
    dimer.position = {12.5f * std::cos(angle), 12.5f * std::sin(angle),
                      row * 8.0f};

    mt.dimers.push_back(dimer);
  }

  // Initialize prime resonances
  mt.prime_resonances = {2, 3, 5, 7, 11, 13};

  microtubules.push_back(mt);
  return mt.id;
}

Microtubule *HardwareSimulator::get_microtubule(const std::string &id) {
  for (auto &mt : microtubules) {
    if (mt.id == id)
      return &mt;
  }
  return nullptr;
}

const Microtubule *
HardwareSimulator::get_microtubule(const std::string &id) const {
  for (const auto &mt : microtubules) {
    if (mt.id == id)
      return &mt;
  }
  return nullptr;
}

bool HardwareSimulator::remove_microtubule(const std::string &id) {
  auto it = std::find_if(microtubules.begin(), microtubules.end(),
                         [&id](const auto &mt) { return mt.id == id; });
  if (it != microtubules.end()) {
    microtubules.erase(it);
    return true;
  }
  return false;
}

void HardwareSimulator::update_dimer(TubulinDimer &dimer, float dt) {
  // Quantum oscillation
  float omega = 8.0e6f; // 8 MHz tubulin frequency
  dimer.alpha_state += 0.5f * std::sin(omega * simulation_time) * dt;
  dimer.beta_state += 0.5f * std::cos(omega * simulation_time) * dt;

  // Normalize states to [0, 1]
  dimer.alpha_state = std::max(0.0f, std::min(1.0f, dimer.alpha_state));
  dimer.beta_state = std::max(0.0f, std::min(1.0f, dimer.beta_state));

  // GTP hydrolysis decay
  dimer.gtp_energy *= (1.0f - 0.001f * dt);

  // Thermal decoherence
  float decoherence = thermal_decoherence_rate();
  dimer.coherence *= (1.0f - 0.01f * decoherence * dt);
  dimer.coherence = std::max(0.0f, dimer.coherence);
}

void HardwareSimulator::update_microtubules(float dt) {
  for (auto &mt : microtubules) {
    for (auto &dimer : mt.dimers) {
      update_dimer(dimer, dt);
    }
  }
}

MicrotubuleMetrics
HardwareSimulator::calculate_metrics(const std::string &id) const {
  MicrotubuleMetrics metrics;
  metrics.average_coherence = 0.0f;
  metrics.collective_oscillation = 0.0f;
  metrics.information_capacity = 0.0f;
  metrics.active_dimers = 0;
  metrics.quantum_effects_ratio = 0.0f;

  auto mt = get_microtubule(id);
  if (!mt || mt->dimers.empty())
    return metrics;

  float sum_coh = 0.0f;
  float sum_alpha = 0.0f;
  int quantum_active = 0;

  for (const auto &dimer : mt->dimers) {
    sum_coh += dimer.coherence;
    sum_alpha += dimer.alpha_state;
    if (dimer.coherence > 0.5f) {
      metrics.active_dimers++;
      quantum_active++;
    }
  }

  int n = static_cast<int>(mt->dimers.size());
  metrics.average_coherence = sum_coh / n;
  metrics.collective_oscillation = sum_alpha / n;
  metrics.quantum_effects_ratio = static_cast<float>(quantum_active) / n;

  // Information capacity in bits (2 bits per coherent dimer)
  metrics.information_capacity = 2.0f * metrics.active_dimers;

  return metrics;
}

// ================================================================
// Quantum Effects
// ================================================================

float HardwareSimulator::calculate_network_coherence() const {
  if (microtubules.empty())
    return 0.0f;

  float sum = 0.0f;
  int count = 0;

  for (const auto &mt : microtubules) {
    for (const auto &dimer : mt.dimers) {
      sum += dimer.coherence;
      count++;
    }
  }

  return (count > 0) ? sum / count : 0.0f;
}

bool HardwareSimulator::simulate_orchestrated_reduction(
    const std::string &mt_id) {
  auto mt = get_microtubule(mt_id);
  if (!mt)
    return false;

  // Collapse superpositions to definite states
  for (auto &dimer : mt->dimers) {
    if (dimer.coherence > 0.5f) {
      // Collapse to one of the states
      dimer.alpha_state = (dimer.alpha_state > 0.5f) ? 1.0f : 0.0f;
      dimer.beta_state = (dimer.beta_state > 0.5f) ? 1.0f : 0.0f;
      dimer.coherence = 0.1f; // Reset coherence after collapse
    }
  }

  return true;
}

std::vector<float> HardwareSimulator::get_quantum_state() const {
  std::vector<float> state;
  for (const auto &mt : microtubules) {
    for (const auto &dimer : mt.dimers) {
      state.push_back(dimer.alpha_state);
      state.push_back(dimer.beta_state);
      state.push_back(dimer.coherence);
    }
  }
  return state;
}

// ================================================================
// Time Crystal Integration
// ================================================================

void HardwareSimulator::couple_to_time_crystal() {
  if (!tc_kernel)
    return;

  for (auto &mt : microtubules) {
    // Get TC coherence for this MT's prime resonances
    float tc_coherence = tc_kernel->compute_ppm_coherence(mt.prime_resonances);

    // Transfer coherence to dimers
    for (auto &dimer : mt.dimers) {
      dimer.coherence = dimer.coherence * 0.9f + tc_coherence * 0.1f;
    }
  }
}

void HardwareSimulator::transfer_coherence(float amount) {
  for (auto &mt : microtubules) {
    for (auto &dimer : mt.dimers) {
      dimer.coherence = std::min(1.0f, dimer.coherence + amount);
    }
  }
}

// ================================================================
// Simulation Control
// ================================================================

void HardwareSimulator::simulate(float duration_ms, float timestep_ms) {
  float dt = timestep_ms / 1000.0f; // Convert to seconds
  int steps = static_cast<int>(duration_ms / timestep_ms);

  for (int i = 0; i < steps; i++) {
    update_thermal(dt);
    update_microtubules(dt);
    simulation_time += dt;
  }
}

// ================================================================
// Statistics
// ================================================================

size_t HardwareSimulator::total_dimers() const {
  size_t total = 0;
  for (const auto &mt : microtubules) {
    total += mt.dimers.size();
  }
  return total;
}

float HardwareSimulator::average_coherence() const {
  return calculate_network_coherence();
}

// ================================================================
// OrchORModel Implementation
// ================================================================

OrchORModel::OrchORModel(HardwareSimulator *hw) : hw_sim(hw) {}

void OrchORModel::initialize() { event_history.clear(); }

float OrchORModel::calculate_collapse_time(int num_tubulins) const {
  // Penrose formula: τ ≈ ℏ / E_G
  // where E_G is gravitational self-energy
  float mass = num_tubulins * TUBULIN_MASS_KG;
  float size = 25.0e-9f; // ~25nm microtubule diameter
  float G = 6.674e-11f;  // Gravitational constant

  // Gravitational self-energy
  float E_G = G * mass * mass / size;

  // Planck's constant
  float hbar = 1.054e-34f;

  return hbar / E_G; // Returns time in seconds
}

bool OrchORModel::should_collapse(float superposition_mass) const {
  float collapse_time = calculate_collapse_time(
      static_cast<int>(superposition_mass / TUBULIN_MASS_KG));

  // Compare with current simulation time
  return hw_sim && (hw_sim->get_simulation_time() > collapse_time);
}

OrchOREvent OrchORModel::orchestrated_reduction(const std::string &mt_id) {
  OrchOREvent event;
  event.timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
                        std::chrono::system_clock::now().time_since_epoch())
                        .count();
  event.microtubule_id = mt_id;

  if (!hw_sim) {
    event.num_tubulins_collapsed = 0;
    event.coherence_before = 0;
    event.coherence_after = 0;
    event.information_integrated = 0;
    event.conscious_moment = false;
    return event;
  }

  auto mt = hw_sim->get_microtubule(mt_id);
  if (!mt) {
    event.num_tubulins_collapsed = 0;
    return event;
  }

  // Calculate before state
  float coherence_sum = 0.0f;
  int coherent_count = 0;
  for (const auto &dimer : mt->dimers) {
    if (dimer.coherence > 0.5f) {
      coherence_sum += dimer.coherence;
      coherent_count++;
    }
  }
  event.coherence_before =
      (coherent_count > 0) ? coherence_sum / coherent_count : 0.0f;
  event.num_tubulins_collapsed = coherent_count;

  // Perform reduction
  hw_sim->simulate_orchestrated_reduction(mt_id);

  // Calculate after state
  coherence_sum = 0.0f;
  int new_coherent = 0;
  for (const auto &dimer : mt->dimers) {
    if (dimer.coherence > 0.5f) {
      coherence_sum += dimer.coherence;
      new_coherent++;
    }
  }
  event.coherence_after =
      (new_coherent > 0) ? coherence_sum / new_coherent : 0.0f;

  // Information integrated (bits)
  event.information_integrated =
      2.0f * event.num_tubulins_collapsed * event.coherence_before;

  // Conscious moment if Φ exceeds threshold
  event.conscious_moment = calculate_phi() > PHI_THRESHOLD;

  event_history.push_back(event);
  return event;
}

float OrchORModel::calculate_phi() const {
  if (!hw_sim)
    return 0.0f;

  // Simplified IIT Φ using network coherence as proxy
  float coherence = hw_sim->calculate_network_coherence();
  size_t dimers = hw_sim->total_dimers();

  // Φ scales with coherence and logarithm of network size
  return coherence * std::log2(static_cast<float>(dimers) + 1);
}

std::vector<OrchOREvent> OrchORModel::get_event_history() const {
  return event_history;
}
