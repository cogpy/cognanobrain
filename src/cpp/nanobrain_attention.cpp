#include "nanobrain_attention.h"
#include <algorithm>
#include <chrono>
#include <cmath>
#include <iostream>
#include <numeric>

// ================================================================
// Constructor / Destructor
// ================================================================

AttentionAllocationEngine::AttentionAllocationEngine(
    NanoBrainKernel *kernel, const AttentionAllocationConfig &config)
    : kernel(kernel), config(config) {
  global_attention.initialized = false;
}

AttentionAllocationEngine::~AttentionAllocationEngine() { reset(); }

int64_t AttentionAllocationEngine::current_time_millis() const {
  return std::chrono::duration_cast<std::chrono::milliseconds>(
             std::chrono::system_clock::now().time_since_epoch())
      .count();
}

// ================================================================
// Initialization
// ================================================================

void AttentionAllocationEngine::initialize(size_t max_nodes) {
  initialize_global_attention(max_nodes);
  std::cout << "[AttentionEngine] Initialized with max " << max_nodes
            << " nodes" << std::endl;
}

void AttentionAllocationEngine::initialize_global_attention(size_t num_nodes) {
  if (num_nodes == 0)
    num_nodes = 1000; // Default

  // Create main attention distribution tensor
  global_attention.attention_distribution =
      kernel->create_tensor({static_cast<int64_t>(num_nodes)});

  // Create gradient tensor
  global_attention.attention_gradients =
      kernel->create_tensor({static_cast<int64_t>(num_nodes)});

  // Create temperature tensor (single value)
  global_attention.temperature_tensor = kernel->create_tensor({1});
  std::vector<float> temp_data = {config.temperature};
  kernel->set_data(global_attention.temperature_tensor, temp_data);

  // Initialize attention heads for multi-head attention
  int embedding_dim = 128;
  int head_dim = embedding_dim / config.attention_heads;

  for (int h = 0; h < config.attention_heads; h++) {
    AttentionHead head;
    head.head_id = h;
    head.head_scale = 1.0f / std::sqrt(static_cast<float>(head_dim));

    // Create projection tensors
    head.query_projection = kernel->create_tensor(
        {static_cast<int64_t>(embedding_dim), static_cast<int64_t>(head_dim)});
    head.key_projection = kernel->create_tensor(
        {static_cast<int64_t>(embedding_dim), static_cast<int64_t>(head_dim)});
    head.value_projection = kernel->create_tensor(
        {static_cast<int64_t>(embedding_dim), static_cast<int64_t>(head_dim)});

    // Attention weights will be computed dynamically
    head.attention_weights = nullptr;

    global_attention.attention_heads.push_back(head);
  }

  global_attention.initialized = true;
}

// ================================================================
// Attention Update Methods
// ================================================================

AttentionStats AttentionAllocationEngine::update_attention_allocation(
    const std::vector<NodeTensor *> &node_tensors,
    const std::vector<LinkTensor *> &link_tensors) {

  AttentionStats stats;

  switch (config.mechanism) {
  case AttentionMechanism::Softmax:
    stats = apply_softmax_attention(node_tensors, link_tensors);
    break;
  case AttentionMechanism::ECAN:
    stats = apply_ecan_attention(node_tensors, link_tensors);
    break;
  case AttentionMechanism::Hybrid:
  default:
    stats = apply_hybrid_attention(node_tensors, link_tensors);
    break;
  }

  // Record attention flows for monitoring
  record_attention_flows(node_tensors, link_tensors);

  return stats;
}

AttentionStats AttentionAllocationEngine::apply_softmax_attention(
    const std::vector<NodeTensor *> &node_tensors,
    const std::vector<LinkTensor *> &link_tensors) {

  if (node_tensors.empty()) {
    return calculate_attention_stats(node_tensors);
  }

  // Compute attention scores
  auto *attention_scores = compute_attention_scores(node_tensors);

  if (attention_scores) {
    // Apply softmax with temperature
    auto *softmax_attention = kernel->softmax(attention_scores);
    kernel->compute(softmax_attention);

    // Apply attention to update node tensors
    apply_attention_to_values(softmax_attention, node_tensors);
  }

  return calculate_attention_stats(node_tensors);
}

