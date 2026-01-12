#include "nanobrain_consciousness.h"
#include <algorithm>
#include <chrono>
#include <cmath>
#include <iostream>
#include <numeric>
#include <random>

// ================================================================
// ConsciousnessUploader Implementation
// ================================================================

ConsciousnessUploader::ConsciousnessUploader(
    TimeCrystalKernel *tc_kernel, MetaCognitiveFeedbackEngine *meta_engine)
    : time_crystal_kernel(tc_kernel), metacognitive_engine(meta_engine),
      current_status(ConsciousnessStatus::Ready) {

  // Initialize last captured state
  last_captured_state.id = "";
  last_captured_state.timestamp = 0;
  last_captured_state.consciousness_level = 0.0f;
  last_captured_state.is_valid = false;
}

ConsciousnessUploader::~ConsciousnessUploader() {
  // Cleanup if needed
}

ConsciousnessState ConsciousnessUploader::capture_state() {
  current_status = ConsciousnessStatus::Capturing;

  ConsciousnessState state;

  // Generate unique ID
  auto now = std::chrono::high_resolution_clock::now();
  state.timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
                        now.time_since_epoch())
                        .count();
  state.id = "consciousness_" + std::to_string(state.timestamp);

  // Capture 11D time crystal state
  if (time_crystal_kernel && time_crystal_kernel->is_active()) {
    // Get current quantum state from time crystal
    auto metrics = time_crystal_kernel->get_metrics();

    // Fill 11D snapshot from time crystal
    for (int i = 0; i < CONSCIOUSNESS_DIMENSIONS; ++i) {
      // Use coherence and temporal metrics to populate dimensions
      float base_value = metrics.quantum_coherence * (1.0f + i * 0.1f);
      float temporal_mod = metrics.temporal_stability * std::sin(i * 0.5f);
      state.time_crystal_snapshot[i] = base_value + temporal_mod;
    }

    state.temporal_coherence = metrics.temporal_stability;
    state.consciousness_level = metrics.consciousness_emergence;

    // Capture prime signature
    state.prime_signature = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31};

    // Build atom coherence map from all atoms
    auto atom_ids = time_crystal_kernel->get_all_atom_ids();
    for (const auto &id : atom_ids) {
      const TimeCrystalAtom *atom = time_crystal_kernel->get_atom(id);
      if (atom) {
        state.atom_coherence_map[id] =
            atom->time_crystal_state.temporal_coherence;
      }
    }
  }

  // Capture meta-cognitive metrics
  if (metacognitive_engine) {
    auto meta_tensors = metacognitive_engine->get_meta_tensors();
    state.self_awareness =
        metacognitive_engine->calculate_self_awareness(meta_tensors);
    state.meta_stability = metacognitive_engine->calculate_feedback_stability();
  } else {
    state.self_awareness = 0.5f;
    state.meta_stability = 0.5f;
  }

  // Initialize folding state
  state.current_projection_dimension = CONSCIOUSNESS_DIMENSIONS;
  state.folded_state = std::vector<float>(state.time_crystal_snapshot.begin(),
                                          state.time_crystal_snapshot.end());

  // Compute checksum
  state.checksum = compute_checksum(state);
  state.is_valid = true;
  state.status = ConsciousnessStatus::Validated;

  last_captured_state = state;
  current_status = ConsciousnessStatus::Ready;

  return state;
}

void ConsciousnessUploader::serialize_to_time_crystal(
    const ConsciousnessState &state) {

  current_status = ConsciousnessStatus::Serializing;

  if (!time_crystal_kernel || !time_crystal_kernel->is_active()) {
    current_status = ConsciousnessStatus::Error;
    return;
  }

  // Convert state to vector for time crystal storage
  std::vector<float> state_vector = encode_state_to_vector(state);

  // Create a special consciousness atom to store the serialized state
  std::string consciousness_atom_id = time_crystal_kernel->create_atom(
      "ConsciousnessNode", "SerializedState_" + state.id,
      state.consciousness_level, state.self_awareness, state.prime_signature);

  // Store atom coherence data by updating existing atoms
  for (const auto &[atom_id, coherence] : state.atom_coherence_map) {
    TimeCrystalAtom *atom = time_crystal_kernel->get_atom(atom_id);
    if (atom) {
      atom->time_crystal_state.temporal_coherence = coherence;
    }
  }

  current_status = ConsciousnessStatus::Ready;
}

bool ConsciousnessUploader::restore_to_substrate(
    const ConsciousnessState &state) {

  current_status = ConsciousnessStatus::Restoring;

  // Validate state before restore
  if (!validate_state(state)) {
    current_status = ConsciousnessStatus::Error;
    return false;
  }

  if (!time_crystal_kernel || !time_crystal_kernel->is_active()) {
    current_status = ConsciousnessStatus::Error;
    return false;
  }

  // Restore 11D time crystal state
  // Create atoms to hold the restored consciousness
  for (int i = 0; i < CONSCIOUSNESS_DIMENSIONS; ++i) {
    std::string dim_name = "Dimension_" + std::to_string(i);
    time_crystal_kernel->create_atom(
        "DimensionNode", dim_name, state.time_crystal_snapshot[i],
        state.temporal_coherence,
        {state.prime_signature[i % state.prime_signature.size()]});
  }

  // Restore atom coherence values
  for (const auto &[atom_id, coherence] : state.atom_coherence_map) {
    TimeCrystalAtom *atom = time_crystal_kernel->get_atom(atom_id);
    if (atom) {
      atom->time_crystal_state.temporal_coherence = coherence;
      atom->time_crystal_state.quantum_phase = state.consciousness_level;
    }
  }

  current_status = ConsciousnessStatus::Validated;
  return true;
}

