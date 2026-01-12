#include "nanobrain_attention.h"
#include "nanobrain_encoder.h"
#include "nanobrain_kernel.h"
#include "nanobrain_reasoning.h"
#include "nanobrain_time_crystal.h"
#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>

int main() {
  std::cout << "=================================================="
            << std::endl;
  std::cout << "    NanoBrain C++ Kernel Test Suite               "
            << std::endl;
  std::cout << "=================================================="
            << std::endl;

  // ================================================================
  // Part 1: Basic Tensor Operations
  // ================================================================
  std::cout << "\n--- Part 1: Basic Tensor Operations ---" << std::endl;

  NanoBrainConfig config;
  config.memory_size = 1024 * 1024 * 64; // 64 MB
  config.use_gpu = false;

  NanoBrainKernel kernel(config);

  auto A = kernel.create_tensor({2, 3});
  auto B = kernel.create_tensor({3, 2});

  std::cout << "Tensor A created: " << A->id << std::endl;
  std::cout << "Tensor B created: " << B->id << std::endl;

  auto C = kernel.matmul(A, B);
  kernel.compute(C);

  std::cout << "Result C (Matmul A*B):" << std::endl;
  kernel.print_tensor(C);

  // ================================================================
  // Part 2: Element-wise & Math Operations
  // ================================================================
  std::cout << "\n--- Part 2: Element-wise & Math Operations ---" << std::endl;

  auto SinA = kernel.sin(A);
  kernel.compute(SinA);
  std::cout << "Sin(A):" << std::endl;
  kernel.print_tensor(SinA);

  // ================================================================
  // Part 3: Phase Prime Metric Coherence
  // ================================================================
  std::cout << "\n--- Part 3: Phase Prime Metric Coherence ---" << std::endl;

  auto Primes = kernel.create_tensor({3});
  kernel.set_data(Primes, {2.0f, 3.0f, 5.0f});

  auto Coherence = kernel.compute_coherence(Primes);
  kernel.compute(Coherence);
  std::cout << "Coherence Score for [2, 3, 5]:" << std::endl;
  kernel.print_tensor(Coherence);
  std::cout << "Expected (approx): 0.994" << std::endl;

  // ================================================================
  // Part 4: AtomSpace Tensor Encoder
  // ================================================================
  std::cout << "\n--- Part 4: AtomSpace Tensor Encoder ---" << std::endl;

  // Create a mock Atom using correct struct from nanobrain_types.h
  Atom mock_atom;
  mock_atom.id = "atom_test_1";
  mock_atom.type = "ConceptNode";
  mock_atom.name = "TestAtom";
  mock_atom.truth_value[0] = 0.8f;       // strength
  mock_atom.truth_value[1] = 0.9f;       // confidence
  mock_atom.truth_value[2] = 10.0f;      // count
  mock_atom.attention_value[0] = 100.0f; // sti
  mock_atom.attention_value[1] = 50.0f;  // lti
  mock_atom.attention_value[2] = 1.0f;   // vlti

  // Initialize the encoder
  AtomSpaceTensorEncoder encoder(&kernel);

  // Encode the atom
  NodeTensor *embedding = encoder.encode_atom(mock_atom);

  std::cout << "Encoded Atom (TestAtom):" << std::endl;
  std::cout << "  Embedding ID: " << embedding->id << std::endl;
  std::cout << "  Atom ID: " << embedding->atom_id << std::endl;
  std::cout << "  Shape: [" << embedding->shape[0] << "]" << std::endl;

  // ================================================================
  // Part 5: Time Crystal Kernel
  // ================================================================
  std::cout << "\n--- Part 5: Time Crystal Kernel ---" << std::endl;

  TimeCrystalConfig tc_config;
  tc_config.memory_size = 1024 * 1024 * 32; // 32 MB
  tc_config.temporal_processing_frequency = 60.0f;

  TimeCrystalKernel time_crystal(tc_config);
  time_crystal.initialize();

  // Get metrics
  NanoBrainMetrics metrics = time_crystal.get_metrics();
  std::cout << "Time Crystal Metrics:" << std::endl;
  std::cout << "  Total Atoms: " << metrics.total_atoms << std::endl;
  std::cout << "  Quantum Coherence: " << metrics.quantum_coherence
            << std::endl;
  std::cout << "  Prime Alignment: " << metrics.prime_alignment << std::endl;

  // Test PPM coherence
  float ppm_coherence = time_crystal.compute_ppm_coherence({2, 3, 5, 7});
  std::cout << "  PPM Coherence [2,3,5,7]: " << ppm_coherence << std::endl;

  // ================================================================
  // Part 6: Recursive Reasoning Engine
  // ================================================================
  std::cout << "\n--- Part 6: Recursive Reasoning Engine ---" << std::endl;

  ReasoningEngineConfig reasoning_config;
  reasoning_config.max_reasoning_depth = 5;
  reasoning_config.confidence_threshold = 0.1f;
  reasoning_config.chain_length_limit = 20;

  RecursiveReasoningEngine reasoning(&kernel, reasoning_config);
  reasoning.initialize();

  // Start a reasoning chain
  std::string chain_id = reasoning.start_reasoning_chain({"node1", "node2"});
  std::cout << "Started reasoning chain: " << chain_id << std::endl;

  // Get stats
  ReasoningStats stats = reasoning.get_stats();
  std::cout << "Reasoning Stats:" << std::endl;
  std::cout << "  Total Chains: " << stats.total_chains << std::endl;
  std::cout << "  Active Chains: " << stats.active_chains << std::endl;
  std::cout << "  Available Rules: " << reasoning.get_rule_ids().size()
            << std::endl;

  // Print available rules
  std::cout << "  Rules: ";
  for (const auto &rule_id : reasoning.get_rule_ids()) {
    std::cout << rule_id << " ";
  }
  std::cout << std::endl;

  // ================================================================
  // Part 7: Attention Allocation Engine
  // ================================================================
  std::cout << "\n--- Part 7: Attention Allocation Engine ---" << std::endl;

  AttentionAllocationConfig attention_config;
  attention_config.mechanism = AttentionMechanism::Hybrid;
  attention_config.temperature = 1.0f;
  attention_config.resource_budget = 1000.0f;
  attention_config.attention_heads = 4;

  AttentionAllocationEngine attention(&kernel, attention_config);
  attention.initialize(100); // Max 100 nodes

  std::cout << "Attention Engine Initialized:" << std::endl;
  std::cout << "  Mechanism: Hybrid (Softmax + ECAN)" << std::endl;
  std::cout << "  Attention Heads: " << attention_config.attention_heads
            << std::endl;
  std::cout << "  Resource Budget: " << attention_config.resource_budget
            << std::endl;
  std::cout << "  Initialized: " << (attention.is_initialized() ? "Yes" : "No")
            << std::endl;

  // Create mock node tensors for testing
  std::vector<NodeTensor *> node_tensors;
  std::vector<LinkTensor *> link_tensors;

  for (int i = 0; i < 5; i++) {
    NodeTensor *node = new NodeTensor();
    node->id = "node_tensor_" + std::to_string(i);
    node->atom_id = "atom_" + std::to_string(i);
    node->embedding = kernel.create_tensor({128});
    node->attention_weights = kernel.create_tensor({1});
    node->truth_value_tensor = kernel.create_tensor({3});
    node->symbolic_features = kernel.create_tensor({128});
    node->shape = {128};

    // Set initial attention
    std::vector<float> att_data = {10.0f + i * 5.0f};
    kernel.set_data(node->attention_weights, att_data);

    // Set truth values
    std::vector<float> tv_data = {0.8f, 0.9f, 5.0f};
    kernel.set_data(node->truth_value_tensor, tv_data);

    node_tensors.push_back(node);
  }

  // Create a mock link
  LinkTensor *link = new LinkTensor();
  link->id = "link_tensor_0";
  link->atom_id = "link_0";
  link->source_nodes = {"atom_0", "atom_1"};
  link->target_nodes = {"atom_2", "atom_3"};
  link->relation_tensor = kernel.create_tensor({64});
  link->attention_weights = kernel.create_tensor({1});
  link->truth_value_tensor = kernel.create_tensor({3});
  kernel.set_data(link->truth_value_tensor, {0.7f, 0.8f, 3.0f});
  link_tensors.push_back(link);

  // Test attention allocation
  AttentionStats attention_stats =
      attention.update_attention_allocation(node_tensors, link_tensors);

  std::cout << "Attention Allocation Stats:" << std::endl;
  std::cout << "  Total Attention: " << attention_stats.total_attention
            << std::endl;
  std::cout << "  Average Attention: " << attention_stats.average_attention
            << std::endl;
  std::cout << "  Attention Entropy: " << attention_stats.attention_entropy
            << std::endl;
  std::cout << "  Resource Utilization: "
            << attention_stats.resource_utilization << std::endl;
  std::cout << "  Convergence Rate: " << attention_stats.convergence_rate
            << std::endl;

  // Test different attention mechanisms
  std::cout << "\n  Testing Softmax Attention..." << std::endl;
  attention.update_config(attention_config);
  AttentionAllocationConfig softmax_config = attention_config;
  softmax_config.mechanism = AttentionMechanism::Softmax;
  attention.update_config(softmax_config);
  AttentionStats softmax_stats =
      attention.update_attention_allocation(node_tensors, link_tensors);
  std::cout << "  Softmax Total Attention: " << softmax_stats.total_attention
            << std::endl;

  std::cout << "  Testing ECAN Attention..." << std::endl;
  AttentionAllocationConfig ecan_config = attention_config;
  ecan_config.mechanism = AttentionMechanism::ECAN;
  attention.update_config(ecan_config);
  AttentionStats ecan_stats =
      attention.update_attention_allocation(node_tensors, link_tensors);
  std::cout << "  ECAN Total Attention: " << ecan_stats.total_attention
            << std::endl;

  // Cleanup mock tensors
  for (auto *node : node_tensors) {
    delete node;
  }
  delete link;

  // ================================================================
  // Done
  // ================================================================
  std::cout << "\n==================================================="
            << std::endl;
  std::cout << "    All Tests Complete                            "
            << std::endl;
  std::cout << "    NanoBrain C++ llama.cpp Adaptation Ready      "
            << std::endl;
  std::cout << "==================================================="
            << std::endl;

  return 0;
}
