#include "nanobrain_brain_jelly.h"
#include <algorithm>
#include <chrono>
#include <cmath>
#include <iostream>
#include <numeric>
#include <random>
#include <sstream>

// ================================================================
// Bio-Morphic Device Registry Implementation
// ================================================================

BioMorphicDeviceRegistry::BioMorphicDeviceRegistry(
    NanoBrainKernel *kernel, const BioMorphicRegistryConfig &config)
    : kernel(kernel), config(config), device_counter(0) {}

BioMorphicDeviceRegistry::~BioMorphicDeviceRegistry() { devices.clear(); }

std::string BioMorphicDeviceRegistry::create_device(BioMorphicDeviceType type,
                                                    const std::string &name) {
  std::string id =
      name.empty() ? "device_" + std::to_string(device_counter++) : name;

  auto device = std::make_unique<BioMorphicDevice>();
  device->id = id;
  device->type = type;
  device->activation_level = 0.0f;
  device->threshold = config.default_threshold;
  device->refractory_period = 2.0f; // 2ms refractory
  device->last_fire_time = -1000.0f;
  device->ppm_coherence = 1.0f;

  // Initialize with default prime encoding based on device type
  int type_index = static_cast<int>(type);
  device->prime_encoding = {2, 3, 5, 7, 11};
  device->prime_encoding.push_back(13 + type_index * 2);

  // Create state tensor
  device->state_tensor = kernel->create_tensor(4); // 4D state

  devices[id] = std::move(device);
  return id;
}

BioMorphicDevice *BioMorphicDeviceRegistry::get_device(const std::string &id) {
  auto it = devices.find(id);
  return it != devices.end() ? it->second.get() : nullptr;
}

const BioMorphicDevice *
BioMorphicDeviceRegistry::get_device(const std::string &id) const {
  auto it = devices.find(id);
  return it != devices.end() ? it->second.get() : nullptr;
}

bool BioMorphicDeviceRegistry::remove_device(const std::string &id) {
  return devices.erase(id) > 0;
}

std::vector<std::string> BioMorphicDeviceRegistry::get_all_device_ids() const {
  std::vector<std::string> ids;
  ids.reserve(devices.size());
  for (const auto &pair : devices) {
    ids.push_back(pair.first);
  }
  return ids;
}

bool BioMorphicDeviceRegistry::connect_devices(const std::string &source_id,
                                               const std::string &target_id,
                                               float weight) {
  auto *source = get_device(source_id);
  auto *target = get_device(target_id);

  if (!source || !target)
    return false;

  source->output_connections.push_back(target_id);
  target->input_connections.push_back(source_id);
  target->connection_weights.push_back(weight);

  return true;
}

bool BioMorphicDeviceRegistry::disconnect_devices(
    const std::string &source_id, const std::string &target_id) {
  auto *source = get_device(source_id);
  auto *target = get_device(target_id);

  if (!source || !target)
    return false;

  // Remove from source outputs
  auto it = std::find(source->output_connections.begin(),
                      source->output_connections.end(), target_id);
  if (it != source->output_connections.end()) {
    source->output_connections.erase(it);
  }

  // Remove from target inputs
  for (size_t i = 0; i < target->input_connections.size(); ++i) {
    if (target->input_connections[i] == source_id) {
      target->input_connections.erase(target->input_connections.begin() + i);
      if (i < target->connection_weights.size()) {
        target->connection_weights.erase(target->connection_weights.begin() +
                                         i);
      }
      break;
    }
  }

  return true;
}

void BioMorphicDeviceRegistry::update_all_devices(float delta_time) {
  for (auto &pair : devices) {
    update_device(*pair.second, delta_time);
  }
}

void BioMorphicDeviceRegistry::update_device(BioMorphicDevice &device,
                                             float delta_time) {
  // Compute input from connected devices
  float input_sum = compute_input_sum(device);

  // Update activation with leaky integration
  float decay = 0.1f * delta_time;
  device.activation_level = device.activation_level * (1.0f - decay) +
                            input_sum * config.communication_rate;

  // Clamp activation
  device.activation_level =
      std::max(0.0f, std::min(1.0f, device.activation_level));

  // Check for firing
  float current_time = delta_time; // Simplified time tracking
  if (device.activation_level >= device.threshold &&
      (current_time - device.last_fire_time) >= device.refractory_period) {
    fire_device(device, current_time);
  }
}

