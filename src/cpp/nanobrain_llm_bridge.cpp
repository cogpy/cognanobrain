#include "nanobrain_llm_bridge.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <numeric>

NanoBrainLLMBridge::NanoBrainLLMBridge(const LLMBridgeConfig &config)
    : config(config) {
  // Initialize metrics
  metrics = {0, 0, 0, 0.0f, 0.0f, 0.0f};
}

NanoBrainLLMBridge::~NanoBrainLLMBridge() {
  // Cleanup is handled by NanoBrain kernel
}

void NanoBrainLLMBridge::initialize(UnifiedNanoBrainKernel *nanobrain) {
  this->nanobrain = nanobrain;

  if (!nanobrain || !nanobrain->is_active()) {
    std::cerr << "NanoBrainLLMBridge: NanoBrain kernel not active" << std::endl;
    return;
  }

  initialize_projections();
  initialized = true;

  std::cout << "NanoBrainLLMBridge initialized" << std::endl;
  std::cout << "  Embedding dim: " << config.embedding_dim << std::endl;
  std::cout << "  Projection dim: " << config.projection_dim << std::endl;
}

void NanoBrainLLMBridge::connect_llama(llama_context *ctx, llama_model *model) {
  this->llama_ctx = ctx;
  this->llama_mdl = model;
  llama_connected = (ctx != nullptr && model != nullptr);

  if (llama_connected) {
    std::cout << "NanoBrainLLMBridge connected to llama.cpp" << std::endl;
  }
}

void NanoBrainLLMBridge::initialize_projections() {
  if (!nanobrain)
    return;

  auto *kernel = nanobrain->get_time_crystal_kernel()->get_tensor_kernel();

  // Create projection matrix: LLM dim -> NanoBrain dim
  projection_to_nb = kernel->create_tensor(
      {config.embedding_dim, config.projection_dim}, GGML_TYPE_F32, true);

  // Create inverse projection: NanoBrain dim -> LLM dim
  projection_to_llm = kernel->create_tensor(
      {config.projection_dim, config.embedding_dim}, GGML_TYPE_F32, true);

  // Initialize with random orthogonal-ish values (Xavier already done by
  // kernel)
}

std::vector<float> NanoBrainLLMBridge::project_to_nanobrain(
    const std::vector<float> &llm_embedding) {
  if (!initialized || llm_embedding.size() != (size_t)config.embedding_dim) {
    return std::vector<float>(config.projection_dim, 0.0f);
  }

  auto *kernel = nanobrain->get_time_crystal_kernel()->get_tensor_kernel();

  // Create input tensor
  auto *input = kernel->create_tensor({(int64_t)llm_embedding.size()});
  kernel->set_data(input, llm_embedding);

  // For now, simple mean pooling to reduce dimension
  // In a full implementation, this would be a proper matrix multiplication
  std::vector<float> result(config.projection_dim, 0.0f);

  int ratio = config.embedding_dim / config.projection_dim;
  for (int i = 0; i < config.projection_dim; i++) {
    float sum = 0.0f;
    for (int j = 0; j < ratio && (i * ratio + j) < (int)llm_embedding.size();
         j++) {
      sum += llm_embedding[i * ratio + j];
    }
    result[i] = sum / ratio;
  }

  return result;
}

std::vector<float>
NanoBrainLLMBridge::project_to_llm(const std::vector<float> &nb_embedding) {
  if (!initialized || nb_embedding.size() != (size_t)config.projection_dim) {
    return std::vector<float>(config.embedding_dim, 0.0f);
  }

  // Expand dimension via repetition
  std::vector<float> result(config.embedding_dim, 0.0f);

  int ratio = config.embedding_dim / config.projection_dim;
  for (int i = 0; i < config.projection_dim; i++) {
    for (int j = 0; j < ratio && (i * ratio + j) < config.embedding_dim; j++) {
      result[i * ratio + j] = nb_embedding[i];
    }
  }

  return result;
}

