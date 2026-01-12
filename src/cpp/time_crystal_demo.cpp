/**
 * NanoBrain Time Crystal Demo
 *
 * Demonstrates the C++ adaptation of the OpenCog NanoBrain Time Crystal
 * Architecture, including:
 * - Phase Prime Metric (PPM) coherence calculations
 * - 11D quantum manifold processing
 * - Geometric Musical Language (GML) resonance
 * - ECAN attention allocation
 * - PLN reasoning with time crystal enhancement
 * - Recursive reasoning engine
 * - Attention allocation subsystem
 */

#include "nanobrain_attention.h"
#include "nanobrain_reasoning.h"
#include "nanobrain_time_crystal.h"
#include <iomanip>
#include <iostream>

void print_separator(const std::string &title) {
  std::cout << "\n";
  std::cout
      << "================================================================"
         "================\n";
  std::cout << "  " << title << "\n";
  std::cout
      << "================================================================"
         "================\n";
}

void demo_ppm_coherence(TimeCrystalKernel &kernel) {
  print_separator("Phase Prime Metric (PPM) Coherence Demo");

  // Test with fundamental primes
  std::vector<std::vector<int>> test_primes = {
      {2, 3, 5},                        // First 3 primes
      {7, 11, 13},                      // Middle primes
      {2, 3, 5, 7, 11},                 // First 5 primes
      {2, 3, 5, 7, 11, 13, 17, 19, 23}, // First 9 primes
      {41, 43, 47}                      // Last 3 fundamental primes
  };

  std::cout << std::fixed << std::setprecision(6);

  for (const auto &primes : test_primes) {
    float coherence = kernel.compute_ppm_coherence(primes);
    float importance = kernel.calculate_prime_importance(primes);
    float frequency = kernel.calculate_resonance_frequency(primes);

    std::cout << "  Primes: [";
    for (size_t i = 0; i < primes.size(); i++) {
      std::cout << primes[i] << (i < primes.size() - 1 ? ", " : "");
    }
    std::cout << "]\n";
    std::cout << "    Coherence:  " << coherence << "\n";
    std::cout << "    Importance: " << importance << "\n";
    std::cout << "    Frequency:  " << frequency << " Hz\n\n";
  }
}

void demo_time_crystal_states(TimeCrystalKernel &kernel) {
  print_separator("Time Crystal Quantum States Demo");

  auto atom_ids = kernel.get_all_atom_ids();

  std::cout << "  Total atoms in AtomSpace: " << atom_ids.size() << "\n\n";
  std::cout << "  Sample Time Crystal States:\n";
  std::cout << std::fixed << std::setprecision(4);

  int count = 0;
  for (const auto &id : atom_ids) {
    if (count >= 5)
      break;

    const auto *atom = kernel.get_atom(id);
    if (atom) {
      std::cout << "  [" << atom->name << "] (" << atom->type << ")\n";
      std::cout << "    Temporal Coherence: "
                << atom->time_crystal_state.temporal_coherence << "\n";
      std::cout << "    Fractal Dimension:  "
                << atom->time_crystal_state.fractal_dimension << "\n";
      std::cout << "    Quantum Phase:      "
                << atom->time_crystal_state.quantum_phase << " rad\n";
      std::cout << "    Resonance Freq:     "
                << atom->time_crystal_state.resonance_frequency << " Hz\n";
      std::cout << "    11D Coordinates:    [";
      for (int i = 0; i < 3; i++) {
        std::cout << atom->time_crystal_state.dimensions[i];
        if (i < 2)
          std::cout << ", ";
      }
      std::cout << ", ...]\n\n";
    }
    count++;
  }
}

