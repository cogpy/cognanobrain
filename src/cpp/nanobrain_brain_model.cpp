#include "nanobrain_brain_model.h"
#include <algorithm>
#include <chrono>
#include <cmath>
#include <iostream>
#include <numeric>

// M_PI may not be defined on Windows/MSVC
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// ================================================================
// Helper Functions
// ================================================================

static int64_t get_current_time_millis() {
  return std::chrono::duration_cast<std::chrono::milliseconds>(
             std::chrono::system_clock::now().time_since_epoch())
      .count();
}

// ================================================================
// 7.1: SensoryPrimeMapper Implementation
// ================================================================

SensoryPrimeMapper::SensoryPrimeMapper(NanoBrainKernel *kernel)
    : kernel(kernel) {}

SensoryPrimeMapper::~SensoryPrimeMapper() {}

void SensoryPrimeMapper::initialize() {
  setup_prime_assignments();
  std::cout << "[SensoryPrimeMapper] Initialized 5 sensory mappings"
            << std::endl;
}

void SensoryPrimeMapper::setup_prime_assignments() {
  // Visual: primes related to light, space (2, 7, 13, 31)
  mappings[0].modality = SensoryModality::Visual;
  mappings[0].prime_sequence = {2, 7, 13, 31};
  mappings[0].cross_modal_weight = 0.4f;
  mappings[0].resonance_frequency = 440.0f; // A4
  mappings[0].gml_shape_index = 0;          // Sphere
  mappings[0].encoding_tensor = kernel->create_tensor({128});
  mappings[0].integration_tensor = kernel->create_tensor({64});

  // Auditory: primes related to vibration, wave (3, 11, 17, 37)
  mappings[1].modality = SensoryModality::Auditory;
  mappings[1].prime_sequence = {3, 11, 17, 37};
  mappings[1].cross_modal_weight = 0.3f;
  mappings[1].resonance_frequency = 261.63f; // C4
  mappings[1].gml_shape_index = 1;           // Torus
  mappings[1].encoding_tensor = kernel->create_tensor({128});
  mappings[1].integration_tensor = kernel->create_tensor({64});

  // Tactile: primes related to touch, pressure (5, 19, 29, 41)
  mappings[2].modality = SensoryModality::Tactile;
  mappings[2].prime_sequence = {5, 19, 29, 41};
  mappings[2].cross_modal_weight = 0.15f;
  mappings[2].resonance_frequency = 329.63f; // E4
  mappings[2].gml_shape_index = 2;           // Cube
  mappings[2].encoding_tensor = kernel->create_tensor({128});
  mappings[2].integration_tensor = kernel->create_tensor({64});

  // Olfactory: primes related to chemistry, molecules (23, 43, 47, 53)
  mappings[3].modality = SensoryModality::Olfactory;
  mappings[3].prime_sequence = {23, 43, 47, 53};
  mappings[3].cross_modal_weight = 0.08f;
  mappings[3].resonance_frequency = 392.0f; // G4
  mappings[3].gml_shape_index = 3;          // Tetrahedron
  mappings[3].encoding_tensor = kernel->create_tensor({128});
  mappings[3].integration_tensor = kernel->create_tensor({64});

  // Gustatory: primes related to taste, chemistry (59, 61, 67, 71)
  mappings[4].modality = SensoryModality::Gustatory;
  mappings[4].prime_sequence = {59, 61, 67, 71};
  mappings[4].cross_modal_weight = 0.07f;
  mappings[4].resonance_frequency = 493.88f; // B4
  mappings[4].gml_shape_index = 4;           // Octahedron
  mappings[4].encoding_tensor = kernel->create_tensor({128});
  mappings[4].integration_tensor = kernel->create_tensor({64});
}

NanoBrainTensor *SensoryPrimeMapper::map_input(SensoryModality modality,
                                               NanoBrainTensor *input) {
  int idx = static_cast<int>(modality);
  if (idx < 0 || idx >= 5 || !input)
    return nullptr;

  return apply_prime_encoding(input, mappings[idx].prime_sequence);
}

NanoBrainTensor *
SensoryPrimeMapper::apply_prime_encoding(NanoBrainTensor *input,
                                         const std::vector<int> &primes) {
  // Compute PPM-based coherence from primes
  float prime_product = 1.0f;
  float prime_sum = 0.0f;
  for (int p : primes) {
    prime_product *= static_cast<float>(p);
    prime_sum += static_cast<float>(p);
  }

  float coherence =
      0.5f + 0.5f * std::sin(std::sqrt(prime_product) * M_PI / prime_sum);

  // Scale input by coherence
  auto *scaled = kernel->mul_scalar(input, coherence);
  kernel->compute(scaled);
  return scaled;
}