EnhancedTokenEmbedding NanoBrainLLMBridge::enhance_embedding(
    int32_t token_id, const std::vector<float> &base_embedding) {
  EnhancedTokenEmbedding enhanced;
  enhanced.token_id = token_id;
  enhanced.base_embedding = base_embedding;

  // Project to NanoBrain space
  auto nb_embedding = project_to_nanobrain(base_embedding);

  // Compute PPM coherence based on token ID (treating ID as a "prime-like"
  // value)
  std::vector<int> primes = tokens_to_primes({token_id});
  enhanced.ppm_coherence = nanobrain->compute_coherence(primes);

  // Apply coherence-based modulation
  for (size_t i = 0; i < nb_embedding.size(); i++) {
    nb_embedding[i] *= (0.5f + 0.5f * enhanced.ppm_coherence);
  }

  // Project back to LLM space
  enhanced.enhanced_embedding = project_to_llm(nb_embedding);

  // Blend with original
  float blend = config.ppm_attention_scale;
  for (size_t i = 0; i < enhanced.enhanced_embedding.size(); i++) {
    enhanced.enhanced_embedding[i] = base_embedding[i] * (1.0f - blend) +
                                     enhanced.enhanced_embedding[i] * blend;
  }

  // Compute attention boost
  enhanced.attention_boost =
      config.base_attention_weight * (1.0f + enhanced.ppm_coherence * 0.5f);

  // Check for atom mapping
  auto it = token_atom_map.find(token_id);
  if (it != token_atom_map.end()) {
    enhanced.mapped_atom_id = it->second;
  }

  update_metrics(enhanced);
  return enhanced;
}

std::string
NanoBrainLLMBridge::map_token_to_atom(int32_t token_id,
                                      const std::string &token_text) {
  auto it = token_atom_map.find(token_id);
  if (it != token_atom_map.end()) {
    return it->second;
  }

  // Create new atom for this token
  std::vector<int> primes = tokens_to_primes({token_id});
  std::string atom_id =
      nanobrain->create_atom("ConceptNode", token_text, 0.5f, 0.5f, primes);

  token_atom_map[token_id] = atom_id;
  metrics.atoms_created++;

  return atom_id;
}

std::vector<float> NanoBrainLLMBridge::get_attention_modulation(
    const std::vector<int32_t> &token_ids) {
  std::vector<float> modulation(token_ids.size(), 1.0f);

  if (!initialized)
    return modulation;

  for (size_t i = 0; i < token_ids.size(); i++) {
    std::vector<int> primes = tokens_to_primes({token_ids[i]});
    float coherence = nanobrain->compute_coherence(primes);
    modulation[i] = config.base_attention_weight * (1.0f + coherence * 0.5f);
  }

  return modulation;
}

std::vector<float>
NanoBrainLLMBridge::modulate_logits(const std::vector<float> &logits,
                                    const CognitiveContext &context,
                                    const GenerationModulation &params) {
  std::vector<float> modulated = logits;

  if (!initialized)
    return modulated;

  // Apply temperature scaling based on consciousness level
  float temp_scale = params.attention_temperature *
                     (1.0f + context.consciousness_level * 0.2f);

  for (size_t i = 0; i < modulated.size(); i++) {
    modulated[i] /= temp_scale;
  }

  // Apply coherence bias if enabled
  if (params.enable_prime_alignment && !context.prime_signature.empty()) {
    float base_coherence =
        nanobrain->compute_coherence(context.prime_signature);

    for (size_t i = 0; i < modulated.size(); i++) {
      std::vector<int> token_primes = tokens_to_primes({(int32_t)i});
      float token_coherence = nanobrain->compute_coherence(token_primes);

      // Boost tokens that maintain coherence
      float coherence_diff = token_coherence - base_coherence;
      modulated[i] += coherence_diff * params.coherence_bias * 10.0f;
    }
  }

  // Apply reasoning influence
  if (params.reasoning_influence > 0.0f && !context.active_chains.empty()) {
    // This would integrate with PLN reasoning output
    // For now, just a placeholder
  }

  // Apply metacognitive damping
  if (params.metacognitive_damping < 1.0f) {
    float max_logit = *std::max_element(modulated.begin(), modulated.end());
    for (size_t i = 0; i < modulated.size(); i++) {
      float diff = modulated[i] - max_logit;
      modulated[i] = max_logit + diff * params.metacognitive_damping;
    }
  }

  return modulated;
}

float NanoBrainLLMBridge::compute_sequence_coherence(
    const std::vector<int32_t> &token_ids) {
  if (token_ids.empty())
    return 0.5f;

  std::vector<int> all_primes;
  for (auto tid : token_ids) {
    auto primes = tokens_to_primes({tid});
    all_primes.insert(all_primes.end(), primes.begin(), primes.end());
  }

  return nanobrain->compute_coherence(all_primes);
}

