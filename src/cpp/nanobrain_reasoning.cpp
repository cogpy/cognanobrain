#include "nanobrain_reasoning.h"
#include <algorithm>
#include <chrono>
#include <cmath>
#include <iostream>
#include <numeric>
#include <sstream>

// ================================================================
// Constructor / Destructor
// ================================================================

RecursiveReasoningEngine::RecursiveReasoningEngine(
    NanoBrainKernel *kernel, const ReasoningEngineConfig &config)
    : kernel(kernel), config(config), chain_counter(0), step_counter(0),
      start_time(0), total_inferences(0) {}

RecursiveReasoningEngine::~RecursiveReasoningEngine() { clear_all_chains(); }

int64_t RecursiveReasoningEngine::current_time_millis() const {
  return std::chrono::duration_cast<std::chrono::milliseconds>(
             std::chrono::system_clock::now().time_since_epoch())
      .count();
}

std::string RecursiveReasoningEngine::generate_chain_id() {
  std::stringstream ss;
  ss << "chain_" << chain_counter++;
  return ss.str();
}

std::string RecursiveReasoningEngine::generate_step_id() {
  std::stringstream ss;
  ss << "step_" << step_counter++;
  return ss.str();
}

// ================================================================
// Initialization
// ================================================================

void RecursiveReasoningEngine::initialize() {
  start_time = current_time_millis();
  add_default_rules();
  std::cout << "[ReasoningEngine] Initialized with " << inference_rules.size()
            << " rules" << std::endl;
}

void RecursiveReasoningEngine::add_default_rules() {
  // Deduction rule: If A->B and B->C then A->C
  InferenceRuleConfig deduction;
  deduction.id = "deduction";
  deduction.name = "Deduction";
  deduction.type = InferenceRuleTypePLN::Deduction;
  deduction.premise_patterns = {"InheritanceLink(A,B)", "InheritanceLink(B,C)"};
  deduction.conclusion_pattern = "InheritanceLink(A,C)";
  deduction.confidence_formula = "s_ab * s_bc * (c_ab * c_bc)";
  deduction.tensor_operation = TensorOperationType::Contraction;
  deduction.weight_tensor_shape = {128, 128};
  deduction.priority_weight = 1.0f;
  inference_rules[deduction.id] = deduction;

  // Induction rule
  InferenceRuleConfig induction;
  induction.id = "induction";
  induction.name = "Induction";
  induction.type = InferenceRuleTypePLN::Induction;
  induction.premise_patterns = {"InheritanceLink(A,B)", "InheritanceLink(A,C)"};
  induction.conclusion_pattern = "InheritanceLink(B,C)";
  induction.confidence_formula = "s_ab * s_ac * sqrt(c_ab * c_ac)";
  induction.tensor_operation = TensorOperationType::Composition;
  induction.weight_tensor_shape = {128, 64};
  induction.priority_weight = 0.8f;
  inference_rules[induction.id] = induction;

  // Abduction rule
  InferenceRuleConfig abduction;
  abduction.id = "abduction";
  abduction.name = "Abduction";
  abduction.type = InferenceRuleTypePLN::Abduction;
  abduction.premise_patterns = {"InheritanceLink(A,B)", "InheritanceLink(C,B)"};
  abduction.conclusion_pattern = "SimilarityLink(A,C)";
  abduction.confidence_formula = "s_ab * s_cb * sqrt(c_ab * c_cb) * 0.5";
  abduction.tensor_operation = TensorOperationType::Composition;
  abduction.weight_tensor_shape = {128, 64};
  abduction.priority_weight = 0.6f;
  inference_rules[abduction.id] = abduction;

  // Revision rule
  InferenceRuleConfig revision;
  revision.id = "revision";
  revision.name = "Revision";
  revision.type = InferenceRuleTypePLN::Revision;
  revision.premise_patterns = {"TruthValue(A,s1,c1)", "TruthValue(A,s2,c2)"};
  revision.conclusion_pattern = "TruthValue(A,s_new,c_new)";
  revision.confidence_formula = "weighted_average";
  revision.tensor_operation = TensorOperationType::Transformation;
  revision.weight_tensor_shape = {6, 3}; // Truth value vectors
  revision.priority_weight = 0.9f;
  inference_rules[revision.id] = revision;

  // Similarity rule (symmetric)
  InferenceRuleConfig similarity;
  similarity.id = "similarity";
  similarity.name = "Similarity";
  similarity.type = InferenceRuleTypePLN::Deduction;
  similarity.premise_patterns = {"SimilarityLink(A,B)"};
  similarity.conclusion_pattern = "SimilarityLink(B,A)";
  similarity.confidence_formula = "s_ab * c_ab";
  similarity.tensor_operation = TensorOperationType::Transformation;
  similarity.weight_tensor_shape = {128, 128};
  similarity.priority_weight = 0.5f;
  inference_rules[similarity.id] = similarity;
}