AttentionStats AttentionAllocationEngine::apply_ecan_attention(
    const std::vector<NodeTensor *> &node_tensors,
    const std::vector<LinkTensor *> &link_tensors) {

  std::vector<NodeTensor *> mutable_nodes(node_tensors.begin(),
                                          node_tensors.end());
  std::vector<LinkTensor *> mutable_links(link_tensors.begin(),
                                          link_tensors.end());

  // 1. Collect rent from low-utility atoms
  collect_rent(mutable_nodes);

  // 2. Apply attention diffusion
  apply_attention_diffusion(mutable_nodes, mutable_links);

  // 3. Calculate rent pool (sum of collected rent)
  float rent_pool = 0.0f;
  for (const auto *node : mutable_nodes) {
    if (node && node->attention_weights) {
      kernel->compute(node->attention_weights);
      float val = kernel->get_value(node->attention_weights, 0);
      rent_pool += config.rent_collection_rate * std::max(0.0f, val);
    }
  }

  // 4. Distribute wages to high-utility atoms
  distribute_wages(mutable_nodes, rent_pool);

  // 5. Update link attention
  update_link_attention(mutable_links, mutable_nodes);

  // 6. Enforce resource constraints
  enforce_resource_constraints(mutable_nodes);

  return calculate_attention_stats(node_tensors);
}

AttentionStats AttentionAllocationEngine::apply_hybrid_attention(
    const std::vector<NodeTensor *> &node_tensors,
    const std::vector<LinkTensor *> &link_tensors) {

  // First apply softmax attention for global distribution
  AttentionStats softmax_stats =
      apply_softmax_attention(node_tensors, link_tensors);

  // Then apply ECAN for economic dynamics
  AttentionStats ecan_stats = apply_ecan_attention(node_tensors, link_tensors);

  // Apply gradient updates if enabled
  std::vector<NodeTensor *> mutable_nodes(node_tensors.begin(),
                                          node_tensors.end());
  std::vector<LinkTensor *> mutable_links(link_tensors.begin(),
                                          link_tensors.end());
  apply_gradient_updates(mutable_nodes, mutable_links);

  // Combine stats
  AttentionStats hybrid_stats;
  hybrid_stats.total_attention =
      (softmax_stats.total_attention + ecan_stats.total_attention) / 2.0f;
  hybrid_stats.average_attention =
      (softmax_stats.average_attention + ecan_stats.average_attention) / 2.0f;
  hybrid_stats.attention_entropy =
      (softmax_stats.attention_entropy + ecan_stats.attention_entropy) / 2.0f;
  hybrid_stats.resource_utilization =
      (softmax_stats.resource_utilization + ecan_stats.resource_utilization) /
      2.0f;
  hybrid_stats.gradient_norm = softmax_stats.gradient_norm;
  hybrid_stats.convergence_rate = ecan_stats.convergence_rate;

  return hybrid_stats;
}

// ================================================================
// Tensor-Based Attention Operations
// ================================================================

NanoBrainTensor *AttentionAllocationEngine::compute_attention_scores(
    const std::vector<NodeTensor *> &nodes) {

  if (nodes.empty())
    return nullptr;

  // Create score tensor
  auto *scores = kernel->create_tensor({static_cast<int64_t>(nodes.size())});
  std::vector<float> score_data(nodes.size(), 0.0f);

  // Compute scores based on embeddings
  for (size_t i = 0; i < nodes.size(); i++) {
    if (nodes[i] && nodes[i]->embedding) {
      // Use embedding norm as base score
      auto *abs_embed = kernel->abs(nodes[i]->embedding);
      auto *sum_embed = kernel->sum(abs_embed);
      kernel->compute(sum_embed);
      score_data[i] = kernel->get_value(sum_embed, 0);
    }
  }

  // Apply temperature scaling
  for (float &score : score_data) {
    score /= config.temperature;
  }

  kernel->set_data(scores, score_data);
  return scores;
}

