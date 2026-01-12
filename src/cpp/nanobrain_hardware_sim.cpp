#include "nanobrain_hardware_sim.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <random>
#include <sstream>

// ================================================================
// Constructor / Destructor
// ================================================================

HardwareSimulator::HardwareSimulator(NanoBrainKernel *kernel,
                                     const HardwareSimConfig &config)
    : kernel(kernel), config(config), active(false), cycle_count(0),
      mt_counter(0) {}

HardwareSimulator::~HardwareSimulator() { shutdown(); }

std::string HardwareSimulator::generate_mt_id() {
  std::stringstream ss;
  ss << "mt_" << mt_counter++;
  return ss.str();
}

// ================================================================
// Initialization
// ================================================================

void HardwareSimulator::initialize() {
  if (active)
    return;

  // Initialize thermal state
  thermal_state.temperature = config.base_temperature;
  thermal_state.breathing_phase = config.breathing_phase_offset;
  thermal_state.energy_level = 1.0f;
  thermal_state.entropy = 0.5f;
  thermal_state.thermal_conductivity = 0.1f;
  thermal_state.in_coherent_regime = true;

  // Create initial microtubules
  for (int i = 0; i < config.microtubule_count; i++) {
    create_microtubule(config.tubulin_per_tubule);
  }

  // Create resonance links between nearby MTs
  auto mt_ids = get_all_microtubule_ids();
  for (size_t i = 0; i < mt_ids.size(); i++) {
    for (size_t j = i + 1; j < std::min(i + 5, mt_ids.size()); j++) {
      float coupling = 0.2f + (j - i) * 0.1f;
      create_resonance_link(mt_ids[i], mt_ids[j], coupling);
    }
  }

  active = true;
  std::cout << "[HardwareSimulator] Initialized with " << microtubules.size()
            << " microtubules" << std::endl;
}

void HardwareSimulator::shutdown() {
  active = false;
  microtubules.clear();
  resonance_links.clear();
  std::cout << "[HardwareSimulator] Shutdown after " << cycle_count << " cycles"
            << std::endl;
}

// ================================================================
// Thermal Breathing Model
// ================================================================

void HardwareSimulator::update_thermal_breathing(float delta_ms) {
  // Update breathing phase
  float phase_increment = (2.0f * PI * delta_ms) / BREATHING_PERIOD_MS;
  thermal_state.breathing_phase += phase_increment;
  while (thermal_state.breathing_phase >= 2.0f * PI) {
    thermal_state.breathing_phase -= 2.0f * PI;
  }

  // Temperature modulation with breathing
  float breath_modulation = std::sin(thermal_state.breathing_phase);
  thermal_state.temperature =
      config.base_temperature + breath_modulation * config.temperature_variance;

  // Add thermal noise if enabled
  if (config.enable_thermal_noise) {
    static std::mt19937 rng(std::random_device{}());
    static std::normal_distribution<float> noise(0.0f, 0.1f);
    thermal_state.temperature +=
        noise(rng) * config.temperature_variance * 0.1f;
  }

  // Energy follows breathing cycle
  thermal_state.energy_level = 0.8f + 0.2f * breath_modulation;

  // Entropy increases with temperature variation
  thermal_state.entropy = 0.5f + 0.2f * std::abs(breath_modulation);

  // Check coherent regime (temperature dependent)
  thermal_state.in_coherent_regime =
      (thermal_state.temperature >= config.base_temperature - 5.0f &&
       thermal_state.temperature <= config.base_temperature + 5.0f);
}

void HardwareSimulator::set_temperature(float temp_kelvin) {
  thermal_state.temperature = temp_kelvin;
}

void HardwareSimulator::add_thermal_perturbation(float delta_temp) {
  thermal_state.temperature += delta_temp;
  thermal_state.entropy += std::abs(delta_temp) * 0.01f;
}

bool HardwareSimulator::in_coherent_regime() const {
  return thermal_state.in_coherent_regime;
}

// ================================================================
// Microtubule Dynamics
// ================================================================

