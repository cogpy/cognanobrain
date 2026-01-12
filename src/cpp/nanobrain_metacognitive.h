#ifndef NANOBRAIN_METACOGNITIVE_H
#define NANOBRAIN_METACOGNITIVE_H

#include "nanobrain_attention.h"
#include "nanobrain_kernel.h"
#include "nanobrain_reasoning.h"
#include "nanobrain_types.h"
#include <array>
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

/**
 * Meta-cognitive system configuration
 */
struct MetaCognitiveConfig {
  int meta_levels = 3;                    // Number of meta-cognitive levels
  int self_monitoring_frequency = 10;     // Monitoring cycles per update
  float adaptation_learning_rate = 0.01f; // Learning rate for self-modification
  float convergence_threshold = 0.001f;   // Convergence detection threshold
  float membrane_permeability = 0.5f;     // P-System membrane permeability
  float feedback_damping = 0.9f;          // Damping factor for feedback loops
  float plasticity_factor = 0.1f;         // System plasticity for adaptation
  float meta_attention_allocation =
      0.1f; // Attention reserved for meta-cognition
};

/**
 * System convergence status
 */
enum class ConvergenceState {
  Stable,    // System is stable
  Adapting,  // System is actively adapting
  Diverging, // System is diverging
  Critical   // Critical state requiring intervention
};

/**
 * System state representation
 */
struct SystemState {
  float cognitive_load;
  std::vector<float> attention_distribution;
  float reasoning_activity;
  float memory_usage;
  float processing_efficiency;
  ConvergenceState convergence_status;
  int64_t timestamp;
};

/**
 * Meta-cognitive feedback loop
 */
struct FeedbackLoop {
  std::string id;
  std::string
      source_component; // "attention", "reasoning", "encoding", "memory"
  std::string target_component;
  NanoBrainTensor *feedback_tensor;
  float feedback_strength;
  std::string loop_type; // "positive", "negative", "oscillatory"
  float stability_measure;
};

/**
 * Self-modification record
 */
struct SelfModification {
  std::string id;
  int64_t timestamp;
  std::string modification_type; // "parameter_adjustment", "structure_change",
                                 // "rule_addition", "rule_removal"
  std::string target_component;
  NanoBrainTensor *modification_tensor;
  float effectiveness_score;
  bool reversible;
};

/**
 * Meta-cognitive tensor for self-representation
 */
struct MetaCognitiveTensor {
  int level;
  NanoBrainTensor *activation_pattern;
  NanoBrainTensor *self_model;
  NanoBrainTensor *feedback_signal;
  float coherence;
  int64_t last_updated;
};

/**
 * P-System inspired membrane structure
 */
struct CognitiveMembrane {
  std::string id;
  int level;
  float permeability;
  std::vector<std::string> child_membranes;
  NanoBrainTensor *state_tensor;
  std::vector<std::string> contained_atoms;
};

/**
 * Meta-cognitive metrics
 */
struct MetaCognitiveMetrics {
  float self_awareness_level;
  float adaptation_rate;
  float system_coherence;
  float meta_attention_efficiency;
  float feedback_loop_stability;
  float self_modification_frequency;
  float convergence_rate;
};

/**
 * Meta-cognitive Feedback Engine
 *
 * Implements meta-tensor structures representing the system's own state
 * and activation patterns, with feedback mechanisms for self-monitoring
 * and adaptive self-modification.
 */
class MetaCognitiveFeedbackEngine {
public:
  MetaCognitiveFeedbackEngine(NanoBrainKernel *kernel,
                              AttentionAllocationEngine *attention_engine,
                              RecursiveReasoningEngine *reasoning_engine,
                              const MetaCognitiveConfig &config);
  ~MetaCognitiveFeedbackEngine();

  // ================================================================
  // Initialization
  // ================================================================

  // Initialize the meta-cognitive system
  void initialize();

  // Initialize meta-cognitive tensor hierarchy
  void initialize_meta_cognitive_tensors();

  // Initialize P-System inspired membranes
  void initialize_membranes();

  // Initialize feedback loops between components
  void initialize_feedback_loops();

  // ================================================================
  // Main Update Cycle
  // ================================================================

  // Update meta-cognitive state and perform self-monitoring
  MetaCognitiveMetrics
  update_meta_cognitive(const std::vector<NodeTensor *> &node_tensors,
                        const std::vector<LinkTensor *> &link_tensors,
                        const AttentionStats &attention_stats,
                        const ReasoningStats &reasoning_stats);

  // Monitor current system state
  SystemState
  monitor_system_state(const std::vector<NodeTensor *> &node_tensors,
                       const std::vector<LinkTensor *> &link_tensors,
                       const AttentionStats &attention_stats,
                       const ReasoningStats &reasoning_stats);