float BioMorphicDeviceRegistry::compute_input_sum(
    const BioMorphicDevice &device) {
  float sum = 0.0f;
  for (size_t i = 0; i < device.input_connections.size(); ++i) {
    auto *input_device = get_device(device.input_connections[i]);
    if (input_device) {
      float weight = i < device.connection_weights.size()
                         ? device.connection_weights[i]
                         : 1.0f;
      sum += input_device->activation_level * weight;
    }
  }
  return sum;
}

void BioMorphicDeviceRegistry::fire_device(BioMorphicDevice &device,
                                           float current_time) {
  device.last_fire_time = current_time;
  device.activation_level = 0.0f; // Reset after firing

  // Propagate to outputs
  for (const auto &output_id : device.output_connections) {
    auto *output_device = get_device(output_id);
    if (output_device) {
      output_device->activation_level += 0.5f; // Spike propagation
    }
  }
}

void BioMorphicDeviceRegistry::propagate_signals() {
  // Two-phase update to avoid order dependency
  std::vector<std::pair<std::string, float>> updates;

  for (const auto &pair : devices) {
    const auto &device = pair.second;
    if (device->activation_level >= device->threshold) {
      for (const auto &output_id : device->output_connections) {
        updates.emplace_back(output_id, 0.3f);
      }
    }
  }

  for (const auto &update : updates) {
    auto *device = get_device(update.first);
    if (device) {
      device->activation_level += update.second;
      device->activation_level = std::min(1.0f, device->activation_level);
    }
  }
}

CollectiveResponse BioMorphicDeviceRegistry::compute_collective_response() {
  CollectiveResponse response;

  if (devices.empty()) {
    response.synchronization_index = 0.0f;
    response.collective_activation = 0.0f;
    response.emergence_level = 0.0f;
    return response;
  }

  // Compute collective activation
  float total_activation = 0.0f;
  float total_variance = 0.0f;

  for (const auto &pair : devices) {
    total_activation += pair.second->activation_level;
  }

  float mean_activation = total_activation / devices.size();
  response.collective_activation = mean_activation;

  // Compute synchronization (variance-based)
  for (const auto &pair : devices) {
    float diff = pair.second->activation_level - mean_activation;
    total_variance += diff * diff;
  }

  float variance = total_variance / devices.size();
  response.synchronization_index = 1.0f / (1.0f + variance);

  // Compute emergence level (based on synchronization and activation)
  response.emergence_level =
      response.synchronization_index * response.collective_activation;

  // Compute harmonic modes (simplified)
  response.harmonic_modes = {10.0f, 20.0f, 40.0f, 80.0f}; // Alpha multiples

  return response;
}

std::vector<std::string>
BioMorphicDeviceRegistry::get_devices_by_type(BioMorphicDeviceType type) const {
  std::vector<std::string> result;
  for (const auto &pair : devices) {
    if (pair.second->type == type) {
      result.push_back(pair.first);
    }
  }
  return result;
}

int BioMorphicDeviceRegistry::count_device_type(
    BioMorphicDeviceType type) const {
  int count = 0;
  for (const auto &pair : devices) {
    if (pair.second->type == type) {
      count++;
    }
  }
  return count;
}

void BioMorphicDeviceRegistry::initialize_full_registry() {
  // Create one device of each of the 17 types
  for (int i = 0; i < 17; ++i) {
    auto type = static_cast<BioMorphicDeviceType>(i);
    create_device(type, get_device_type_name(type));
  }

  // Create some basic connections between devices
  connect_devices("Neuristor", "Synapstor", 0.8f);
  connect_devices("Synapstor", "Denristor", 0.9f);
  connect_devices("Denristor", "Axonistor", 0.7f);
  connect_devices("Astrocystor", "Neuristor", 0.3f);
  connect_devices("Pyramidistor", "Basketistor", 0.5f);
}

// ================================================================
// Fractal Condensation Implementation
// ================================================================

FractalCondensation::FractalCondensation(
    NanoBrainKernel *kernel, const FractalCondensationConfig &config)
    : kernel(kernel), config(config) {
  current_pattern = {2, 3, 5, 7, 11, 13, 17};
  field.field_tensor = nullptr;
  field.total_condensation = 0.0f;
  field.field_coherence = 1.0f;
}

FractalCondensation::~FractalCondensation() { clear_points(); }