std::string HardwareSimulator::create_microtubule(int tubulin_count) {
  MicrotubuleDynamics mt;
  mt.id = generate_mt_id();
  mt.coherence = 0.5f;
  mt.resonance_frequency = 8.0f + (mt_counter % 10); // 8-18 MHz range
  mt.collective_dipole = 0.0f;
  mt.polymerizing = true;

  // Initialize tubulin dimers
  for (int i = 0; i < tubulin_count; i++) {
    mt.tubulin_states.push_back(create_tubulin(i));
  }

  // Initialize protofilament phases
  for (int pf = 0; pf < PROTOFILAMENTS_PER_TUBULE; pf++) {
    mt.protofilament_phases[pf] = (2.0f * PI * pf) / PROTOFILAMENTS_PER_TUBULE;
  }

  microtubules[mt.id] = mt;
  return mt.id;
}

TubulinState HardwareSimulator::create_tubulin(int index) {
  static std::mt19937 rng(std::random_device{}());
  static std::uniform_real_distribution<float> dist(0.0f, 1.0f);

  TubulinState tubulin;
  tubulin.index = index;
  tubulin.alpha_state = dist(rng);
  tubulin.beta_state = dist(rng);
  tubulin.dipole_moment = (tubulin.alpha_state - tubulin.beta_state);
  tubulin.gtp_bound = (dist(rng) > 0.3f); // 70% GTP, 30% GDP

  return tubulin;
}

const MicrotubuleDynamics *
HardwareSimulator::get_microtubule(const std::string &id) const {
  auto it = microtubules.find(id);
  if (it != microtubules.end()) {
    return &it->second;
  }
  return nullptr;
}

void HardwareSimulator::update_microtubule_dynamics(float delta_ms) {
  for (auto &[id, mt] : microtubules) {
    // Update each tubulin
    for (auto &tubulin : mt.tubulin_states) {
      update_tubulin(tubulin, delta_ms);
    }

    // Update protofilament phases
    update_protofilament_phases(mt, delta_ms);

    // Calculate collective dipole
    mt.collective_dipole = compute_collective_dipole(mt);

    // Calculate coherence
    mt.coherence = calculate_mt_coherence(mt);

    // Apply thermal noise
    if (config.enable_thermal_noise) {
      apply_thermal_noise(mt);
    }

    // Apply quantum effects
    if (config.enable_quantum_effects && thermal_state.in_coherent_regime) {
      apply_quantum_effects(mt);
    }
  }
}

std::vector<std::string> HardwareSimulator::get_all_microtubule_ids() const {
  std::vector<std::string> ids;
  ids.reserve(microtubules.size());
  for (const auto &[id, _] : microtubules) {
    ids.push_back(id);
  }
  return ids;
}

float HardwareSimulator::calculate_mt_coherence(const MicrotubuleDynamics &mt) {
  if (mt.tubulin_states.empty()) {
    return 0.0f;
  }

  // Measure alignment of dipole moments
  float sum_dipole = 0.0f;
  float sum_abs_dipole = 0.0f;

  for (const auto &tubulin : mt.tubulin_states) {
    sum_dipole += tubulin.dipole_moment;
    sum_abs_dipole += std::abs(tubulin.dipole_moment);
  }

  if (sum_abs_dipole < 0.001f) {
    return 0.5f;
  }

  // Coherence = alignment ratio
  return std::abs(sum_dipole) / sum_abs_dipole;
}

void HardwareSimulator::set_polymerization_state(const std::string &mt_id,
                                                 bool polymerizing) {
  auto it = microtubules.find(mt_id);
  if (it != microtubules.end()) {
    it->second.polymerizing = polymerizing;
  }
}

void HardwareSimulator::update_tubulin(TubulinState &tubulin, float delta_ms) {
  static std::mt19937 rng(std::random_device{}());
  static std::uniform_real_distribution<float> dist(-0.01f, 0.01f);

  // Small random state changes
  tubulin.alpha_state += dist(rng) * delta_ms / 10.0f;
  tubulin.beta_state += dist(rng) * delta_ms / 10.0f;

  // Clamp to [0, 1]
  tubulin.alpha_state = std::max(0.0f, std::min(1.0f, tubulin.alpha_state));
  tubulin.beta_state = std::max(0.0f, std::min(1.0f, tubulin.beta_state));

  // Update dipole
  tubulin.dipole_moment = tubulin.alpha_state - tubulin.beta_state;

  // GTP hydrolysis (slow process)
  if (tubulin.gtp_bound && dist(rng) + 0.01f > 0.009f * delta_ms / 10.0f) {
    // 1% chance per 100ms to hydrolyze
    tubulin.gtp_bound = false;
  }
}