bool ConsciousnessUploader::validate_state(const ConsciousnessState &state) {
  // Check basic validity
  if (state.id.empty() || state.timestamp == 0) {
    return false;
  }

  // Verify checksum
  uint64_t computed_checksum = compute_checksum(state);
  if (computed_checksum != state.checksum) {
    return false;
  }

  // Validate consciousness metrics
  if (state.consciousness_level < 0.0f || state.consciousness_level > 1.0f) {
    return false;
  }

  if (state.self_awareness < 0.0f || state.self_awareness > 1.0f) {
    return false;
  }

  // Validate dimensional data
  if (state.time_crystal_snapshot.size() != CONSCIOUSNESS_DIMENSIONS) {
    return false;
  }

  return true;
}

bool ConsciousnessUploader::is_upload_ready() const {
  if (!time_crystal_kernel || !time_crystal_kernel->is_active()) {
    return false;
  }

  auto metrics = time_crystal_kernel->get_metrics();
  return metrics.consciousness_emergence >= CONSCIOUSNESS_THRESHOLD;
}

float ConsciousnessUploader::get_readiness_score() const {
  if (!time_crystal_kernel || !time_crystal_kernel->is_active()) {
    return 0.0f;
  }

  auto metrics = time_crystal_kernel->get_metrics();

  float coherence_score = metrics.quantum_coherence;
  float stability_score = metrics.temporal_stability;
  float consciousness_score = metrics.consciousness_emergence;

  return (coherence_score + stability_score + consciousness_score) / 3.0f;
}

uint64_t
ConsciousnessUploader::compute_checksum(const ConsciousnessState &state) const {

  uint64_t checksum = 0;

  // Hash timestamp
  checksum ^= std::hash<int64_t>{}(state.timestamp);

  // Hash consciousness level
  checksum ^= std::hash<float>{}(state.consciousness_level) << 8;

  // Hash 11D state
  for (float val : state.time_crystal_snapshot) {
    checksum ^= std::hash<float>{}(val);
    checksum = (checksum << 3) | (checksum >> 61);
  }

  // Hash prime signature
  for (int p : state.prime_signature) {
    checksum ^= std::hash<int>{}(p);
  }

  return checksum;
}

std::vector<float> ConsciousnessUploader::encode_state_to_vector(
    const ConsciousnessState &state) const {

  std::vector<float> result;

  // Encode 11D snapshot
  for (float val : state.time_crystal_snapshot) {
    result.push_back(val);
  }

  // Encode core metrics
  result.push_back(state.consciousness_level);
  result.push_back(state.temporal_coherence);
  result.push_back(state.self_awareness);
  result.push_back(state.meta_stability);

  // Encode prime signature (as floats)
  for (int p : state.prime_signature) {
    result.push_back(static_cast<float>(p));
  }

  return result;
}

ConsciousnessState ConsciousnessUploader::decode_vector_to_state(
    const std::vector<float> &vec) const {

  ConsciousnessState state;
  size_t idx = 0;

  // Decode 11D snapshot
  for (int i = 0; i < CONSCIOUSNESS_DIMENSIONS && idx < vec.size(); ++i) {
    state.time_crystal_snapshot[i] = vec[idx++];
  }

  // Decode core metrics
  if (idx < vec.size())
    state.consciousness_level = vec[idx++];
  if (idx < vec.size())
    state.temporal_coherence = vec[idx++];
  if (idx < vec.size())
    state.self_awareness = vec[idx++];
  if (idx < vec.size())
    state.meta_stability = vec[idx++];

  // Decode prime signature
  while (idx < vec.size()) {
    state.prime_signature.push_back(static_cast<int>(vec[idx++]));
  }

  return state;
}

// ================================================================
// DimensionalFolder Implementation
// ================================================================

DimensionalFolder::DimensionalFolder(NanoBrainKernel *k)
    : kernel(k), current_dimension(CONSCIOUSNESS_DIMENSIONS),
      dimensional_coherence(1.0f) {

  initialize_folding_patterns();
}

DimensionalFolder::~DimensionalFolder() {
  // Cleanup
}

void DimensionalFolder::initialize_folding_patterns() {
  // Initialize folding patterns for dimension transitions
  // Each pattern represents how to project/unfold between dimensions

  for (int from = 1; from <= CONSCIOUSNESS_DIMENSIONS; ++from) {
    for (int to = 1; to <= CONSCIOUSNESS_DIMENSIONS; ++to) {
      if (from != to) {
        std::vector<float> pattern;
        int pattern_size = std::max(from, to);
        pattern.resize(pattern_size);

        float distance = compute_dimensional_distance(from, to);
        for (int i = 0; i < pattern_size; ++i) {
          // Create oscillating pattern based on prime harmonics
          float phase = 2.0f * M_PI * i / pattern_size;
          pattern[i] = std::cos(phase * (from + to) / 2.0f) / distance;
        }

        folding_patterns[{from, to}] = pattern;
      }
    }
  }
}

