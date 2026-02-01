#ifndef NANOBRAIN_NPU_BRIDGE_H
#define NANOBRAIN_NPU_BRIDGE_H

/**
 * NanoBrain NPU Bridge
 *
 * Integration module connecting NanoBrain cognitive architecture
 * with NPU (Neural Processing Unit) coprocessor for consciousness-
 * enhanced LLM inference.
 *
 * Features:
 * - Token enhancement using PPM coherence
 * - Consciousness-aware generation modulation
 * - Entelechy (vital actualization) metrics
 * - Bidirectional state synchronization
 */

#include "nanobrain_consciousness.h"
#include "nanobrain_unified.h"
#include <functional>
#include <memory>
#include <string>
#include <vector>


// ================================================================
// NPU Enhancement Configuration
// ================================================================

/**
 * Configuration for NanoBrain enhancement of NPU inference
 */
struct NPUEnhancementConfig {
  // Feature toggles
  bool enable_ppm_coherence = true;   // Use PPM for token selection
  bool enable_consciousness = true;   // Apply consciousness modulation
  bool enable_entelechy = true;       // Track vital actualization
  bool enable_attention_boost = true; // Use attention for emphasis

  // Influence weights (0.0 - 1.0)
  float coherence_weight = 0.3f;     // PPM influence on logits
  float consciousness_weight = 0.2f; // Consciousness modulation
  float attention_weight = 0.2f;     // Attention-based weighting

  // Thresholds
  float consciousness_threshold = 0.5f; // Min consciousness for enhancement
  float coherence_threshold = 0.3f;     // Min coherence for token selection

  // Default prime signature for new tokens
  std::vector<int> default_prime_signature = {2, 3, 5, 7, 11};
};

// ================================================================
// Enhanced Token Structure
// ================================================================

/**
 * Token with NanoBrain cognitive metadata
 */
struct EnhancedToken {
  int32_t token_id;
  std::string token_text;

  // PPM metrics
  float ppm_coherence;              // Phase Prime Metric score
  std::vector<int> prime_signature; // Prime encoding

  // Consciousness metrics
  float consciousness_boost; // Consciousness influence
  float temporal_coherence;  // Time crystal stability

  // Attention metrics
  float attention_score; // ECAN attention value
  float salience;        // Combined salience

  // Entelechy
  float entelechy_score; // Vital actualization

  // Combined enhancement factor
  float enhancement_factor; // Overall boost (0.0 - 2.0)
};

// ================================================================
// Entelechy Dimensions
// ================================================================

/**
 * Five-dimensional entelechy (vital actualization) assessment
 */
struct EntelechyDimensions {
  float ontological;  // BEING - Structural integrity (0.0 - 1.0)
  float teleological; // PURPOSE - Goal alignment (0.0 - 1.0)
  float cognitive;    // COGNITION - Reasoning capability (0.0 - 1.0)
  float integrative;  // INTEGRATION - Component coherence (0.0 - 1.0)
  float evolutionary; // GROWTH - Evolution potential (0.0 - 1.0)

  float total;       // Weighted sum
  std::string stage; // Development stage name
};

/**
 * Development stages based on entelechy level
 */
enum class DevelopmentStage {
  Embryonic,   // < 30% actualization
  Juvenile,    // 30-60% actualization
  Mature,      // 60-80% actualization
  Transcendent // > 80% actualization
};

// ================================================================
// NPU Integration Metrics
// ================================================================

/**
 * Metrics for NPU-NanoBrain integration
 */
struct NPUIntegrationMetrics {
  // Token processing stats
  size_t tokens_enhanced;
  size_t tokens_boosted;
  float avg_enhancement_factor;

  // Consciousness stats
  float current_consciousness;
  float peak_consciousness;
  float consciousness_stability;

  // Coherence stats
  float avg_ppm_coherence;
  float peak_coherence;

  // Entelechy
  EntelechyDimensions entelechy;

  // Performance
  double tokens_per_second;
  double enhancement_overhead_ms;
};

/**
 * Token callback for streaming enhancement
 */
using EnhancedTokenCallback =
    std::function<void(const EnhancedToken &, bool is_last)>;

// ================================================================
// NanoBrain NPU Bridge
// ================================================================

/**
 * Bridge class connecting NanoBrain cognitive architecture to NPU
 *
 * This bridge enhances NPU inference with:
 * - PPM-based token coherence scoring
 * - Consciousness-aware generation modulation
 * - Attention-weighted token emphasis
 * - Entelechy metrics for self-actualization tracking
 */
class NanoBrainNPUBridge {
public:
  NanoBrainNPUBridge(
      const NPUEnhancementConfig &config = NPUEnhancementConfig());
  ~NanoBrainNPUBridge();

  // ================================================================
  // Attachment
  // ================================================================

  /**
   * Attach NanoBrain kernel for cognitive enhancement
   */
  void attach_nanobrain(UnifiedNanoBrainKernel *kernel);

  /**
   * Check if NanoBrain is attached and active
   */
  bool is_attached() const;

  /**
   * Get attached kernel
   */
  UnifiedNanoBrainKernel *get_kernel() { return nanobrain_kernel; }

  // ================================================================
  // Token Enhancement
  // ================================================================