void demo_gml_resonance(TimeCrystalKernel &kernel) {
  print_separator("Geometric Musical Language (GML) Resonance Demo");

  auto atom_ids = kernel.get_all_atom_ids();

  // Find GML atoms
  std::vector<std::string> gml_atoms;
  for (const auto &id : atom_ids) {
    const auto *atom = kernel.get_atom(id);
    if (atom && atom->name.find("GMLShape") != std::string::npos) {
      gml_atoms.push_back(id);
    }
  }

  std::cout << "  Found " << gml_atoms.size() << " GML shape atoms\n\n";

  // Compare resonances between shapes
  if (gml_atoms.size() >= 3) {
    std::cout << "  Shape Resonance Comparisons:\n";
    std::cout << std::fixed << std::setprecision(4);

    for (size_t i = 0; i < 3; i++) {
      for (size_t j = i + 1; j < 3; j++) {
        const auto *atom1 = kernel.get_atom(gml_atoms[i]);
        const auto *atom2 = kernel.get_atom(gml_atoms[j]);

        if (atom1 && atom2) {
          float resonance = kernel.calculate_geometric_resonance(
              atom1->fractal_geometry, atom2->fractal_geometry);

          std::cout << "    " << atom1->name << " <-> " << atom2->name << ": "
                    << resonance << "\n";
        }
      }
    }
  }

  // Musical harmony demo
  std::cout << "\n  Musical Harmony Between Notes:\n";
  std::vector<MusicalNote> notes = {MusicalNote::C, MusicalNote::E,
                                    MusicalNote::G, MusicalNote::A};

  for (size_t i = 0; i < notes.size(); i++) {
    for (size_t j = i + 1; j < notes.size(); j++) {
      float harmony = kernel.calculate_musical_harmony(notes[i], notes[j]);
      std::cout << "    " << musical_note_to_string(notes[i]) << " <-> "
                << musical_note_to_string(notes[j]) << ": " << harmony << "\n";
    }
  }
}

void demo_attention_allocation(TimeCrystalKernel &kernel) {
  print_separator("ECAN Attention Allocation Demo");

  // Run a few attention allocation cycles
  std::cout << "  Running 5 attention allocation cycles...\n\n";

  for (int i = 0; i < 5; i++) {
    kernel.perform_attention_allocation();
    kernel.apply_attention_decay();
  }

  // Get top attention atoms
  auto top_atoms = kernel.get_top_attention_atoms(5);

  std::cout << "  Top 5 Atoms by Attention:\n";
  std::cout << std::fixed << std::setprecision(4);

  for (const auto &id : top_atoms) {
    const auto *atom = kernel.get_atom(id);
    if (atom) {
      std::cout << "    " << std::left << std::setw(30) << atom->name
                << " STI: " << std::setw(10) << atom->attention_value.sti
                << " LTI: " << atom->attention_value.lti << "\n";
    }
  }
}

void demo_pln_reasoning(TimeCrystalKernel &kernel) {
  print_separator("PLN Reasoning with Time Crystal Enhancement Demo");

  // Run PLN reasoning
  std::cout << "  Performing PLN reasoning cycle...\n\n";
  kernel.perform_pln_reasoning();

  auto inference_ids = kernel.get_all_inference_ids();
  std::cout << "  Generated " << inference_ids.size() << " inferences\n\n";

  if (!inference_ids.empty()) {
    std::cout << "  Sample Inferences:\n";
    std::cout << std::fixed << std::setprecision(4);

    int count = 0;
    for (const auto &id : inference_ids) {
      if (count >= 3)
        break;

      const auto *inference = kernel.get_inference(id);
      if (inference) {
        std::cout << "  [Inference " << (count + 1) << "]\n";
        std::cout << "    Premises: ";
        for (size_t i = 0; i < inference->premise_ids.size(); i++) {
          const auto *premise = kernel.get_atom(inference->premise_ids[i]);
          if (premise) {
            std::cout << premise->name;
            if (i < inference->premise_ids.size() - 1)
              std::cout << ", ";
          }
        }
        std::cout << "\n";

        const auto *conclusion = kernel.get_atom(inference->conclusion_id);
        if (conclusion) {
          std::cout << "    Conclusion: " << conclusion->name << "\n";
        }

        std::cout << "    Temporal Flow:     " << inference->temporal_flow
                  << "\n";
        std::cout << "    Prime Consistency: " << inference->prime_consistency
                  << "\n";
        std::cout << "    Quantum Coherence: " << inference->quantum_coherence
                  << "\n\n";
      }
      count++;
    }
  }
}