std::vector<int>
SensoryPrimeMapper::get_prime_sequence(SensoryModality modality) const {
  int idx = static_cast<int>(modality);
  if (idx < 0 || idx >= 5)
    return {};
  return mappings[idx].prime_sequence;
}

CrossModalResult SensoryPrimeMapper::cross_modal_integrate(
    const std::map<SensoryModality, NanoBrainTensor *> &inputs) {

  CrossModalResult result;
  result.integrated_features.resize(64, 0.0f);
  result.coherence_score = 0.0f;

  float total_weight = 0.0f;

  for (const auto &[modality, tensor] : inputs) {
    int idx = static_cast<int>(modality);
    if (idx < 0 || idx >= 5 || !tensor)
      continue;

    float weight = mappings[idx].cross_modal_weight;
    result.modality_contributions[modality] = weight;
    total_weight += weight;

    // Encode and add weighted contribution
    auto *encoded = map_input(modality, tensor);
    if (encoded) {
      kernel->compute(encoded);
      // Sum contribution (simplified - in production would do proper fusion)
      for (size_t i = 0; i < result.integrated_features.size(); i++) {
        result.integrated_features[i] +=
            weight * kernel->get_value(encoded, i % 128);
      }
    }
  }

  // Normalize
  if (total_weight > 0) {
    for (float &f : result.integrated_features) {
      f /= total_weight;
    }
  }

  // Calculate coherence based on number of active modalities
  result.coherence_score = std::min(1.0f, total_weight);

  return result;
}

float SensoryPrimeMapper::calculate_cross_resonance(SensoryModality a,
                                                    SensoryModality b) const {
  int idx_a = static_cast<int>(a);
  int idx_b = static_cast<int>(b);
  if (idx_a < 0 || idx_a >= 5 || idx_b < 0 || idx_b >= 5)
    return 0.0f;

  // Resonance based on frequency ratio
  float freq_a = mappings[idx_a].resonance_frequency;
  float freq_b = mappings[idx_b].resonance_frequency;
  float ratio = std::max(freq_a, freq_b) / std::min(freq_a, freq_b);

  // Check for harmonic relationships
  float harmonic_score = 1.0f - std::fmod(ratio, 1.0f);
  return harmonic_score;
}

const SensoryPrimeMapping *
SensoryPrimeMapper::get_mapping(SensoryModality modality) const {
  int idx = static_cast<int>(modality);
  if (idx < 0 || idx >= 5)
    return nullptr;
  return &mappings[idx];
}

// ================================================================
// 7.2: MemoryCarrierSystem Implementation
// ================================================================

MemoryCarrierSystem::MemoryCarrierSystem(NanoBrainKernel *kernel,
                                         AttentionAllocationEngine *attention)
    : kernel(kernel), attention(attention), memory_counter(0) {}

MemoryCarrierSystem::~MemoryCarrierSystem() {}

void MemoryCarrierSystem::initialize() {
  memories.clear();
  memory_counter = 0;
  std::cout << "[MemoryCarrierSystem] Initialized 12 memory carrier types"
            << std::endl;
}

int64_t MemoryCarrierSystem::current_time_millis() const {
  return get_current_time_millis();
}

std::string MemoryCarrierSystem::generate_memory_id(MemoryCarrier type) {
  return "mem_" + std::to_string(static_cast<int>(type)) + "_" +
         std::to_string(memory_counter++);
}

float MemoryCarrierSystem::get_decay_rate(MemoryCarrier type) const {
  // Different decay rates for different memory types
  switch (type) {
  case MemoryCarrier::Working:
    return 0.1f; // Fast decay
  case MemoryCarrier::Episodic:
    return 0.01f;
  case MemoryCarrier::Semantic:
    return 0.001f; // Very slow decay
  case MemoryCarrier::Procedural:
    return 0.0001f; // Almost permanent
  case MemoryCarrier::Priming:
    return 0.05f;
  default:
    return 0.02f;
  }
}

std::string MemoryCarrierSystem::encode_memory(
    MemoryCarrier type, NanoBrainTensor *content,
    const std::vector<std::string> &associated_atoms) {
  std::string id = generate_memory_id(type);

  MemoryCarrierState state;
  state.type = type;
  state.id = id;
  state.content_tensor = content;
  state.activation_level = 1.0f;
  state.decay_rate = get_decay_rate(type);
  state.consolidation_strength = 0.0f;
  state.creation_time = current_time_millis();
  state.last_access = state.creation_time;
  state.access_count = 1;
  state.associated_atoms = associated_atoms;

  memories[id] = state;
  return id;
}

NanoBrainTensor *
MemoryCarrierSystem::retrieve_memory(const std::string &memory_id) {
  auto it = memories.find(memory_id);
  if (it == memories.end())
    return nullptr;

  it->second.last_access = current_time_millis();
  it->second.access_count++;
  it->second.activation_level =
      std::min(1.0f, it->second.activation_level + 0.1f);

  return it->second.content_tensor;
}