void FractalCondensation::condense_everywhere() {
  if (!config.enable_everywhere)
    return;

  clear_points();

  // Sample 11D space at grid points
  float step = config.spatial_resolution * 10.0f;
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<float> dist(-1.0f, 1.0f);

  for (int i = 0; i < config.max_condensation_points; ++i) {
    std::array<float, 11> seed_position;
    for (int d = 0; d < 11; ++d) {
      seed_position[d] = dist(gen);
    }

    CondensationPoint point = find_condensation_point(seed_position);
    if (point.condensation_strength > config.prime_threshold) {
      active_points.push_back(point);
    }
  }

  // Apply prime pattern to all points
  apply_prime_pattern();
}

void FractalCondensation::apply_prime_pattern() {
  for (auto &point : active_points) {
    apply_prime_at_point(point);
  }
}

std::vector<CondensationPoint> FractalCondensation::get_points() const {
  return active_points;
}

void FractalCondensation::add_condensation_point(
    const CondensationPoint &point) {
  if (active_points.size() <
      static_cast<size_t>(config.max_condensation_points)) {
    active_points.push_back(point);
  }
}

void FractalCondensation::clear_points() {
  active_points.clear();
  field.active_points.clear();
  field.total_condensation = 0.0f;
}

CondensationField FractalCondensation::compute_field() {
  field.active_points = active_points;

  float total = 0.0f;
  for (const auto &point : active_points) {
    total += point.condensation_strength;
  }
  field.total_condensation = total;

  // Compute field coherence
  if (active_points.size() >= 2) {
    float coherence_sum = 0.0f;
    for (size_t i = 0; i < active_points.size() - 1; ++i) {
      for (size_t j = i + 1; j < active_points.size(); ++j) {
        // Compute phase alignment
        float phase_diff = std::abs(active_points[i].temporal_phase -
                                    active_points[j].temporal_phase);
        coherence_sum += std::cos(phase_diff);
      }
    }
    int pair_count =
        static_cast<int>(active_points.size() * (active_points.size() - 1) / 2);
    field.field_coherence = (coherence_sum / pair_count + 1.0f) / 2.0f;
  } else {
    field.field_coherence = 1.0f;
  }

  return field;
}

float FractalCondensation::get_field_value(
    const std::array<float, 11> &position) {
  float value = 0.0f;

  for (const auto &point : active_points) {
    // Compute distance in 11D
    float distance_sq = 0.0f;
    for (int d = 0; d < 11; ++d) {
      float diff = position[d] - point.position[d];
      distance_sq += diff * diff;
    }
    float distance = std::sqrt(distance_sq);

    // Inverse square falloff
    value += point.condensation_strength / (1.0f + distance * distance);
  }

  return value;
}

void FractalCondensation::set_prime_pattern(const std::vector<int> &primes) {
  current_pattern = primes;
}

float FractalCondensation::compute_pattern_coherence() const {
  if (active_points.empty())
    return 0.0f;

  float total_coherence = 0.0f;
  for (const auto &point : active_points) {
    // Compute coherence based on prime pattern overlap
    int overlap = 0;
    for (int p : point.prime_pattern) {
      if (std::find(current_pattern.begin(), current_pattern.end(), p) !=
          current_pattern.end()) {
        overlap++;
      }
    }
    float point_coherence =
        static_cast<float>(overlap) /
        std::max(point.prime_pattern.size(), current_pattern.size());
    total_coherence += point_coherence;
  }

  return total_coherence / active_points.size();
}

CondensationPoint FractalCondensation::find_condensation_point(
    const std::array<float, 11> &seed_position) {
  CondensationPoint point;
  point.position = seed_position;
  point.condensation_order = config.condensation_order;

  // Compute local density
  float density = compute_local_density(seed_position);
  point.condensation_strength = std::min(1.0f, density);

  // Set resonance frequency based on position
  float pos_sum = 0.0f;
  for (int d = 0; d < 11; ++d) {
    pos_sum += std::abs(seed_position[d]);
  }
  point.resonance_frequency = 10.0f + pos_sum * 5.0f; // 10-60 Hz

  // Compute temporal phase
  point.temporal_phase = std::fmod(pos_sum * 2.0f * 3.14159f, 2.0f * 3.14159f);

  // Initialize prime pattern
  point.prime_pattern = current_pattern;

  return point;
}