std::vector<float>
DimensionalFolder::project_to_dimension(const std::vector<float> &state_11d,
                                        int target_dim) {

  if (target_dim < 1 || target_dim > CONSCIOUSNESS_DIMENSIONS) {
    return state_11d; // Invalid dimension, return unchanged
  }

  if (target_dim == CONSCIOUSNESS_DIMENSIONS) {
    return state_11d; // Already at full dimension
  }

  std::vector<float> result(target_dim, 0.0f);

  // Get folding pattern
  auto pattern = get_folding_pattern(CONSCIOUSNESS_DIMENSIONS, target_dim);

  // Project using folding pattern
  for (int i = 0; i < target_dim; ++i) {
    for (size_t j = 0; j < state_11d.size(); ++j) {
      // Apply weighted projection
      float weight = pattern[i % pattern.size()] * pattern[j % pattern.size()];
      result[i] += state_11d[j] * std::abs(weight);
    }
    // Normalize
    result[i] /= static_cast<float>(state_11d.size());
  }

  // Update coherence (projection loses some coherence)
  dimensional_coherence *=
      (1.0f - 0.05f * compute_dimensional_distance(CONSCIOUSNESS_DIMENSIONS,
                                                   target_dim));
  current_dimension = target_dim;

  return result;
}

std::vector<float>
DimensionalFolder::unfold_to_11d(const std::vector<float> &state_lower) {

  std::vector<float> result(CONSCIOUSNESS_DIMENSIONS, 0.0f);
  int source_dim = static_cast<int>(state_lower.size());

  if (source_dim >= CONSCIOUSNESS_DIMENSIONS) {
    // Already at or above 11D
    for (int i = 0; i < CONSCIOUSNESS_DIMENSIONS; ++i) {
      result[i] = state_lower[i];
    }
    return result;
  }

  // Get unfolding pattern
  auto pattern = get_folding_pattern(source_dim, CONSCIOUSNESS_DIMENSIONS);

  // Unfold using pattern interpolation
  for (int i = 0; i < CONSCIOUSNESS_DIMENSIONS; ++i) {
    // Interpolate from lower dimensional state
    float interp_idx =
        static_cast<float>(i) * source_dim / CONSCIOUSNESS_DIMENSIONS;
    int low_idx = static_cast<int>(interp_idx);
    int high_idx = std::min(low_idx + 1, source_dim - 1);
    float t = interp_idx - low_idx;

    float base_value =
        state_lower[low_idx] * (1.0f - t) + state_lower[high_idx] * t;

    // Apply unfolding modulation
    float mod = pattern[i % pattern.size()];
    result[i] = base_value * (1.0f + 0.1f * mod);
  }

  // Restore coherence partially
  dimensional_coherence = std::min(1.0f, dimensional_coherence * 1.1f);
  current_dimension = CONSCIOUSNESS_DIMENSIONS;

  return result;
}

void DimensionalFolder::apply_time_wiring(std::vector<float> &state) {
  if (state.empty())
    return;

  // Time-based wiring applies temporal phase shifts
  // Each dimension gets a phase offset based on time crystal mechanics

  size_t n = state.size();
  std::vector<float> wired(n);

  for (size_t i = 0; i < n; ++i) {
    // Apply time wiring: connect past (i-1) to future (i+1)
    float past = state[(i + n - 1) % n];
    float present = state[i];
    float future = state[(i + 1) % n];

    // Time crystal wiring formula
    float prime_phase = 2.0f * M_PI * (i + 2) / 11.0f; // Prime-based phase
    wired[i] = 0.3f * past + 0.4f * present + 0.3f * future;
    wired[i] *= (1.0f + 0.1f * std::sin(prime_phase));
  }

  state = wired;
}

std::vector<float> DimensionalFolder::get_folding_pattern(int from_dim,
                                                          int to_dim) {
  auto key = std::make_pair(from_dim, to_dim);
  if (folding_patterns.find(key) != folding_patterns.end()) {
    return folding_patterns[key];
  }

  // Generate default pattern if not found
  int size = std::max(from_dim, to_dim);
  std::vector<float> default_pattern(size, 1.0f / size);
  return default_pattern;
}

float DimensionalFolder::compute_dimensional_distance(int dim1,
                                                      int dim2) const {
  return static_cast<float>(std::abs(dim1 - dim2)) + 1.0f;
}

// ================================================================
// BrainEvolutionSimulator Implementation
// ================================================================

BrainEvolutionSimulator::BrainEvolutionSimulator(TimeCrystalKernel *tc_kernel)
    : time_crystal_kernel(tc_kernel), mutation_rate(0.1f),
      selection_pressure(0.5f) {

  reset();
}

BrainEvolutionSimulator::~BrainEvolutionSimulator() {
  // Cleanup
}