std::vector<std::string>
MemoryCarrierSystem::get_memories_by_type(MemoryCarrier type) const {
  std::vector<std::string> result;
  for (const auto &[id, state] : memories) {
    if (state.type == type) {
      result.push_back(id);
    }
  }
  return result;
}

void MemoryCarrierSystem::apply_decay(float time_delta) {
  auto it = memories.begin();
  while (it != memories.end()) {
    it->second.activation_level -= it->second.decay_rate * time_delta;

    if (it->second.activation_level <= 0.0f) {
      it = memories.erase(it);
    } else {
      ++it;
    }
  }
}

ConsolidationResult MemoryCarrierSystem::consolidate_memories() {
  ConsolidationResult result;
  result.consolidation_strength = 0.0f;
  result.memories_merged = 0;

  // Find working memories with high activation
  for (auto &[id, state] : memories) {
    if (state.type == MemoryCarrier::Working && state.activation_level > 0.7f) {
      // Consolidate to episodic
      state.consolidation_strength += 0.1f;

      if (state.consolidation_strength >= 1.0f) {
        // Convert to episodic memory
        state.type = MemoryCarrier::Episodic;
        state.consolidation_strength = 0.0f;
        state.decay_rate = get_decay_rate(MemoryCarrier::Episodic);
        result.consolidated_ids.push_back(id);
        result.memories_merged++;
      }
    }
  }

  if (!result.consolidated_ids.empty()) {
    result.consolidation_strength = 1.0f;
  }

  return result;
}

const MemoryCarrierState *
MemoryCarrierSystem::get_memory_state(const std::string &id) const {
  auto it = memories.find(id);
  if (it == memories.end())
    return nullptr;
  return &it->second;
}

void MemoryCarrierSystem::update_activations() {
  // Could integrate with attention engine here
}

MemoryCarrierSystem::CarrierStats MemoryCarrierSystem::get_stats() const {
  CarrierStats stats;
  stats.total_memories = memories.size();
  stats.memories_per_type.fill(0);
  stats.average_activation = 0.0f;
  stats.average_consolidation = 0.0f;

  for (const auto &[id, state] : memories) {
    int type_idx = static_cast<int>(state.type);
    if (type_idx >= 0 && type_idx < 12) {
      stats.memories_per_type[type_idx]++;
    }
    stats.average_activation += state.activation_level;
    stats.average_consolidation += state.consolidation_strength;
  }

  if (!memories.empty()) {
    stats.average_activation /= memories.size();
    stats.average_consolidation /= memories.size();
  }

  return stats;
}

// ================================================================
// 7.3: H3DecisionDevice Implementation
// ================================================================

H3DecisionDevice::H3DecisionDevice(NanoBrainKernel *kernel, int input_dim,
                                   int action_count)
    : kernel(kernel), input_dim(input_dim), action_count(action_count) {}

H3DecisionDevice::~H3DecisionDevice() {}

void H3DecisionDevice::initialize() {
  // Layer 1: input_dim -> 64
  init_layer(0, input_dim, 64);
  layers[0].confidence_threshold = 0.3f;

  // Layer 2: 64 -> 32
  init_layer(1, 64, 32);
  layers[1].confidence_threshold = 0.5f;

  // Layer 3: 32 -> action_count
  init_layer(2, 32, action_count);
  layers[2].confidence_threshold = 0.7f;

  // Voting weights
  voting_weights = kernel->create_tensor({3});
  std::vector<float> vote_data = {0.2f, 0.3f,
                                  0.5f}; // Later layers weighted more
  kernel->set_data(voting_weights, vote_data);

  std::cout << "[H3DecisionDevice] Initialized 3-layer decision architecture"
            << std::endl;
}

void H3DecisionDevice::init_layer(int idx, int in_dim, int out_dim) {
  layers[idx].layer_index = idx;
  layers[idx].weights = kernel->create_tensor({in_dim, out_dim});
  layers[idx].bias = kernel->create_tensor({out_dim});
  layers[idx].activation = nullptr;

  // Xavier initialization is handled by create_tensor
}

NanoBrainTensor *H3DecisionDevice::layer_forward(int layer_idx,
                                                 NanoBrainTensor *input) {
  if (layer_idx < 0 || layer_idx >= 3 || !input)
    return nullptr;

  auto &layer = layers[layer_idx];
  auto *output = kernel->matmul(input, layer.weights);
  output = kernel->add(output, layer.bias);
  output = kernel->softmax(output);
  kernel->compute(output);

  layer.activation = output;
  return output;
}

