#ifndef NANOBRAIN_UNIFIED_H
#define NANOBRAIN_UNIFIED_H

/**
 * NanoBrain Unified C++ Adaptation for llama.cpp
 *
 * This header provides a single entry point for the complete NanoBrain
 * cognitive architecture implemented using ggml tensors. It integrates:
 *
 * - NanoBrainKernel: Core tensor operations via ggml
 * - TimeCrystalKernel: 11D quantum processing with PPM/GML
 * - RecursiveReasoningEngine: PLN-style differentiable inference
 * - AttentionAllocationEngine: Softmax/ECAN attention mechanisms
 * - MetaCognitiveFeedbackEngine: Self-monitoring and adaptation
 * - AtomSpaceTensorEncoder: AtomSpace to tensor conversion
 */

#include "nanobrain_attention.h"
#include "nanobrain_encoder.h"
#include "nanobrain_kernel.h"
#include "nanobrain_metacognitive.h"
#include "nanobrain_reasoning.h"
#include "nanobrain_time_crystal.h"
#include "nanobrain_types.h"

#include <memory>
#include <string>
#include <vector>

/**
 * Unified NanoBrain configuration
 */
struct UnifiedNanoBrainConfig {
  // Core kernel settings
  size_t memory_size = 1024 * 1024 * 128; // 128 MB
  bool use_gpu = false;

  // Time Crystal settings
  int time_crystal_dimensions = 11;
  int fractal_resolution = 5;
  float quantum_coherence_threshold = 0.5f;

  // Reasoning settings
  int max_reasoning_depth = 5;
  float confidence_threshold = 0.1f;
  int parallel_chains = 4;

  // Attention settings
  AttentionMechanism attention_mechanism = AttentionMechanism::Hybrid;
  float attention_temperature = 1.0f;
  float resource_budget = 1000.0f;

  // Meta-cognitive settings
  int meta_levels = 3;
  float adaptation_learning_rate = 0.01f;
  float feedback_damping = 0.9f;
};

/**
 * Unified NanoBrain performance metrics
 */
struct UnifiedNanoBrainMetrics {
  // System metrics
  size_t total_atoms;
  size_t total_links;
  size_t total_inferences;
  float uptime_seconds;

  // Time Crystal metrics
  float quantum_coherence;
  float temporal_stability;
  float prime_alignment;
  float consciousness_emergence;

  // Attention metrics
  float total_attention;
  float attention_entropy;
  float resource_utilization;

  // Reasoning metrics
  size_t active_chains;
  size_t converged_chains;
  float reasoning_throughput;

  // Meta-cognitive metrics
  float self_awareness_level;
  float system_coherence;
  float adaptation_rate;
};

/**
 * Unified NanoBrain Cognitive Kernel
 *
 * Provides a high-level interface to the complete NanoBrain system,
 * managing all subcomponents and their interactions.
 */
class UnifiedNanoBrainKernel {
public:
  UnifiedNanoBrainKernel(const UnifiedNanoBrainConfig &config);
  ~UnifiedNanoBrainKernel();

  // ================================================================
  // Lifecycle
  // ================================================================

  // Initialize all subsystems
  void initialize();

  // Shutdown all subsystems
  void shutdown();

  // Check if system is active
  bool is_active() const { return active; }

  // ================================================================
  // Processing Cycle
  // ================================================================

  // Execute one full cognitive processing cycle
  UnifiedNanoBrainMetrics process_cycle();

  // Run N cycles
  UnifiedNanoBrainMetrics run_cycles(int n);

  // ================================================================
  // Atom Management
  // ================================================================

  // Create a new atom
  std::string create_atom(const std::string &type, const std::string &name,
                          float strength = 1.0f, float confidence = 1.0f,
                          const std::vector<int> &prime_encoding = {2, 3, 5});

  // Get atom by ID
  const TimeCrystalAtom *get_atom(const std::string &id) const;

  // Remove atom
  bool remove_atom(const std::string &id);

  // Get all atom IDs
  std::vector<std::string> get_all_atom_ids() const;

  // ================================================================
  // Reasoning
  // ================================================================

  // Start a new reasoning chain
  std::string start_reasoning(const std::vector<std::string> &input_atom_ids);

  // Execute reasoning step
  ReasoningStats execute_reasoning_step();

  // Get reasoning chain by ID
  const ReasoningChain *get_reasoning_chain(const std::string &chain_id) const;

  // Get all active chain IDs
  std::vector<std::string> get_active_chain_ids() const;

  // ================================================================
  // Attention
  // ================================================================

  // Update attention allocation
  AttentionStats update_attention();

  // Get top-k atoms by attention
  std::vector<std::string> get_top_attention_atoms(size_t k) const;

  // ================================================================
  // Tensor Operations
  // ================================================================

  // Encode atom to tensor
  NanoBrainTensor *encode_atom_to_tensor(const std::string &atom_id);

  // Compute coherence for primes
  float compute_coherence(const std::vector<int> &primes);

  // ================================================================
  // Metrics & Statistics
  // ================================================================

  // Get comprehensive metrics
  UnifiedNanoBrainMetrics get_metrics() const;

  // Get cycle count
  size_t get_cycle_count() const { return cycle_count; }

  // ================================================================
  // Subsystem Access
  // ================================================================

  // Get Time Crystal kernel
  TimeCrystalKernel *get_time_crystal_kernel() {
    return time_crystal_kernel.get();
  }

  // Get Reasoning engine
  RecursiveReasoningEngine *get_reasoning_engine() {
    return reasoning_engine.get();
  }

  // Get Attention engine
  AttentionAllocationEngine *get_attention_engine() {
    return attention_engine.get();
  }

  // Get Meta-cognitive engine
  MetaCognitiveFeedbackEngine *get_metacognitive_engine() {
    return metacognitive_engine.get();
  }

  // Get tensor encoder
  AtomSpaceTensorEncoder *get_encoder() { return encoder.get(); }

  // Get configuration
  const UnifiedNanoBrainConfig &get_config() const { return config; }

private:
  UnifiedNanoBrainConfig config;

  // Subsystems
  std::unique_ptr<TimeCrystalKernel> time_crystal_kernel;
  std::unique_ptr<RecursiveReasoningEngine> reasoning_engine;
  std::unique_ptr<AttentionAllocationEngine> attention_engine;
  std::unique_ptr<MetaCognitiveFeedbackEngine> metacognitive_engine;
  std::unique_ptr<AtomSpaceTensorEncoder> encoder;

  // Tensor caches
  std::vector<NodeTensor *> node_tensors;
  std::vector<LinkTensor *> link_tensors;

  // State
  bool active = false;
  size_t cycle_count = 0;
  int64_t start_time = 0;

  // Private helpers
  void sync_tensors();
  void build_node_tensors();
  void build_link_tensors();
  int64_t current_time_millis() const;
};

// ================================================================
// Version Information
// ================================================================

constexpr const char *NANOBRAIN_CPP_VERSION = "1.0.0";
constexpr const char *NANOBRAIN_CPP_BUILD_DATE = __DATE__;

/**
 * Get version string
 */
inline std::string get_nanobrain_version() {
  return std::string(NANOBRAIN_CPP_VERSION) + " (built " +
         NANOBRAIN_CPP_BUILD_DATE + ")";
}

#endif // NANOBRAIN_UNIFIED_H
