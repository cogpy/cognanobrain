#include "nanobrain_spontaneous.h"
#include <algorithm>
#include <chrono>
#include <cmath>
#include <numeric>

// ================================================================
// SpontaneousReplyEngine Implementation
// ================================================================

SpontaneousReplyEngine::SpontaneousReplyEngine(TimeCrystalKernel *tc,
                                               GardenOfGardens *g,
                                               const SpontaneousConfig &cfg)
    : tc_kernel(tc), gog(g), config(cfg), initialized(false),
      continuous_mode(false), pattern_counter(0) {}

SpontaneousReplyEngine::~SpontaneousReplyEngine() = default;

void SpontaneousReplyEngine::initialize() {
  patterns.clear();
  while (!spontaneous_queue.empty())
    spontaneous_queue.pop();
  pattern_counter = 0;
  initialized = true;
}

void SpontaneousReplyEngine::reset() {
  patterns.clear();
  while (!spontaneous_queue.empty())
    spontaneous_queue.pop();
  continuous_mode = false;
  initialized = false;
}

std::string SpontaneousReplyEngine::generate_pattern_id() {
  return "sp_" + std::to_string(++pattern_counter);
}

// ================================================================
// Pattern Registration
// ================================================================

std::string
SpontaneousReplyEngine::register_pattern(const std::vector<int> &primes,
                                         const std::vector<float> &embedding,
                                         const std::string &label) {
  RegisteredPattern pattern;
  pattern.id = generate_pattern_id();
  pattern.label = label.empty() ? pattern.id : label;
  pattern.primes = primes;
  pattern.embedding = embedding;
  pattern.current_resonance = 0.0f;
  pattern.phase = 0.0f;

  patterns.push_back(pattern);
  return pattern.id;
}

bool SpontaneousReplyEngine::unregister_pattern(const std::string &id) {
  auto it = std::find_if(patterns.begin(), patterns.end(),
                         [&id](const auto &p) { return p.id == id; });
  if (it != patterns.end()) {
    patterns.erase(it);
    return true;
  }
  return false;
}

// ================================================================
// Resonance Calculations
// ================================================================

float SpontaneousReplyEngine::calculate_prime_resonance(
    const std::vector<int> &a, const std::vector<int> &b) const {
  if (a.empty() || b.empty())
    return 0.0f;

  // Count matching primes
  int matches = 0;
  for (int pa : a) {
    for (int pb : b) {
      if (pa == pb) {
        matches++;
        break;
      }
      // Also check for harmonic relationship
      float ratio = static_cast<float>(pa) / static_cast<float>(pb);
      if (std::abs(ratio - std::round(ratio)) < 0.1f) {
        matches++;
        break;
      }
    }
  }

  return static_cast<float>(matches) / std::max(a.size(), b.size());
}

float SpontaneousReplyEngine::calculate_embedding_similarity(
    const std::vector<float> &a, const std::vector<float> &b) const {
  if (a.empty() || b.empty())
    return 0.0f;

  size_t len = std::min(a.size(), b.size());
  float dot = 0.0f, norm_a = 0.0f, norm_b = 0.0f;

  for (size_t i = 0; i < len; i++) {
    dot += a[i] * b[i];
    norm_a += a[i] * a[i];
    norm_b += b[i] * b[i];
  }

  float denom = std::sqrt(norm_a) * std::sqrt(norm_b);
  return (denom > 0.001f) ? dot / denom : 0.0f;
}

void SpontaneousReplyEngine::update_pattern_resonances(
    const SpontaneousQuery &query) {
  for (auto &pattern : patterns) {
    float prime_res =
        calculate_prime_resonance(query.prime_pattern, pattern.primes);
    float embed_sim =
        calculate_embedding_similarity(query.embedding, pattern.embedding);

    // Combined resonance with mode weighting
    float combined = 0.0f;
    switch (config.mode) {
    case SyncMode::Phase:
      combined = prime_res; // Pure prime matching
      break;
    case SyncMode::Frequency:
      combined = embed_sim; // Pure embedding
      break;
    case SyncMode::Resonance:
      combined = (prime_res + embed_sim) / 2.0f; // Balanced
      break;
    case SyncMode::Coherence:
      combined = std::max(prime_res, embed_sim); // Best of both
      break;
    }

    // Update pattern resonance with decay
    pattern.current_resonance =
        pattern.current_resonance * 0.9f + combined * 0.1f * query.urgency;
    pattern.phase += combined * 0.1f;
  }
}