void RecursiveReasoningEngine::add_inference_rule(
    const InferenceRuleConfig &rule) {
  inference_rules[rule.id] = rule;
}

bool RecursiveReasoningEngine::remove_inference_rule(
    const std::string &rule_id) {
  return inference_rules.erase(rule_id) > 0;
}

std::vector<std::string> RecursiveReasoningEngine::get_rule_ids() const {
  std::vector<std::string> ids;
  ids.reserve(inference_rules.size());
  for (const auto &[id, _] : inference_rules) {
    ids.push_back(id);
  }
  return ids;
}

// ================================================================
// Reasoning Chain Management
// ================================================================

std::string RecursiveReasoningEngine::start_reasoning_chain(
    const std::vector<std::string> &input_node_ids) {
  std::string chain_id = generate_chain_id();

  ReasoningChain chain;
  chain.id = chain_id;
  chain.input_node_ids = input_node_ids;
  chain.current_depth = 0;
  chain.confidence_score = 1.0f;
  chain.convergence_status = ConvergenceStatus::Active;

  chains[chain_id] = chain;
  return chain_id;
}

const ReasoningChain *
RecursiveReasoningEngine::get_chain(const std::string &chain_id) const {
  auto it = chains.find(chain_id);
  if (it != chains.end()) {
    return &it->second;
  }
  return nullptr;
}

ReasoningChain *
RecursiveReasoningEngine::get_mutable_chain(const std::string &chain_id) {
  auto it = chains.find(chain_id);
  if (it != chains.end()) {
    return &it->second;
  }
  return nullptr;
}

std::vector<std::string>
RecursiveReasoningEngine::get_active_chain_ids() const {
  std::vector<std::string> active_ids;
  for (const auto &[id, chain] : chains) {
    if (chain.convergence_status == ConvergenceStatus::Active) {
      active_ids.push_back(id);
    }
  }
  return active_ids;
}

void RecursiveReasoningEngine::terminate_chain(const std::string &chain_id) {
  auto *chain = get_mutable_chain(chain_id);
  if (chain) {
    chain->convergence_status = ConvergenceStatus::Terminated;
  }
}

void RecursiveReasoningEngine::clear_all_chains() {
  chains.clear();
  chain_counter = 0;
  step_counter = 0;
}

// ================================================================
// Reasoning Execution
// ================================================================