void HardwareSimulator::update_protofilament_phases(MicrotubuleDynamics &mt,
                                                    float delta_ms) {
  float phase_increment =
      (2.0f * PI * mt.resonance_frequency * delta_ms) / 1000.0f;

  for (int pf = 0; pf < PROTOFILAMENTS_PER_TUBULE; pf++) {
    mt.protofilament_phases[pf] += phase_increment;
    while (mt.protofilament_phases[pf] >= 2.0f * PI) {
      mt.protofilament_phases[pf] -= 2.0f * PI;
    }
  }
}

float HardwareSimulator::compute_collective_dipole(
    const MicrotubuleDynamics &mt) {
  float sum = 0.0f;
  for (const auto &tubulin : mt.tubulin_states) {
    sum += tubulin.dipole_moment;
  }
  return sum;
}

void HardwareSimulator::apply_thermal_noise(MicrotubuleDynamics &mt) {
  static std::mt19937 rng(std::random_device{}());

  float noise_amplitude =
      (thermal_state.temperature - BODY_TEMPERATURE) * 0.01f;
  std::normal_distribution<float> noise(0.0f, std::abs(noise_amplitude));

  for (auto &tubulin : mt.tubulin_states) {
    tubulin.dipole_moment += noise(rng);
  }
}

void HardwareSimulator::apply_quantum_effects(MicrotubuleDynamics &mt) {
  // Quantum coherence enhances synchronization
  float avg_phase = 0.0f;
  for (int pf = 0; pf < PROTOFILAMENTS_PER_TUBULE; pf++) {
    avg_phase += mt.protofilament_phases[pf];
  }
  avg_phase /= PROTOFILAMENTS_PER_TUBULE;

  // Pull phases toward average (quantum entanglement effect)
  for (int pf = 0; pf < PROTOFILAMENTS_PER_TUBULE; pf++) {
    float diff = avg_phase - mt.protofilament_phases[pf];
    mt.protofilament_phases[pf] += diff * 0.1f; // 10% pull toward coherence
  }
}

// ================================================================
// Resonance Network
// ================================================================

void HardwareSimulator::create_resonance_link(const std::string &source_id,
                                              const std::string &target_id,
                                              float coupling_strength) {
  ResonanceLink link;
  link.source_mt = source_id;
  link.target_mt = target_id;
  link.coupling_strength = coupling_strength;
  link.phase_difference = 0.0f;
  link.synchronized = false;

  resonance_links.push_back(link);
}

void HardwareSimulator::update_resonance_network(float delta_ms) {
  for (auto &link : resonance_links) {
    auto *source = get_microtubule(link.source_mt);
    auto *target = get_microtubule(link.target_mt);

    if (!source || !target)
      continue;

    // Calculate phase difference from protofilament 0
    link.phase_difference = std::abs(source->protofilament_phases[0] -
                                     target->protofilament_phases[0]);

    // Synchronized if phase difference is small
    link.synchronized = (link.phase_difference < 0.5f);

    // Coupling affects coherence
    if (link.synchronized) {
      auto *source_mt = const_cast<MicrotubuleDynamics *>(source);
      auto *target_mt = const_cast<MicrotubuleDynamics *>(target);

      // Boost coherence of coupled MTs
      source_mt->coherence += link.coupling_strength * 0.01f;
      target_mt->coherence += link.coupling_strength * 0.01f;

      source_mt->coherence = std::min(1.0f, source_mt->coherence);
      target_mt->coherence = std::min(1.0f, target_mt->coherence);
    }
  }
}

std::vector<std::vector<std::string>>
HardwareSimulator::find_synchronized_clusters() {
  std::vector<std::vector<std::string>> clusters;

  std::map<std::string, int> mt_to_cluster;
  int cluster_id = 0;

  for (const auto &link : resonance_links) {
    if (!link.synchronized)
      continue;

    auto it_source = mt_to_cluster.find(link.source_mt);
    auto it_target = mt_to_cluster.find(link.target_mt);

    if (it_source == mt_to_cluster.end() && it_target == mt_to_cluster.end()) {
      // Neither in a cluster, create new
      mt_to_cluster[link.source_mt] = cluster_id;
      mt_to_cluster[link.target_mt] = cluster_id;
      cluster_id++;
    } else if (it_source != mt_to_cluster.end() &&
               it_target == mt_to_cluster.end()) {
      // Source has cluster, add target
      mt_to_cluster[link.target_mt] = it_source->second;
    } else if (it_source == mt_to_cluster.end() &&
               it_target != mt_to_cluster.end()) {
      // Target has cluster, add source
      mt_to_cluster[link.source_mt] = it_target->second;
    }
    // Both already clustered - could merge, but skip for simplicity
  }

  // Build cluster list
  std::map<int, std::vector<std::string>> cluster_map;
  for (const auto &[mt_id, cid] : mt_to_cluster) {
    cluster_map[cid].push_back(mt_id);
  }

  for (const auto &[cid, members] : cluster_map) {
    clusters.push_back(members);
  }

  return clusters;
}

