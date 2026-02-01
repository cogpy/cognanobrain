#include "nanobrain_unified.h"
#include <chrono>
#include <iostream>

// ================================================================
// Constructor / Destructor
// ================================================================

UnifiedNanoBrainKernel::UnifiedNanoBrainKernel(
    const UnifiedNanoBrainConfig &cfg)
    : config(cfg), active(false), cycle_count(0), start_time(0) {}

UnifiedNanoBrainKernel::~UnifiedNanoBrainKernel() { shutdown(); }

int64_t UnifiedNanoBrainKernel::current_time_millis() const {
  return std::chrono::duration_cast<std::chrono::milliseconds>(
             std::chrono::system_clock::now().time_since_epoch())
      .count();
}

// ================================================================
// Lifecycle
// ================================================================

void UnifiedNanoBrainKernel::initialize() {
  if (active)
    return;

  start_time = current_time_millis();

  std::cout << "========================================" << std::endl;
  std::cout << " NanoBrain C++ Unified Kernel v" << NANOBRAIN_CPP_VERSION
            << std::endl;
  std::cout << " llama.cpp/ggml Adaptation" << std::endl;
  std::cout << "========================================" << std::endl;

  // 1. Initialize Time Crystal Kernel (includes base NanoBrainKernel)
  TimeCrystalConfig tc_config;
  tc_config.memory_size = config.memory_size;
  tc_config.time_crystal_dimensions = config.time_crystal_dimensions;
  tc_config.fractal_resolution = config.fractal_resolution;
  tc_config.quantum_coherence_threshold = config.quantum_coherence_threshold;
  tc_config.resource_budget = config.resource_budget;

  time_crystal_kernel = std::make_unique<TimeCrystalKernel>(tc_config);
  time_crystal_kernel->initialize();

  // 2. Initialize Tensor Encoder
  encoder = std::make_unique<AtomSpaceTensorEncoder>(
      time_crystal_kernel->get_tensor_kernel());

  // 3. Initialize Reasoning Engine
  ReasoningEngineConfig re_config;
  re_config.max_reasoning_depth = config.max_reasoning_depth;
  re_config.confidence_threshold = config.confidence_threshold;
  re_config.parallel_chains = config.parallel_chains;

  reasoning_engine = std::make_unique<RecursiveReasoningEngine>(
      time_crystal_kernel->get_tensor_kernel(), re_config);
  reasoning_engine->initialize();

  // 4. Initialize Attention Allocation Engine
  AttentionAllocationConfig aa_config;
  aa_config.mechanism = config.attention_mechanism;
  aa_config.temperature = config.attention_temperature;
  aa_config.resource_budget = config.resource_budget;

  attention_engine = std::make_unique<AttentionAllocationEngine>(
      time_crystal_kernel->get_tensor_kernel(), aa_config);
  attention_engine->initialize(1000); // Max 1000 nodes

  // 5. Initialize Meta-Cognitive Feedback Engine
  MetaCognitiveConfig mc_config;
  mc_config.meta_levels = config.meta_levels;
  mc_config.adaptation_learning_rate = config.adaptation_learning_rate;
  mc_config.feedback_damping = config.feedback_damping;

  metacognitive_engine = std::make_unique<MetaCognitiveFeedbackEngine>(
      time_crystal_kernel->get_tensor_kernel(), attention_engine.get(),
      reasoning_engine.get(), mc_config);
  metacognitive_engine->initialize();

  active = true;

  std::cout << "[UnifiedKernel] All subsystems initialized" << std::endl;
  std::cout << "  - Time Crystal: " << config.time_crystal_dimensions
            << " dimensions" << std::endl;
  std::cout << "  - Reasoning: depth " << config.max_reasoning_depth << ", "
            << config.parallel_chains << " parallel chains" << std::endl;
  std::cout << "  - Attention: "
            << (config.attention_mechanism == AttentionMechanism::Hybrid
                    ? "Hybrid"
                : config.attention_mechanism == AttentionMechanism::ECAN
                    ? "ECAN"
                    : "Softmax")
            << " mechanism" << std::endl;
  std::cout << "  - Meta-cognitive: " << config.meta_levels << " levels"
            << std::endl;
}

void UnifiedNanoBrainKernel::shutdown() {
  if (!active)
    return;

  float uptime = (current_time_millis() - start_time) / 1000.0f;

  std::cout << "[UnifiedKernel] Shutting down after " << cycle_count
            << " cycles (" << uptime << " seconds)" << std::endl;

  metacognitive_engine.reset();
  attention_engine.reset();
  reasoning_engine.reset();
  encoder.reset();
  time_crystal_kernel.reset();

  // Clear tensor caches
  for (auto *node : node_tensors) {
    delete node;
  }
  node_tensors.clear();

  for (auto *link : link_tensors) {
    delete link;
  }
  link_tensors.clear();

  active = false;
}