// ================================================================
// Query Functions
// ================================================================

SyncLookupResult SpontaneousReplyEngine::query(const SpontaneousQuery &query) {
  auto start = std::chrono::high_resolution_clock::now();
  SyncLookupResult result;
  result.sync_cycles = 0;

  // Reset resonances
  for (auto &pattern : patterns) {
    pattern.current_resonance = 0.0f;
  }

  // Run synchronization cycles
  for (int cycle = 0; cycle < config.sync_timeout_cycles; cycle++) {
    update_pattern_resonances(query);
    result.sync_cycles++;

    // Check for convergent matches
    int matches_found = 0;
    for (const auto &pattern : patterns) {
      if (pattern.current_resonance >= config.resonance_threshold) {
        matches_found++;
      }
    }

    if (matches_found >= config.max_matches ||
        (matches_found > 0 && !config.allow_partial_match)) {
      break;
    }
  }

  // Collect matches
  for (const auto &pattern : patterns) {
    if (pattern.current_resonance >= config.resonance_threshold) {
      SpontaneousReply reply;
      reply.query_id = query.id;
      reply.match_id = pattern.id;
      reply.confidence = pattern.current_resonance;
      reply.resonance_strength = pattern.current_resonance;
      reply.matched_primes = pattern.primes;
      reply.is_spontaneous = false;
      result.matches.push_back(reply);
    }
  }

  // Sort by confidence
  std::sort(
      result.matches.begin(), result.matches.end(),
      [](const auto &a, const auto &b) { return a.confidence > b.confidence; });

  // Limit results
  if (result.matches.size() > static_cast<size_t>(config.max_matches)) {
    result.matches.resize(config.max_matches);
  }

  // Calculate total coherence
  result.total_coherence = 0.0f;
  for (const auto &match : result.matches) {
    result.total_coherence += match.confidence;
  }
  if (!result.matches.empty()) {
    result.total_coherence /= result.matches.size();
  }

  auto end = std::chrono::high_resolution_clock::now();
  result.total_time_us =
      std::chrono::duration_cast<std::chrono::microseconds>(end - start)
          .count();

  // Set latency on matches
  for (auto &match : result.matches) {
    match.latency_us = result.total_time_us;
  }

  return result;
}

SyncLookupResult
SpontaneousReplyEngine::query_primes(const std::vector<int> &primes) {
  SpontaneousQuery q;
  q.id = "quick_" + std::to_string(pattern_counter);
  q.prime_pattern = primes;
  q.urgency = 1.0f;
  q.timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::system_clock::now().time_since_epoch())
                    .count();
  return query(q);
}

SyncLookupResult
SpontaneousReplyEngine::query_embedding(const std::vector<float> &embedding) {
  SpontaneousQuery q;
  q.id = "quick_" + std::to_string(pattern_counter);
  q.embedding = embedding;
  q.urgency = 1.0f;
  q.timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::system_clock::now().time_since_epoch())
                    .count();
  return query(q);
}

SyncLookupResult
SpontaneousReplyEngine::sync_lookup(const std::vector<int> &target_primes,
                                    int max_cycles) {
  SpontaneousConfig old_config = config;
  config.sync_timeout_cycles = max_cycles;
  auto result = query_primes(target_primes);
  config = old_config;
  return result;
}

std::vector<std::string>
SpontaneousReplyEngine::get_resonating_patterns(float threshold) const {
  std::vector<std::string> result;
  for (const auto &pattern : patterns) {
    if (pattern.current_resonance >= threshold) {
      result.push_back(pattern.id);
    }
  }
  return result;
}

// ================================================================
// Continuous Mode
// ================================================================

void SpontaneousReplyEngine::enable_continuous_mode() {
  continuous_mode = true;
}

void SpontaneousReplyEngine::disable_continuous_mode() {
  continuous_mode = false;
}

std::vector<SpontaneousReply>
SpontaneousReplyEngine::get_spontaneous_matches() {
  std::vector<SpontaneousReply> result;
  while (!spontaneous_queue.empty()) {
    result.push_back(spontaneous_queue.front());
    spontaneous_queue.pop();
  }
  return result;
}

void SpontaneousReplyEngine::update_config(
    const SpontaneousConfig &new_config) {
  config = new_config;
}
