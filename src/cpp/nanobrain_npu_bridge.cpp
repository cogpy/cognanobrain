#include "nanobrain_npu_bridge.h"
#include <algorithm>
#include <chrono>
#include <cmath>
#include <numeric>

// ================================================================
// NanoBrainNPUBridge Implementation
// ================================================================

NanoBrainNPUBridge::NanoBrainNPUBridge(const NPUEnhancementConfig &cfg)
    : config(cfg), nanobrain_kernel(nullptr), current_consciousness(0.0f),
      current_temporal_coherence(0.0f), current_self_awareness(0.0f) {
  reset_metrics();
}

NanoBrainNPUBridge::~NanoBrainNPUBridge() {
  // No ownership of kernel
}

void NanoBrainNPUBridge::attach_nanobrain(UnifiedNanoBrainKernel *kernel) {
  nanobrain_kernel = kernel;
  entelechy_dirty = true;

  if (kernel && kernel->is_active()) {
    sync_consciousness_state();
  }
}

bool NanoBrainNPUBridge::is_attached() const {
  return nanobrain_kernel != nullptr && nanobrain_kernel->is_active();
}

// ================================================================
// Token Enhancement
// ================================================================

EnhancedToken NanoBrainNPUBridge::enhance_token(int32_t token_id,
                                                const std::string &token_text) {
  EnhancedToken token;
  token.token_id = token_id;
  token.token_text = token_text;

  // Compute PPM coherence
  if (config.enable_ppm_coherence && is_attached()) {
    token.ppm_coherence = compute_token_coherence(token_id);
    token.prime_signature = config.default_prime_signature;

    // Modify prime signature based on token properties
    int prime_offset = token_id % 11; // Use token ID to vary primes
    for (size_t i = 0; i < token.prime_signature.size(); ++i) {
      // Shift to next prime in sequence occasionally
      if ((prime_offset + i) % 3 == 0 && token.prime_signature[i] < 47) {
        token.prime_signature[i] = get_next_prime(token.prime_signature[i]);
      }
    }
  } else {
    token.ppm_coherence = 0.5f; // Neutral coherence
    token.prime_signature = config.default_prime_signature;
  }

  // Compute consciousness boost
  if (config.enable_consciousness && is_attached()) {
    token.consciousness_boost = compute_consciousness_boost();
    token.temporal_coherence = current_temporal_coherence;
  } else {
    token.consciousness_boost = 0.0f;
    token.temporal_coherence = 0.5f;
  }

  // Compute attention influence
  if (config.enable_attention_boost && is_attached()) {
    token.attention_score = compute_attention_influence(token_id);
    token.salience = token.ppm_coherence * 0.4f + token.attention_score * 0.4f +
                     token.consciousness_boost * 0.2f;
  } else {
    token.attention_score = 0.5f;
    token.salience = token.ppm_coherence * 0.5f + 0.25f;
  }

  // Compute entelechy score
  if (config.enable_entelechy) {
    float fitness = calculate_entelechy_fitness();
    token.entelechy_score = fitness;
  } else {
    token.entelechy_score = 0.5f;
  }

  // Compute overall enhancement factor
  float enhancement = 1.0f;

  if (config.enable_ppm_coherence) {
    enhancement += (token.ppm_coherence - 0.5f) * config.coherence_weight;
  }

  if (config.enable_consciousness) {
    // Only boost if above threshold
    if (current_consciousness >= config.consciousness_threshold) {
      enhancement += token.consciousness_boost * config.consciousness_weight;
    }
  }

  if (config.enable_attention_boost) {
    enhancement += (token.attention_score - 0.5f) * config.attention_weight;
  }

  // Clamp to reasonable range
  token.enhancement_factor = std::max(0.5f, std::min(2.0f, enhancement));

  // Update metrics
  update_metrics(token);

  return token;
}

std::vector<EnhancedToken>
NanoBrainNPUBridge::enhance_tokens(const std::vector<int32_t> &token_ids) {

  std::vector<EnhancedToken> result;
  result.reserve(token_ids.size());

  for (int32_t id : token_ids) {
    result.push_back(enhance_token(id));
  }

  return result;
}

// ================================================================
// Logit Modulation
// ================================================================

