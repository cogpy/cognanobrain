#include "nanobrain_metacognitive.h"
#include <algorithm>
#include <chrono>
#include <cmath>
#include <iostream>
#include <numeric>
#include <sstream>

// ================================================================
// Constructor / Destructor
// ================================================================

MetaCognitiveFeedbackEngine::MetaCognitiveFeedbackEngine(
    NanoBrainKernel *kernel, AttentionAllocationEngine *attention_engine,
    RecursiveReasoningEngine *reasoning_engine,
    const MetaCognitiveConfig &config)
    : kernel(kernel), attention_engine(attention_engine),
      reasoning_engine(reasoning_engine), config(config), cycle_counter(0),
      modification_counter(0), start_time(0) {}

MetaCognitiveFeedbackEngine::~MetaCognitiveFeedbackEngine() { reset(); }

int64_t MetaCognitiveFeedbackEngine::current_time_millis() const {
  return std::chrono::duration_cast<std::chrono::milliseconds>(
             std::chrono::system_clock::now().time_since_epoch())
      .count();
}

std::string MetaCognitiveFeedbackEngine::generate_membrane_id() {
  static int counter = 0;
  std::stringstream ss;
  ss << "membrane_" << counter++;
  return ss.str();
}

std::string MetaCognitiveFeedbackEngine::generate_modification_id() {
  std::stringstream ss;
  ss << "mod_" << modification_counter++;
  return ss.str();
}

// ================================================================
// Initialization
// ================================================================

void MetaCognitiveFeedbackEngine::initialize() {
  start_time = current_time_millis();

  // Initialize meta-cognitive tensor hierarchy
  initialize_meta_cognitive_tensors();

  // Initialize P-System membranes
  initialize_membranes();

  // Initialize feedback loops
  initialize_feedback_loops();

  std::cout << "[MetaCognitive] Initialized with " << config.meta_levels
            << " meta-levels" << std::endl;
}

void MetaCognitiveFeedbackEngine::initialize_meta_cognitive_tensors() {
  meta_tensors.clear();

  for (int level = 0; level < config.meta_levels; level++) {
    MetaCognitiveTensor meta_tensor;
    meta_tensor.level = level;

    // Higher levels have smaller representation (pyramid)
    int tensor_size = 128 >> level; // 128, 64, 32, ...
    tensor_size = std::max(16, tensor_size);

    meta_tensor.activation_pattern =
        kernel->create_tensor({static_cast<int64_t>(tensor_size)});
    meta_tensor.self_model =
        kernel->create_tensor({static_cast<int64_t>(tensor_size)});
    meta_tensor.feedback_signal =
        kernel->create_tensor({static_cast<int64_t>(tensor_size)});
    meta_tensor.coherence = 1.0f;
    meta_tensor.last_updated = current_time_millis();

    meta_tensors.push_back(meta_tensor);
  }
}

void MetaCognitiveFeedbackEngine::initialize_membranes() {
  membranes.clear();

  // Create hierarchical membrane structure (P-System inspired)
  // Level 0: Root membrane
  CognitiveMembrane root;
  root.id = generate_membrane_id();
  root.level = 0;
  root.permeability = 1.0f;
  root.state_tensor = kernel->create_tensor({64});

  // Level 1: Component membranes
  std::vector<std::string> components = {"attention", "reasoning", "encoding",
                                         "memory"};

  for (const auto &comp : components) {
    CognitiveMembrane membrane;
    membrane.id = generate_membrane_id();
    membrane.level = 1;
    membrane.permeability = config.membrane_permeability;
    membrane.state_tensor = kernel->create_tensor({32});
    root.child_membranes.push_back(membrane.id);
    membranes.push_back(membrane);
  }

  membranes.insert(membranes.begin(), root);
}

void MetaCognitiveFeedbackEngine::initialize_feedback_loops() {
  feedback_loops.clear();

  // Create feedback loops between components
  std::vector<std::pair<std::string, std::string>> loop_pairs = {
      {"attention", "reasoning"},
      {"reasoning", "attention"},
      {"attention", "encoding"},
      {"reasoning", "memory"},
      {"memory", "attention"}};

  int loop_id = 0;
  for (const auto &[source, target] : loop_pairs) {
    FeedbackLoop loop;
    loop.id = "loop_" + std::to_string(loop_id++);
    loop.source_component = source;
    loop.target_component = target;
    loop.feedback_tensor = kernel->create_tensor({32});
    loop.feedback_strength = 0.5f;
    loop.loop_type = (source == target) ? "oscillatory" : "negative";
    loop.stability_measure = 1.0f;

    feedback_loops.push_back(loop);
  }
}

