#ifndef NANOBRAIN_REASONING_H
#define NANOBRAIN_REASONING_H

#include "nanobrain_kernel.h"
#include "nanobrain_types.h"
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

// Forward declarations
class AtomSpaceTensorEncoder;
class AttentionAllocationEngine;

/**
 * Inference rule types for PLN-style reasoning
 */
enum class InferenceRuleTypePLN {
  Deduction, // If A->B and B->C then A->C
  Induction, // If A->B and A->C then B->C (probabilistic)
  Abduction, // If A->B and C->B then A->C (probabilistic)
  Revision,  // Combine truth values from multiple sources
  Choice     // Select best alternative based on truth values
};

/**
 * Tensor operation types for inference
 */
enum class TensorOperationType {
  Contraction,   // Tensor contraction (dot product generalization)
  Composition,   // Sequential tensor composition
  Transformation // Learned transformation via weight matrix
};

/**
 * PLN-style inference rule configuration
 */
struct InferenceRuleConfig {
  std::string id;
  std::string name;
  InferenceRuleTypePLN type;
  std::vector<std::string> premise_patterns; // Patterns to match
  std::string conclusion_pattern;
  std::string confidence_formula;
  TensorOperationType tensor_operation;
  std::vector<int> weight_tensor_shape;
  float priority_weight; // For rule selection
};

/**
 * Individual reasoning step
 */
struct ReasoningStep {
  std::string id;
  int step_number;
  std::string rule_applied;
  std::vector<NanoBrainTensor *> input_tensors;
  NanoBrainTensor *output_tensor;
  float confidence_change;
  std::vector<std::string> tensor_operations;

  // Metadata
  float reasoning_time_ms;
  size_t memory_usage;
  float attention_consumed;
};

/**
 * Convergence status for reasoning chains
 */
enum class ConvergenceStatus {
  Active,    // Still processing
  Converged, // Reached stable conclusion
  Diverged,  // No convergence possible
  Terminated // Manually stopped or depth limit reached
};

/**
 * Reasoning chain state
 */
struct ReasoningChain {
  std::string id;
  std::vector<std::string> input_node_ids;
  std::vector<ReasoningStep> reasoning_steps;
  int current_depth;
  float confidence_score;
  ConvergenceStatus convergence_status;
  std::vector<NanoBrainTensor *> output_tensors;

  // Rule usage tracking
  std::map<std::string, int> rules_used_count;
};

/**
 * Reasoning engine configuration
 */
struct ReasoningEngineConfig {
  int max_reasoning_depth = 5;
  float confidence_threshold = 0.1f;
  std::string tensor_contraction_method = "dot"; // dot, einstein, multilinear
  int chain_length_limit = 20;
  int parallel_chains = 4;
  bool gradient_flow_enabled = true;
  bool meta_reasoning_enabled = true;
  float attention_budget_per_step = 10.0f;
  float min_confidence_gain = 0.01f;
};

/**
 * Reasoning statistics
 */
struct ReasoningStats {
  size_t total_chains;
  size_t active_chains;
  size_t converged_chains;
  float average_depth;
  size_t total_inferences;
  float average_confidence;
  size_t tensor_memory_usage;
  float reasoning_throughput; // Inferences per second

  // Rule usage stats
  std::map<std::string, int> rule_usage;
};

/**
 * Recursive Reasoning Engine
 *
 * Implements tensor-based recursive reasoning that applies logical inference
 * (PLN-style) via tensor contractions, enabling chainable, differentiable
 * inference paths.
 */
class RecursiveReasoningEngine {
public:
  RecursiveReasoningEngine(NanoBrainKernel *kernel,
                           const ReasoningEngineConfig &config);
  ~RecursiveReasoningEngine();

  // ================================================================
  // Initialization
  // ================================================================

  // Initialize with default PLN inference rules
  void initialize();

  // Add a custom inference rule
  void add_inference_rule(const InferenceRuleConfig &rule);

  // Remove an inference rule by ID
  bool remove_inference_rule(const std::string &rule_id);

  // Get all available rules
  std::vector<std::string> get_rule_ids() const;

  // ================================================================
  // Reasoning Chain Management
  // ================================================================

  // Start a new reasoning chain from input nodes
  std::string
  start_reasoning_chain(const std::vector<std::string> &input_node_ids);