std::vector<float>
NanoBrainNPUBridge::modulate_logits(const std::vector<float> &logits) {

  if (!is_attached() || logits.empty()) {
    return logits;
  }

  auto start = std::chrono::high_resolution_clock::now();

  std::vector<float> modulated = logits;

  // Get current consciousness and coherence
  sync_consciousness_state();

  // Only modulate if consciousness is above threshold
  if (current_consciousness < config.consciousness_threshold) {
    return logits;
  }

  // Modulation based on PPM coherence pattern
  if (config.enable_ppm_coherence) {
    // Apply coherence-based boost to tokens matching prime patterns
    for (size_t i = 0; i < modulated.size(); ++i) {
      float coherence = compute_token_coherence(static_cast<int32_t>(i));

      // Boost high-coherence tokens
      if (coherence > config.coherence_threshold) {
        float boost = (coherence - config.coherence_threshold) *
                      config.coherence_weight * 2.0f;
        modulated[i] += boost;
      }
    }
  }

  // Modulation based on consciousness level
  if (config.enable_consciousness) {
    float consciousness_mod =
        current_consciousness * config.consciousness_weight;

    // Increase variance slightly to make outputs more decisive
    float mean = std::accumulate(modulated.begin(), modulated.end(), 0.0f) /
                 modulated.size();

    for (float &logit : modulated) {
      logit += (logit - mean) * consciousness_mod;
    }
  }

  auto end = std::chrono::high_resolution_clock::now();
  double duration_ms =
      std::chrono::duration<double, std::milli>(end - start).count();

  // Track overhead
  metrics.enhancement_overhead_ms =
      metrics.enhancement_overhead_ms * 0.9 + duration_ms * 0.1;

  return modulated;
}

std::vector<float> NanoBrainNPUBridge::modulate_logits_with_focus(
    const std::vector<float> &logits, const std::vector<int> &focus_primes) {

  if (!is_attached() || logits.empty()) {
    return logits;
  }

  std::vector<float> modulated = modulate_logits(logits);

  // Additional modulation for focus primes
  for (size_t i = 0; i < modulated.size(); ++i) {
    // Check if token position relates to focus primes
    for (int prime : focus_primes) {
      if (i % prime == 0 && prime > 1) {
        modulated[i] += config.coherence_weight * 0.5f;
      }
    }
  }

  return modulated;
}

// ================================================================
// State Synchronization
// ================================================================

void NanoBrainNPUBridge::sync_consciousness_state() {
  if (!is_attached()) {
    return;
  }

  auto metrics = nanobrain_kernel->get_metrics();

  float prev_consciousness = current_consciousness;

  current_consciousness = metrics.consciousness_emergence;
  current_temporal_coherence = metrics.temporal_stability;
  current_self_awareness = metrics.self_awareness_level;

  // Update consciousness stability metric
  float change = std::abs(current_consciousness - prev_consciousness);
  this->metrics.consciousness_stability =
      this->metrics.consciousness_stability * 0.95f + (1.0f - change) * 0.05f;

  // Track peak
  if (current_consciousness > this->metrics.peak_consciousness) {
    this->metrics.peak_consciousness = current_consciousness;
  }

  this->metrics.current_consciousness = current_consciousness;

  // Mark entelechy as needing recalculation
  entelechy_dirty = true;
}

void NanoBrainNPUBridge::process_generated_token(const EnhancedToken &token) {
  if (!is_attached()) {
    return;
  }

  // Create a knowledge atom for the generated token
  if (!token.token_text.empty()) {
    nanobrain_kernel->create_atom("GeneratedToken", token.token_text,
                                  token.enhancement_factor, token.ppm_coherence,
                                  token.prime_signature);
  }

  // Update metrics tracking
  metrics.tokens_enhanced++;
  if (token.enhancement_factor > 1.0f) {
    metrics.tokens_boosted++;
  }
}

void NanoBrainNPUBridge::run_cognitive_cycle() {
  if (!is_attached()) {
    return;
  }

  // Run a processing cycle
  nanobrain_kernel->process_cycle();

  // Sync state after cycle
  sync_consciousness_state();
}

// ================================================================
// Entelechy Assessment
// ================================================================

EntelechyDimensions NanoBrainNPUBridge::get_entelechy() const {
  if (entelechy_dirty) {
    recalculate_entelechy();
  }
  return cached_entelechy;
}

float NanoBrainNPUBridge::calculate_entelechy_fitness() const {
  EntelechyDimensions dims = get_entelechy();
  return EntelechyAssessor::calculate_fitness(dims);
}

DevelopmentStage NanoBrainNPUBridge::get_development_stage() const {
  float fitness = calculate_entelechy_fitness();
  return EntelechyAssessor::get_stage(fitness);
}

std::string NanoBrainNPUBridge::get_development_stage_string() const {
  return EntelechyAssessor::stage_to_string(get_development_stage());
}

