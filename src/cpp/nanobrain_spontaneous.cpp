#include "nanobrain_spontaneous.h"
#include <algorithm>
#include <chrono>
#include <cmath>
#include <iostream>
#include <random>
#include <thread>

// ================================================================
// Constructor / Destructor
// ================================================================

SpontaneousReplySystem::SpontaneousReplySystem(
    NanoBrainKernel *kernel, const SpontaneousReplyConfig &config)
    : kernel(kernel), config(config) {}

SpontaneousReplySystem::~SpontaneousReplySystem() {}

int64_t SpontaneousReplySystem::current_time_millis() const {
  return std::chrono::duration_cast<std::chrono::milliseconds>(
             std::chrono::system_clock::now().time_since_epoch())
      .count();
}

// ================================================================
// Pattern Registration
// ================================================================

bool SpontaneousReplySystem::register_pattern(
    const std::string &id, const std::string &label, NanoBrainTensor *pattern,
    const std::vector<int> &prime_encoding) {
  if (patterns.size() >= static_cast<size_t>(config.max_patterns)) {
    std::cerr << "[SpontaneousReply] Max patterns reached" << std::endl;
    return false;
  }

  if (patterns.find(id) != patterns.end()) {
    std::cerr << "[SpontaneousReply] Pattern ID already exists: " << id
              << std::endl;
    return false;
  }

  RegisteredPattern rp;
  rp.id = id;
  rp.label = label;
  rp.pattern_tensor = pattern;
  rp.prime_encoding =
      prime_encoding.empty() ? encode_query_primes(pattern) : prime_encoding;
  rp.activation = 0.0f;
  rp.base_resonance = 0.5f;
  rp.registered_at = current_time_millis();
  rp.hit_count = 0;

  // Create quantum state
  rp.quantum_state = create_quantum_state(pattern, rp.prime_encoding);

  patterns[id] = rp;
  return true;
}

bool SpontaneousReplySystem::unregister_pattern(const std::string &id) {
  return patterns.erase(id) > 0;
}

const RegisteredPattern *
SpontaneousReplySystem::get_pattern(const std::string &id) const {
  auto it = patterns.find(id);
  if (it != patterns.end()) {
    return &it->second;
  }
  return nullptr;
}

std::vector<std::string> SpontaneousReplySystem::get_all_pattern_ids() const {
  std::vector<std::string> ids;
  ids.reserve(patterns.size());
  for (const auto &[id, _] : patterns) {
    ids.push_back(id);
  }
  return ids;
}

// ================================================================
// Synchronization-Based Lookup
// ================================================================

SpontaneousReply
SpontaneousReplySystem::await_synchronization(NanoBrainTensor *query,
                                              float timeout_seconds) {
  SpontaneousReply reply;
  reply.found = false;
  reply.confidence = 0.0f;
  reply.synchronization_time_ms = 0.0f;

  if (!query || patterns.empty()) {
    return reply;
  }

  float timeout =
      timeout_seconds > 0 ? timeout_seconds : config.timeout_seconds;
  auto start_time = std::chrono::high_resolution_clock::now();

  // Reset activations
  reset_activations();

  // Encode query primes
  std::vector<int> query_primes = encode_query_primes(query);

  int cycles = 0;
  while (cycles < config.synchronization_cycles * 10) { // Max iterations
    // Run sync cycle
    run_sync_cycle(query);
    cycles++;

    // Check for synchronized patterns
    for (auto &[id, pattern] : patterns) {
      float similarity = compute_pattern_similarity(pattern, query);
      float prime_overlap =
          calculate_prime_overlap(query_primes, pattern.prime_encoding);
      float sync_level =
          (similarity + prime_overlap + pattern.activation) / 3.0f;

      if (sync_level >= config.synchronization_threshold) {
        reply.found = true;
        reply.pattern_id = id;
        reply.label = pattern.label;
        reply.confidence = sync_level;

        // Record sync event
        SyncEvent event;
        event.pattern_id = id;
        event.sync_level = sync_level;
        event.phase_alignment = pattern.quantum_state.quantum_phase;
        event.prime_overlap = prime_overlap;
        event.cycles_to_sync = cycles;
        reply.all_syncs.push_back(event);

        // Update hit count
        pattern.hit_count++;

        auto end_time = std::chrono::high_resolution_clock::now();
        reply.synchronization_time_ms =
            std::chrono::duration<float, std::milli>(end_time - start_time)
                .count();

        return reply;
      }
    }

    // Check timeout
    auto current_time = std::chrono::high_resolution_clock::now();
    float elapsed =
        std::chrono::duration<float>(current_time - start_time).count();
    if (elapsed >= timeout) {
      break;
    }
  }

  // No match found, but collect partial sync events
  for (const auto &[id, pattern] : patterns) {
    float similarity = compute_pattern_similarity(pattern, query);
    if (similarity > 0.3f) {
      SyncEvent event;
      event.pattern_id = id;
      event.sync_level = similarity;
      event.phase_alignment = pattern.quantum_state.quantum_phase;
      event.prime_overlap =
          calculate_prime_overlap(query_primes, pattern.prime_encoding);
      event.cycles_to_sync = cycles;
      reply.all_syncs.push_back(event);
    }
  }

  auto end_time = std::chrono::high_resolution_clock::now();
  reply.synchronization_time_ms =
      std::chrono::duration<float, std::milli>(end_time - start_time).count();

  return reply;
}