std::vector<float>
H3DecisionDevice::vote(const std::array<NanoBrainTensor *, 3> &layer_outputs) {
  std::vector<float> result(action_count, 0.0f);

  kernel->compute(voting_weights);

  for (int l = 0; l < 3; l++) {
    if (!layer_outputs[l])
      continue;

    float weight = kernel->get_value(voting_weights, l);
    int size = std::min(action_count,
                        static_cast<int>(layer_outputs[l]->ggml_tensor->ne[0]));

    for (int a = 0; a < size; a++) {
      result[a] += weight * kernel->get_value(layer_outputs[l], a);
    }
  }

  return result;
}

H3DecisionResult H3DecisionDevice::decide(NanoBrainTensor *input) {
  H3DecisionResult result;
  result.selected_action = 0;
  result.confidence = 0.0f;
  result.consensus_reached = false;

  std::array<NanoBrainTensor *, 3> layer_outputs;

  // Forward through each layer
  NanoBrainTensor *x = input;
  for (int l = 0; l < 3; l++) {
    x = layer_forward(l, x);
    layer_outputs[l] = x;

    // Calculate layer contribution
    if (x) {
      kernel->compute(x);
      float max_val = 0.0f;
      for (int i = 0; i < action_count; i++) {
        max_val = std::max(max_val, kernel->get_value(x, i));
      }
      result.layer_contributions[l] = max_val;
    }
  }

  // Vote across layers
  result.action_probabilities = vote(layer_outputs);

  // Find best action
  float max_prob = 0.0f;
  for (int a = 0; a < action_count; a++) {
    if (result.action_probabilities[a] > max_prob) {
      max_prob = result.action_probabilities[a];
      result.selected_action = a;
    }
  }
  result.confidence = max_prob;

  // Check consensus
  result.consensus_reached = (max_prob > layers[2].confidence_threshold);

  return result;
}

void H3DecisionDevice::update_weights(int layer_idx, NanoBrainTensor *gradient,
                                      float learning_rate) {
  if (layer_idx < 0 || layer_idx >= 3 || !gradient)
    return;

  auto *scaled_grad = kernel->mul_scalar(gradient, -learning_rate);
  layers[layer_idx].weights =
      kernel->add(layers[layer_idx].weights, scaled_grad);
  kernel->compute(layers[layer_idx].weights);
}

const DecisionLayer *H3DecisionDevice::get_layer(int idx) const {
  if (idx < 0 || idx >= 3)
    return nullptr;
  return &layers[idx];
}

void H3DecisionDevice::set_thresholds(float l1, float l2, float l3) {
  layers[0].confidence_threshold = l1;
  layers[1].confidence_threshold = l2;
  layers[2].confidence_threshold = l3;
}

// ================================================================
// 7.4: Brain Region Simulators
// ================================================================

BrainRegionSimulator::BrainRegionSimulator(NanoBrainKernel *kernel,
                                           BrainRegion region)
    : kernel(kernel), region(region) {
  state.region = region;
  state.activity_level = 0.0f;
  state.coherence = 1.0f;
}

BrainRegionSimulator::~BrainRegionSimulator() {}

// --- Cerebellum ---

CerebellumSimulator::CerebellumSimulator(NanoBrainKernel *kernel)
    : BrainRegionSimulator(kernel, BrainRegion::Cerebellum) {}

void CerebellumSimulator::initialize() {
  timing_model = kernel->create_tensor({64});
  coordination_weights = kernel->create_tensor({64, 64});
  output_tensor = kernel->create_tensor({64});
  std::cout << "[Cerebellum] Initialized" << std::endl;
}

void CerebellumSimulator::process_cycle(
    const std::vector<NodeTensor *> &inputs) {
  if (inputs.empty()) {
    state.activity_level = 0.0f;
    return;
  }

  // Aggregate motor-related inputs
  float total_activity = 0.0f;
  for (const auto *node : inputs) {
    if (node && node->embedding) {
      kernel->compute(node->embedding);
      total_activity += std::abs(kernel->get_value(node->embedding, 0));
    }
  }

  state.activity_level = std::min(1.0f, total_activity / inputs.size());
  state.coherence = 0.5f + 0.5f * std::cos(state.activity_level * M_PI);
}

NanoBrainTensor *CerebellumSimulator::get_output() const {
  return output_tensor;
}

float CerebellumSimulator::predict_timing(NanoBrainTensor *motor_input) {
  if (!motor_input)
    return 0.0f;
  kernel->compute(motor_input);
  return kernel->get_value(motor_input, 0) * 100.0f; // ms prediction
}

NanoBrainTensor *CerebellumSimulator::coordinate(
    const std::vector<NanoBrainTensor *> &motor_signals) {
  // Simple averaging for coordination
  if (motor_signals.empty())
    return output_tensor;

  std::vector<float> output(64, 0.0f);
  for (const auto *signal : motor_signals) {
    if (signal) {
      kernel->compute(signal);
      for (int i = 0; i < 64; i++) {
        output[i] += kernel->get_value(signal, i % signal->ggml_tensor->ne[0]);
      }
    }
  }

  for (float &v : output) {
    v /= motor_signals.size();
  }

  kernel->set_data(output_tensor, output);
  return output_tensor;
}