// ================================================================
// Processing Cycle
// ================================================================

UnifiedNanoBrainMetrics UnifiedNanoBrainKernel::process_cycle() {
  if (!active) {
    return UnifiedNanoBrainMetrics{};
  }

  // 1. Sync tensors from AtomSpace
  sync_tensors();

  // 2. Time Crystal update
  time_crystal_kernel->process_cycle();

  // 3. Attention update
  AttentionStats att_stats =
      attention_engine->update_attention_allocation(node_tensors, link_tensors);

  // 4. Reasoning step
  ReasoningStats reas_stats =
      reasoning_engine->execute_reasoning_step(node_tensors, link_tensors);

  // 5. Meta-cognitive update
  MetaCognitiveMetrics meta_metrics =
      metacognitive_engine->update_meta_cognitive(node_tensors, link_tensors,
                                                  att_stats, reas_stats);

  cycle_count++;

  return get_metrics();
}

UnifiedNanoBrainMetrics UnifiedNanoBrainKernel::run_cycles(int n) {
  UnifiedNanoBrainMetrics metrics;
  for (int i = 0; i < n; i++) {
    metrics = process_cycle();
  }
  return metrics;
}

void UnifiedNanoBrainKernel::sync_tensors() {
  build_node_tensors();
  build_link_tensors();
}

void UnifiedNanoBrainKernel::build_node_tensors() {
  // Clear old tensors
  for (auto *node : node_tensors) {
    delete node;
  }
  node_tensors.clear();

  // Build new tensors from AtomSpace
  auto atom_ids = time_crystal_kernel->get_all_atom_ids();
  for (const auto &atom_id : atom_ids) {
    const auto *atom = time_crystal_kernel->get_atom(atom_id);
    if (!atom)
      continue;

    // Convert to Atom struct for encoder
    Atom a;
    a.id = atom->id;
    a.type = atom->type;
    a.name = atom->name;
    a.truth_value[0] = atom->truth_value.strength;
    a.truth_value[1] = atom->truth_value.confidence;
    a.truth_value[2] = atom->truth_value.count;
    a.attention_value[0] = atom->attention_value.sti;
    a.attention_value[1] = atom->attention_value.lti;
    a.attention_value[2] = atom->attention_value.vlti;

    NodeTensor *node_tensor = encoder->encode_atom(a);
    if (node_tensor) {
      node_tensors.push_back(node_tensor);
    }
  }
}

void UnifiedNanoBrainKernel::build_link_tensors() {
  // Clear old link tensors
  for (auto *link : link_tensors) {
    delete link;
  }
  link_tensors.clear();

  // Build link tensors from inference links
  auto inference_ids = time_crystal_kernel->get_all_inference_ids();
  for (const auto &inf_id : inference_ids) {
    const auto *inference = time_crystal_kernel->get_inference(inf_id);
    if (!inference)
      continue;

    LinkTensor *link_tensor = new LinkTensor();
    link_tensor->id = "link_" + inf_id;
    link_tensor->atom_id = inference->conclusion_id;
    link_tensor->source_nodes = inference->premise_ids;
    link_tensor->target_nodes = {inference->conclusion_id};
    link_tensor->relation_tensor =
        time_crystal_kernel->get_tensor_kernel()->create_tensor({32});
    link_tensor->attention_weights =
        time_crystal_kernel->get_tensor_kernel()->create_tensor({1});
    link_tensor->truth_value_tensor =
        time_crystal_kernel->get_tensor_kernel()->create_tensor({3});

    // Set truth value based on inference quality
    std::vector<float> tv_data = {inference->fractal_convergence,
                                  inference->quantum_coherence, 1.0f};
    time_crystal_kernel->get_tensor_kernel()->set_data(
        link_tensor->truth_value_tensor, tv_data);

    link_tensors.push_back(link_tensor);
  }
}

// ================================================================
// Atom Management
// ================================================================

std::string UnifiedNanoBrainKernel::create_atom(
    const std::string &type, const std::string &name, float strength,
    float confidence, const std::vector<int> &prime_encoding) {

  if (!active)
    return "";

  TruthValue tv{strength, confidence, 1.0f};
  AttentionValue av{100.0f, 50.0f, 25.0f};

  GeometricPattern geom;
  geom.shape = GMLShape::Sphere;
  geom.dimensions = config.time_crystal_dimensions;
  geom.symmetry_group =
      "SO(" + std::to_string(config.time_crystal_dimensions) + ")";
  geom.musical_note = MusicalNote::C;
  geom.prime_resonance = prime_encoding;
  geom.scale_factor = 1.0f;

  return time_crystal_kernel->create_atom(type, name, tv, av, prime_encoding,
                                          geom);
}

const TimeCrystalAtom *
UnifiedNanoBrainKernel::get_atom(const std::string &id) const {
  if (!time_crystal_kernel)
    return nullptr;
  return time_crystal_kernel->get_atom(id);
}