float FractalCondensation::compute_local_density(
    const std::array<float, 11> &position) {
  // Fractal-inspired density function
  float density = 0.0f;

  for (int d = 0; d < 11; ++d) {
    float x = position[d];
    // Use prime-based harmonics
    for (size_t p = 0; p < current_pattern.size(); ++p) {
      density += std::sin(x * current_pattern[p]) / current_pattern[p];
    }
  }

  return std::abs(density) / 11.0f;
}

void FractalCondensation::apply_prime_at_point(CondensationPoint &point) {
  // Modulate condensation strength by prime coherence
  float coherence = 0.0f;
  for (int p : current_pattern) {
    for (int d = 0; d < 11; ++d) {
      coherence += std::cos(point.position[d] * p);
    }
  }
  coherence = std::abs(coherence) / (11.0f * current_pattern.size());

  point.condensation_strength *= coherence;
  point.prime_pattern = current_pattern;
}

// ================================================================
// Brain Jelly Simulator Implementation
// ================================================================

BrainJellySimulator::BrainJellySimulator(NanoBrainKernel *kernel,
                                         const BrainJellyConfig &config)
    : kernel(kernel), config(config), current_time(0.0f) {
  generate_megamer_chain();
}

BrainJellySimulator::~BrainJellySimulator() { megamer_chain.clear(); }

void BrainJellySimulator::generate_megamer_chain() {
  megamer_chain.clear();
  megamer_chain.reserve(config.megamer_chain_length);

  for (int i = 0; i < config.megamer_chain_length; ++i) {
    MegamerUnit unit;
    unit.id = "megamer_" + std::to_string(i);
    unit.chain_position = i;
    unit.resonance_amplitude = 1.0f;
    unit.phase_offset =
        static_cast<float>(i) * 2.0f * 3.14159f / config.megamer_chain_length;

    // Prime encoding based on position
    unit.prime_encoding = {2, 3, 5};
    if (i % 2 == 0)
      unit.prime_encoding.push_back(7);
    if (i % 3 == 0)
      unit.prime_encoding.push_back(11);

    // Initialize quantum state
    for (int d = 0; d < 11; ++d) {
      unit.quantum_state[d] = std::sin(static_cast<float>(i + d) * 0.5f);
    }

    megamer_chain.push_back(unit);
  }
}

void BrainJellySimulator::update_chain(float delta_time) {
  current_time += delta_time;

  for (auto &unit : megamer_chain) {
    // Update resonance based on time and position
    float phase = config.base_frequency * 2.0f * 3.14159f * current_time +
                  unit.phase_offset;
    unit.resonance_amplitude = 0.5f + 0.5f * std::sin(phase);

    // Update quantum state
    for (int d = 0; d < 11; ++d) {
      unit.quantum_state[d] = std::sin(phase + d * 0.1f);
    }
  }

  propagate_resonance();
}

EEGSignal BrainJellySimulator::generate_eeg_signal(float timestamp) {
  EEGSignal signal;
  signal.timestamp = timestamp;
  signal.channel_values.resize(config.eeg_channels);

  // Generate EEG for each channel
  for (int ch = 0; ch < config.eeg_channels; ++ch) {
    signal.channel_values[ch] = compute_eeg_sample(ch, timestamp);
  }

  // Compute band powers
  for (int b = 0; b < 5; ++b) {
    signal.band_power[b] =
        compute_band_power(static_cast<EEGBand>(b), timestamp);
  }

  // Find dominant band
  int max_band = 0;
  float max_power = signal.band_power[0];
  for (int b = 1; b < 5; ++b) {
    if (signal.band_power[b] > max_power) {
      max_power = signal.band_power[b];
      max_band = b;
    }
  }
  signal.dominant_band = static_cast<EEGBand>(max_band);

  // Compute consciousness index (alpha/theta ratio)
  signal.consciousness_index =
      signal.band_power[2] / (signal.band_power[1] + 0.01f);

  return signal;
}

std::vector<EEGSignal> BrainJellySimulator::generate_eeg_epoch(float start_time,
                                                               float duration) {
  std::vector<EEGSignal> epoch;
  float sample_interval = 1.0f / config.sample_rate;
  int sample_count = static_cast<int>(duration * config.sample_rate);

  epoch.reserve(sample_count);
  for (int i = 0; i < sample_count; ++i) {
    float t = start_time + i * sample_interval;
    epoch.push_back(generate_eeg_signal(t));
  }

  return epoch;
}

