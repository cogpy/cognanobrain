#ifndef NANOBRAIN_ATTENTION_H
#define NANOBRAIN_ATTENTION_H

#include "nanobrain_kernel.h"
#include "nanobrain_types.h"
#include <map>
#include <memory>
#include <string>
#include <vector>

/**
 * Attention allocation mechanism types
 */
enum class AttentionMechanism {
  Softmax, // Standard softmax attention
  ECAN,    // Economic Attention Allocation
  Hybrid   // Combination of softmax and ECAN
};

/**
 * Attention allocation configuration
 */
struct AttentionAllocationConfig {
  AttentionMechanism mechanism = AttentionMechanism::Hybrid;
  float temperature = 1.0f;            // Softmax temperature
  float resource_budget = 1000.0f;     // Total attention budget
  int update_frequency = 10;           // Updates per second
  float decay_rate = 0.01f;            // Attention decay rate
  float diffusion_strength = 0.1f;     // How much attention spreads
  float rent_collection_rate = 0.01f;  // ECAN rent collection
  float wage_distribution_rate = 0.8f; // ECAN wage distribution
  int attention_heads = 8;             // Multi-head attention count
  float gradient_clipping = 1.0f;      // Gradient clipping value
};

/**
 * Attention allocation statistics
 */
struct AttentionStats {
  float total_attention;
  float average_attention;
  float attention_entropy;
  float resource_utilization;
  float gradient_norm;
  float convergence_rate;
};

/**
 * Attention flow record for monitoring
 */
struct AttentionFlow {
  int64_t timestamp;
  std::string source_id;
  std::string target_id;
  float attention_transfer;
  std::string reason; // "diffusion", "rent", "wage", "gradient"
};

/**
 * Attention head state for multi-head attention
 */
struct AttentionHead {
  int head_id;
  NanoBrainTensor *query_projection;
  NanoBrainTensor *key_projection;
  NanoBrainTensor *value_projection;
  NanoBrainTensor *attention_weights;
  float head_scale;
};

/**
 * Global attention tensor structure
 */
struct GlobalAttentionTensor {
  NanoBrainTensor *attention_distribution;
  NanoBrainTensor *attention_gradients;
  NanoBrainTensor *temperature_tensor;
  std::vector<AttentionHead> attention_heads;
  bool initialized = false;
};

/**
 * Attention Allocation Engine
 *
 * Implements dynamic attention mechanisms (softmax and ECAN-inspired)
 * that assign and update attention weights across the AtomSpace tensor field.
 */
class AttentionAllocationEngine {
public:
  AttentionAllocationEngine(NanoBrainKernel *kernel,
                            const AttentionAllocationConfig &config);
  ~AttentionAllocationEngine();

  // ================================================================
  // Initialization
  // ================================================================

  // Initialize the attention system
  void initialize(size_t max_nodes);

  // Initialize global attention tensor structure
  void initialize_global_attention(size_t num_nodes);

  // Check if initialized
  bool is_initialized() const { return global_attention.initialized; }

  // ================================================================
  // Attention Update Methods
  // ================================================================

  // Update attention allocation across all nodes
  AttentionStats
  update_attention_allocation(const std::vector<NodeTensor *> &node_tensors,
                              const std::vector<LinkTensor *> &link_tensors);

  // Apply softmax attention mechanism
  AttentionStats
  apply_softmax_attention(const std::vector<NodeTensor *> &node_tensors,
                          const std::vector<LinkTensor *> &link_tensors);

  // Apply ECAN (Economic Attention Allocation) mechanism
  AttentionStats
  apply_ecan_attention(const std::vector<NodeTensor *> &node_tensors,
                       const std::vector<LinkTensor *> &link_tensors);

  // Apply hybrid attention mechanism
  AttentionStats
  apply_hybrid_attention(const std::vector<NodeTensor *> &node_tensors,
                         const std::vector<LinkTensor *> &link_tensors);

  // ================================================================
  // Tensor-Based Attention Operations
  // ================================================================

  // Compute attention scores using query-key mechanism
  NanoBrainTensor *
  compute_attention_scores(const std::vector<NodeTensor *> &nodes);

  // Apply attention to values
  NanoBrainTensor *
  apply_attention_to_values(NanoBrainTensor *attention_weights,
                            const std::vector<NodeTensor *> &nodes);

  // Multi-head self-attention
  NanoBrainTensor *
  multi_head_self_attention(const std::vector<NodeTensor *> &nodes);

  // ================================================================
  // ECAN Economic Mechanisms
  // ================================================================

  // Apply attention diffusion across connected nodes
  void apply_attention_diffusion(const std::vector<NodeTensor *> &node_tensors,
                                 const std::vector<LinkTensor *> &link_tensors);

  // Collect rent from low-utility atoms
  void collect_rent(std::vector<NodeTensor *> &node_tensors);

  // Distribute wages to high-utility atoms
  void distribute_wages(std::vector<NodeTensor *> &node_tensors,
                        float pool_amount);

  // Update link attention based on connected nodes
  void update_link_attention(std::vector<LinkTensor *> &link_tensors,
                             const std::vector<NodeTensor *> &node_tensors);

  // ================================================================
  // Gradient-Based Updates
  // ================================================================

  // Apply gradient-based updates to attention weights
  void apply_gradient_updates(std::vector<NodeTensor *> &node_tensors,
                              const std::vector<LinkTensor *> &link_tensors);

  // Compute attention gradients
  NanoBrainTensor *
  compute_attention_gradients(const std::vector<NodeTensor *> &nodes);

  // ================================================================
  // Constraint Enforcement
  // ================================================================

  // Enforce resource constraints on attention allocation
  void enforce_resource_constraints(std::vector<NodeTensor *> &node_tensors);

  // Normalize attention values
  void normalize_attention(std::vector<NodeTensor *> &node_tensors);

  // ================================================================
  // Monitoring and Statistics
  // ================================================================

  // Record attention flows for monitoring
  void record_attention_flows(const std::vector<NodeTensor *> &nodes,
                              const std::vector<LinkTensor *> &links);

  // Calculate attention statistics
  AttentionStats
  calculate_attention_stats(const std::vector<NodeTensor *> &node_tensors);

  // Get attention flow history
  std::vector<AttentionFlow> get_attention_flows() const;

  // Get current attention statistics
  AttentionStats
  get_current_stats(const std::vector<NodeTensor *> &node_tensors);

  // Get global attention tensor
  const GlobalAttentionTensor &get_global_attention() const {
    return global_attention;
  }

  // ================================================================
  // State Management
  // ================================================================

  // Reset attention allocation system
  void reset();

  // Update configuration
  void update_config(const AttentionAllocationConfig &new_config);

  // Get configuration
  const AttentionAllocationConfig &get_config() const { return config; }

private:
  NanoBrainKernel *kernel;
  AttentionAllocationConfig config;
  GlobalAttentionTensor global_attention;
  std::vector<AttentionFlow> attention_flows;
  size_t max_flow_history = 1000;

  // Private helpers
  float calculate_entropy(const std::vector<float> &distribution);
  float calculate_gradient_norm(const std::vector<NanoBrainTensor *> &tensors);
  int64_t current_time_millis() const;
};

#endif // NANOBRAIN_ATTENTION_H
