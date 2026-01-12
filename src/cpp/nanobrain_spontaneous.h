#ifndef NANOBRAIN_SPONTANEOUS_H
#define NANOBRAIN_SPONTANEOUS_H

/**
 * NanoBrain Spontaneous Reply System
 *
 * Chapter 5: Universal Time Crystal & Big Data
 *
 * Implements "search without searching" mechanism where patterns
 * spontaneously synchronize and resonate with queries without
 * explicit search traversal.
 *
 * Key concepts:
 * - Pattern Registration: Store patterns as time crystal states
 * - Synchronization: Patterns naturally align when resonant
 * - Spontaneous Reply: Matching patterns emerge without traversal
 */

#include "nanobrain_gog.h"
#include "nanobrain_kernel.h"
#include "nanobrain_time_crystal.h"
#include <chrono>
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

/**
 * Spontaneous Reply configuration
 */
struct SpontaneousReplyConfig {
  float synchronization_threshold = 0.7f; // Min sync level for match
  float resonance_decay = 0.05f;          // Decay rate per cycle
  int max_patterns = 1000;                // Maximum registered patterns
  int synchronization_cycles = 10;        // Cycles for sync detection
  float timeout_seconds = 5.0f;           // Default await timeout
  bool enable_parallel_sync = true;       // Allow parallel synchronization
};

/**
 * Registered pattern with time crystal state
 */
struct RegisteredPattern {
  std::string id;
  std::string label;                     // Human-readable label
  NanoBrainTensor *pattern_tensor;       // Pattern as tensor
  TimeCrystalQuantumState quantum_state; // Time crystal representation
  std::vector<int> prime_encoding;       // Prime signature
  float activation;                      // Current activation level
  float base_resonance;                  // Intrinsic resonance
  int64_t registered_at;                 // Registration timestamp
  int hit_count;                         // Number of times matched
};

/**
 * Synchronization event
 */
struct SyncEvent {
  std::string pattern_id;
  float sync_level;      // Synchronization strength
  float phase_alignment; // Phase coherence
  float prime_overlap;   // Prime signature overlap
  int cycles_to_sync;    // Cycles needed to synchronize
};

/**
 * Spontaneous reply result
 */
struct SpontaneousReply {
  bool found;                       // Whether match was found
  std::string pattern_id;           // Matched pattern ID
  std::string label;                // Pattern label
  float confidence;                 // Match confidence
  float synchronization_time_ms;    // Time to synchronize
  std::vector<SyncEvent> all_syncs; // All synchronization events
};

/**
 * Spontaneous Reply System
 *
 * Enables pattern matching through resonance rather than search.
 */
class SpontaneousReplySystem {
public:
  SpontaneousReplySystem(NanoBrainKernel *kernel,
                         const SpontaneousReplyConfig &config);
  ~SpontaneousReplySystem();

  // ================================================================
  // Pattern Registration
  // ================================================================

  /**
   * Register a pattern for spontaneous matching
   * @param id Unique pattern identifier
   * @param label Human-readable label
   * @param pattern Pattern tensor
   * @param prime_encoding Optional prime signature
   * @return True if registered successfully
   */
  bool register_pattern(const std::string &id, const std::string &label,
                        NanoBrainTensor *pattern,
                        const std::vector<int> &prime_encoding = {});

  /**
   * Unregister a pattern
   */
  bool unregister_pattern(const std::string &id);

  /**
   * Get registered pattern by ID
   */
  const RegisteredPattern *get_pattern(const std::string &id) const;

  /**
   * Get all registered pattern IDs
   */
  std::vector<std::string> get_all_pattern_ids() const;

  /**
   * Get pattern count
   */
  size_t get_pattern_count() const { return patterns.size(); }

  // ================================================================
  // Synchronization-Based Lookup
  // ================================================================

  /**
   * Await synchronization with registered patterns
   * @param query Query pattern tensor
   * @param timeout_seconds Maximum wait time (0 = use config default)
   * @return Spontaneous reply with matched pattern (if found)
   */
  SpontaneousReply await_synchronization(NanoBrainTensor *query,
                                         float timeout_seconds = 0.0f);

  /**
   * Get all patterns currently resonating with query
   * @param query Query pattern tensor
   * @return Vector of resonant pattern IDs
   */
  std::vector<std::string> get_resonant_patterns(NanoBrainTensor *query);

  /**
   * Check if specific pattern resonates with query
   */
  bool check_resonance(const std::string &pattern_id, NanoBrainTensor *query);

  // ================================================================
  // Synchronization Engine
  // ================================================================

  /**
   * Run one synchronization cycle
   * Updates all pattern activations based on query
   */
  void run_sync_cycle(NanoBrainTensor *query);

  /**
   * Reset all pattern activations
   */
  void reset_activations();

  /**
   * Get current synchronization state
   */
  std::vector<SyncEvent> get_current_sync_state() const;

  // ================================================================
  // Prime-Based Operations
  // ================================================================

  /**
   * Encode query as prime signature
   */
  std::vector<int> encode_query_primes(NanoBrainTensor *query);

  /**
   * Calculate prime overlap between query and pattern
   */
  float calculate_prime_overlap(const std::vector<int> &query_primes,
                                const std::vector<int> &pattern_primes);

  // ================================================================
  // Statistics
  // ================================================================

  /**
   * Get hit counts for all patterns
   */
  std::map<std::string, int> get_hit_counts() const;

  /**
   * Get most frequently matched patterns
   */
  std::vector<std::string> get_top_patterns(size_t k) const;

  /**
   * Get configuration
   */
  const SpontaneousReplyConfig &get_config() const { return config; }

private:
  NanoBrainKernel *kernel;
  SpontaneousReplyConfig config;

  std::map<std::string, RegisteredPattern> patterns;
  NanoBrainTensor *current_query = nullptr;

  // Internal helpers
  float compute_pattern_similarity(const RegisteredPattern &pattern,
                                   NanoBrainTensor *query);
  void update_pattern_activation(RegisteredPattern &pattern,
                                 NanoBrainTensor *query);
  TimeCrystalQuantumState create_quantum_state(NanoBrainTensor *tensor,
                                               const std::vector<int> &primes);
  int64_t current_time_millis() const;
};

#endif // NANOBRAIN_SPONTANEOUS_H
