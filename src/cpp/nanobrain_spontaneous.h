#ifndef NANOBRAIN_SPONTANEOUS_H
#define NANOBRAIN_SPONTANEOUS_H

/**
 * Spontaneous Reply System - Chapter 5, Task 5.3
 *
 * "Search without searching" - using time crystal synchronization
 * for spontaneous pattern matching and associative lookup.
 */

#include "nanobrain_gog.h"
#include "nanobrain_time_crystal.h"
#include <functional>
#include <queue>
#include <vector>

// ================================================================
// Synchronization Types
// ================================================================

enum class SyncMode {
  Phase,     // Phase-locked synchronization
  Frequency, // Frequency matching
  Resonance, // Resonance-based coupling
  Coherence  // Coherence maximization
};

// ================================================================
// Query & Response Structures
// ================================================================

struct SpontaneousQuery {
  std::string id;
  std::vector<int> prime_pattern;
  std::vector<float> embedding;
  float urgency;
  int64_t timestamp;
};

struct SpontaneousReply {
  std::string query_id;
  std::string match_id;
  float confidence;
  float resonance_strength;
  std::vector<int> matched_primes;
  int64_t latency_us;
  bool is_spontaneous; // True if emerged without explicit search
};

struct SyncLookupResult {
  std::vector<SpontaneousReply> matches;
  float total_coherence;
  int sync_cycles;
  int64_t total_time_us;
};

// ================================================================
// Configuration
// ================================================================

struct SpontaneousConfig {
  SyncMode mode = SyncMode::Resonance;
  int max_matches = 10;
  float coherence_threshold = 0.6f;
  float resonance_threshold = 0.5f;
  int sync_timeout_cycles = 100;
  bool allow_partial_match = true;
};

// ================================================================
// Spontaneous Reply Engine
// ================================================================

class SpontaneousReplyEngine {
public:
  SpontaneousReplyEngine(TimeCrystalKernel *tc_kernel,
                         GardenOfGardens *gog = nullptr,
                         const SpontaneousConfig &config = {});
  ~SpontaneousReplyEngine();

  void initialize();
  void reset();
  bool is_initialized() const { return initialized; }

  // ================================================================
  // Pattern Registration
  // ================================================================

  // Register a pattern for spontaneous matching
  std::string register_pattern(const std::vector<int> &primes,
                               const std::vector<float> &embedding,
                               const std::string &label = "");

  // Remove pattern
  bool unregister_pattern(const std::string &id);

  // Get registered pattern count
  size_t pattern_count() const { return patterns.size(); }

  // ================================================================
  // Spontaneous Query
  // ================================================================

  // Submit query and let system find matches spontaneously
  SyncLookupResult query(const SpontaneousQuery &query);

  // Quick lookup by primes only
  SyncLookupResult query_primes(const std::vector<int> &primes);

  // Quick lookup by embedding only
  SyncLookupResult query_embedding(const std::vector<float> &embedding);

  // ================================================================
  // Synchronization-Based Lookup
  // ================================================================

  // Run sync cycles until matches emerge or timeout
  SyncLookupResult sync_lookup(const std::vector<int> &target_primes,
                               int max_cycles = 50);

  // Check if any patterns are currently resonating
  std::vector<std::string>
  get_resonating_patterns(float threshold = 0.5f) const;

  // ================================================================
  // Continuous Mode
  // ================================================================

  // Enable continuous listening for spontaneous matches
  void enable_continuous_mode();
  void disable_continuous_mode();
  bool is_continuous_mode() const { return continuous_mode; }

  // Get spontaneous matches that emerged
  std::vector<SpontaneousReply> get_spontaneous_matches();

  // ================================================================
  // Configuration
  // ================================================================

  const SpontaneousConfig &get_config() const { return config; }
  void update_config(const SpontaneousConfig &new_config);

private:
  TimeCrystalKernel *tc_kernel;
  GardenOfGardens *gog;
  SpontaneousConfig config;
  bool initialized = false;
  bool continuous_mode = false;

  // Registered patterns
  struct RegisteredPattern {
    std::string id;
    std::string label;
    std::vector<int> primes;
    std::vector<float> embedding;
    float current_resonance;
    float phase;
  };
  std::vector<RegisteredPattern> patterns;

  // Spontaneous match queue
  std::queue<SpontaneousReply> spontaneous_queue;

  // Counter
  int pattern_counter = 0;

  // Private helpers
  std::string generate_pattern_id();
  float calculate_prime_resonance(const std::vector<int> &a,
                                  const std::vector<int> &b) const;
  float calculate_embedding_similarity(const std::vector<float> &a,
                                       const std::vector<float> &b) const;
  void update_pattern_resonances(const SpontaneousQuery &query);
};

#endif // NANOBRAIN_SPONTANEOUS_H