  // Get a reasoning chain by ID
  const ReasoningChain *get_chain(const std::string &chain_id) const;

  // Get mutable chain
  ReasoningChain *get_mutable_chain(const std::string &chain_id);

  // Get all active chain IDs
  std::vector<std::string> get_active_chain_ids() const;

  // Terminate a reasoning chain
  void terminate_chain(const std::string &chain_id);

  // Clear all chains
  void clear_all_chains();

  // ================================================================
  // Reasoning Execution
  // ================================================================

  // Execute one reasoning step for all active chains
  ReasoningStats
  execute_reasoning_step(const std::vector<NodeTensor *> &node_tensors,
                         const std::vector<LinkTensor *> &link_tensors);

  // Process a single chain
  void process_chain(ReasoningChain &chain,
                     const std::vector<NodeTensor *> &node_tensors,
                     const std::vector<LinkTensor *> &link_tensors);

  // Find applicable rules for a chain
  std::vector<InferenceRuleConfig *>
  find_applicable_rules(const ReasoningChain &chain,
                        const std::vector<NodeTensor *> &nodes,
                        const std::vector<LinkTensor *> &links);

  // Select the best rule based on attention and confidence
  InferenceRuleConfig *
  select_best_rule(const std::vector<InferenceRuleConfig *> &candidates,
                   const ReasoningChain &chain);

  // Apply an inference rule
  ReasoningStep apply_inference_rule(const InferenceRuleConfig &rule,
                                     ReasoningChain &chain,
                                     const std::vector<NodeTensor *> &nodes,
                                     const std::vector<LinkTensor *> &links);

  // ================================================================
  // Tensor Operations for Reasoning
  // ================================================================

  // Perform tensor contraction for inference
  NanoBrainTensor *perform_contraction(NanoBrainTensor *a, NanoBrainTensor *b,
                                       const std::string &method);

  // Compose tensors sequentially
  NanoBrainTensor *
  compose_tensors(const std::vector<NanoBrainTensor *> &tensors);

  // Apply transformation via weight matrix
  NanoBrainTensor *apply_transformation(NanoBrainTensor *input,
                                        NanoBrainTensor *weights);

  // ================================================================
  // PLN Truth Value Calculations
  // ================================================================

  // Calculate deductive confidence: P(C|A) ≈ P(B|A) * P(C|B)
  float calculate_deductive_confidence(float strength_ab, float conf_ab,
                                       float strength_bc, float conf_bc);

  // Calculate inductive confidence
  float calculate_inductive_confidence(float strength_ab, float conf_ab,
                                       float strength_ac, float conf_ac);

  // Calculate abductive confidence
  float calculate_abductive_confidence(float strength_ab, float conf_ab,
                                       float strength_cb, float conf_cb);

  // Perform truth value revision
  void revise_truth_values(float &strength, float &confidence,
                           float new_strength, float new_confidence);

  // ================================================================
  // Meta-Reasoning
  // ================================================================

  // Evaluate reasoning quality
  float evaluate_reasoning_quality(const ReasoningChain &chain);

  // Suggest rule modifications
  std::vector<std::string>
  suggest_rule_modifications(const ReasoningChain &chain);

  // Prune low-quality chains
  void prune_chains(float quality_threshold);

  // ================================================================
  // Statistics
  // ================================================================

  // Get current statistics
  ReasoningStats get_stats() const;

  // Reset statistics
  void reset_stats();

  // Get configuration
  const ReasoningEngineConfig &get_config() const { return config; }

  // Update configuration
  void update_config(const ReasoningEngineConfig &new_config);

private:
  NanoBrainKernel *kernel;
  ReasoningEngineConfig config;

  std::map<std::string, InferenceRuleConfig> inference_rules;
  std::map<std::string, ReasoningChain> chains;

  int chain_counter = 0;
  int step_counter = 0;
  int64_t start_time = 0;
  size_t total_inferences = 0;

  // Private helpers
  std::string generate_chain_id();
  std::string generate_step_id();
  void add_default_rules();
  int64_t current_time_millis() const;
  bool check_pattern_match(const std::string &pattern,
                           const ReasoningChain &chain,
                           const std::vector<NodeTensor *> &nodes);
};

#endif // NANOBRAIN_REASONING_H