ReasoningStats RecursiveReasoningEngine::execute_reasoning_step(
    const std::vector<NodeTensor *> &node_tensors,
    const std::vector<LinkTensor *> &link_tensors) {

  ReasoningStats stats;
  stats.total_chains = chains.size();
  stats.active_chains = 0;
  stats.converged_chains = 0;
  stats.total_inferences = 0;
  stats.average_depth = 0.0f;
  stats.average_confidence = 0.0f;
  stats.tensor_memory_usage = 0;

  float total_depth = 0.0f;
  float total_confidence = 0.0f;

  for (auto &[chain_id, chain] : chains) {
    if (chain.convergence_status == ConvergenceStatus::Active) {
      stats.active_chains++;
      process_chain(chain, node_tensors, link_tensors);
    }

    if (chain.convergence_status == ConvergenceStatus::Converged) {
      stats.converged_chains++;
    }

    total_depth += chain.current_depth;
    total_confidence += chain.confidence_score;
    stats.total_inferences += chain.reasoning_steps.size();

    // Accumulate rule usage
    for (const auto &[rule_id, count] : chain.rules_used_count) {
      stats.rule_usage[rule_id] += count;
    }
  }

  if (!chains.empty()) {
    stats.average_depth = total_depth / chains.size();
    stats.average_confidence = total_confidence / chains.size();
  }

  int64_t elapsed_ms = current_time_millis() - start_time;
  stats.reasoning_throughput =
      elapsed_ms > 0 ? (stats.total_inferences * 1000.0f / elapsed_ms) : 0.0f;

  total_inferences += stats.total_inferences;

  return stats;
}

void RecursiveReasoningEngine::process_chain(
    ReasoningChain &chain, const std::vector<NodeTensor *> &node_tensors,
    const std::vector<LinkTensor *> &link_tensors) {

  // Check depth limit
  if (chain.current_depth >= config.max_reasoning_depth) {
    chain.convergence_status = ConvergenceStatus::Terminated;
    return;
  }

  // Check chain length limit
  if (static_cast<int>(chain.reasoning_steps.size()) >=
      config.chain_length_limit) {
    chain.convergence_status = ConvergenceStatus::Terminated;
    return;
  }

  // Find applicable rules
  auto applicable_rules =
      find_applicable_rules(chain, node_tensors, link_tensors);

  if (applicable_rules.empty()) {
    // No more rules can be applied - check if converged or diverged
    if (chain.confidence_score >= config.confidence_threshold) {
      chain.convergence_status = ConvergenceStatus::Converged;
    } else {
      chain.convergence_status = ConvergenceStatus::Diverged;
    }
    return;
  }

  // Select best rule
  auto *selected_rule = select_best_rule(applicable_rules, chain);
  if (!selected_rule) {
    chain.convergence_status = ConvergenceStatus::Diverged;
    return;
  }

  // Apply the rule
  ReasoningStep step =
      apply_inference_rule(*selected_rule, chain, node_tensors, link_tensors);

  chain.reasoning_steps.push_back(step);
  chain.current_depth++;
  chain.confidence_score += step.confidence_change;
  chain.rules_used_count[selected_rule->id]++;

  // Check for convergence based on confidence change
  if (std::abs(step.confidence_change) < config.min_confidence_gain) {
    chain.convergence_status = ConvergenceStatus::Converged;
  }
}

std::vector<InferenceRuleConfig *>
RecursiveReasoningEngine::find_applicable_rules(
    const ReasoningChain &chain, const std::vector<NodeTensor *> &nodes,
    const std::vector<LinkTensor *> &links) {

  std::vector<InferenceRuleConfig *> applicable;

  for (auto &[rule_id, rule] : inference_rules) {
    bool all_patterns_match = true;

    for (const auto &pattern : rule.premise_patterns) {
      if (!check_pattern_match(pattern, chain, nodes)) {
        all_patterns_match = false;
        break;
      }
    }

    if (all_patterns_match) {
      applicable.push_back(&rule);
    }
  }

  return applicable;
}

bool RecursiveReasoningEngine::check_pattern_match(
    const std::string &pattern, const ReasoningChain &chain,
    const std::vector<NodeTensor *> &nodes) {
  // Simplified pattern matching
  // In a full implementation, this would parse the pattern and check
  // against the available nodes and existing conclusions

  // For now, we check if we have enough nodes to potentially match
  if (nodes.empty()) {
    return false;
  }

  // Check pattern type
  if (pattern.find("InheritanceLink") != std::string::npos) {
    // Need at least 2 nodes for inheritance
    return nodes.size() >= 2 || chain.input_node_ids.size() >= 2;
  } else if (pattern.find("SimilarityLink") != std::string::npos) {
    return nodes.size() >= 2 || chain.input_node_ids.size() >= 2;
  } else if (pattern.find("TruthValue") != std::string::npos) {
    // Truth value patterns match any node
    return !nodes.empty() || !chain.input_node_ids.empty();
  }

  return true; // Default to matching if pattern type unknown
}