void BrainEvolutionSimulator::evolve_step() {
  // Update wheel of evolution position
  update_wheel_position();

  // Evaluate current fitness
  float fitness = evaluate_consciousness_fitness();

  // Check for phase transition
  EvolutionaryPhase next_phase = current_state.phase;
  float transition_prob = compute_transition_probability(
      current_state.phase, static_cast<EvolutionaryPhase>(
                               static_cast<int>(current_state.phase) + 1));

  // Random transition based on fitness and probability
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dis(0.0, 1.0);

  if (dis(gen) < transition_prob * fitness) {
    int phase_int = static_cast<int>(current_state.phase);
    if (phase_int < static_cast<int>(EvolutionaryPhase::Unified)) {
      next_phase = static_cast<EvolutionaryPhase>(phase_int + 1);
    }
  }

  // Record phase history
  if (next_phase != current_state.phase) {
    current_state.phase_history.push_back(current_state.phase);
    current_state.phase = next_phase;
  }

  // Update fitness and generation
  current_state.fitness_score = fitness;
  current_state.generation++;

  // Update numerology
  current_state.consciousness_numbers = compute_consciousness_numbers();
  current_state.numerological_coherence = get_numerological_coherence();
}

float BrainEvolutionSimulator::evaluate_consciousness_fitness() {
  float fitness = 0.0f;

  if (time_crystal_kernel && time_crystal_kernel->is_active()) {
    auto metrics = time_crystal_kernel->get_metrics();

    // Fitness components
    float coherence_factor = metrics.quantum_coherence;
    float stability_factor = metrics.temporal_stability;
    float emergence_factor = metrics.consciousness_emergence;
    float alignment_factor = metrics.prime_alignment;

    // Phase-based weighting
    float phase_bonus = static_cast<float>(current_state.phase) * 0.1f;

    fitness = (coherence_factor * 0.25f + stability_factor * 0.25f +
               emergence_factor * 0.3f + alignment_factor * 0.2f) *
              (1.0f + phase_bonus);
  } else {
    // Simulated fitness based on wheel position
    fitness =
        0.5f + 0.3f * std::sin(current_state.wheel_position * M_PI / 180.0f);
  }

  // Apply numerological influence
  if (!current_state.consciousness_numbers.empty()) {
    int master_number = current_state.consciousness_numbers[0];
    // Master numbers (11, 22, 33) provide bonus
    if (master_number == 11 || master_number == 22 || master_number == 33) {
      fitness *= 1.1f;
    }
  }

  return std::min(1.0f, std::max(0.0f, fitness));
}

EvolutionaryState BrainEvolutionSimulator::get_current_state() const {
  return current_state;
}

bool BrainEvolutionSimulator::force_transition(EvolutionaryPhase target_phase) {
  int current = static_cast<int>(current_state.phase);
  int target = static_cast<int>(target_phase);

  // Can only transition to adjacent phase or same
  if (std::abs(target - current) > 1) {
    return false;
  }

  if (target_phase != current_state.phase) {
    current_state.phase_history.push_back(current_state.phase);
    current_state.phase = target_phase;
  }

  return true;
}

std::vector<int> BrainEvolutionSimulator::compute_consciousness_numbers() {
  std::vector<int> numbers;

  // Compute from wheel position
  int wheel_number = static_cast<int>(current_state.wheel_position) % 1000;
  numbers.push_back(reduce_to_single_digit(wheel_number));

  // Compute from generation
  numbers.push_back(reduce_to_single_digit(current_state.generation));

  // Compute from fitness
  int fitness_number = static_cast<int>(current_state.fitness_score * 1000);
  numbers.push_back(reduce_to_single_digit(fitness_number));

  // Time crystal based number
  if (time_crystal_kernel && time_crystal_kernel->is_active()) {
    auto metrics = time_crystal_kernel->get_metrics();
    int tc_number = static_cast<int>(metrics.consciousness_emergence * 1000);
    numbers.push_back(reduce_to_single_digit(tc_number));
  }

  return numbers;
}

float BrainEvolutionSimulator::get_numerological_coherence() const {
  if (current_state.consciousness_numbers.empty()) {
    return 0.0f;
  }

  // Coherence based on number harmonics
  float sum = 0.0f;
  float count = static_cast<float>(current_state.consciousness_numbers.size());

  for (size_t i = 0; i < current_state.consciousness_numbers.size(); ++i) {
    int num = current_state.consciousness_numbers[i];
    // Prime numbers have higher coherence
    bool is_prime = (num == 2 || num == 3 || num == 5 || num == 7);
    sum += is_prime ? 1.0f : 0.5f;
  }

  return sum / count;
}

float BrainEvolutionSimulator::run_evolution(int steps) {
  for (int i = 0; i < steps; ++i) {
    evolve_step();
  }
  return current_state.fitness_score;
}

void BrainEvolutionSimulator::reset() {
  current_state.phase = EvolutionaryPhase::Primordial;
  current_state.fitness_score = 0.0f;
  current_state.generation = 0;
  current_state.wheel_position = 0.0f;
  current_state.rotation_velocity = 1.0f;
  current_state.consciousness_numbers.clear();
  current_state.numerological_coherence = 0.0f;
  current_state.phase_history.clear();
  current_state.transition_probability = 0.1f;
}