NanoBrainTensor *AttentionAllocationEngine::apply_attention_to_values(
    NanoBrainTensor *attention_weights,
    const std::vector<NodeTensor *> &nodes) {

  if (!attention_weights || nodes.empty())
    return nullptr;

  kernel->compute(attention_weights);

  // Update each node's attention based on the distribution
  for (size_t i = 0; i < nodes.size(); i++) {
    if (nodes[i] && nodes[i]->attention_weights) {
      float weight = kernel->get_value(attention_weights, static_cast<int>(i));

      // Get current attention and update
      if (nodes[i]->attention_weights) {
        kernel->compute(nodes[i]->attention_weights);
        // Create update tensor
        auto *update = kernel->create_tensor({1});
        std::vector<float> update_data = {weight * config.resource_budget /
                                          nodes.size()};
        kernel->set_data(update, update_data);

        // Note: In a real implementation, we'd update the node's attention
        // Here we just compute to ensure the graph is evaluated
        auto *new_attention = kernel->add(nodes[i]->attention_weights, update);
        kernel->compute(new_attention);
      }
    }
  }

  return attention_weights;
}

NanoBrainTensor *AttentionAllocationEngine::multi_head_self_attention(
    const std::vector<NodeTensor *> &nodes) {

  if (nodes.empty() || !global_attention.initialized)
    return nullptr;

  std::vector<NanoBrainTensor *> head_outputs;

  for (auto &head : global_attention.attention_heads) {
    // For each node, compute Q, K, V projections
    std::vector<NanoBrainTensor *> queries, keys, values;

    for (const auto *node : nodes) {
      if (node && node->embedding) {
        auto *q = kernel->matmul(node->embedding, head.query_projection);
        auto *k = kernel->matmul(node->embedding, head.key_projection);
        auto *v = kernel->matmul(node->embedding, head.value_projection);

        queries.push_back(q);
        keys.push_back(k);
        values.push_back(v);
      }
    }

    if (queries.empty())
      continue;

    // Compute attention scores: Q * K^T / sqrt(d_k)
    // Simplified: use first query against all keys for now
    if (!queries.empty() && !keys.empty()) {
      auto *attention_logits = kernel->contract(queries[0], keys[0]);

      // Scale
      auto *scale = kernel->create_tensor({1});
      std::vector<float> scale_data = {head.head_scale};
      kernel->set_data(scale, scale_data);
      auto *scaled_logits = kernel->mul(attention_logits, scale);

      // Softmax
      auto *attention_probs = kernel->softmax(scaled_logits);
      kernel->compute(attention_probs);

      head.attention_weights = attention_probs;

      // Apply to values
      if (!values.empty()) {
        auto *head_output = kernel->mul(attention_probs, values[0]);
        head_outputs.push_back(head_output);
      }
    }
  }

  // Concatenate head outputs
  if (head_outputs.empty())
    return nullptr;

  // For simplicity, average the head outputs
  NanoBrainTensor *result = head_outputs[0];
  for (size_t i = 1; i < head_outputs.size(); i++) {
    result = kernel->add(result, head_outputs[i]);
  }

  auto *num_heads = kernel->create_tensor({1});
  std::vector<float> num_data = {static_cast<float>(head_outputs.size())};
  kernel->set_data(num_heads, num_data);
  result = kernel->div(result, num_heads);

  kernel->compute(result);
  return result;
}

// ================================================================
// ECAN Economic Mechanisms
// ================================================================

void AttentionAllocationEngine::apply_attention_diffusion(
    const std::vector<NodeTensor *> &node_tensors,
    const std::vector<LinkTensor *> &link_tensors) {

  // For each link, spread attention from source to target
  for (const auto *link : link_tensors) {
    if (!link)
      continue;

    // Find source and target nodes
    float source_attention = 0.0f;

    // Calculate total source attention
    for (const auto &source_id : link->source_nodes) {
      for (const auto *node : node_tensors) {
        if (node && node->atom_id == source_id && node->attention_weights) {
          kernel->compute(node->attention_weights);
          source_attention += kernel->get_value(node->attention_weights, 0);
        }
      }
    }

    // Diffuse to targets
    float diffusion_amount = source_attention * config.diffusion_strength;
    size_t target_count = link->target_nodes.size();

    if (target_count > 0) {
      float amount_per_target = diffusion_amount / target_count;

      for (const auto &target_id : link->target_nodes) {
        for (auto *node : node_tensors) {
          if (node && node->atom_id == target_id && node->attention_weights) {
            auto *delta = kernel->create_tensor({1});
            std::vector<float> delta_data = {amount_per_target};
            kernel->set_data(delta, delta_data);

            auto *new_att = kernel->add(node->attention_weights, delta);
            kernel->compute(new_att);
          }
        }
      }
    }
  }
}

