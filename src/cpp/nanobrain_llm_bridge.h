#ifndef NANOBRAIN_LLM_BRIDGE_H
#define NANOBRAIN_LLM_BRIDGE_H

/**
 * NanoBrain LLM Integration Bridge for llama.cpp
 *
 * This header provides the interface for integrating NanoBrain's cognitive
 * architecture with Large Language Model inference in llama.cpp.
 *
 * Key capabilities:
 * - Semantic embedding alignment between LLM token embeddings and AtomSpace
 * - PPM-enhanced attention modulation for LLM inference
 * - Time Crystal-based context memory management
 * - PLN reasoning integration with LLM outputs
 * - Cognitive state injection into generation
 */

#include "nanobrain_unified.h"
#include <functional>
#include <memory>
#include <string>
#include <vector>

// Forward declarations for llama.cpp types (to avoid header dependency)
struct llama_context;
struct llama_model;
struct llama_token;

/**
 * Token embedding with NanoBrain enhancement
 */
struct EnhancedTokenEmbedding {
  int32_t token_id;
  std::vector<float> base_embedding;     // Original llama embedding
  std::vector<float> enhanced_embedding; // PPM-enhanced embedding
  float ppm_coherence;                   // Phase Prime Metric coherence
  float attention_boost;                 // Attention modulation factor
  std::string mapped_atom_id; // Corresponding AtomSpace atom (if any)
};

/**
 * Cognitive context for LLM generation
 */
struct CognitiveContext {
  std::vector<std::string> focus_atoms;   // High-attention atoms to emphasize
  std::vector<std::string> active_chains; // Active reasoning chains
  float consciousness_level;              // Current consciousness emergence
  float temporal_coherence;               // Time crystal stability
  std::vector<int> prime_signature;       // Current prime state
};

/**
 * Generation modulation parameters
 */
struct GenerationModulation {
  float attention_temperature;  // Temperature adjustment based on attention
  float coherence_bias;         // Bias toward coherent token sequences
  float reasoning_influence;    // How much PLN reasoning affects logits
  float metacognitive_damping;  // Self-monitoring influence
  bool enable_prime_alignment;  // Use PPM for token selection
  bool enable_temporal_context; // Use time crystal memory
};

/**
 * LLM Bridge configuration
 */
struct LLMBridgeConfig {
  // Embedding alignment
  int embedding_dim = 4096;     // LLM embedding dimension
  int projection_dim = 128;     // NanoBrain projection dimension
  bool learn_projection = true; // Learn embedding projection matrix

  // Attention modulation
  float base_attention_weight = 1.0f;
  float ppm_attention_scale = 0.5f;
  bool modulate_attention = true;

  // Reasoning integration
  bool enable_reasoning = true;
  int reasoning_lookahead = 3; // Steps to reason ahead
  float reasoning_threshold = 0.5f;

  // Memory and context
  int max_context_atoms = 100;
  float context_decay_rate = 0.01f;
  bool persist_context = true;
};

/**
 * NanoBrain LLM Integration Bridge
 *
 * Connects NanoBrain's cognitive architecture to llama.cpp for enhanced
 * language model inference with symbolic reasoning, attention allocation,
 * and temporal coherence.
 */
class NanoBrainLLMBridge {
public:
  NanoBrainLLMBridge(const LLMBridgeConfig &config);
  ~NanoBrainLLMBridge();

  // ================================================================
  // Initialization
  // ================================================================

  /**
   * Initialize the bridge with NanoBrain kernel
   * @param nanobrain Initialized NanoBrain unified kernel
   */
  void initialize(UnifiedNanoBrainKernel *nanobrain);

  /**
   * Connect to llama.cpp context
   * @param ctx llama context pointer
   * @param model llama model pointer
   */
  void connect_llama(llama_context *ctx, llama_model *model);

  /**
   * Check if bridge is ready
   */
  bool is_ready() const { return initialized && llama_connected; }

  // ================================================================
  // Embedding Operations
  // ================================================================

  /**
   * Project LLM embedding to NanoBrain space
   * @param llm_embedding Original LLM token embedding
   * @return Projected embedding in NanoBrain dimension
   */
  std::vector<float>
  project_to_nanobrain(const std::vector<float> &llm_embedding);

  /**
   * Project NanoBrain embedding back to LLM space
   * @param nb_embedding NanoBrain embedding
   * @return Projected embedding in LLM dimension
   */
  std::vector<float> project_to_llm(const std::vector<float> &nb_embedding);

  /**
   * Enhance token embedding with PPM coherence
   * @param token_id Token ID
   * @param base_embedding Original embedding
   * @return Enhanced embedding with cognitive features
   */
  EnhancedTokenEmbedding
  enhance_embedding(int32_t token_id, const std::vector<float> &base_embedding);

  /**
   * Create semantic mapping between token and AtomSpace
   * @param token_id Token ID
   * @param token_text Token string representation
   * @return Mapped atom ID (creates new atom if needed)
   */
  std::string map_token_to_atom(int32_t token_id,
                                const std::string &token_text);

  // ================================================================
  // Attention Modulation
  // ================================================================