std::vector<ResonanceLink>
HardwareSimulator::get_resonance_links(const std::string &mt_id) const {
  std::vector<ResonanceLink> result;
  for (const auto &link : resonance_links) {
    if (link.source_mt == mt_id || link.target_mt == mt_id) {
      result.push_back(link);
    }
  }
  return result;
}

// ================================================================
// Information Processing
// ================================================================

void HardwareSimulator::inject_pattern(NanoBrainTensor *pattern,
                                       const std::vector<std::string> &mt_ids) {
  if (!pattern || !pattern->ggml_tensor)
    return;

  float *data = static_cast<float *>(pattern->ggml_tensor->data);
  int64_t size = ggml_nelements(pattern->ggml_tensor);

  for (const auto &mt_id : mt_ids) {
    auto it = microtubules.find(mt_id);
    if (it == microtubules.end())
      continue;

    auto &mt = it->second;

    // Inject pattern into tubulin dipoles
    for (size_t i = 0; i < mt.tubulin_states.size() && i < (size_t)size; i++) {
      mt.tubulin_states[i].dipole_moment += data[i] * 0.1f;
    }
  }
}

NanoBrainTensor *HardwareSimulator::read_collective_state() {
  std::vector<float> collective_state;
  collective_state.reserve(microtubules.size());

  for (const auto &[id, mt] : microtubules) {
    collective_state.push_back(mt.collective_dipole * mt.coherence);
  }

  auto *tensor = kernel->create_tensor({(int64_t)collective_state.size()});
  kernel->set_data(tensor, collective_state);

  return tensor;
}

void HardwareSimulator::process_step() {
  // One processing step: propagate information through resonance
  for (const auto &link : resonance_links) {
    if (!link.synchronized)
      continue;

    auto it_source = microtubules.find(link.source_mt);
    auto it_target = microtubules.find(link.target_mt);

    if (it_source == microtubules.end() || it_target == microtubules.end())
      continue;

    // Transfer dipole information
    float transfer_amount =
        it_source->second.collective_dipole * link.coupling_strength * 0.1f;
    it_target->second.collective_dipole += transfer_amount;
  }
}

// ================================================================
// Full Simulation Cycle
// ================================================================

void HardwareSimulator::run_cycle(float delta_ms) {
  if (!active)
    return;

  update_thermal_breathing(delta_ms);
  update_microtubule_dynamics(delta_ms);
  update_resonance_network(delta_ms);
  process_step();

  cycle_count++;
}

void HardwareSimulator::run_cycles(int count, float delta_ms) {
  for (int i = 0; i < count; i++) {
    run_cycle(delta_ms);
  }
}

// ================================================================
// Metrics
// ================================================================

HardwareMetrics HardwareSimulator::get_metrics() const {
  HardwareMetrics metrics{};

  metrics.average_temperature = thermal_state.temperature;

  float total_coherence = 0.0f;
  float total_dipole = 0.0f;
  for (const auto &[id, mt] : microtubules) {
    total_coherence += mt.coherence;
    total_dipole += std::abs(mt.collective_dipole);
  }

  if (!microtubules.empty()) {
    metrics.average_mt_coherence = total_coherence / microtubules.size();
  }
  metrics.total_dipole_moment = total_dipole;

  // Count synchronized pairs
  for (const auto &link : resonance_links) {
    if (link.synchronized) {
      metrics.synchronized_mt_pairs++;
    }
  }

  // Estimate processing capacity (arbitrary)
  metrics.processing_capacity =
      metrics.average_mt_coherence * microtubules.size() * 1e6f;

  // Energy consumption
  metrics.energy_consumption = thermal_state.energy_level * microtubules.size();

  return metrics;
}