void AttentionAllocationEngine::collect_rent(
    std::vector<NodeTensor *> &node_tensors) {

  for (auto *node : node_tensors) {
    if (!node || !node->attention_weights)
      continue;

    // Calculate rent based on current attention held
    kernel->compute(node->attention_weights);
    float current_attention = kernel->get_value(node->attention_weights, 0);

    // Collect rent (reduce attention)
    float rent = current_attention * config.rent_collection_rate;
    float new_attention = std::max(0.0f, current_attention - rent);

    auto *new_att_tensor = kernel->create_tensor({1});
    std::vector<float> att_data = {new_attention};
    kernel->set_data(new_att_tensor, att_data);

    // Note: In actual implementation, we'd update the node's tensor
    // For now, we just track the change
  }
}

void AttentionAllocationEngine::distribute_wages(
    std::vector<NodeTensor *> &node_tensors, float pool_amount) {

  if (node_tensors.empty() || pool_amount <= 0)
    return;

  // Calculate utility scores for each node
  std::vector<std::pair<NodeTensor *, float>> utility_scores;

  for (auto *node : node_tensors) {
    if (!node || !node->embedding)
      continue;

    // Use embedding magnitude as utility proxy
    auto *abs_embed = kernel->abs(node->embedding);
    auto *mean_embed = kernel->mean(abs_embed);
    kernel->compute(mean_embed);
    float utility = kernel->get_value(mean_embed, 0);

    utility_scores.push_back({node, utility});
  }

  // Sort by utility (descending)
  std::sort(utility_scores.begin(), utility_scores.end(),
            [](const auto &a, const auto &b) { return a.second > b.second; });

  // Distribute wages proportionally
  float total_utility = 0.0f;
  for (const auto &[_, util] : utility_scores) {
    total_utility += util;
  }

  if (total_utility <= 0)
    return;

  float distributable = pool_amount * config.wage_distribution_rate;

  for (const auto &[node, utility] : utility_scores) {
    float wage = (utility / total_utility) * distributable;

    if (node->attention_weights) {
      kernel->compute(node->attention_weights);
      float current = kernel->get_value(node->attention_weights, 0);

      auto *new_att = kernel->create_tensor({1});
      std::vector<float> att_data = {current + wage};
      kernel->set_data(new_att, att_data);
      // Update node's attention (conceptually)
    }
  }
}

void AttentionAllocationEngine::update_link_attention(
    std::vector<LinkTensor *> &link_tensors,
    const std::vector<NodeTensor *> &node_tensors) {

  for (auto *link : link_tensors) {
    if (!link)
      continue;

    float total_source_attention = 0.0f;
    float total_target_attention = 0.0f;

    // Sum source attention
    for (const auto &source_id : link->source_nodes) {
      for (const auto *node : node_tensors) {
        if (node && node->atom_id == source_id && node->attention_weights) {
          kernel->compute(node->attention_weights);
          total_source_attention +=
              kernel->get_value(node->attention_weights, 0);
        }
      }
    }

    // Sum target attention
    for (const auto &target_id : link->target_nodes) {
      for (const auto *node : node_tensors) {
        if (node && node->atom_id == target_id && node->attention_weights) {
          kernel->compute(node->attention_weights);
          total_target_attention +=
              kernel->get_value(node->attention_weights, 0);
        }
      }
    }

    // Link attention is geometric mean of source and target
    float link_attention =
        std::sqrt(total_source_attention * total_target_attention);

    if (link->attention_weights) {
      auto *new_att = kernel->create_tensor({1});
      std::vector<float> att_data = {link_attention};
      kernel->set_data(new_att, att_data);
      // Update link attention (conceptually)
    }
  }
}

// ================================================================
// Gradient-Based Updates
// ================================================================