std::vector<std::string>
SpontaneousReplySystem::get_resonant_patterns(NanoBrainTensor *query) {
  std::vector<std::string> resonant;

  if (!query) {
    return resonant;
  }

  std::vector<int> query_primes = encode_query_primes(query);

  for (const auto &[id, pattern] : patterns) {
    float similarity = compute_pattern_similarity(pattern, query);
    float prime_overlap =
        calculate_prime_overlap(query_primes, pattern.prime_encoding);

    if ((similarity + prime_overlap) / 2.0f >=
        config.synchronization_threshold * 0.5f) {
      resonant.push_back(id);
    }
  }

  return resonant;
}

bool SpontaneousReplySystem::check_resonance(const std::string &pattern_id,
                                             NanoBrainTensor *query) {
  auto it = patterns.find(pattern_id);
  if (it == patterns.end() || !query) {
    return false;
  }

  float similarity = compute_pattern_similarity(it->second, query);
  return similarity >= config.synchronization_threshold;
}

// ================================================================
// Synchronization Engine
// ================================================================

void SpontaneousReplySystem::run_sync_cycle(NanoBrainTensor *query) {
  if (!query) {
    return;
  }

  for (auto &[id, pattern] : patterns) {
    update_pattern_activation(pattern, query);

    // Decay activation
    pattern.activation *= (1.0f - config.resonance_decay);

    // Update quantum phase
    pattern.quantum_state.quantum_phase += 0.1f;
    if (pattern.quantum_state.quantum_phase >= 2.0f * PI) {
      pattern.quantum_state.quantum_phase -= 2.0f * PI;
    }
  }
}

void SpontaneousReplySystem::reset_activations() {
  for (auto &[id, pattern] : patterns) {
    pattern.activation = 0.0f;
  }
}

std::vector<SyncEvent> SpontaneousReplySystem::get_current_sync_state() const {
  std::vector<SyncEvent> events;

  for (const auto &[id, pattern] : patterns) {
    SyncEvent event;
    event.pattern_id = id;
    event.sync_level = pattern.activation;
    event.phase_alignment = pattern.quantum_state.quantum_phase;
    event.prime_overlap = 0.0f; // Would need query to compute
    event.cycles_to_sync = 0;
    events.push_back(event);
  }

  return events;
}

// ================================================================
// Prime-Based Operations
// ================================================================

std::vector<int>
SpontaneousReplySystem::encode_query_primes(NanoBrainTensor *query) {
  std::vector<int> primes;

  if (!query || !query->ggml_tensor) {
    return primes;
  }

  float *data = static_cast<float *>(query->ggml_tensor->data);
  int64_t size = ggml_nelements(query->ggml_tensor);

  // Convert tensor values to prime indices
  for (int64_t i = 0; i < std::min(size, int64_t(15)); i++) {
    float val = std::abs(data[i]);
    int prime_idx = static_cast<int>(val * FUNDAMENTAL_PRIMES_COUNT) %
                    FUNDAMENTAL_PRIMES_COUNT;
    primes.push_back(FUNDAMENTAL_PRIMES[prime_idx]);
  }

  // Remove duplicates and sort
  std::sort(primes.begin(), primes.end());
  primes.erase(std::unique(primes.begin(), primes.end()), primes.end());

  return primes;
}

float SpontaneousReplySystem::calculate_prime_overlap(
    const std::vector<int> &query_primes,
    const std::vector<int> &pattern_primes) {
  if (query_primes.empty() || pattern_primes.empty()) {
    return 0.0f;
  }

  int overlap = 0;
  for (int p : query_primes) {
    if (std::find(pattern_primes.begin(), pattern_primes.end(), p) !=
        pattern_primes.end()) {
      overlap++;
    }
  }

  return static_cast<float>(overlap) /
         std::max(query_primes.size(), pattern_primes.size());
}