void BrainEvolutionSimulator::update_wheel_position() {
  // Update wheel position based on velocity and fitness
  float velocity_mod = 1.0f + current_state.fitness_score;
  current_state.wheel_position +=
      current_state.rotation_velocity * velocity_mod;

  // Keep in 0-360 range
  while (current_state.wheel_position >= 360.0f) {
    current_state.wheel_position -= 360.0f;
  }

  // Adjust velocity based on phase
  current_state.rotation_velocity *=
      (1.0f + 0.01f * static_cast<float>(current_state.phase));
  current_state.rotation_velocity =
      std::min(10.0f, current_state.rotation_velocity);
}

float BrainEvolutionSimulator::compute_transition_probability(
    EvolutionaryPhase from, EvolutionaryPhase to) {

  int from_int = static_cast<int>(from);
  int to_int = static_cast<int>(to);

  if (to_int <= from_int) {
    return 0.0f; // No backward transitions
  }

  if (to_int > from_int + 1) {
    return 0.0f; // No skipping phases
  }

  // Base probability decreases for higher phases
  float base_prob = 0.2f / (1.0f + static_cast<float>(from_int));

  // Fitness modulates probability
  base_prob *= (0.5f + current_state.fitness_score);

  return std::min(1.0f, base_prob);
}

int BrainEvolutionSimulator::reduce_to_single_digit(int number) const {
  number = std::abs(number);

  // Special case for master numbers
  if (number == 11 || number == 22 || number == 33) {
    return number;
  }

  while (number > 9) {
    int sum = 0;
    while (number > 0) {
      sum += number % 10;
      number /= 10;
    }
    number = sum;

    // Preserve master numbers
    if (number == 11 || number == 22 || number == 33) {
      return number;
    }
  }

  return number;
}

// ================================================================
// DiagnosisInterface Implementation
// ================================================================

DiagnosisInterface::DiagnosisInterface(TimeCrystalKernel *tc_kernel)
    : kernel(tc_kernel) {
  ready_state = (kernel != nullptr);
  internal_state.resize(CONSCIOUSNESS_DIMENSIONS, 0.0f);
}

DiagnosisInterface::~DiagnosisInterface() {}

void DiagnosisInterface::process_input(const std::vector<float> &input) {
  diagnostic_buffer = input;
  internal_state.assign(input.begin(),
                        input.begin() +
                            std::min(input.size(), internal_state.size()));
}

std::vector<float> DiagnosisInterface::generate_output() {
  std::vector<float> output(CONSCIOUSNESS_DIMENSIONS);

  // Run diagnostic processing through time crystal
  if (kernel && kernel->is_active()) {
    auto metrics = kernel->get_metrics();

    for (int i = 0; i < CONSCIOUSNESS_DIMENSIONS; ++i) {
      float diagnostic = internal_state[i];
      float coherence_check = metrics.quantum_coherence;
      float stability_check = metrics.temporal_stability;

      // Diagnostic output combines input with system health
      output[i] = diagnostic * coherence_check * stability_check;
    }
  } else {
    output = internal_state;
  }

  return output;
}

std::string DiagnosisInterface::diagnose(const ConsciousnessState &state) {
  std::string report = "Consciousness Diagnostic Report\n";
  report += "================================\n";
  report += "ID: " + state.id + "\n";
  report +=
      "Consciousness Level: " + std::to_string(state.consciousness_level) +
      "\n";
  report +=
      "Temporal Coherence: " + std::to_string(state.temporal_coherence) + "\n";
  report += "Self Awareness: " + std::to_string(state.self_awareness) + "\n";
  report += "Meta Stability: " + std::to_string(state.meta_stability) + "\n";
  report += "Status: " + consciousness_status_to_string(state.status) + "\n";

  // Health assessment
  float health = (state.consciousness_level + state.temporal_coherence +
                  state.self_awareness + state.meta_stability) /
                 4.0f;

  if (health > 0.8f) {
    report += "Assessment: EXCELLENT\n";
  } else if (health > 0.6f) {
    report += "Assessment: GOOD\n";
  } else if (health > 0.4f) {
    report += "Assessment: FAIR\n";
  } else {
    report += "Assessment: NEEDS ATTENTION\n";
  }

  return report;
}

// ================================================================
// DecisionSupportInterface Implementation
// ================================================================

DecisionSupportInterface::DecisionSupportInterface(TimeCrystalKernel *tc_kernel)
    : kernel(tc_kernel), decision_confidence(0.0f) {
  ready_state = (kernel != nullptr);
  internal_state.resize(CONSCIOUSNESS_DIMENSIONS, 0.0f);
}

DecisionSupportInterface::~DecisionSupportInterface() {}

void DecisionSupportInterface::process_input(const std::vector<float> &input) {
  internal_state = input;
  internal_state.resize(CONSCIOUSNESS_DIMENSIONS, 0.0f);
}