// ================================================================
// Main Update Cycle
// ================================================================

MetaCognitiveMetrics MetaCognitiveFeedbackEngine::update_meta_cognitive(
    const std::vector<NodeTensor *> &node_tensors,
    const std::vector<LinkTensor *> &link_tensors,
    const AttentionStats &attention_stats,
    const ReasoningStats &reasoning_stats) {

  cycle_counter++;

  // 1. Monitor current system state
  SystemState current_state = monitor_system_state(
      node_tensors, link_tensors, attention_stats, reasoning_stats);

  // 2. Update meta-cognitive tensors
  update_meta_tensors(current_state, node_tensors, link_tensors);

  // 3. Process feedback loops
  process_feedback_loops(current_state);

  // 4. Update membrane states
  update_membrane_states(node_tensors);

  // 5. Calculate metrics
  MetaCognitiveMetrics metrics = calculate_metrics(current_state);

  // 6. Perform self-modification if needed
  if (cycle_counter % config.self_monitoring_frequency == 0) {
    perform_self_modification(current_state, metrics);
  }

  // 7. Store state history
  state_history.push_back(current_state);
  if (state_history.size() > max_history_size) {
    state_history.erase(state_history.begin());
  }

  return metrics;
}

SystemState MetaCognitiveFeedbackEngine::monitor_system_state(
    const std::vector<NodeTensor *> &node_tensors,
    const std::vector<LinkTensor *> &link_tensors,
    const AttentionStats &attention_stats,
    const ReasoningStats &reasoning_stats) {

  SystemState state;
  state.timestamp = current_time_millis();

  // Calculate cognitive load based on attention and reasoning activity
  state.cognitive_load =
      (attention_stats.resource_utilization +
       std::min(1.0f, reasoning_stats.reasoning_throughput / 100.0f)) /
      2.0f;

  // Build attention distribution
  state.attention_distribution.clear();
  for (const auto *node : node_tensors) {
    if (node && node->attention_weights) {
      kernel->compute(node->attention_weights);
      state.attention_distribution.push_back(
          kernel->get_value(node->attention_weights, 0));
    }
  }

  // Reasoning activity
  state.reasoning_activity =
      std::min(1.0f, static_cast<float>(reasoning_stats.active_chains) / 10.0f);

  // Memory usage (estimated from tensor count)
  state.memory_usage = static_cast<float>(node_tensors.size() * 128 * 4) /
                       (1024.0f * 1024.0f); // MB

  // Processing efficiency
  if (state.cognitive_load > 0) {
    float output =
        attention_stats.total_attention + reasoning_stats.total_inferences;
    state.processing_efficiency =
        std::min(1.0f, output / (state.cognitive_load * 1000.0f));
  } else {
    state.processing_efficiency = 0.0f;
  }

  // Detect convergence
  state.convergence_status = detect_convergence(state);

  return state;
}

// ================================================================
// Self-Monitoring
// ================================================================

void MetaCognitiveFeedbackEngine::update_meta_tensors(
    const SystemState &system_state,
    const std::vector<NodeTensor *> &node_tensors,
    const std::vector<LinkTensor *> &link_tensors) {

  for (auto &meta_tensor : meta_tensors) {
    // Update activation patterns
    update_activation_patterns(meta_tensor, node_tensors, link_tensors,
                               meta_tensor.level);

    // Update self-monitoring
    update_self_monitoring(meta_tensor, system_state, meta_tensor.level);

    // Update feedback tensor
    update_feedback_tensor(meta_tensor, system_state, meta_tensor.level);

    meta_tensor.last_updated = current_time_millis();
  }
}