bool UnifiedNanoBrainKernel::remove_atom(const std::string &id) {
  if (!time_crystal_kernel)
    return false;
  return time_crystal_kernel->remove_atom(id);
}

std::vector<std::string> UnifiedNanoBrainKernel::get_all_atom_ids() const {
  if (!time_crystal_kernel)
    return {};
  return time_crystal_kernel->get_all_atom_ids();
}

// ================================================================
// Reasoning
// ================================================================

std::string UnifiedNanoBrainKernel::start_reasoning(
    const std::vector<std::string> &input_atom_ids) {
  if (!reasoning_engine)
    return "";
  return reasoning_engine->start_reasoning_chain(input_atom_ids);
}

ReasoningStats UnifiedNanoBrainKernel::execute_reasoning_step() {
  if (!reasoning_engine)
    return ReasoningStats{};
  return reasoning_engine->execute_reasoning_step(node_tensors, link_tensors);
}

const ReasoningChain *
UnifiedNanoBrainKernel::get_reasoning_chain(const std::string &chain_id) const {
  if (!reasoning_engine)
    return nullptr;
  return reasoning_engine->get_chain(chain_id);
}

std::vector<std::string> UnifiedNanoBrainKernel::get_active_chain_ids() const {
  if (!reasoning_engine)
    return {};
  return reasoning_engine->get_active_chain_ids();
}

// ================================================================
// Attention
// ================================================================

AttentionStats UnifiedNanoBrainKernel::update_attention() {
  if (!attention_engine)
    return AttentionStats{};
  return attention_engine->update_attention_allocation(node_tensors,
                                                       link_tensors);
}

std::vector<std::string>
UnifiedNanoBrainKernel::get_top_attention_atoms(size_t k) const {
  if (!time_crystal_kernel)
    return {};
  return time_crystal_kernel->get_top_attention_atoms(k);
}

// ================================================================
// Tensor Operations
// ================================================================

NanoBrainTensor *
UnifiedNanoBrainKernel::encode_atom_to_tensor(const std::string &atom_id) {
  if (!time_crystal_kernel)
    return nullptr;
  return time_crystal_kernel->encode_atom_to_tensor(atom_id);
}

float UnifiedNanoBrainKernel::compute_coherence(
    const std::vector<int> &primes) {
  if (!time_crystal_kernel)
    return 0.0f;
  return time_crystal_kernel->compute_ppm_coherence(primes);
}

// ================================================================
// Metrics
// ================================================================

UnifiedNanoBrainMetrics UnifiedNanoBrainKernel::get_metrics() const {
  UnifiedNanoBrainMetrics metrics;

  if (!active) {
    return metrics;
  }

  // System metrics
  auto tc_metrics = time_crystal_kernel->get_metrics();
  metrics.total_atoms = tc_metrics.total_atoms;
  metrics.total_links = tc_metrics.total_links;
  metrics.uptime_seconds = (current_time_millis() - start_time) / 1000.0f;

  // Time Crystal metrics
  metrics.quantum_coherence = tc_metrics.quantum_coherence;
  metrics.temporal_stability = tc_metrics.temporal_stability;
  metrics.prime_alignment = tc_metrics.prime_alignment;
  metrics.consciousness_emergence = tc_metrics.consciousness_emergence;

  // Reasoning metrics
  if (reasoning_engine) {
    auto reas_stats = reasoning_engine->get_stats();
    metrics.total_inferences = reas_stats.total_inferences;
    metrics.active_chains = reas_stats.active_chains;
    metrics.converged_chains = reas_stats.converged_chains;
    metrics.reasoning_throughput = reas_stats.reasoning_throughput;
  }

  // Attention metrics
  if (attention_engine) {
    auto att_stats = attention_engine->get_current_stats(node_tensors);
    metrics.total_attention = att_stats.total_attention;
    metrics.attention_entropy = att_stats.attention_entropy;
    metrics.resource_utilization = att_stats.resource_utilization;
  }

  // Meta-cognitive metrics
  if (metacognitive_engine) {
    auto meta_tensors = metacognitive_engine->get_meta_tensors();
    float total_coherence = 0.0f;
    for (const auto &mt : meta_tensors) {
      total_coherence += mt.coherence;
    }
    metrics.self_awareness_level =
        meta_tensors.empty() ? 0.0f : total_coherence / meta_tensors.size();

    auto loops = metacognitive_engine->get_feedback_loops();
    float total_stability = 0.0f;
    for (const auto &loop : loops) {
      total_stability += loop.stability_measure;
    }
    metrics.system_coherence =
        loops.empty() ? 0.0f : total_stability / loops.size();

    auto mods = metacognitive_engine->get_self_modifications();
    metrics.adaptation_rate =
        cycle_count > 0
            ? static_cast<float>(mods.size()) / static_cast<float>(cycle_count)
            : 0.0f;
  }

  return metrics;
}