void AttentionAllocationEngine::apply_gradient_updates(
    std::vector<NodeTensor *> &node_tensors,
    const std::vector<LinkTensor *> &link_tensors) {

  auto *gradients = compute_attention_gradients(node_tensors);
  if (!gradients)
    return;

  kernel->compute(gradients);

  // Apply gradient updates with clipping
  for (size_t i = 0; i < node_tensors.size(); i++) {
    auto *node = node_tensors[i];
    if (!node || !node->attention_weights)
      continue;

    float grad = kernel->get_value(gradients, static_cast<int>(i));

    // Clip gradient
    grad = std::max(-config.gradient_clipping,
                    std::min(config.gradient_clipping, grad));

    // Apply update (gradient descent)
    kernel->compute(node->attention_weights);
    float current = kernel->get_value(node->attention_weights, 0);
    float learning_rate = 0.01f;
    float updated = current + learning_rate * grad;

    // Ensure non-negative
    updated = std::max(0.0f, updated);

    auto *new_att = kernel->create_tensor({1});
    std::vector<float> att_data = {updated};
    kernel->set_data(new_att, att_data);
  }
}

NanoBrainTensor *AttentionAllocationEngine::compute_attention_gradients(
    const std::vector<NodeTensor *> &nodes) {

  if (nodes.empty())
    return nullptr;

  auto *gradients = kernel->create_tensor({static_cast<int64_t>(nodes.size())});
  std::vector<float> grad_data(nodes.size(), 0.0f);

  // Simple gradient: difference from mean attention
  float mean_attention = 0.0f;
  int count = 0;

  for (const auto *node : nodes) {
    if (node && node->attention_weights) {
      kernel->compute(node->attention_weights);
      mean_attention += kernel->get_value(node->attention_weights, 0);
      count++;
    }
  }

  if (count > 0) {
    mean_attention /= count;
  }

  for (size_t i = 0; i < nodes.size(); i++) {
    if (nodes[i] && nodes[i]->attention_weights) {
      kernel->compute(nodes[i]->attention_weights);
      float current = kernel->get_value(nodes[i]->attention_weights, 0);
      // Gradient points toward mean (stabilizing)
      grad_data[i] = mean_attention - current;
    }
  }

  kernel->set_data(gradients, grad_data);
  return gradients;
}

// ================================================================
// Constraint Enforcement
// ================================================================

void AttentionAllocationEngine::enforce_resource_constraints(
    std::vector<NodeTensor *> &node_tensors) {

  float total_attention = 0.0f;

  // Calculate total attention
  for (const auto *node : node_tensors) {
    if (node && node->attention_weights) {
      kernel->compute(node->attention_weights);
      total_attention += kernel->get_value(node->attention_weights, 0);
    }
  }

  // If over budget, scale down proportionally
  if (total_attention > config.resource_budget) {
    float scale = config.resource_budget / total_attention;

    for (auto *node : node_tensors) {
      if (node && node->attention_weights) {
        kernel->compute(node->attention_weights);
        float current = kernel->get_value(node->attention_weights, 0);

        auto *scaled = kernel->create_tensor({1});
        std::vector<float> scaled_data = {current * scale};
        kernel->set_data(scaled, scaled_data);
      }
    }
  }
}

void AttentionAllocationEngine::normalize_attention(
    std::vector<NodeTensor *> &node_tensors) {

  float total_attention = 0.0f;

  for (const auto *node : node_tensors) {
    if (node && node->attention_weights) {
      kernel->compute(node->attention_weights);
      total_attention +=
          std::max(0.0f, kernel->get_value(node->attention_weights, 0));
    }
  }

  if (total_attention > 0) {
    for (auto *node : node_tensors) {
      if (node && node->attention_weights) {
        kernel->compute(node->attention_weights);
        float current = kernel->get_value(node->attention_weights, 0);
        float normalized = current / total_attention;

        auto *norm_tensor = kernel->create_tensor({1});
        std::vector<float> norm_data = {normalized};
        kernel->set_data(norm_tensor, norm_data);
      }
    }
  }
}

// ================================================================
// Monitoring and Statistics
// ================================================================