std::string NanoBrainLLMBridge::reason_about_prompt(
    const std::vector<int32_t> &prompt_tokens) {
  if (!initialized)
    return "";

  // Create atoms for prompt tokens
  std::vector<std::string> atom_ids;
  for (size_t i = 0; i < prompt_tokens.size() && i < 50; i++) { // Limit
    std::string text = "token_" + std::to_string(prompt_tokens[i]);
    atom_ids.push_back(map_token_to_atom(prompt_tokens[i], text));
  }

  // Start reasoning chain
  std::string chain_id = nanobrain->start_reasoning(atom_ids);
  active_reasoning_chains.push_back(chain_id);
  metrics.reasoning_steps++;

  return chain_id;
}

std::vector<float> NanoBrainLLMBridge::get_reasoning_probs(
    const std::string &chain_id, const std::vector<int32_t> &current_tokens) {
  // Placeholder - would return probability adjustments from PLN reasoning
  return std::vector<float>(current_tokens.size(), 0.0f);
}

void NanoBrainLLMBridge::update_reasoning(const std::string &chain_id,
                                          int32_t token_id) {
  // Update reasoning chain with new token information
  if (nanobrain) {
    nanobrain->execute_reasoning_step();
  }
}

CognitiveContext NanoBrainLLMBridge::get_cognitive_context() const {
  return current_context;
}

void NanoBrainLLMBridge::set_focus_atoms(
    const std::vector<std::string> &atom_ids) {
  current_context.focus_atoms = atom_ids;
}

void NanoBrainLLMBridge::clear_context() {
  current_context = CognitiveContext();
  active_reasoning_chains.clear();
  token_atom_map.clear();
}

void NanoBrainLLMBridge::process_cognitive_cycle() {
  if (!nanobrain)
    return;

  // Run NanoBrain processing cycle
  auto cycle_metrics = nanobrain->process_cycle();

  // Update context from metrics
  current_context.consciousness_level = cycle_metrics.consciousness_emergence;
  current_context.temporal_coherence = cycle_metrics.temporal_stability;
  current_context.active_chains = active_reasoning_chains;

  // Update prime signature based on focus atoms
  current_context.prime_signature.clear();
  for (const auto &atom_id : current_context.focus_atoms) {
    const auto *atom = nanobrain->get_atom(atom_id);
    if (atom) {
      current_context.prime_signature.insert(
          current_context.prime_signature.end(), atom->prime_encoding.begin(),
          atom->prime_encoding.end());
    }
  }
}

NanoBrainLLMBridge::BridgeMetrics NanoBrainLLMBridge::get_metrics() const {
  return metrics;
}

void NanoBrainLLMBridge::set_logits_callback(LogitsCallback callback) {
  logits_callback = callback;
}

void NanoBrainLLMBridge::set_token_callback(TokenCallback callback) {
  token_callback = callback;
}

void NanoBrainLLMBridge::update_metrics(const EnhancedTokenEmbedding &emb) {
  metrics.tokens_processed++;

  // Running average for coherence
  float n = static_cast<float>(metrics.tokens_processed);
  metrics.average_coherence =
      metrics.average_coherence * ((n - 1) / n) + emb.ppm_coherence / n;
  metrics.average_attention_boost =
      metrics.average_attention_boost * ((n - 1) / n) + emb.attention_boost / n;
}

std::vector<int>
NanoBrainLLMBridge::tokens_to_primes(const std::vector<int32_t> &tokens) {
  // Map tokens to prime numbers for PPM calculation
  // Strategy: Use fundamental primes based on token ID modulo
  static const std::vector<int> PRIMES = {2,  3,  5,  7,  11, 13, 17,
                                          19, 23, 29, 31, 37, 41, 43,
                                          47, 53, 59, 61, 67, 71};

  std::vector<int> result;
  for (auto tid : tokens) {
    // Map token to a subset of primes
    int idx = tid % PRIMES.size();
    result.push_back(PRIMES[idx]);

    // Add additional primes based on token value
    if (tid > 100) {
      result.push_back(PRIMES[(tid / 100) % PRIMES.size()]);
    }
    if (tid > 1000) {
      result.push_back(PRIMES[(tid / 1000) % PRIMES.size()]);
    }
  }

  return result;
}