InferenceRuleConfig *RecursiveReasoningEngine::select_best_rule(
    const std::vector<InferenceRuleConfig *> &candidates,
    const ReasoningChain &chain) {

  if (candidates.empty())
    return nullptr;

  InferenceRuleConfig *best = nullptr;
  float best_score = -1.0f;

  for (auto *rule : candidates) {
    // Calculate score based on priority and usage frequency
    float usage_penalty = 0.0f;
    auto it = chain.rules_used_count.find(rule->id);
    if (it != chain.rules_used_count.end()) {
      usage_penalty = it->second * 0.1f; // Penalize overuse
    }

    float score = rule->priority_weight - usage_penalty;

    // Prefer rules appropriate for current depth
    if (rule->type == InferenceRuleTypePLN::Deduction &&
        chain.current_depth < 2) {
      score += 0.2f; // Prefer deduction early
    }
    if (rule->type == InferenceRuleTypePLN::Revision &&
        chain.current_depth >= 2) {
      score += 0.2f; // Prefer revision later
    }

    if (score > best_score) {
      best_score = score;
      best = rule;
    }
  }

  return best;
}

ReasoningStep RecursiveReasoningEngine::apply_inference_rule(
    const InferenceRuleConfig &rule, ReasoningChain &chain,
    const std::vector<NodeTensor *> &nodes,
    const std::vector<LinkTensor *> &links) {

  auto step_start = current_time_millis();

  ReasoningStep step;
  step.id = generate_step_id();
  step.step_number = static_cast<int>(chain.reasoning_steps.size()) + 1;
  step.rule_applied = rule.id;
  step.attention_consumed = config.attention_budget_per_step;

  // Collect input tensors
  for (size_t i = 0; i < std::min(nodes.size(), size_t(2)); i++) {
    if (nodes[i] && nodes[i]->embedding) {
      step.input_tensors.push_back(nodes[i]->embedding);
    }
  }

  // Perform tensor operation based on rule type
  NanoBrainTensor *result = nullptr;

  switch (rule.tensor_operation) {
  case TensorOperationType::Contraction:
    if (step.input_tensors.size() >= 2) {
      result = perform_contraction(step.input_tensors[0], step.input_tensors[1],
                                   config.tensor_contraction_method);
      step.tensor_operations.push_back("contraction");
    }
    break;

  case TensorOperationType::Composition:
    result = compose_tensors(step.input_tensors);
    step.tensor_operations.push_back("composition");
    break;

  case TensorOperationType::Transformation: {
    if (!step.input_tensors.empty()) {
      // Create weight tensor
      std::vector<int64_t> weight_shape(rule.weight_tensor_shape.begin(),
                                        rule.weight_tensor_shape.end());
      auto *weights = kernel->create_tensor(weight_shape);
      result = apply_transformation(step.input_tensors[0], weights);
      step.tensor_operations.push_back("transformation");
    }
  } break;
  }

  step.output_tensor = result;
  if (result) {
    chain.output_tensors.push_back(result);
  }

  // Calculate confidence change based on rule type
  float conf_change = 0.0f;
  switch (rule.type) {
  case InferenceRuleTypePLN::Deduction:
    conf_change = 0.1f * (1.0f - chain.current_depth * 0.1f);
    break;
  case InferenceRuleTypePLN::Induction:
    conf_change = 0.05f;
    break;
  case InferenceRuleTypePLN::Abduction:
    conf_change = 0.03f;
    break;
  case InferenceRuleTypePLN::Revision:
    conf_change = 0.08f;
    break;
  case InferenceRuleTypePLN::Choice:
    conf_change = 0.02f;
    break;
  }

  step.confidence_change = conf_change;
  step.reasoning_time_ms =
      static_cast<float>(current_time_millis() - step_start);
  step.memory_usage = step.input_tensors.size() * 128 * sizeof(float);

  return step;
}