void MetaCognitiveFeedbackEngine::update_activation_patterns(
    MetaCognitiveTensor &meta_tensor,
    const std::vector<NodeTensor *> &node_tensors,
    const std::vector<LinkTensor *> &link_tensors, int level) {

  if (!meta_tensor.activation_pattern)
    return;

  int64_t size = meta_tensor.activation_pattern->ggml_tensor->ne[0];
  std::vector<float> pattern(size, 0.0f);

  // Aggregate node activations at this meta-level
  size_t nodes_per_bin = std::max(size_t(1), node_tensors.size() / size);

  for (size_t i = 0;
       i < node_tensors.size() && i / nodes_per_bin < (size_t)size; i++) {
    const auto *node = node_tensors[i];
    if (node && node->embedding) {
      kernel->compute(node->embedding);
      float activation = kernel->get_value(node->embedding, 0);
      pattern[i / nodes_per_bin] += std::abs(activation);
    }
  }

  // Normalize by level (higher levels more abstract)
  float level_factor = 1.0f / (level + 1);
  for (float &p : pattern) {
    p *= level_factor;
  }

  kernel->set_data(meta_tensor.activation_pattern, pattern);
}

void MetaCognitiveFeedbackEngine::update_self_monitoring(
    MetaCognitiveTensor &meta_tensor, const SystemState &system_state,
    int level) {

  if (!meta_tensor.self_model)
    return;

  int64_t size = meta_tensor.self_model->ggml_tensor->ne[0];
  std::vector<float> self_model(size, 0.0f);

  // Encode system state into self-model
  if (size >= 6) {
    self_model[0] = system_state.cognitive_load;
    self_model[1] = system_state.reasoning_activity;
    self_model[2] = system_state.memory_usage / 100.0f; // Normalize
    self_model[3] = system_state.processing_efficiency;
    self_model[4] = static_cast<float>(system_state.convergence_status) / 3.0f;
    self_model[5] = static_cast<float>(level) / config.meta_levels;
  }

  // Fill remaining with attention distribution summary
  if (!system_state.attention_distribution.empty() && size > 6) {
    float mean_attention =
        std::accumulate(system_state.attention_distribution.begin(),
                        system_state.attention_distribution.end(), 0.0f) /
        system_state.attention_distribution.size();

    for (size_t i = 6; i < (size_t)size; i++) {
      self_model[i] = mean_attention * ((float)(i - 5) / (size - 6));
    }
  }

  kernel->set_data(meta_tensor.self_model, self_model);
}

void MetaCognitiveFeedbackEngine::update_feedback_tensor(
    MetaCognitiveTensor &meta_tensor, const SystemState &system_state,
    int level) {

  if (!meta_tensor.feedback_signal || !meta_tensor.self_model)
    return;

  int64_t size = meta_tensor.feedback_signal->ggml_tensor->ne[0];
  std::vector<float> feedback(size, 0.0f);

  // Generate feedback based on deviation from optimal state
  float optimal_load = 0.7f;
  float load_error = system_state.cognitive_load - optimal_load;

  float optimal_efficiency = 0.8f;
  float efficiency_error =
      optimal_efficiency - system_state.processing_efficiency;

  // Feedback signal encodes error
  for (size_t i = 0; i < (size_t)size; i++) {
    float position = static_cast<float>(i) / size;
    feedback[i] = load_error * std::sin(position * M_PI) +
                  efficiency_error * std::cos(position * M_PI);
    feedback[i] *= config.feedback_damping;
  }

  kernel->set_data(meta_tensor.feedback_signal, feedback);

  // Update coherence based on feedback magnitude
  float feedback_magnitude = 0.0f;
  for (float f : feedback) {
    feedback_magnitude += std::abs(f);
  }
  meta_tensor.coherence = std::max(0.0f, 1.0f - (feedback_magnitude / size));
}

// ================================================================
// Feedback Processing
// ================================================================

void MetaCognitiveFeedbackEngine::process_feedback_loops(
    const SystemState &system_state) {

  for (auto &loop : feedback_loops) {
    // Update feedback loop
    update_feedback_loop(loop, system_state);

    // Apply feedback to target
    apply_feedback(loop);

    // Update stability measure
    loop.stability_measure = check_loop_stability(loop);
  }
}