float BrainJellySimulator::get_band_power(EEGBand band) const {
  return compute_band_power(band, current_time);
}

void BrainJellySimulator::write_prime_pattern(const std::vector<int> &primes) {
  if (!config.enable_prime_writing)
    return;

  written_primes = primes;

  // Modulate megamer chain with prime pattern
  for (size_t i = 0; i < megamer_chain.size(); ++i) {
    megamer_chain[i].prime_encoding = primes;

    // Adjust phase based on primes
    float phase_mod = 0.0f;
    for (int p : primes) {
      phase_mod += std::sin(static_cast<float>(i * p) * 0.1f);
    }
    megamer_chain[i].phase_offset += phase_mod * 0.01f;
  }
}

std::vector<int> BrainJellySimulator::read_prime_pattern() const {
  if (!megamer_chain.empty()) {
    return megamer_chain[0].prime_encoding;
  }
  return written_primes;
}

float BrainJellySimulator::get_total_resonance() const {
  float total = 0.0f;
  for (const auto &unit : megamer_chain) {
    total += unit.resonance_amplitude;
  }
  return total / megamer_chain.size();
}

float BrainJellySimulator::get_consciousness_index() const {
  // Based on alpha power and coherence
  float alpha = compute_band_power(EEGBand::Alpha, current_time);
  float resonance = get_total_resonance();
  return alpha * resonance;
}

float BrainJellySimulator::compute_eeg_sample(int channel, float timestamp) {
  float sample = 0.0f;

  // Delta (0.5-4 Hz)
  sample +=
      20.0f * std::sin(2.0f * 3.14159f * 2.0f * timestamp + channel * 0.2f);

  // Theta (4-8 Hz)
  sample +=
      15.0f * std::sin(2.0f * 3.14159f * 6.0f * timestamp + channel * 0.3f);

  // Alpha (8-13 Hz) - base rhythm
  sample +=
      25.0f * std::sin(2.0f * 3.14159f * config.base_frequency * timestamp +
                       channel * 0.1f);

  // Beta (13-30 Hz)
  sample +=
      10.0f * std::sin(2.0f * 3.14159f * 20.0f * timestamp + channel * 0.4f);

  // Gamma (30-100 Hz)
  sample +=
      5.0f * std::sin(2.0f * 3.14159f * 40.0f * timestamp + channel * 0.5f);

  // Add megamer influence
  if (channel < static_cast<int>(megamer_chain.size())) {
    sample += megamer_chain[channel].resonance_amplitude * 10.0f;
  }

  return sample;
}

float BrainJellySimulator::compute_band_power(EEGBand band,
                                              float timestamp) const {
  float center_freq;
  switch (band) {
  case EEGBand::Delta:
    center_freq = 2.0f;
    break;
  case EEGBand::Theta:
    center_freq = 6.0f;
    break;
  case EEGBand::Alpha:
    center_freq = config.base_frequency;
    break;
  case EEGBand::Beta:
    center_freq = 20.0f;
    break;
  case EEGBand::Gamma:
    center_freq = 40.0f;
    break;
  default:
    center_freq = 10.0f;
  }

  // Simplified power estimation
  float phase = 2.0f * 3.14159f * center_freq * timestamp;
  float power = 0.5f + 0.5f * std::cos(phase);

  // Modulate by megamer resonance
  float resonance = get_total_resonance();
  power *= (0.5f + 0.5f * resonance);

  return power;
}

void BrainJellySimulator::propagate_resonance() {
  // Propagate resonance through the chain
  std::vector<float> new_amplitudes(megamer_chain.size());

  for (size_t i = 0; i < megamer_chain.size(); ++i) {
    float sum = megamer_chain[i].resonance_amplitude;
    int count = 1;

    if (i > 0) {
      sum += megamer_chain[i - 1].resonance_amplitude * 0.3f;
      count++;
    }
    if (i < megamer_chain.size() - 1) {
      sum += megamer_chain[i + 1].resonance_amplitude * 0.3f;
      count++;
    }

    new_amplitudes[i] = sum / count;
  }

  for (size_t i = 0; i < megamer_chain.size(); ++i) {
    megamer_chain[i].resonance_amplitude = new_amplitudes[i];
  }
}

// ================================================================
// Cortical Pen Freezer Implementation
// ================================================================