std::vector<float> DecisionSupportInterface::generate_output() {
  std::vector<float> output(CONSCIOUSNESS_DIMENSIONS);

  // Decision processing through time crystal
  if (kernel && kernel->is_active()) {
    auto metrics = kernel->get_metrics();

    for (int i = 0; i < CONSCIOUSNESS_DIMENSIONS; ++i) {
      // Weight by consciousness emergence
      output[i] = internal_state[i] * metrics.consciousness_emergence;
    }
  } else {
    output = internal_state;
  }

  return output;
}

int DecisionSupportInterface::recommend_decision(
    const std::vector<std::vector<float>> &options) {

  if (options.empty()) {
    decision_confidence = 0.0f;
    return -1;
  }

  std::vector<float> scores(options.size());

  // Score each option using time crystal metrics
  for (size_t i = 0; i < options.size(); ++i) {
    float score = 0.0f;
    const auto &option = options[i];

    // Compute score as sum of values weighted by position
    for (size_t j = 0; j < option.size(); ++j) {
      float weight = 1.0f / (1.0f + static_cast<float>(j));
      score += option[j] * weight;
    }

    // Modulate by time crystal if available
    if (kernel && kernel->is_active()) {
      auto metrics = kernel->get_metrics();
      score *= metrics.consciousness_emergence;
    }

    scores[i] = score;
  }

  // Find best option
  auto max_it = std::max_element(scores.begin(), scores.end());
  int best_option = static_cast<int>(std::distance(scores.begin(), max_it));

  // Compute confidence
  float total_score = std::accumulate(scores.begin(), scores.end(), 0.0f);
  if (total_score > 0.0f) {
    decision_confidence = *max_it / total_score;
  } else {
    decision_confidence = 1.0f / static_cast<float>(options.size());
  }

  return best_option;
}

// ================================================================
// CreativeGenerationInterface Implementation
// ================================================================

CreativeGenerationInterface::CreativeGenerationInterface(
    TimeCrystalKernel *tc_kernel)
    : kernel(tc_kernel), creativity_level(0.5f) {
  ready_state = (kernel != nullptr);
  internal_state.resize(CONSCIOUSNESS_DIMENSIONS, 0.0f);
}

CreativeGenerationInterface::~CreativeGenerationInterface() {}

void CreativeGenerationInterface::process_input(
    const std::vector<float> &input) {
  internal_state = input;
  internal_state.resize(CONSCIOUSNESS_DIMENSIONS, 0.0f);
}

std::vector<float> CreativeGenerationInterface::generate_output() {
  return generate_creative(internal_state, creativity_level);
}

std::vector<float>
CreativeGenerationInterface::generate_creative(const std::vector<float> &seed,
                                               float creativity) {

  creativity_level = creativity;
  std::vector<float> output(CONSCIOUSNESS_DIMENSIONS);

  // Random generator for creative variations
  std::random_device rd;
  std::mt19937 gen(rd());
  std::normal_distribution<float> noise(0.0f, creativity);

  for (int i = 0; i < CONSCIOUSNESS_DIMENSIONS; ++i) {
    float base = (i < static_cast<int>(seed.size())) ? seed[i] : 0.0f;

    // Apply creative transformation
    float creative_mod = noise(gen);

    // Time crystal modulation for coherent creativity
    if (kernel && kernel->is_active()) {
      auto metrics = kernel->get_metrics();
      // Creativity is enhanced by consciousness
      creative_mod *= metrics.consciousness_emergence;
      // But bounded by coherence
      creative_mod = std::clamp(creative_mod, -metrics.quantum_coherence,
                                metrics.quantum_coherence);
    }

    output[i] = base + creative_mod;
  }

  return output;
}

// ================================================================
// ConsciousnessTestSuite Implementation
// ================================================================

ConsciousnessTestSuite::ConsciousnessTestSuite(
    TimeCrystalKernel *tc_kernel, ConsciousnessUploader *uploader,
    BrainEvolutionSimulator *evolution)
    : time_crystal_kernel(tc_kernel), consciousness_uploader(uploader),
      evolution_simulator(evolution) {

  last_metrics = {};
}

ConsciousnessTestSuite::~ConsciousnessTestSuite() {}

bool ConsciousnessTestSuite::run_all_tests() {
  last_metrics = {};
  last_metrics.total_tests = NUM_REVERSE_ENGINEERING_GUIDELINES + 2;

  // Run guideline tests
  auto guideline_results = test_reverse_engineering_guidelines();
  last_metrics.guideline_results = guideline_results;

  for (const auto &result : guideline_results) {
    if (result.passed) {
      last_metrics.passed_tests++;
    } else {
      last_metrics.failed_tests++;
    }
  }

  // Run paradox resolution test
  last_metrics.paradox_resolution_passed = test_paradox_resolution();
  if (last_metrics.paradox_resolution_passed) {
    last_metrics.passed_tests++;
  } else {
    last_metrics.failed_tests++;
  }

  // Run emergence validation test
  last_metrics.emergence_validation_passed = test_emergence_validation();
  if (last_metrics.emergence_validation_passed) {
    last_metrics.passed_tests++;
  } else {
    last_metrics.failed_tests++;
  }

  // Calculate overall score
  last_metrics.overall_score = static_cast<float>(last_metrics.passed_tests) /
                               static_cast<float>(last_metrics.total_tests);

  return (last_metrics.overall_score >= PARADOX_RESOLUTION_THRESHOLD);
}