void MetaCognitiveFeedbackEngine::update_feedback_loop(
    FeedbackLoop &loop, const SystemState &state) {

  if (!loop.feedback_tensor)
    return;

  int64_t size = loop.feedback_tensor->ggml_tensor->ne[0];
  std::vector<float> feedback_data(size, 0.0f);

  // Generate feedback based on source component state
  float source_activity = 0.0f;
  if (loop.source_component == "attention") {
    source_activity = std::accumulate(state.attention_distribution.begin(),
                                      state.attention_distribution.end(), 0.0f);
  } else if (loop.source_component == "reasoning") {
    source_activity = state.reasoning_activity;
  } else if (loop.source_component == "memory") {
    source_activity = state.memory_usage;
  } else if (loop.source_component == "encoding") {
    source_activity = state.processing_efficiency;
  }

  // Calculate feedback
  for (size_t i = 0; i < (size_t)size; i++) {
    float phase = 2.0f * M_PI * i / size;

    if (loop.loop_type == "positive") {
      feedback_data[i] = source_activity * std::sin(phase);
    } else if (loop.loop_type == "negative") {
      feedback_data[i] = -source_activity * std::sin(phase);
    } else { // oscillatory
      feedback_data[i] =
          source_activity * std::sin(phase + cycle_counter * 0.1f);
    }

    feedback_data[i] *= loop.feedback_strength * config.feedback_damping;
  }

  kernel->set_data(loop.feedback_tensor, feedback_data);
}

void MetaCognitiveFeedbackEngine::apply_feedback(const FeedbackLoop &loop) {
  // In a full implementation, this would adjust target component parameters
  // For now, we track the feedback strength

  if (loop.target_component == "attention" && attention_engine) {
    // Could adjust attention configuration based on feedback
  } else if (loop.target_component == "reasoning" && reasoning_engine) {
    // Could adjust reasoning configuration based on feedback
  }
}

float MetaCognitiveFeedbackEngine::check_loop_stability(
    const FeedbackLoop &loop) {

  if (!loop.feedback_tensor)
    return 0.0f;

  // Calculate variance of feedback signal
  kernel->compute(loop.feedback_tensor);

  int64_t size = loop.feedback_tensor->ggml_tensor->ne[0];
  float sum = 0.0f;
  float sum_sq = 0.0f;

  for (int i = 0; i < size; i++) {
    float val = kernel->get_value(loop.feedback_tensor, i);
    sum += val;
    sum_sq += val * val;
  }

  float mean = sum / size;
  float variance = (sum_sq / size) - (mean * mean);

  // Lower variance = more stable
  return std::max(0.0f, 1.0f - std::sqrt(std::abs(variance)));
}

// ================================================================
// Self-Modification
// ================================================================

void MetaCognitiveFeedbackEngine::perform_self_modification(
    const SystemState &system_state, const MetaCognitiveMetrics &metrics) {

  // Check if adaptation is needed
  if (metrics.system_coherence > 0.9f &&
      system_state.convergence_status == ConvergenceState::Stable) {
    return; // System is performing well, no modification needed
  }

  // Handle critical states
  if (system_state.convergence_status == ConvergenceState::Critical) {
    handle_critical_state(system_state);
    return;
  }

  // Adjust parameters based on metrics
  adjust_parameters(metrics);
}

void MetaCognitiveFeedbackEngine::adjust_parameters(
    const MetaCognitiveMetrics &metrics) {

  NanoBrainTensor *mod_tensor = kernel->create_tensor({4});
  std::vector<float> adjustments(4, 0.0f);

  // Adjust based on various metrics
  if (metrics.self_awareness_level < 0.5f) {
    adjustments[0] = config.adaptation_learning_rate;
    // Could increase meta_attention_allocation
  }

  if (metrics.feedback_loop_stability < 0.5f) {
    adjustments[1] = -config.adaptation_learning_rate;
    // Could adjust feedback_damping
  }

  if (metrics.adaptation_rate > 0.5f) {
    adjustments[2] = -config.adaptation_learning_rate * 0.5f;
    // Could reduce plasticity to stabilize
  }

  if (metrics.convergence_rate < 0.3f) {
    adjustments[3] = config.adaptation_learning_rate;
    // Could adjust convergence_threshold
  }

  kernel->set_data(mod_tensor, adjustments);
  record_self_modification("parameter_adjustment", "config", mod_tensor,
                           metrics.system_coherence);
}

void MetaCognitiveFeedbackEngine::record_self_modification(
    const std::string &type, const std::string &target, NanoBrainTensor *tensor,
    float effectiveness) {

  SelfModification mod;
  mod.id = generate_modification_id();
  mod.timestamp = current_time_millis();
  mod.modification_type = type;
  mod.target_component = target;
  mod.modification_tensor = tensor;
  mod.effectiveness_score = effectiveness;
  mod.reversible = true;

  modifications.push_back(mod);

  // Limit modification history
  size_t max_modifications = 100;
  if (modifications.size() > max_modifications) {
    modifications.erase(modifications.begin());
  }
}