// --- Hippocampus ---

HippocampusSimulator::HippocampusSimulator(NanoBrainKernel *kernel)
    : BrainRegionSimulator(kernel, BrainRegion::Hippocampus) {}

void HippocampusSimulator::initialize() {
  place_cells = kernel->create_tensor({64});
  grid_cells = kernel->create_tensor({64});
  output_tensor = kernel->create_tensor({128});
  std::cout << "[Hippocampus] Initialized" << std::endl;
}

void HippocampusSimulator::process_cycle(
    const std::vector<NodeTensor *> &inputs) {
  if (inputs.empty()) {
    state.activity_level = 0.0f;
    return;
  }

  // Process spatial/memory inputs
  float total_activity = 0.0f;
  for (const auto *node : inputs) {
    if (node && node->embedding) {
      kernel->compute(node->embedding);
      total_activity += std::abs(kernel->get_value(node->embedding, 0));
    }
  }

  state.activity_level = std::min(1.0f, total_activity / inputs.size());
}

NanoBrainTensor *HippocampusSimulator::get_output() const {
  return output_tensor;
}

NanoBrainTensor *
HippocampusSimulator::encode_episodic(NanoBrainTensor *experience) {
  if (!experience)
    return nullptr;

  // Store in memory buffer
  memory_buffer.push_back(experience);
  if (memory_buffer.size() > 100) {
    memory_buffer.erase(memory_buffer.begin());
  }

  return experience;
}

NanoBrainTensor *
HippocampusSimulator::spatial_map(NanoBrainTensor *sensory_input) {
  if (!sensory_input)
    return place_cells;

  // Update place cells based on input
  kernel->compute(sensory_input);
  std::vector<float> place_data(64);
  for (int i = 0; i < 64; i++) {
    place_data[i] =
        std::sin(kernel->get_value(sensory_input,
                                   i % sensory_input->ggml_tensor->ne[0]) *
                 M_PI * (i + 1));
  }
  kernel->set_data(place_cells, place_data);

  return place_cells;
}

std::vector<NanoBrainTensor *>
HippocampusSimulator::replay_memories(int count) {
  std::vector<NanoBrainTensor *> result;
  int start = std::max(0, static_cast<int>(memory_buffer.size()) - count);
  for (size_t i = start; i < memory_buffer.size(); i++) {
    result.push_back(memory_buffer[i]);
  }
  return result;
}

// --- Hypothalamus ---

HypothalamusSimulator::HypothalamusSimulator(NanoBrainKernel *kernel)
    : BrainRegionSimulator(kernel, BrainRegion::Hypothalamus) {}

void HypothalamusSimulator::initialize() {
  drive_tensor = kernel->create_tensor({8}); // 8 basic drives
  output_tensor = kernel->create_tensor({32});

  // Initialize homeostatic setpoints
  homeostatic_setpoints["energy"] = 0.7f;
  homeostatic_setpoints["temperature"] = 0.5f;
  homeostatic_setpoints["arousal"] = 0.5f;
  homeostatic_setpoints["stress"] = 0.3f;

  // Current values start at setpoints
  current_values = homeostatic_setpoints;

  std::cout << "[Hypothalamus] Initialized" << std::endl;
}

void HypothalamusSimulator::process_cycle(
    const std::vector<NodeTensor *> &inputs) {
  // Calculate drive states based on deviation from setpoints
  std::vector<float> drives(8, 0.0f);
  int idx = 0;
  for (const auto &[system, setpoint] : homeostatic_setpoints) {
    if (idx < 8) {
      float deviation = current_values[system] - setpoint;
      drives[idx++] = std::abs(deviation);
    }
  }

  kernel->set_data(drive_tensor, drives);

  // Update activity level
  float total_drive = std::accumulate(drives.begin(), drives.end(), 0.0f);
  state.activity_level = std::min(1.0f, total_drive / 4.0f);
}

NanoBrainTensor *HypothalamusSimulator::get_output() const {
  return output_tensor;
}

NanoBrainTensor *
HypothalamusSimulator::calculate_drive(const std::string &drive_type) {
  return drive_tensor;
}

float HypothalamusSimulator::get_regulation_signal(const std::string &system) {
  auto it_set = homeostatic_setpoints.find(system);
  auto it_cur = current_values.find(system);
  if (it_set == homeostatic_setpoints.end() || it_cur == current_values.end()) {
    return 0.0f;
  }

  return it_set->second - it_cur->second;
}

// ================================================================
// 7.5: ConsciousnessExpressionEngine Implementation
// ================================================================

ConsciousnessExpressionEngine::ConsciousnessExpressionEngine(
    NanoBrainKernel *kernel)
    : kernel(kernel) {
  expression_templates.fill(nullptr);
}