CorticalPenFreezer::CorticalPenFreezer(NanoBrainKernel *kernel,
                                       TimeCrystalKernel *time_crystal,
                                       const CorticalPenConfig &config)
    : kernel(kernel), time_crystal(time_crystal), config(config),
      capsule_counter(0) {}

CorticalPenFreezer::~CorticalPenFreezer() { capsules.clear(); }

std::string CorticalPenFreezer::freeze_dynamics(
    NanoBrainTensor *current_state,
    const std::vector<std::string> &related_atoms) {
  if (!current_state)
    return "";

  std::string id = "capsule_" + std::to_string(capsule_counter++);

  auto capsule = std::make_unique<ProblemCapsule>();
  capsule->id = id;
  capsule->frozen_state = encode_to_time_crystal(current_state);
  capsule->prime_signature = compute_signature(current_state);
  capsule->temporal_anchor = static_cast<float>(capsule_counter);
  capsule->complexity_order = static_cast<int>(related_atoms.size());
  capsule->related_atoms = related_atoms;

  capsules[id] = std::move(capsule);
  return id;
}

ProblemCapsule *
CorticalPenFreezer::create_problem_capsule(const std::string &problem_id) {
  // Create a new capsule for a problem
  auto capsule = std::make_unique<ProblemCapsule>();
  capsule->id = problem_id;
  capsule->frozen_state = kernel->create_tensor(config.time_crystal_resolution);
  capsule->prime_signature = {2, 3, 5, 7, 11};
  capsule->temporal_anchor = static_cast<float>(capsule_counter++);
  capsule->complexity_order = 5;

  ProblemCapsule *ptr = capsule.get();
  capsules[problem_id] = std::move(capsule);
  return ptr;
}

ProblemCapsule *CorticalPenFreezer::get_capsule(const std::string &id) {
  auto it = capsules.find(id);
  return it != capsules.end() ? it->second.get() : nullptr;
}

const ProblemCapsule *
CorticalPenFreezer::get_capsule(const std::string &id) const {
  auto it = capsules.find(id);
  return it != capsules.end() ? it->second.get() : nullptr;
}

bool CorticalPenFreezer::delete_capsule(const std::string &id) {
  return capsules.erase(id) > 0;
}

std::vector<std::string> CorticalPenFreezer::get_all_capsule_ids() const {
  std::vector<std::string> ids;
  ids.reserve(capsules.size());
  for (const auto &pair : capsules) {
    ids.push_back(pair.first);
  }
  return ids;
}

NanoBrainTensor *CorticalPenFreezer::thaw_capsule(const std::string &id) {
  auto *capsule = get_capsule(id);
  if (!capsule)
    return nullptr;

  // Create a copy of the frozen state
  NanoBrainTensor *thawed =
      kernel->create_tensor(config.time_crystal_resolution);

  // In a real implementation, would copy data from frozen_state to thawed

  return thawed;
}

std::vector<int>
CorticalPenFreezer::get_capsule_signature(const std::string &id) const {
  auto *capsule = get_capsule(id);
  if (capsule) {
    return capsule->prime_signature;
  }
  return {};
}

std::vector<int> CorticalPenFreezer::compute_signature(NanoBrainTensor *state) {
  // Compute prime signature from tensor state
  std::vector<int> signature = {2, 3, 5, 7, 11, 13, 17};

  // Would analyze tensor content to determine signature
  // For now, return default primes

  return signature;
}

NanoBrainTensor *
CorticalPenFreezer::encode_to_time_crystal(NanoBrainTensor *dynamics) {
  // Encode dynamic state into time crystal format
  NanoBrainTensor *encoded =
      kernel->create_tensor(config.time_crystal_resolution);

  // Would use time crystal kernel's encoding methods

  return encoded;
}

// ================================================================
// Humanoid Avatar Interface Implementation
// ================================================================

HumanoidAvatarInterface::HumanoidAvatarInterface(
    NanoBrainKernel *kernel, TimeCrystalKernel *time_crystal,
    BrainJellySimulator *brain_jelly, const HumanoidAvatarConfig &config)
    : kernel(kernel), time_crystal(time_crystal), brain_jelly(brain_jelly),
      config(config), neural_state(nullptr) {
  motor_targets.resize(config.motor_count, 0.0f);
  triad_state.motor_outputs.resize(config.motor_count, 0.0f);
  triad_state.jelly_currents.resize(16, 0.0f);
  triad_state.consciousness_level = 0.0f;
  neural_state = kernel->create_tensor(128); // 128D neural state
}