float MetaCognitiveFeedbackEngine::evaluate_modification_effectiveness(
    const SelfModification &mod) {
  // Compare system performance before/after modification
  return mod.effectiveness_score;
}

void MetaCognitiveFeedbackEngine::revert_modifications(
    float effectiveness_threshold) {

  // Remove ineffective modifications (in reverse order)
  auto it = modifications.rbegin();
  while (it != modifications.rend()) {
    if (it->effectiveness_score < effectiveness_threshold && it->reversible) {
      // Mark for removal
      it = std::reverse_iterator(modifications.erase(std::next(it).base()));
    } else {
      ++it;
    }
  }
}

// ================================================================
// P-System Membrane Operations
// ================================================================

void MetaCognitiveFeedbackEngine::update_membrane_states(
    const std::vector<NodeTensor *> &node_tensors) {

  // Update each membrane's state based on contained atoms
  for (auto &membrane : membranes) {
    if (!membrane.state_tensor)
      continue;

    int64_t size = membrane.state_tensor->ggml_tensor->ne[0];
    std::vector<float> state_data(size, 0.0f);

    // Aggregate state from contained atoms
    for (size_t i = 0; i < node_tensors.size(); i++) {
      const auto *node = node_tensors[i];
      if (!node || !node->embedding)
        continue;

      size_t bin = i % size;
      kernel->compute(node->embedding);
      state_data[bin] += kernel->get_value(node->embedding, 0);
    }

    // Normalize
    for (float &s : state_data) {
      s /= std::max(1.0f, static_cast<float>(node_tensors.size() / size));
    }

    kernel->set_data(membrane.state_tensor, state_data);
  }

  // Apply membrane rules
  apply_membrane_rules();
}

void MetaCognitiveFeedbackEngine::apply_membrane_rules() {
  // Transfer between membranes based on permeability
  for (size_t i = 0; i < membranes.size(); i++) {
    auto &source = membranes[i];

    for (const auto &child_id : source.child_membranes) {
      // Find child membrane
      for (auto &target : membranes) {
        if (target.id == child_id) {
          float flow = calculate_membrane_flow(source, target);
          // Apply flow (would modify membrane states)
          break;
        }
      }
    }
  }
}

float MetaCognitiveFeedbackEngine::calculate_membrane_flow(
    const CognitiveMembrane &source, const CognitiveMembrane &target) {

  if (!source.state_tensor || !target.state_tensor)
    return 0.0f;

  // Calculate pressure differential
  kernel->compute(source.state_tensor);
  kernel->compute(target.state_tensor);

  auto *source_sum = kernel->sum(source.state_tensor);
  auto *target_sum = kernel->sum(target.state_tensor);
  kernel->compute(source_sum);
  kernel->compute(target_sum);

  float source_pressure = kernel->get_value(source_sum, 0);
  float target_pressure = kernel->get_value(target_sum, 0);

  // Flow based on pressure difference and permeability
  float flow = (source_pressure - target_pressure) * target.permeability;
  return flow;
}

// ================================================================
// Convergence Detection
// ================================================================

ConvergenceState MetaCognitiveFeedbackEngine::detect_convergence(
    const SystemState &current_state) {

  if (state_history.empty()) {
    return ConvergenceState::Adapting;
  }

  // Compare with recent history
  float avg_change = 0.0f;
  size_t compare_count = std::min(size_t(10), state_history.size());

  for (size_t i = state_history.size() - compare_count;
       i < state_history.size(); i++) {
    const auto &prev = state_history[i];
    avg_change += std::abs(current_state.cognitive_load - prev.cognitive_load);
    avg_change += std::abs(current_state.processing_efficiency -
                           prev.processing_efficiency);
  }
  avg_change /= (compare_count * 2);

  // Classify convergence state
  if (avg_change < config.convergence_threshold) {
    return ConvergenceState::Stable;
  } else if (avg_change < config.convergence_threshold * 10) {
    return ConvergenceState::Adapting;
  } else if (avg_change < config.convergence_threshold * 100) {
    return ConvergenceState::Diverging;
  } else {
    return ConvergenceState::Critical;
  }
}