void demo_recursive_reasoning(NanoBrainKernel *tensor_kernel) {
  print_separator("Recursive Reasoning Engine Demo");

  ReasoningEngineConfig config;
  config.max_reasoning_depth = 3;
  config.confidence_threshold = 0.2f;
  config.chain_length_limit = 10;

  RecursiveReasoningEngine reasoning_engine(tensor_kernel, config);
  reasoning_engine.initialize();

  std::cout << "  Available inference rules: "
            << reasoning_engine.get_rule_ids().size() << "\n";

  for (const auto &rule_id : reasoning_engine.get_rule_ids()) {
    std::cout << "    - " << rule_id << "\n";
  }

  // Create some test nodes for reasoning
  std::cout << "\n  Creating test reasoning chain...\n";

  std::string chain_id = reasoning_engine.start_reasoning_chain(
      {"test_node_1", "test_node_2", "test_node_3"});

  std::cout << "  Chain ID: " << chain_id << "\n";

  // Execute reasoning steps
  std::vector<NodeTensor *> test_nodes;
  std::vector<LinkTensor *> test_links;

  // Create dummy node tensors
  NodeTensor node1, node2;
  node1.id = "node_tensor_1";
  node1.atom_id = "test_node_1";
  node1.embedding = tensor_kernel->create_tensor({128});
  node1.attention_weights = tensor_kernel->create_tensor({1});
  tensor_kernel->set_data(node1.attention_weights, {100.0f});

  node2.id = "node_tensor_2";
  node2.atom_id = "test_node_2";
  node2.embedding = tensor_kernel->create_tensor({128});
  node2.attention_weights = tensor_kernel->create_tensor({1});
  tensor_kernel->set_data(node2.attention_weights, {80.0f});

  test_nodes.push_back(&node1);
  test_nodes.push_back(&node2);

  // Execute 3 reasoning steps
  for (int i = 0; i < 3; i++) {
    ReasoningStats stats =
        reasoning_engine.execute_reasoning_step(test_nodes, test_links);
    std::cout << "\n  After step " << (i + 1) << ":\n";
    std::cout << "    Active chains: " << stats.active_chains << "\n";
    std::cout << "    Total inferences: " << stats.total_inferences << "\n";
    std::cout << "    Average confidence: " << stats.average_confidence << "\n";
  }

  // Get final chain state
  const auto *chain = reasoning_engine.get_chain(chain_id);
  if (chain) {
    std::cout << "\n  Final Chain State:\n";
    std::cout << "    Depth: " << chain->current_depth << "\n";
    std::cout << "    Confidence: " << chain->confidence_score << "\n";
    std::cout << "    Status: ";
    switch (chain->convergence_status) {
    case ConvergenceStatus::Active:
      std::cout << "Active";
      break;
    case ConvergenceStatus::Converged:
      std::cout << "Converged";
      break;
    case ConvergenceStatus::Diverged:
      std::cout << "Diverged";
      break;
    case ConvergenceStatus::Terminated:
      std::cout << "Terminated";
      break;
    }
    std::cout << "\n";

    float quality = reasoning_engine.evaluate_reasoning_quality(*chain);
    std::cout << "    Quality Score: " << quality << "\n";
  }
}