// ================================================================
// Tensor Operations for Reasoning
// ================================================================

NanoBrainTensor *RecursiveReasoningEngine::perform_contraction(
    NanoBrainTensor *a, NanoBrainTensor *b, const std::string &method) {
  if (!a || !b)
    return nullptr;

  if (method == "dot") {
    // Use matrix multiplication for dot-product-like contraction
    return kernel->contract(a, b);
  } else if (method == "einstein") {
    // Einstein summation - for now, same as dot
    return kernel->contract(a, b);
  } else if (method == "multilinear") {
    // Multilinear contraction - element-wise then sum
    auto *product = kernel->mul(a, b);
    return kernel->sum(product);
  }

  return kernel->contract(a, b);
}

NanoBrainTensor *RecursiveReasoningEngine::compose_tensors(
    const std::vector<NanoBrainTensor *> &tensors) {
  if (tensors.empty())
    return nullptr;
  if (tensors.size() == 1)
    return tensors[0];

  // Sequential composition: add all tensors element-wise
  NanoBrainTensor *result = tensors[0];
  for (size_t i = 1; i < tensors.size(); i++) {
    if (tensors[i]) {
      result = kernel->add(result, tensors[i]);
    }
  }

  // Normalize
  auto *count = kernel->create_tensor({1});
  std::vector<float> count_data = {static_cast<float>(tensors.size())};
  kernel->set_data(count, count_data);
  result = kernel->div(result, count);

  return result;
}

NanoBrainTensor *
RecursiveReasoningEngine::apply_transformation(NanoBrainTensor *input,
                                               NanoBrainTensor *weights) {
  if (!input || !weights)
    return nullptr;

  // Matrix multiplication for transformation
  return kernel->matmul(input, weights);
}

// ================================================================
// PLN Truth Value Calculations
// ================================================================

float RecursiveReasoningEngine::calculate_deductive_confidence(
    float strength_ab, float conf_ab, float strength_bc, float conf_bc) {
  // Deduction: P(C|A) ≈ P(B|A) * P(C|B)
  float new_strength = strength_ab * strength_bc;
  float new_confidence = conf_ab * conf_bc * 0.9f; // Slight discount
  return new_strength * new_confidence;
}

float RecursiveReasoningEngine::calculate_inductive_confidence(
    float strength_ab, float conf_ab, float strength_ac, float conf_ac) {
  // Induction: weaker form of reasoning
  float new_strength = (strength_ab + strength_ac) / 2.0f;
  float new_confidence = std::sqrt(conf_ab * conf_ac) * 0.7f;
  return new_strength * new_confidence;
}

float RecursiveReasoningEngine::calculate_abductive_confidence(
    float strength_ab, float conf_ab, float strength_cb, float conf_cb) {
  // Abduction: weakest form of reasoning
  float new_strength = (strength_ab + strength_cb) / 2.0f;
  float new_confidence = std::sqrt(conf_ab * conf_cb) * 0.5f;
  return new_strength * new_confidence;
}

void RecursiveReasoningEngine::revise_truth_values(float &strength,
                                                   float &confidence,
                                                   float new_strength,
                                                   float new_confidence) {
  // Weighted average based on confidence
  float total_conf = confidence + new_confidence;
  if (total_conf > 0) {
    strength =
        (strength * confidence + new_strength * new_confidence) / total_conf;
    confidence = std::min(1.0f, confidence + new_confidence * 0.5f);
  }
}

// ================================================================
// Meta-Reasoning
// ================================================================