// ================================================================
// Statistics
// ================================================================

std::map<std::string, int> SpontaneousReplySystem::get_hit_counts() const {
  std::map<std::string, int> counts;
  for (const auto &[id, pattern] : patterns) {
    counts[id] = pattern.hit_count;
  }
  return counts;
}

std::vector<std::string>
SpontaneousReplySystem::get_top_patterns(size_t k) const {
  std::vector<std::pair<std::string, int>> sorted;
  for (const auto &[id, pattern] : patterns) {
    sorted.push_back({id, pattern.hit_count});
  }

  std::sort(sorted.begin(), sorted.end(),
            [](const auto &a, const auto &b) { return a.second > b.second; });

  std::vector<std::string> result;
  for (size_t i = 0; i < std::min(k, sorted.size()); i++) {
    result.push_back(sorted[i].first);
  }

  return result;
}

// ================================================================
// Internal Helpers
// ================================================================

float SpontaneousReplySystem::compute_pattern_similarity(
    const RegisteredPattern &pattern, NanoBrainTensor *query) {
  if (!pattern.pattern_tensor || !query) {
    return 0.0f;
  }

  if (!pattern.pattern_tensor->ggml_tensor || !query->ggml_tensor) {
    return 0.0f;
  }

  int64_t size1 = ggml_nelements(pattern.pattern_tensor->ggml_tensor);
  int64_t size2 = ggml_nelements(query->ggml_tensor);

  if (size1 != size2) {
    return 0.0f;
  }

  float *data1 =
      static_cast<float *>(pattern.pattern_tensor->ggml_tensor->data);
  float *data2 = static_cast<float *>(query->ggml_tensor->data);

  // Cosine similarity
  float dot = 0.0f;
  float norm1 = 0.0f;
  float norm2 = 0.0f;

  for (int64_t i = 0; i < size1; i++) {
    dot += data1[i] * data2[i];
    norm1 += data1[i] * data1[i];
    norm2 += data2[i] * data2[i];
  }

  float magnitude = std::sqrt(norm1) * std::sqrt(norm2);
  if (magnitude < 1e-6f) {
    return 0.0f;
  }

  return (dot / magnitude + 1.0f) / 2.0f; // Normalize to [0, 1]
}

void SpontaneousReplySystem::update_pattern_activation(
    RegisteredPattern &pattern, NanoBrainTensor *query) {
  float similarity = compute_pattern_similarity(pattern, query);

  // Activation grows with similarity, modulated by quantum coherence
  float coherence_factor = pattern.quantum_state.temporal_coherence;
  float activation_boost =
      similarity * coherence_factor * pattern.base_resonance;

  pattern.activation += activation_boost;

  // Clamp to [0, 1]
  pattern.activation = std::min(1.0f, std::max(0.0f, pattern.activation));
}

TimeCrystalQuantumState
SpontaneousReplySystem::create_quantum_state(NanoBrainTensor *tensor,
                                             const std::vector<int> &primes) {
  TimeCrystalQuantumState state;

  // Initialize dimensions from tensor
  for (int d = 0; d < TIME_CRYSTAL_DIMENSIONS; d++) {
    state.dimensions[d] = 0.0f;
  }

  if (tensor && tensor->ggml_tensor) {
    float *data = static_cast<float *>(tensor->ggml_tensor->data);
    int64_t size = ggml_nelements(tensor->ggml_tensor);

    for (int d = 0;
         d < std::min(TIME_CRYSTAL_DIMENSIONS, static_cast<int>(size)); d++) {
      state.dimensions[d] = data[d];
    }
  }

  state.prime_signature = primes;

  // Compute initial phase from primes
  float phase = 0.0f;
  for (int p : primes) {
    phase += std::sin(p * PI / 180.0f);
  }
  state.quantum_phase = std::fmod(phase, 2.0f * PI);

  // Coherence from prime product
  float product = 1.0f;
  float sum = 0.0f;
  for (int p : primes) {
    product *= p;
    sum += p;
  }
  if (sum > 0 && !primes.empty()) {
    state.temporal_coherence =
        0.5f + 0.5f * std::sin(std::sqrt(product) * PI / sum);
  } else {
    state.temporal_coherence = 0.5f;
  }

  state.fractal_dimension = 3.0f + (primes.size() % 5);
  state.resonance_frequency = 440.0f;

  return state;
}