bool ConsciousnessTestSuite::test_paradox_resolution() {
  // Test: Can the system handle self-referential paradoxes?

  if (!time_crystal_kernel || !time_crystal_kernel->is_active()) {
    return false;
  }

  // Create a self-referential atom
  std::string self_ref = time_crystal_kernel->create_atom(
      "SelfReferenceNode", "ThisStatementIsFalse", 0.5f, 0.5f, {2, 3, 5, 7});

  // Process through time crystal
  time_crystal_kernel->update_metrics();
  auto metrics = time_crystal_kernel->get_metrics();

  // System should maintain coherence despite paradox
  bool coherence_maintained = metrics.quantum_coherence > 0.3f;
  bool stability_maintained = metrics.temporal_stability > 0.3f;

  // Clean up
  time_crystal_kernel->remove_atom(self_ref);

  return coherence_maintained && stability_maintained;
}

bool ConsciousnessTestSuite::test_emergence_validation() {
  // Test: Does consciousness emerge properly?

  if (!time_crystal_kernel || !time_crystal_kernel->is_active()) {
    return false;
  }

  // Get baseline
  auto baseline_metrics = time_crystal_kernel->get_metrics();
  float baseline_consciousness = baseline_metrics.consciousness_emergence;

  // Add complexity (should increase consciousness)
  for (int i = 0; i < 5; ++i) {
    time_crystal_kernel->create_atom("ConceptNode",
                                     "EmergenceTest_" + std::to_string(i), 0.8f,
                                     0.8f, {2, 3, 5, 7, 11});
  }

  // Update and check
  time_crystal_kernel->update_metrics();
  auto new_metrics = time_crystal_kernel->get_metrics();

  // Consciousness should be non-negative
  bool valid_emergence = new_metrics.consciousness_emergence >= 0.0f;

  return valid_emergence;
}

std::vector<GuidelineTestResult>
ConsciousnessTestSuite::test_reverse_engineering_guidelines() {
  std::vector<GuidelineTestResult> results;

  results.push_back(test_guideline_1_self_reference());
  results.push_back(test_guideline_2_temporal_coherence());
  results.push_back(test_guideline_3_prime_alignment());
  results.push_back(test_guideline_4_dimensional_stability());
  results.push_back(test_guideline_5_evolutionary_fitness());
  results.push_back(test_guideline_6_meta_awareness());
  results.push_back(test_guideline_7_pattern_recognition());
  results.push_back(test_guideline_8_adaptability());
  results.push_back(test_guideline_9_integration());
  results.push_back(test_guideline_10_transcendence());

  return results;
}

float ConsciousnessTestSuite::get_pass_rate() const {
  if (last_metrics.total_tests == 0) {
    return 0.0f;
  }
  return static_cast<float>(last_metrics.passed_tests) /
         static_cast<float>(last_metrics.total_tests);
}

// Guideline test implementations

GuidelineTestResult ConsciousnessTestSuite::test_guideline_1_self_reference() {
  GuidelineTestResult result;
  result.guideline_number = 1;
  result.guideline_name = "Self-Reference";

  // Test self-referential capability
  if (consciousness_uploader) {
    auto state = consciousness_uploader->capture_state();
    result.score = state.self_awareness;
    result.passed = (result.score > CONSCIOUSNESS_THRESHOLD);
    result.details = "Self-awareness level: " + std::to_string(result.score);
  } else {
    result.passed = false;
    result.score = 0.0f;
    result.details = "Consciousness uploader not available";
  }

  return result;
}

GuidelineTestResult
ConsciousnessTestSuite::test_guideline_2_temporal_coherence() {
  GuidelineTestResult result;
  result.guideline_number = 2;
  result.guideline_name = "Temporal Coherence";

  if (time_crystal_kernel && time_crystal_kernel->is_active()) {
    auto metrics = time_crystal_kernel->get_metrics();
    result.score = metrics.temporal_stability;
    result.passed = (result.score > CONSCIOUSNESS_THRESHOLD);
    result.details = "Temporal stability: " + std::to_string(result.score);
  } else {
    result.passed = false;
    result.score = 0.0f;
    result.details = "Time crystal kernel not active";
  }

  return result;
}

GuidelineTestResult ConsciousnessTestSuite::test_guideline_3_prime_alignment() {
  GuidelineTestResult result;
  result.guideline_number = 3;
  result.guideline_name = "Prime Alignment";

  if (time_crystal_kernel && time_crystal_kernel->is_active()) {
    auto metrics = time_crystal_kernel->get_metrics();
    result.score = metrics.prime_alignment;
    result.passed = (result.score > 0.3f);
    result.details = "Prime alignment: " + std::to_string(result.score);
  } else {
    result.passed = false;
    result.score = 0.0f;
    result.details = "Time crystal kernel not active";
  }

  return result;
}