float RecursiveReasoningEngine::evaluate_reasoning_quality(
    const ReasoningChain &chain) {
  if (chain.reasoning_steps.empty())
    return 0.0f;

  // Quality factors:
  // 1. Convergence status
  float convergence_score = 0.0f;
  switch (chain.convergence_status) {
  case ConvergenceStatus::Converged:
    convergence_score = 1.0f;
    break;
  case ConvergenceStatus::Active:
    convergence_score = 0.7f;
    break;
  case ConvergenceStatus::Terminated:
    convergence_score = 0.3f;
    break;
  case ConvergenceStatus::Diverged:
    convergence_score = 0.1f;
    break;
  }

  // 2. Confidence score
  float confidence_score = std::min(1.0f, chain.confidence_score);

  // 3. Diversity of rules used
  float diversity_score =
      static_cast<float>(chain.rules_used_count.size()) /
      std::max(1.0f, static_cast<float>(inference_rules.size()));

  // 4. Efficiency (fewer steps is better for same result)
  float efficiency_score =
      1.0f - std::min(1.0f, chain.reasoning_steps.size() /
                                static_cast<float>(config.chain_length_limit));

  return (convergence_score * 0.4f + confidence_score * 0.3f +
          diversity_score * 0.15f + efficiency_score * 0.15f);
}

std::vector<std::string> RecursiveReasoningEngine::suggest_rule_modifications(
    const ReasoningChain &chain) {
  std::vector<std::string> suggestions;

  // Check for overused rules
  for (const auto &[rule_id, count] : chain.rules_used_count) {
    if (count > 3) {
      suggestions.push_back("Consider reducing priority of rule: " + rule_id);
    }
  }

  // Check for underused rules
  for (const auto &[rule_id, _] : inference_rules) {
    if (chain.rules_used_count.find(rule_id) == chain.rules_used_count.end()) {
      suggestions.push_back("Rule never used, may need new patterns: " +
                            rule_id);
    }
  }

  // Check if chain diverged quickly
  if (chain.convergence_status == ConvergenceStatus::Diverged &&
      chain.current_depth < 2) {
    suggestions.push_back("Early divergence suggests need for more permissive "
                          "premise patterns");
  }

  return suggestions;
}

void RecursiveReasoningEngine::prune_chains(float quality_threshold) {
  std::vector<std::string> to_remove;

  for (const auto &[chain_id, chain] : chains) {
    float quality = evaluate_reasoning_quality(chain);
    if (quality < quality_threshold) {
      to_remove.push_back(chain_id);
    }
  }

  for (const auto &chain_id : to_remove) {
    chains.erase(chain_id);
  }
}

// ================================================================
// Statistics
// ================================================================

ReasoningStats RecursiveReasoningEngine::get_stats() const {
  ReasoningStats stats;
  stats.total_chains = chains.size();
  stats.active_chains = 0;
  stats.converged_chains = 0;
  stats.total_inferences = total_inferences;

  float total_depth = 0.0f;
  float total_confidence = 0.0f;

  for (const auto &[_, chain] : chains) {
    if (chain.convergence_status == ConvergenceStatus::Active) {
      stats.active_chains++;
    }
    if (chain.convergence_status == ConvergenceStatus::Converged) {
      stats.converged_chains++;
    }

    total_depth += chain.current_depth;
    total_confidence += chain.confidence_score;

    for (const auto &[rule_id, count] : chain.rules_used_count) {
      stats.rule_usage[rule_id] += count;
    }
  }

  if (!chains.empty()) {
    stats.average_depth = total_depth / chains.size();
    stats.average_confidence = total_confidence / chains.size();
  }

  int64_t elapsed_ms = current_time_millis() - start_time;
  stats.reasoning_throughput =
      elapsed_ms > 0 ? (total_inferences * 1000.0f / elapsed_ms) : 0.0f;

  stats.tensor_memory_usage = total_inferences * 128 * sizeof(float);

  return stats;
}

void RecursiveReasoningEngine::reset_stats() {
  start_time = current_time_millis();
  total_inferences = 0;
}

void RecursiveReasoningEngine::update_config(
    const ReasoningEngineConfig &new_config) {
  config = new_config;
}