float MetaCognitiveFeedbackEngine::calculate_convergence_rate() {
  if (state_history.size() < 2)
    return 0.0f;

  // Calculate rate of change reduction
  float early_change = 0.0f;
  float late_change = 0.0f;

  size_t mid = state_history.size() / 2;

  for (size_t i = 1; i < mid; i++) {
    early_change += std::abs(state_history[i].cognitive_load -
                             state_history[i - 1].cognitive_load);
  }

  for (size_t i = mid + 1; i < state_history.size(); i++) {
    late_change += std::abs(state_history[i].cognitive_load -
                            state_history[i - 1].cognitive_load);
  }

  if (early_change > 0) {
    return std::max(0.0f, (early_change - late_change) / early_change);
  }
  return 0.0f;
}

void MetaCognitiveFeedbackEngine::handle_critical_state(
    const SystemState &state) {

  std::cout << "[MetaCognitive] CRITICAL STATE detected - initiating recovery"
            << std::endl;

  // Reset feedback loops
  for (auto &loop : feedback_loops) {
    loop.feedback_strength *= 0.5f;
    loop.stability_measure = 1.0f;
  }

  // Reduce plasticity temporarily
  config.plasticity_factor *= 0.5f;

  // Log modification
  record_self_modification("structure_change", "feedback_loops", nullptr, 0.3f);
}

// ================================================================
// Metrics Calculation
// ================================================================

MetaCognitiveMetrics
MetaCognitiveFeedbackEngine::calculate_metrics(const SystemState &state) {

  MetaCognitiveMetrics metrics;

  metrics.self_awareness_level = calculate_self_awareness(meta_tensors);
  metrics.adaptation_rate =
      static_cast<float>(modifications.size()) / std::max(1, cycle_counter);
  metrics.system_coherence = calculate_system_coherence();
  metrics.meta_attention_efficiency =
      config.meta_attention_allocation * state.processing_efficiency;
  metrics.feedback_loop_stability = calculate_feedback_stability();
  metrics.self_modification_frequency = metrics.adaptation_rate;
  metrics.convergence_rate = calculate_convergence_rate();

  return metrics;
}

float MetaCognitiveFeedbackEngine::calculate_self_awareness(
    const std::vector<MetaCognitiveTensor> &tensors) {

  if (tensors.empty())
    return 0.0f;

  float total_coherence = 0.0f;
  for (const auto &tensor : tensors) {
    total_coherence += tensor.coherence;
  }

  return total_coherence / tensors.size();
}

float MetaCognitiveFeedbackEngine::calculate_system_coherence() {
  float total = 0.0f;

  // Meta-tensor coherence
  for (const auto &tensor : meta_tensors) {
    total += tensor.coherence;
  }

  // Feedback loop stability
  for (const auto &loop : feedback_loops) {
    total += loop.stability_measure;
  }

  int count = meta_tensors.size() + feedback_loops.size();
  return count > 0 ? total / count : 0.0f;
}

float MetaCognitiveFeedbackEngine::calculate_feedback_stability() {
  if (feedback_loops.empty())
    return 1.0f;

  float total = 0.0f;
  for (const auto &loop : feedback_loops) {
    total += loop.stability_measure;
  }

  return total / feedback_loops.size();
}

// ================================================================
// State Access
// ================================================================

std::vector<FeedbackLoop>
MetaCognitiveFeedbackEngine::get_feedback_loops() const {
  return feedback_loops;
}

std::vector<SelfModification>
MetaCognitiveFeedbackEngine::get_self_modifications() const {
  return modifications;
}

SystemState MetaCognitiveFeedbackEngine::get_current_state() const {
  if (state_history.empty()) {
    return SystemState{};
  }
  return state_history.back();
}

std::vector<MetaCognitiveTensor>
MetaCognitiveFeedbackEngine::get_meta_tensors() const {
  return meta_tensors;
}

void MetaCognitiveFeedbackEngine::update_config(
    const MetaCognitiveConfig &new_config) {
  config = new_config;

  // Re-initialize if meta-levels changed
  if (new_config.meta_levels != (int)meta_tensors.size()) {
    initialize_meta_cognitive_tensors();
  }
}

void MetaCognitiveFeedbackEngine::reset() {
  meta_tensors.clear();
  membranes.clear();
  feedback_loops.clear();
  modifications.clear();
  state_history.clear();
  cycle_counter = 0;
  modification_counter = 0;

  std::cout << "[MetaCognitive] Reset" << std::endl;
}