  /**
   * Get attention modulation for a sequence of tokens
   * @param token_ids Sequence of token IDs
   * @return Attention weights for each token
   */
  std::vector<float>
  get_attention_modulation(const std::vector<int32_t> &token_ids);

  /**
   * Modulate logits based on cognitive state
   * @param logits Raw logits from LLM
   * @param context Current cognitive context
   * @param params Modulation parameters
   * @return Modified logits
   */
  std::vector<float> modulate_logits(const std::vector<float> &logits,
                                     const CognitiveContext &context,
                                     const GenerationModulation &params);

  /**
   * Compute PPM coherence for token sequence
   * @param token_ids Sequence of token IDs
   * @return Coherence score [0, 1]
   */
  float compute_sequence_coherence(const std::vector<int32_t> &token_ids);

  // ================================================================
  // Reasoning Integration
  // ================================================================

  /**
   * Start reasoning about a prompt
   * @param prompt_tokens Tokenized prompt
   * @return Reasoning chain ID
   */
  std::string reason_about_prompt(const std::vector<int32_t> &prompt_tokens);

  /**
   * Get reasoning-informed token probabilities
   * @param chain_id Active reasoning chain
   * @param current_tokens Current generation context
   * @return Token probability adjustments
   */
  std::vector<float>
  get_reasoning_probs(const std::string &chain_id,
                      const std::vector<int32_t> &current_tokens);

  /**
   * Update reasoning based on generated token
   * @param chain_id Reasoning chain
   * @param token_id Generated token
   */
  void update_reasoning(const std::string &chain_id, int32_t token_id);

  // ================================================================
  // Context Management
  // ================================================================

  /**
   * Get current cognitive context for generation
   * @return Current cognitive state
   */
  CognitiveContext get_cognitive_context() const;

  /**
   * Set focus atoms for generation
   * @param atom_ids Atoms to focus on
   */
  void set_focus_atoms(const std::vector<std::string> &atom_ids);

  /**
   * Clear context and reset state
   */
  void clear_context();

  /**
   * Run cognitive processing cycle
   * Call this periodically during generation
   */
  void process_cognitive_cycle();

  // ================================================================
  // Metrics and Statistics
  // ================================================================

  /**
   * Get bridge performance metrics
   */
  struct BridgeMetrics {
    size_t tokens_processed;
    size_t atoms_created;
    size_t reasoning_steps;
    float average_coherence;
    float average_attention_boost;
    float embedding_alignment_score;
  };

  BridgeMetrics get_metrics() const;

  /**
   * Get configuration
   */
  const LLMBridgeConfig &get_config() const { return config; }

  // ================================================================
  // Callbacks for llama.cpp integration
  // ================================================================

  using LogitsCallback = std::function<void(std::vector<float> &logits)>;
  using TokenCallback = std::function<void(int32_t token_id)>;

  /**
   * Set callback for logits modification
   */
  void set_logits_callback(LogitsCallback callback);

  /**
   * Set callback for token generation
   */
  void set_token_callback(TokenCallback callback);

private:
  LLMBridgeConfig config;

  // NanoBrain connection
  UnifiedNanoBrainKernel *nanobrain = nullptr;
  bool initialized = false;

  // llama.cpp connection
  llama_context *llama_ctx = nullptr;
  llama_model *llama_mdl = nullptr;
  bool llama_connected = false;

  // Projection matrices (learned or fixed)
  NanoBrainTensor *projection_to_nb = nullptr;
  NanoBrainTensor *projection_to_llm = nullptr;

  // Token to atom mapping cache
  std::map<int32_t, std::string> token_atom_map;

  // Context state
  CognitiveContext current_context;
  std::vector<std::string> active_reasoning_chains;

  // Callbacks
  LogitsCallback logits_callback;
  TokenCallback token_callback;

  // Statistics
  BridgeMetrics metrics;

  // Private helpers
  void initialize_projections();
  void update_metrics(const EnhancedTokenEmbedding &emb);
  std::vector<int> tokens_to_primes(const std::vector<int32_t> &tokens);
};

// ================================================================
// Utility Functions
// ================================================================

/**
 * Create a generation modulation preset
 */
inline GenerationModulation default_modulation() {
  return {
      .attention_temperature = 1.0f,
      .coherence_bias = 0.3f,
      .reasoning_influence = 0.2f,
      .metacognitive_damping = 0.9f,
      .enable_prime_alignment = true,
      .enable_temporal_context = true,
  };
}

/**
 * Create a high-coherence modulation preset
 */
inline GenerationModulation coherent_modulation() {
  return {
      .attention_temperature = 0.8f,
      .coherence_bias = 0.7f,
      .reasoning_influence = 0.4f,
      .metacognitive_damping = 0.95f,
      .enable_prime_alignment = true,
      .enable_temporal_context = true,
  };
}

/**
 * Create a creative/exploratory modulation preset
 */
inline GenerationModulation creative_modulation() {
  return {
      .attention_temperature = 1.3f,
      .coherence_bias = 0.1f,
      .reasoning_influence = 0.1f,
      .metacognitive_damping = 0.7f,
      .enable_prime_alignment = false,
      .enable_temporal_context = true,
  };
}

#endif // NANOBRAIN_LLM_BRIDGE_H