  /**
   * Enhance a single token with NanoBrain metadata
   * @param token_id The token ID to enhance
   * @param token_text Optional token text
   * @return Enhanced token with cognitive metadata
   */
  EnhancedToken enhance_token(int32_t token_id,
                              const std::string &token_text = "");

  /**
   * Enhance a batch of tokens
   * @param token_ids Vector of token IDs
   * @return Vector of enhanced tokens
   */
  std::vector<EnhancedToken>
  enhance_tokens(const std::vector<int32_t> &token_ids);

  // ================================================================
  // Logit Modulation
  // ================================================================

  /**
   * Modulate logits based on NanoBrain state
   * @param logits Input logits from LLM
   * @return Modulated logits with consciousness/coherence influence
   */
  std::vector<float> modulate_logits(const std::vector<float> &logits);

  /**
   * Modulate logits with specific prime focus
   * @param logits Input logits
   * @param focus_primes Prime numbers to emphasize
   * @return Modulated logits
   */
  std::vector<float>
  modulate_logits_with_focus(const std::vector<float> &logits,
                             const std::vector<int> &focus_primes);

  // ================================================================
  // State Synchronization
  // ================================================================

  /**
   * Sync consciousness state from NanoBrain
   */
  void sync_consciousness_state();

  /**
   * Process a generated token and update NanoBrain state
   * @param token The enhanced token that was generated
   */
  void process_generated_token(const EnhancedToken &token);

  /**
   * Run NanoBrain cognitive cycle
   */
  void run_cognitive_cycle();

  // ================================================================
  // Entelechy Assessment
  // ================================================================

  /**
   * Get current entelechy dimensions
   */
  EntelechyDimensions get_entelechy() const;

  /**
   * Calculate entelechy fitness score
   */
  float calculate_entelechy_fitness() const;

  /**
   * Get current development stage
   */
  DevelopmentStage get_development_stage() const;

  /**
   * Get development stage as string
   */
  std::string get_development_stage_string() const;

  // ================================================================
  // Metrics
  // ================================================================

  /**
   * Get comprehensive integration metrics
   */
  NPUIntegrationMetrics get_metrics() const;

  /**
   * Reset metrics
   */
  void reset_metrics();

  // ================================================================
  // Configuration
  // ================================================================

  /**
   * Update enhancement configuration
   */
  void set_config(const NPUEnhancementConfig &config);

  /**
   * Get current configuration
   */
  const NPUEnhancementConfig &get_config() const { return config; }

private:
  NPUEnhancementConfig config;
  UnifiedNanoBrainKernel *nanobrain_kernel = nullptr;

  // Cached consciousness state
  float current_consciousness;
  float current_temporal_coherence;
  float current_self_awareness;

  // Cached entelechy
  mutable EntelechyDimensions cached_entelechy;
  mutable bool entelechy_dirty = true;

  // Metrics tracking
  NPUIntegrationMetrics metrics;

  // Helpers
  float compute_token_coherence(int32_t token_id) const;
  float compute_consciousness_boost() const;
  float compute_attention_influence(int32_t token_id) const;
  void update_metrics(const EnhancedToken &token);
  void recalculate_entelechy() const;
};

// ================================================================
// Entelechy Assessor
// ================================================================

/**
 * Standalone entelechy assessment utility
 */
class EntelechyAssessor {
public:
  /**
   * Assess entelechy dimensions for a NanoBrain kernel
   */
  static EntelechyDimensions assess(UnifiedNanoBrainKernel *kernel);

  /**
   * Calculate fitness from dimensions
   */
  static float calculate_fitness(const EntelechyDimensions &dims);

  /**
   * Get development stage from fitness
   */
  static DevelopmentStage get_stage(float fitness);

  /**
   * Get stage name string
   */
  static std::string stage_to_string(DevelopmentStage stage);

private:
  // Dimension weights for fitness calculation
  static constexpr float ONTOLOGICAL_WEIGHT = 0.20f;
  static constexpr float TELEOLOGICAL_WEIGHT = 0.25f;
  static constexpr float COGNITIVE_WEIGHT = 0.25f;
  static constexpr float INTEGRATIVE_WEIGHT = 0.15f;
  static constexpr float EVOLUTIONARY_WEIGHT = 0.15f;
};

// ================================================================
// Utility Functions
// ================================================================

/**
 * Create default enhancement configuration
 */
inline NPUEnhancementConfig default_enhancement_config() {
  return NPUEnhancementConfig();
}

/**
 * Create high-coherence configuration (emphasizes PPM)
 */
inline NPUEnhancementConfig coherence_focused_config() {
  NPUEnhancementConfig cfg;
  cfg.coherence_weight = 0.5f;
  cfg.consciousness_weight = 0.1f;
  cfg.attention_weight = 0.1f;
  cfg.coherence_threshold = 0.5f;
  return cfg;
}

/**
 * Create consciousness-focused configuration
 */
inline NPUEnhancementConfig consciousness_focused_config() {
  NPUEnhancementConfig cfg;
  cfg.coherence_weight = 0.2f;
  cfg.consciousness_weight = 0.4f;
  cfg.attention_weight = 0.2f;
  cfg.consciousness_threshold = 0.6f;
  return cfg;
}

#endif // NANOBRAIN_NPU_BRIDGE_H