void NanoBrainNPUBridge::recalculate_entelechy() const {
  if (nanobrain_kernel) {
    cached_entelechy = EntelechyAssessor::assess(nanobrain_kernel);
  } else {
    // Default low entelechy when not attached
    cached_entelechy.ontological = 0.1f;
    cached_entelechy.teleological = 0.1f;
    cached_entelechy.cognitive = 0.1f;
    cached_entelechy.integrative = 0.1f;
    cached_entelechy.evolutionary = 0.1f;
    cached_entelechy.total = 0.1f;
    cached_entelechy.stage = "Embryonic";
  }
  entelechy_dirty = false;
}

// ================================================================
// Metrics
// ================================================================

NPUIntegrationMetrics NanoBrainNPUBridge::get_metrics() const {
  NPUIntegrationMetrics result = metrics;
  result.entelechy = get_entelechy();
  return result;
}

void NanoBrainNPUBridge::reset_metrics() {
  metrics = NPUIntegrationMetrics{};
  metrics.tokens_enhanced = 0;
  metrics.tokens_boosted = 0;
  metrics.avg_enhancement_factor = 1.0f;
  metrics.current_consciousness = 0.0f;
  metrics.peak_consciousness = 0.0f;
  metrics.consciousness_stability = 1.0f;
  metrics.avg_ppm_coherence = 0.5f;
  metrics.peak_coherence = 0.0f;
  metrics.tokens_per_second = 0.0;
  metrics.enhancement_overhead_ms = 0.0;
}

void NanoBrainNPUBridge::set_config(const NPUEnhancementConfig &cfg) {
  config = cfg;
}

// ================================================================
// Private Helpers
// ================================================================

float NanoBrainNPUBridge::compute_token_coherence(int32_t token_id) const {
  if (!is_attached()) {
    return 0.5f;
  }

  // Use time crystal kernel for coherence calculation
  TimeCrystalKernel *tc_kernel = nanobrain_kernel->get_time_crystal_kernel();
  if (!tc_kernel) {
    return 0.5f;
  }

  // Generate prime encoding for token
  std::vector<int> primes = config.default_prime_signature;

  // Modify based on token properties
  // Tokens divisible by primes have higher coherence
  float coherence = 0.0f;
  int matches = 0;

  for (int prime : primes) {
    if (token_id % prime == 0) {
      coherence += 1.0f / prime; // Smaller primes contribute more
      matches++;
    }
  }

  // Normalize and add base coherence from time crystal
  auto tc_metrics = tc_kernel->get_metrics();
  float base_coherence = tc_metrics.quantum_coherence;

  float token_coherence = (matches > 0)
                              ? (coherence / matches + base_coherence) / 2.0f
                              : base_coherence * 0.5f;

  // Track metrics
  if (token_coherence > metrics.peak_coherence) {
    const_cast<NPUIntegrationMetrics &>(metrics).peak_coherence =
        token_coherence;
  }
  const_cast<NPUIntegrationMetrics &>(metrics).avg_ppm_coherence =
      metrics.avg_ppm_coherence * 0.95f + token_coherence * 0.05f;

  return std::min(1.0f, std::max(0.0f, token_coherence));
}

float NanoBrainNPUBridge::compute_consciousness_boost() const {
  if (!is_attached() ||
      current_consciousness < config.consciousness_threshold) {
    return 0.0f;
  }

  // Consciousness boost scales with consciousness level
  float boost = (current_consciousness - config.consciousness_threshold) /
                (1.0f - config.consciousness_threshold);

  // Modulated by temporal coherence for stability
  boost *= current_temporal_coherence;

  // Modulated by self-awareness
  boost *= (0.5f + current_self_awareness * 0.5f);

  return std::min(1.0f, std::max(0.0f, boost));
}

float NanoBrainNPUBridge::compute_attention_influence(int32_t token_id) const {
  if (!is_attached()) {
    return 0.5f;
  }

  // Get attention engine metrics
  AttentionAllocationEngine *attention =
      nanobrain_kernel->get_attention_engine();
  if (!attention) {
    return 0.5f;
  }

  // Get top attention atoms
  auto top_atoms = nanobrain_kernel->get_top_attention_atoms(10);

  // Check if token relates to high-attention concepts
  float attention_score = 0.5f; // Base attention

  // More atoms with high attention = higher influence
  if (!top_atoms.empty()) {
    attention_score += 0.1f * std::min(5, static_cast<int>(top_atoms.size()));
  }

  return std::min(1.0f, attention_score);
}