ConsciousnessExpressionEngine::~ConsciousnessExpressionEngine() {}

void ConsciousnessExpressionEngine::initialize() {
  init_expression_templates();
  classifier_weights = kernel->create_tensor({64, 20});
  std::cout << "[ConsciousnessExpressionEngine] Initialized 20 expressions"
            << std::endl;
}

void ConsciousnessExpressionEngine::init_expression_templates() {
  // Create unique template for each expression
  for (int i = 0; i < 20; i++) {
    expression_templates[i] = kernel->create_tensor({64});
    std::vector<float> template_data(64);

    // Generate unique pattern for each expression
    for (int j = 0; j < 64; j++) {
      float phase =
          (i * 0.5f + j * 0.1f) * M_PI; // Unique phase for each expression
      template_data[j] = std::sin(phase) * 0.5f + 0.5f;
    }

    kernel->set_data(expression_templates[i], template_data);
  }
}

ExpressionClassification ConsciousnessExpressionEngine::classify_state(
    const SystemState &system_state,
    const std::vector<MetaCognitiveTensor> &meta_tensors) {

  ExpressionClassification result;
  result.expression_probabilities.fill(0.0f);

  // Build feature vector from system state
  std::vector<float> features = {
      system_state.cognitive_load, system_state.reasoning_activity,
      system_state.processing_efficiency,
      static_cast<float>(system_state.convergence_status) / 3.0f};

  // Add attention distribution stats
  if (!system_state.attention_distribution.empty()) {
    float mean_att =
        std::accumulate(system_state.attention_distribution.begin(),
                        system_state.attention_distribution.end(), 0.0f) /
        system_state.attention_distribution.size();
    features.push_back(mean_att);
  }

  // Add meta-tensor coherence
  float meta_coherence = 0.0f;
  for (const auto &mt : meta_tensors) {
    meta_coherence += mt.coherence;
  }
  if (!meta_tensors.empty()) {
    meta_coherence /= meta_tensors.size();
  }
  features.push_back(meta_coherence);

  // Simple classification based on features
  float load = system_state.cognitive_load;
  float efficiency = system_state.processing_efficiency;

  // Dodecanion states
  if (load > 0.7f && efficiency > 0.6f) {
    result.expression_probabilities[static_cast<int>(
        ConsciousExpression::Focus)] = 0.8f;
  } else if (load < 0.3f) {
    result.expression_probabilities[static_cast<int>(
        ConsciousExpression::Diffuse)] = 0.7f;
  }

  if (meta_coherence > 0.8f) {
    result.expression_probabilities[static_cast<int>(
        ConsciousExpression::Unified)] = 0.6f;
  } else if (meta_coherence < 0.3f) {
    result.expression_probabilities[static_cast<int>(
        ConsciousExpression::Fragmented)] = 0.6f;
  }

  // Octonion states
  if (system_state.convergence_status == ConvergenceState::Stable) {
    result
        .expression_probabilities[static_cast<int>(ConsciousExpression::Calm)] =
        0.5f;
  } else if (system_state.convergence_status == ConvergenceState::Critical) {
    result.expression_probabilities[static_cast<int>(
        ConsciousExpression::Anxious)] = 0.7f;
  }

  // Find primary and secondary expressions
  float max1 = 0.0f, max2 = 0.0f;
  int idx1 = 0, idx2 = 0;
  for (int i = 0; i < 20; i++) {
    if (result.expression_probabilities[i] > max1) {
      max2 = max1;
      idx2 = idx1;
      max1 = result.expression_probabilities[i];
      idx1 = i;
    } else if (result.expression_probabilities[i] > max2) {
      max2 = result.expression_probabilities[i];
      idx2 = i;
    }
  }

  result.primary_expression = static_cast<ConsciousExpression>(idx1);
  result.secondary_expression = static_cast<ConsciousExpression>(idx2);
  result.classification_confidence = max1;
  result.is_dodecanion = is_dodecanion_expression(result.primary_expression);

  return result;
}

NanoBrainTensor *ConsciousnessExpressionEngine::generate_expression(
    ConsciousExpression expression) {
  int idx = static_cast<int>(expression);
  if (idx < 0 || idx >= 20)
    return nullptr;
  return expression_templates[idx];
}

std::string
ConsciousnessExpressionEngine::get_expression_name(ConsciousExpression expr) {
  static const char *names[] = {
      "Focus",   "Diffuse",    "Creative",   "Analytical", "Emotional",
      "Logical", "Intuitive",  "Deliberate", "Receptive",  "Projective",
      "Unified", "Fragmented", "Alert",      "Drowsy",     "Euphoric",
      "Anxious", "Calm",       "Agitated",   "Present",    "Distant"};
  int idx = static_cast<int>(expr);
  if (idx < 0 || idx >= 20)
    return "Unknown";
  return names[idx];
}