void AttentionAllocationEngine::record_attention_flows(
    const std::vector<NodeTensor *> &nodes,
    const std::vector<LinkTensor *> &links) {

  int64_t now = current_time_millis();

  // Record flows through links
  for (const auto *link : links) {
    if (!link)
      continue;

    for (const auto &source_id : link->source_nodes) {
      for (const auto &target_id : link->target_nodes) {
        AttentionFlow flow;
        flow.timestamp = now;
        flow.source_id = source_id;
        flow.target_id = target_id;
        flow.attention_transfer = config.diffusion_strength;
        flow.reason = "diffusion";

        attention_flows.push_back(flow);

        // Limit history
        if (attention_flows.size() > max_flow_history) {
          attention_flows.erase(attention_flows.begin());
        }
      }
    }
  }
}

AttentionStats AttentionAllocationEngine::calculate_attention_stats(
    const std::vector<NodeTensor *> &node_tensors) {

  AttentionStats stats;
  stats.total_attention = 0.0f;
  stats.average_attention = 0.0f;
  stats.attention_entropy = 0.0f;
  stats.resource_utilization = 0.0f;
  stats.gradient_norm = 0.0f;
  stats.convergence_rate = 0.0f;

  if (node_tensors.empty())
    return stats;

  std::vector<float> attention_values;
  attention_values.reserve(node_tensors.size());

  for (const auto *node : node_tensors) {
    if (node && node->attention_weights) {
      kernel->compute(node->attention_weights);
      float val = kernel->get_value(node->attention_weights, 0);
      attention_values.push_back(std::max(0.0f, val));
      stats.total_attention += val;
    }
  }

  if (!attention_values.empty()) {
    stats.average_attention = stats.total_attention / attention_values.size();
    stats.attention_entropy = calculate_entropy(attention_values);
    stats.resource_utilization =
        std::min(1.0f, stats.total_attention / config.resource_budget);

    // Convergence rate based on entropy (higher entropy = less converged)
    stats.convergence_rate = 1.0f - stats.attention_entropy;
  }

  return stats;
}

float AttentionAllocationEngine::calculate_entropy(
    const std::vector<float> &distribution) {

  if (distribution.empty())
    return 0.0f;

  // Normalize to probability distribution
  float sum = 0.0f;
  for (float v : distribution) {
    sum += std::max(0.0f, v);
  }

  if (sum <= 0)
    return 0.0f;

  float entropy = 0.0f;
  for (float v : distribution) {
    float p = std::max(0.0f, v) / sum;
    if (p > 0) {
      entropy -= p * std::log2(p);
    }
  }

  // Normalize by max entropy
  float max_entropy = std::log2(static_cast<float>(distribution.size()));
  return max_entropy > 0 ? entropy / max_entropy : 0.0f;
}

float AttentionAllocationEngine::calculate_gradient_norm(
    const std::vector<NanoBrainTensor *> &tensors) {

  float norm_sq = 0.0f;

  for (const auto *tensor : tensors) {
    if (tensor) {
      auto *abs_t = kernel->abs(const_cast<NanoBrainTensor *>(tensor));
      auto *sum_t = kernel->sum(abs_t);
      kernel->compute(sum_t);
      float val = kernel->get_value(sum_t, 0);
      norm_sq += val * val;
    }
  }

  return std::sqrt(norm_sq);
}

std::vector<AttentionFlow>
AttentionAllocationEngine::get_attention_flows() const {
  return attention_flows;
}

AttentionStats AttentionAllocationEngine::get_current_stats(
    const std::vector<NodeTensor *> &node_tensors) {
  return calculate_attention_stats(node_tensors);
}

// ================================================================
// State Management
// ================================================================

void AttentionAllocationEngine::reset() {
  attention_flows.clear();
  global_attention.attention_heads.clear();
  global_attention.initialized = false;
  std::cout << "[AttentionEngine] Reset" << std::endl;
}

void AttentionAllocationEngine::update_config(
    const AttentionAllocationConfig &new_config) {
  config = new_config;

  // Update temperature tensor if initialized
  if (global_attention.initialized && global_attention.temperature_tensor) {
    std::vector<float> temp_data = {config.temperature};
    kernel->set_data(global_attention.temperature_tensor, temp_data);
  }
}