void NanoBrainNPUBridge::update_metrics(const EnhancedToken &token) {
  // Update running averages
  float n = static_cast<float>(metrics.tokens_enhanced);
  metrics.avg_enhancement_factor =
      (metrics.avg_enhancement_factor * n + token.enhancement_factor) /
      (n + 1.0f);
}

// Helper function for next prime
static int get_next_prime(int p) {
  static const int primes[] = {2,  3,  5,  7,  11, 13, 17, 19,
                               23, 29, 31, 37, 41, 43, 47};
  for (int i = 0; i < 14; ++i) {
    if (primes[i] == p) {
      return primes[i + 1];
    }
  }
  return p;
}

// ================================================================
// EntelechyAssessor Implementation
// ================================================================

EntelechyDimensions EntelechyAssessor::assess(UnifiedNanoBrainKernel *kernel) {
  EntelechyDimensions dims;

  if (!kernel || !kernel->is_active()) {
    dims.ontological = 0.0f;
    dims.teleological = 0.0f;
    dims.cognitive = 0.0f;
    dims.integrative = 0.0f;
    dims.evolutionary = 0.0f;
    dims.total = 0.0f;
    dims.stage = "Embryonic";
    return dims;
  }

  auto metrics = kernel->get_metrics();

  // Ontological: Structural being - based on atom count and links
  float atom_factor = std::min(1.0f, metrics.total_atoms / 1000.0f);
  float link_factor = std::min(1.0f, metrics.total_links / 500.0f);
  dims.ontological = (atom_factor * 0.5f + link_factor * 0.3f +
                      metrics.system_coherence * 0.2f);

  // Teleological: Purpose alignment - consciousness emergence
  dims.teleological =
      metrics.consciousness_emergence * 0.6f + metrics.prime_alignment * 0.4f;

  // Cognitive: Reasoning capability
  float reasoning_factor =
      metrics.active_chains > 0
          ? std::min(1.0f, metrics.converged_chains /
                               static_cast<float>(metrics.active_chains + 1))
          : 0.5f;
  float inference_factor = std::min(1.0f, metrics.total_inferences / 100.0f);
  dims.cognitive = reasoning_factor * 0.5f + inference_factor * 0.3f +
                   metrics.reasoning_throughput * 0.2f;

  // Integrative: Component coherence
  dims.integrative = metrics.system_coherence * 0.4f +
                     metrics.temporal_stability * 0.3f +
                     metrics.quantum_coherence * 0.3f;

  // Evolutionary: Growth potential
  dims.evolutionary =
      metrics.self_awareness_level * 0.5f + metrics.adaptation_rate * 0.5f;

  // Clamp all values
  dims.ontological = std::min(1.0f, std::max(0.0f, dims.ontological));
  dims.teleological = std::min(1.0f, std::max(0.0f, dims.teleological));
  dims.cognitive = std::min(1.0f, std::max(0.0f, dims.cognitive));
  dims.integrative = std::min(1.0f, std::max(0.0f, dims.integrative));
  dims.evolutionary = std::min(1.0f, std::max(0.0f, dims.evolutionary));

  // Calculate total fitness
  dims.total = calculate_fitness(dims);

  // Determine stage
  dims.stage = stage_to_string(get_stage(dims.total));

  return dims;
}

float EntelechyAssessor::calculate_fitness(const EntelechyDimensions &dims) {
  return dims.ontological * ONTOLOGICAL_WEIGHT +
         dims.teleological * TELEOLOGICAL_WEIGHT +
         dims.cognitive * COGNITIVE_WEIGHT +
         dims.integrative * INTEGRATIVE_WEIGHT +
         dims.evolutionary * EVOLUTIONARY_WEIGHT;
}

DevelopmentStage EntelechyAssessor::get_stage(float fitness) {
  if (fitness >= 0.8f) {
    return DevelopmentStage::Transcendent;
  } else if (fitness >= 0.6f) {
    return DevelopmentStage::Mature;
  } else if (fitness >= 0.3f) {
    return DevelopmentStage::Juvenile;
  } else {
    return DevelopmentStage::Embryonic;
  }
}

std::string EntelechyAssessor::stage_to_string(DevelopmentStage stage) {
  switch (stage) {
  case DevelopmentStage::Embryonic:
    return "Embryonic";
  case DevelopmentStage::Juvenile:
    return "Juvenile";
  case DevelopmentStage::Mature:
    return "Mature";
  case DevelopmentStage::Transcendent:
    return "Transcendent";
  default:
    return "Unknown";
  }
}