bool ConsciousnessExpressionEngine::is_dodecanion_expression(
    ConsciousExpression expression) {
  return static_cast<int>(expression) < 12;
}

ConsciousExpression
ConsciousnessExpressionEngine::get_opposite(ConsciousExpression expression) {
  // Define opposite pairs
  static const std::map<ConsciousExpression, ConsciousExpression> opposites = {
      {ConsciousExpression::Focus, ConsciousExpression::Diffuse},
      {ConsciousExpression::Diffuse, ConsciousExpression::Focus},
      {ConsciousExpression::Creative, ConsciousExpression::Analytical},
      {ConsciousExpression::Analytical, ConsciousExpression::Creative},
      {ConsciousExpression::Emotional, ConsciousExpression::Logical},
      {ConsciousExpression::Logical, ConsciousExpression::Emotional},
      {ConsciousExpression::Intuitive, ConsciousExpression::Deliberate},
      {ConsciousExpression::Deliberate, ConsciousExpression::Intuitive},
      {ConsciousExpression::Receptive, ConsciousExpression::Projective},
      {ConsciousExpression::Projective, ConsciousExpression::Receptive},
      {ConsciousExpression::Unified, ConsciousExpression::Fragmented},
      {ConsciousExpression::Fragmented, ConsciousExpression::Unified},
      {ConsciousExpression::Alert, ConsciousExpression::Drowsy},
      {ConsciousExpression::Drowsy, ConsciousExpression::Alert},
      {ConsciousExpression::Euphoric, ConsciousExpression::Anxious},
      {ConsciousExpression::Anxious, ConsciousExpression::Euphoric},
      {ConsciousExpression::Calm, ConsciousExpression::Agitated},
      {ConsciousExpression::Agitated, ConsciousExpression::Calm},
      {ConsciousExpression::Present, ConsciousExpression::Distant},
      {ConsciousExpression::Distant, ConsciousExpression::Present}};

  auto it = opposites.find(expression);
  if (it != opposites.end()) {
    return it->second;
  }
  return expression;
}

NanoBrainTensor *ConsciousnessExpressionEngine::blend_expressions(
    ConsciousExpression a, ConsciousExpression b, float blend_factor) {
  auto *tensor_a = generate_expression(a);
  auto *tensor_b = generate_expression(b);

  if (!tensor_a || !tensor_b)
    return nullptr;

  auto *scaled_a = kernel->mul_scalar(tensor_a, 1.0f - blend_factor);
  auto *scaled_b = kernel->mul_scalar(tensor_b, blend_factor);
  auto *blended = kernel->add(scaled_a, scaled_b);
  kernel->compute(blended);

  return blended;
}

// ================================================================
// IntegratedBrainModel Implementation
// ================================================================

IntegratedBrainModel::IntegratedBrainModel(
    NanoBrainKernel *kernel, AttentionAllocationEngine *attention,
    RecursiveReasoningEngine *reasoning,
    MetaCognitiveFeedbackEngine *metacognitive,
    const IntegratedBrainConfig &config)
    : kernel(kernel), attention(attention), reasoning(reasoning),
      metacognitive(metacognitive), config(config) {}

IntegratedBrainModel::~IntegratedBrainModel() { shutdown(); }

int64_t IntegratedBrainModel::current_time_millis() const {
  return get_current_time_millis();
}

void IntegratedBrainModel::initialize() {
  if (active)
    return;

  start_time = current_time_millis();

  // Initialize subsystems
  sensory_mapper = std::make_unique<SensoryPrimeMapper>(kernel);
  sensory_mapper->initialize();

  memory_system = std::make_unique<MemoryCarrierSystem>(kernel, attention);
  memory_system->initialize();

  decision_device = std::make_unique<H3DecisionDevice>(
      kernel, config.sensory_dim, config.action_count);
  decision_device->initialize();

  expression_engine = std::make_unique<ConsciousnessExpressionEngine>(kernel);
  expression_engine->initialize();

  // Initialize brain regions
  cerebellum = std::make_unique<CerebellumSimulator>(kernel);
  cerebellum->initialize();

  hippocampus = std::make_unique<HippocampusSimulator>(kernel);
  hippocampus->initialize();

  hypothalamus = std::make_unique<HypothalamusSimulator>(kernel);
  hypothalamus->initialize();

  active = true;

  std::cout
      << "[IntegratedBrainModel] Initialized with all Chapter 7 components"
      << std::endl;
}

void IntegratedBrainModel::shutdown() {
  if (!active)
    return;

  expression_engine.reset();
  hypothalamus.reset();
  hippocampus.reset();
  cerebellum.reset();
  decision_device.reset();
  memory_system.reset();
  sensory_mapper.reset();

  active = false;
}