HumanoidAvatarInterface::~HumanoidAvatarInterface() {
  // Tensors cleaned up by kernel
}

void HumanoidAvatarInterface::sense_11d_signals(
    const std::vector<Signal11D> &signals) {
  triad_state.sensory_inputs = signals;
  integrate_sensory_inputs();
}

Signal11D HumanoidAvatarInterface::get_integrated_signal() const {
  return fuse_signals(triad_state.sensory_inputs);
}

void HumanoidAvatarInterface::process_sensor_triad() {
  integrate_sensory_inputs();
  process_jelly_layer();
  compute_motor_outputs();
  update_cognitive_state();
}

std::vector<float> HumanoidAvatarInterface::get_motor_commands() const {
  return triad_state.motor_outputs;
}

void HumanoidAvatarInterface::set_motor_targets(
    const std::vector<float> &targets) {
  motor_targets = targets;
  if (motor_targets.size() != static_cast<size_t>(config.motor_count)) {
    motor_targets.resize(config.motor_count, 0.0f);
  }
}

NanoBrainTensor *HumanoidAvatarInterface::get_neural_output() {
  return neural_state;
}

void HumanoidAvatarInterface::update_neural_state(float delta_time) {
  // Update based on triad state
  process_sensor_triad();

  // Jelly resonance affects neural state
  if (brain_jelly) {
    triad_state.jelly_resonance = brain_jelly->get_total_resonance();
    triad_state.consciousness_level = brain_jelly->get_consciousness_index();
  }
}

AvatarMetrics HumanoidAvatarInterface::get_metrics() const {
  AvatarMetrics metrics;

  metrics.sensory_bandwidth = triad_state.sensory_integration;
  metrics.motor_precision = triad_state.motor_coordination;
  metrics.cognitive_load = 0.5f; // Placeholder
  metrics.embodiment_index = get_embodiment_index();
  metrics.temporal_coherence = 0.8f; // Placeholder

  return metrics;
}

float HumanoidAvatarInterface::get_embodiment_index() const {
  // Combine sensory, motor, and cognitive factors
  float sensory = triad_state.sensory_integration;
  float motor = triad_state.motor_coordination;
  float consciousness = triad_state.consciousness_level;

  return (sensory + motor + consciousness) / 3.0f;
}

void HumanoidAvatarInterface::integrate_sensory_inputs() {
  if (triad_state.sensory_inputs.empty()) {
    triad_state.sensory_integration = 0.0f;
    return;
  }

  // Compute integration metric
  float total_strength = 0.0f;
  for (const auto &signal : triad_state.sensory_inputs) {
    total_strength += signal.signal_strength;
  }

  triad_state.sensory_integration =
      total_strength / triad_state.sensory_inputs.size();
}

void HumanoidAvatarInterface::process_jelly_layer() {
  if (!brain_jelly)
    return;

  triad_state.jelly_resonance = brain_jelly->get_total_resonance();

  // Update jelly currents based on sensory integration
  for (size_t i = 0; i < triad_state.jelly_currents.size(); ++i) {
    triad_state.jelly_currents[i] = triad_state.sensory_integration *
                                    std::sin(static_cast<float>(i) * 0.5f);
  }
}

void HumanoidAvatarInterface::compute_motor_outputs() {
  // Simple motor control based on jelly currents
  float jelly_influence = triad_state.jelly_resonance;

  for (size_t i = 0; i < triad_state.motor_outputs.size(); ++i) {
    float target = i < motor_targets.size() ? motor_targets[i] : 0.0f;

    // Smooth transition towards target
    triad_state.motor_outputs[i] =
        triad_state.motor_outputs[i] * 0.8f + target * jelly_influence * 0.2f;
  }

  // Compute motor coordination
  float variance = 0.0f;
  float mean = 0.0f;
  for (float output : triad_state.motor_outputs) {
    mean += output;
  }
  mean /= triad_state.motor_outputs.size();

  for (float output : triad_state.motor_outputs) {
    variance += (output - mean) * (output - mean);
  }
  variance /= triad_state.motor_outputs.size();

  triad_state.motor_coordination = 1.0f / (1.0f + variance);
}

void HumanoidAvatarInterface::update_cognitive_state() {
  if (brain_jelly) {
    triad_state.consciousness_level = brain_jelly->get_consciousness_index();
  }

  // cognitive_state tensor would be updated here
  triad_state.cognitive_state = neural_state;
}