  // ================================================================
  // Self-Monitoring
  // ================================================================

  // Update meta-cognitive tensors with current state
  void update_meta_tensors(const SystemState &system_state,
                           const std::vector<NodeTensor *> &node_tensors,
                           const std::vector<LinkTensor *> &link_tensors);

  // Update activation patterns for meta-level
  void update_activation_patterns(MetaCognitiveTensor &meta_tensor,
                                  const std::vector<NodeTensor *> &node_tensors,
                                  const std::vector<LinkTensor *> &link_tensors,
                                  int level);

  // Update self-monitoring tensor
  void update_self_monitoring(MetaCognitiveTensor &meta_tensor,
                              const SystemState &system_state, int level);

  // Update feedback tensor for meta-level
  void update_feedback_tensor(MetaCognitiveTensor &meta_tensor,
                              const SystemState &system_state, int level);

  // ================================================================
  // Feedback Processing
  // ================================================================

  // Process all feedback loops
  void process_feedback_loops(const SystemState &system_state);

  // Update individual feedback loop
  void update_feedback_loop(FeedbackLoop &loop, const SystemState &state);

  // Apply feedback to target component
  void apply_feedback(const FeedbackLoop &loop);

  // Check feedback loop stability
  float check_loop_stability(const FeedbackLoop &loop);

  // ================================================================
  // Self-Modification
  // ================================================================

  // Perform self-modification based on current state
  void perform_self_modification(const SystemState &system_state,
                                 const MetaCognitiveMetrics &metrics);

  // Adjust configuration parameters
  void adjust_parameters(const MetaCognitiveMetrics &metrics);

  // Record self-modification
  void record_self_modification(const std::string &type,
                                const std::string &target,
                                NanoBrainTensor *tensor, float effectiveness);

  // Evaluate modification effectiveness
  float evaluate_modification_effectiveness(const SelfModification &mod);

  // Revert ineffective modifications
  void revert_modifications(float effectiveness_threshold);

  // ================================================================
  // P-System Membrane Operations
  // ================================================================

  // Update membrane states
  void update_membrane_states(const std::vector<NodeTensor *> &node_tensors);

  // Apply membrane rules (transfer between membranes)
  void apply_membrane_rules();

  // Calculate inter-membrane flow
  float calculate_membrane_flow(const CognitiveMembrane &source,
                                const CognitiveMembrane &target);

  // ================================================================
  // Convergence Detection
  // ================================================================

  // Detect system convergence
  ConvergenceState detect_convergence(const SystemState &current_state);

  // Calculate convergence rate
  float calculate_convergence_rate();

  // Handle critical states
  void handle_critical_state(const SystemState &state);

  // ================================================================
  // Metrics Calculation
  // ================================================================

  // Calculate meta-cognitive metrics
  MetaCognitiveMetrics calculate_metrics(const SystemState &state);

  // Calculate self-awareness level
  float
  calculate_self_awareness(const std::vector<MetaCognitiveTensor> &tensors);

  // Calculate system coherence
  float calculate_system_coherence();

  // Calculate feedback stability
  float calculate_feedback_stability();

  // ================================================================
  // State Access
  // ================================================================

  // Get feedback loops
  std::vector<FeedbackLoop> get_feedback_loops() const;

  // Get self-modifications
  std::vector<SelfModification> get_self_modifications() const;

  // Get current system state
  SystemState get_current_state() const;

  // Get meta-cognitive tensors
  std::vector<MetaCognitiveTensor> get_meta_tensors() const;

  // Get configuration
  const MetaCognitiveConfig &get_config() const { return config; }

  // Update configuration
  void update_config(const MetaCognitiveConfig &new_config);

  // Reset meta-cognitive system
  void reset();

private:
  NanoBrainKernel *kernel;
  AttentionAllocationEngine *attention_engine;
  RecursiveReasoningEngine *reasoning_engine;
  MetaCognitiveConfig config;

  // Meta-cognitive tensors for each level
  std::vector<MetaCognitiveTensor> meta_tensors;

  // P-System membranes
  std::vector<CognitiveMembrane> membranes;

  // Feedback loops
  std::vector<FeedbackLoop> feedback_loops;

  // Self-modification history
  std::vector<SelfModification> modifications;

  // State history for convergence detection
  std::vector<SystemState> state_history;
  size_t max_history_size = 100;

  // Counters
  int cycle_counter = 0;
  int modification_counter = 0;
  int64_t start_time = 0;

  // Private helpers
  std::string generate_membrane_id();
  std::string generate_modification_id();
  int64_t current_time_millis() const;
};

#endif // NANOBRAIN_METACOGNITIVE_H