NanoBrainTensor *
IntegratedBrainModel::process_sensory_input(SensoryModality modality,
                                            NanoBrainTensor *input) {
  if (!sensory_mapper || !input)
    return nullptr;
  return sensory_mapper->map_input(modality, input);
}

CrossModalResult IntegratedBrainModel::process_multimodal(
    const std::map<SensoryModality, NanoBrainTensor *> &inputs) {
  if (!sensory_mapper)
    return CrossModalResult{};
  return sensory_mapper->cross_modal_integrate(inputs);
}

std::string IntegratedBrainModel::store_memory(MemoryCarrier type,
                                               NanoBrainTensor *content) {
  if (!memory_system || !content)
    return "";
  return memory_system->encode_memory(type, content, {});
}

NanoBrainTensor *
IntegratedBrainModel::recall_memory(const std::string &memory_id) {
  if (!memory_system)
    return nullptr;
  return memory_system->retrieve_memory(memory_id);
}

void IntegratedBrainModel::consolidate() {
  if (memory_system) {
    memory_system->consolidate_memories();
  }
}

H3DecisionResult
IntegratedBrainModel::make_decision(NanoBrainTensor *situation) {
  if (!decision_device || !situation)
    return H3DecisionResult{};
  return decision_device->decide(situation);
}

void IntegratedBrainModel::process_brain_regions(
    const std::vector<NodeTensor *> &node_tensors) {
  if (cerebellum)
    cerebellum->process_cycle(node_tensors);
  if (hippocampus)
    hippocampus->process_cycle(node_tensors);
  if (hypothalamus)
    hypothalamus->process_cycle(node_tensors);
}

NanoBrainTensor *IntegratedBrainModel::get_region_output(BrainRegion region) {
  switch (region) {
  case BrainRegion::Cerebellum:
    return cerebellum ? cerebellum->get_output() : nullptr;
  case BrainRegion::Hippocampus:
    return hippocampus ? hippocampus->get_output() : nullptr;
  case BrainRegion::Hypothalamus:
    return hypothalamus ? hypothalamus->get_output() : nullptr;
  default:
    return nullptr;
  }
}

ExpressionClassification IntegratedBrainModel::get_current_expression() {
  if (!expression_engine || !metacognitive)
    return ExpressionClassification{};

  SystemState state = metacognitive->get_current_state();
  auto meta_tensors = metacognitive->get_meta_tensors();

  return expression_engine->classify_state(state, meta_tensors);
}

void IntegratedBrainModel::set_target_expression(ConsciousExpression target) {
  target_expression = target;
}

IntegratedBrainMetrics IntegratedBrainModel::process_cycle(
    const std::vector<NodeTensor *> &node_tensors,
    const std::vector<LinkTensor *> &link_tensors) {

  cycle_count++;

  // Process brain regions
  process_brain_regions(node_tensors);

  // Apply memory decay
  if (memory_system) {
    memory_system->apply_decay(1.0f);
  }

  // Consolidate periodically
  if (cycle_count % 10 == 0) {
    consolidate();
  }

  return get_metrics();
}

IntegratedBrainMetrics IntegratedBrainModel::get_metrics() const {
  IntegratedBrainMetrics metrics;

  // Sensory activations (placeholder)
  metrics.sensory_activations.fill(0.5f);
  metrics.cross_modal_coherence = 0.7f;

  // Memory stats
  if (memory_system) {
    auto mem_stats = memory_system->get_stats();
    metrics.total_memories = mem_stats.total_memories;
    metrics.average_memory_activation = mem_stats.average_activation;
    metrics.consolidation_rate = mem_stats.average_consolidation;
  }

  // Decision stats
  metrics.decision_confidence = 0.0f;
  metrics.decisions_made = cycle_count;

  // Brain region activities
  if (cerebellum) {
    metrics.region_activities[BrainRegion::Cerebellum] =
        cerebellum->get_state().activity_level;
  }
  if (hippocampus) {
    metrics.region_activities[BrainRegion::Hippocampus] =
        hippocampus->get_state().activity_level;
  }
  if (hypothalamus) {
    metrics.region_activities[BrainRegion::Hypothalamus] =
        hypothalamus->get_state().activity_level;
  }

  // Overall brain coherence
  float total_coherence = 0.0f;
  int region_count = 0;
  if (cerebellum) {
    total_coherence += cerebellum->get_state().coherence;
    region_count++;
  }
  if (hippocampus) {
    total_coherence += hippocampus->get_state().coherence;
    region_count++;
  }
  if (hypothalamus) {
    total_coherence += hypothalamus->get_state().coherence;
    region_count++;
  }
  metrics.overall_brain_coherence =
      region_count > 0 ? total_coherence / region_count : 0.0f;

  // Consciousness
  metrics.current_expression = ConsciousExpression::Focus;
  metrics.expression_stability = 0.8f;

  return metrics;
}