void demo_attention_engine(NanoBrainKernel *tensor_kernel) {
  print_separator("Attention Allocation Engine Demo");

  AttentionAllocationConfig config;
  config.mechanism = AttentionMechanism::Hybrid;
  config.temperature = 1.0f;
  config.resource_budget = 1000.0f;
  config.attention_heads = 4;

  AttentionAllocationEngine attention_engine(tensor_kernel, config);
  attention_engine.initialize(100);

  std::cout << "  Attention mechanism: Hybrid (Softmax + ECAN)\n";
  std::cout << "  Resource budget: " << config.resource_budget << "\n";
  std::cout << "  Attention heads: " << config.attention_heads << "\n\n";

  // Create test nodes
  std::vector<NodeTensor> test_node_storage(5);
  std::vector<NodeTensor *> test_nodes;

  for (int i = 0; i < 5; i++) {
    test_node_storage[i].id = "node_" + std::to_string(i);
    test_node_storage[i].atom_id = "atom_" + std::to_string(i);
    test_node_storage[i].embedding = tensor_kernel->create_tensor({128});
    test_node_storage[i].attention_weights = tensor_kernel->create_tensor({1});
    tensor_kernel->set_data(test_node_storage[i].attention_weights,
                            {100.0f + i * 20.0f});
    test_nodes.push_back(&test_node_storage[i]);
  }

  std::vector<LinkTensor *> test_links;

  std::cout << "  Initial attention values:\n";
  for (const auto *node : test_nodes) {
    tensor_kernel->compute(node->attention_weights);
    float val = tensor_kernel->get_value(node->attention_weights, 0);
    std::cout << "    " << node->id << ": " << val << "\n";
  }

  // Run attention allocation
  std::cout << "\n  Running 3 attention update cycles...\n";

  for (int i = 0; i < 3; i++) {
    AttentionStats stats =
        attention_engine.update_attention_allocation(test_nodes, test_links);

    std::cout << "\n  Cycle " << (i + 1) << " Stats:\n";
    std::cout << "    Total attention: " << stats.total_attention << "\n";
    std::cout << "    Average attention: " << stats.average_attention << "\n";
    std::cout << "    Entropy: " << stats.attention_entropy << "\n";
    std::cout << "    Resource utilization: " << stats.resource_utilization
              << "\n";
  }
}

void demo_full_processing_cycle(TimeCrystalKernel &kernel) {
  print_separator("Full Processing Cycle Demo");

  std::cout << "  Running 10 full processing cycles...\n\n";

  for (int i = 0; i < 10; i++) {
    kernel.process_cycle();
  }

  auto metrics = kernel.get_metrics();

  std::cout << "  After 10 cycles:\n";
  std::cout << std::fixed << std::setprecision(4);
  std::cout << "    Total Atoms:           " << metrics.total_atoms << "\n";
  std::cout << "    Total Links:           " << metrics.total_links << "\n";
  std::cout << "    Average Attention:     " << metrics.average_attention
            << "\n";
  std::cout << "    Quantum Coherence:     " << metrics.quantum_coherence
            << "\n";
  std::cout << "    Temporal Stability:    " << metrics.temporal_stability
            << "\n";
  std::cout << "    Prime Alignment:       " << metrics.prime_alignment << "\n";
  std::cout << "    Fractal Complexity:    " << metrics.fractal_complexity
            << "\n";
  std::cout << "    Inference Rate:        " << metrics.inference_rate
            << " inf/s\n";
  std::cout << "    Consciousness Level:   " << metrics.consciousness_emergence
            << "\n";
  std::cout << "    Processing Cycles:     " << kernel.get_cycle_count()
            << "\n";
}

int main() {
  std::cout << R"(
  ╔══════════════════════════════════════════════════════════════════════════╗
  ║                                                                          ║
  ║       NanoBrain Time Crystal Architecture - C++ / llama.cpp Demo         ║
  ║                                                                          ║
  ║    Implementing OpenCog AtomSpace with Phase Prime Metrics (PPM),        ║
  ║    11D Time Crystal Processing, and Geometric Musical Language (GML)    ║
  ║                                                                          ║
  ╚══════════════════════════════════════════════════════════════════════════╝
)" << std::endl;

  // Initialize Time Crystal Kernel
  TimeCrystalConfig config;
  config.memory_size = 1024 * 1024 * 128; // 128 MB
  config.pln_inference_depth = 5;
  config.temporal_processing_frequency = 60.0f;

  std::cout << "  Initializing Time Crystal Kernel...\n";
  TimeCrystalKernel kernel(config);
  kernel.initialize();

  // Run demos
  demo_ppm_coherence(kernel);
  demo_time_crystal_states(kernel);
  demo_gml_resonance(kernel);
  demo_attention_allocation(kernel);
  demo_pln_reasoning(kernel);
  demo_recursive_reasoning(kernel.get_tensor_kernel());
  demo_attention_engine(kernel.get_tensor_kernel());
  demo_full_processing_cycle(kernel);

  print_separator("Demo Complete");
  std::cout << "\n  All NanoBrain C++ components demonstrated successfully!\n";
  std::cout << "  This implementation is compatible with llama.cpp/ggml.\n\n";

  kernel.shutdown();
  return 0;
}