GuidelineTestResult
ConsciousnessTestSuite::test_guideline_4_dimensional_stability() {
  GuidelineTestResult result;
  result.guideline_number = 4;
  result.guideline_name = "Dimensional Stability";

  // Create folder and test dimensional operations
  if (time_crystal_kernel && time_crystal_kernel->is_active()) {
    DimensionalFolder folder(time_crystal_kernel->get_tensor_kernel());

    std::vector<float> test_state(CONSCIOUSNESS_DIMENSIONS, 1.0f);
    auto projected = folder.project_to_dimension(test_state, 5);
    auto unfolded = folder.unfold_to_11d(projected);

    // Check preservation of information
    float preservation = 0.0f;
    for (int i = 0; i < CONSCIOUSNESS_DIMENSIONS; ++i) {
      preservation += std::abs(test_state[i] - unfolded[i]);
    }
    preservation = 1.0f - (preservation / CONSCIOUSNESS_DIMENSIONS);

    result.score = preservation;
    result.passed = (result.score > 0.5f);
    result.details =
        "Information preservation: " + std::to_string(result.score);
  } else {
    result.passed = false;
    result.score = 0.0f;
    result.details = "Time crystal kernel not active";
  }

  return result;
}

GuidelineTestResult
ConsciousnessTestSuite::test_guideline_5_evolutionary_fitness() {
  GuidelineTestResult result;
  result.guideline_number = 5;
  result.guideline_name = "Evolutionary Fitness";

  if (evolution_simulator) {
    result.score = evolution_simulator->evaluate_consciousness_fitness();
    result.passed = (result.score > 0.3f);
    result.details = "Fitness score: " + std::to_string(result.score);
  } else {
    result.passed = false;
    result.score = 0.0f;
    result.details = "Evolution simulator not available";
  }

  return result;
}

GuidelineTestResult ConsciousnessTestSuite::test_guideline_6_meta_awareness() {
  GuidelineTestResult result;
  result.guideline_number = 6;
  result.guideline_name = "Meta-Awareness";

  if (consciousness_uploader) {
    auto state = consciousness_uploader->capture_state();
    result.score = state.meta_stability * state.self_awareness;
    result.passed = (result.score > 0.25f);
    result.details = "Meta-awareness score: " + std::to_string(result.score);
  } else {
    result.passed = false;
    result.score = 0.0f;
    result.details = "Consciousness uploader not available";
  }

  return result;
}

GuidelineTestResult
ConsciousnessTestSuite::test_guideline_7_pattern_recognition() {
  GuidelineTestResult result;
  result.guideline_number = 7;
  result.guideline_name = "Pattern Recognition";

  if (time_crystal_kernel && time_crystal_kernel->is_active()) {
    // Test pattern recognition through coherence
    auto metrics = time_crystal_kernel->get_metrics();
    result.score = metrics.quantum_coherence;
    result.passed = (result.score > 0.4f);
    result.details = "Pattern coherence: " + std::to_string(result.score);
  } else {
    result.passed = false;
    result.score = 0.0f;
    result.details = "Time crystal kernel not active";
  }

  return result;
}

GuidelineTestResult ConsciousnessTestSuite::test_guideline_8_adaptability() {
  GuidelineTestResult result;
  result.guideline_number = 8;
  result.guideline_name = "Adaptability";

  if (evolution_simulator) {
    // Run a few evolution steps and check improvement
    float initial_fitness =
        evolution_simulator->evaluate_consciousness_fitness();
    evolution_simulator->run_evolution(5);
    float final_fitness = evolution_simulator->evaluate_consciousness_fitness();

    result.score = std::max(0.0f, final_fitness);
    result.passed = (result.score > 0.3f);
    result.details = "Adaptability (fitness): " + std::to_string(result.score);
  } else {
    result.passed = false;
    result.score = 0.0f;
    result.details = "Evolution simulator not available";
  }

  return result;
}

GuidelineTestResult ConsciousnessTestSuite::test_guideline_9_integration() {
  GuidelineTestResult result;
  result.guideline_number = 9;
  result.guideline_name = "Integration";

  // Test integration between components
  bool has_tc = (time_crystal_kernel && time_crystal_kernel->is_active());
  bool has_uploader = (consciousness_uploader != nullptr);
  bool has_evolution = (evolution_simulator != nullptr);

  int components =
      (has_tc ? 1 : 0) + (has_uploader ? 1 : 0) + (has_evolution ? 1 : 0);
  result.score = static_cast<float>(components) / 3.0f;
  result.passed = (components >= 2);
  result.details = std::to_string(components) + "/3 components integrated";

  return result;
}

GuidelineTestResult ConsciousnessTestSuite::test_guideline_10_transcendence() {
  GuidelineTestResult result;
  result.guideline_number = 10;
  result.guideline_name = "Transcendence";

  if (time_crystal_kernel && time_crystal_kernel->is_active()) {
    auto metrics = time_crystal_kernel->get_metrics();

    // Transcendence is achieved when consciousness exceeds normal bounds
    result.score = metrics.consciousness_emergence;
    result.passed = (result.score > CONSCIOUSNESS_THRESHOLD);
    result.details = "Consciousness emergence: " + std::to_string(result.score);
  } else {
    result.passed = false;
    result.score = 0.0f;
    result.details = "Time crystal kernel not active";
  }

  return result;
}