Signal11D HumanoidAvatarInterface::fuse_signals(
    const std::vector<Signal11D> &signals) const {
  Signal11D fused;
  fused.dimensions.fill(0.0f);
  fused.signal_strength = 0.0f;
  fused.timestamp = 0;
  fused.source_sensor = "fused";

  if (signals.empty())
    return fused;

  // Weighted average fusion
  float total_weight = 0.0f;
  for (const auto &signal : signals) {
    total_weight += signal.signal_strength;

    for (int d = 0; d < 11; ++d) {
      fused.dimensions[d] += signal.dimensions[d] * signal.signal_strength;
    }

    if (signal.timestamp > fused.timestamp) {
      fused.timestamp = signal.timestamp;
    }
  }

  if (total_weight > 0.0f) {
    for (int d = 0; d < 11; ++d) {
      fused.dimensions[d] /= total_weight;
    }
    fused.signal_strength = total_weight / signals.size();
  }

  return fused;
}

// ================================================================
// Integrated Brain Jelly System Implementation
// ================================================================

IntegratedBrainJellySystem::IntegratedBrainJellySystem(
    NanoBrainKernel *kernel, TimeCrystalKernel *time_crystal,
    const IntegratedBrainJellyConfig &config)
    : kernel(kernel), time_crystal(time_crystal), config(config), active(false),
      current_time(0.0f) {}

IntegratedBrainJellySystem::~IntegratedBrainJellySystem() { shutdown(); }

void IntegratedBrainJellySystem::initialize() {
  if (active)
    return;

  // Create all subsystems
  device_registry = std::make_unique<BioMorphicDeviceRegistry>(
      kernel, config.registry_config);
  device_registry->initialize_full_registry();

  condensation_engine =
      std::make_unique<FractalCondensation>(kernel, config.condensation_config);

  brain_jelly =
      std::make_unique<BrainJellySimulator>(kernel, config.jelly_config);

  pen_freezer = std::make_unique<CorticalPenFreezer>(kernel, time_crystal,
                                                     config.pen_config);

  avatar_interface = std::make_unique<HumanoidAvatarInterface>(
      kernel, time_crystal, brain_jelly.get(), config.avatar_config);

  active = true;
}

void IntegratedBrainJellySystem::shutdown() {
  if (!active)
    return;

  avatar_interface.reset();
  pen_freezer.reset();
  brain_jelly.reset();
  condensation_engine.reset();
  device_registry.reset();

  active = false;
}

void IntegratedBrainJellySystem::process_cycle(float delta_time) {
  if (!active)
    return;

  current_time += delta_time;

  // Update all subsystems
  if (device_registry) {
    device_registry->update_all_devices(delta_time);
    device_registry->propagate_signals();
  }

  if (brain_jelly) {
    brain_jelly->update_chain(delta_time);
  }

  if (avatar_interface) {
    avatar_interface->update_neural_state(delta_time);
  }
}

BrainJellyMetrics IntegratedBrainJellySystem::get_metrics() const {
  BrainJellyMetrics metrics = {};

  if (device_registry) {
    metrics.total_devices = device_registry->device_count();
    auto response = device_registry->compute_collective_response();
    metrics.collective_activation = response.collective_activation;
    metrics.synchronization_index = response.synchronization_index;
  }

  if (condensation_engine) {
    metrics.condensation_points = condensation_engine->point_count();
    auto field = condensation_engine->compute_field();
    metrics.field_coherence = field.field_coherence;
    metrics.prime_alignment = condensation_engine->compute_pattern_coherence();
  }

  if (brain_jelly) {
    metrics.total_resonance = brain_jelly->get_total_resonance();
    metrics.consciousness_index = brain_jelly->get_consciousness_index();
    metrics.eeg_alpha_power = brain_jelly->get_band_power(EEGBand::Alpha);
  }

  if (pen_freezer) {
    metrics.frozen_capsules = pen_freezer->capsule_count();
    metrics.average_complexity = 5.0f; // Placeholder
  }

  if (avatar_interface) {
    auto avatar_metrics = avatar_interface->get_metrics();
    metrics.embodiment_index = avatar_metrics.embodiment_index;
    metrics.sensory_bandwidth = avatar_metrics.sensory_bandwidth;
    metrics.motor_precision = avatar_metrics.motor_precision;
  }

  return metrics;
}
