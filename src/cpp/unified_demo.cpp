#include "nanobrain_unified.h"
#include <iomanip>
#include <iostream>

/**
 * Unified NanoBrain Demo
 *
 * Demonstrates the complete NanoBrain cognitive architecture including:
 * - Time Crystal 11D quantum processing
 * - Recursive PLN reasoning
 * - Softmax/ECAN attention allocation
 * - Meta-cognitive self-monitoring
 */

void print_header(const std::string &title) {
  std::cout << "\n";
  std::cout << "========================================" << std::endl;
  std::cout << " " << title << std::endl;
  std::cout << "========================================" << std::endl;
}

void print_metrics(const UnifiedNanoBrainMetrics &m) {
  std::cout << std::fixed << std::setprecision(4);
  std::cout << "\n--- System Metrics ---" << std::endl;
  std::cout << "  Atoms: " << m.total_atoms << std::endl;
  std::cout << "  Links: " << m.total_links << std::endl;
  std::cout << "  Inferences: " << m.total_inferences << std::endl;
  std::cout << "  Uptime: " << m.uptime_seconds << " sec" << std::endl;

  std::cout << "\n--- Time Crystal Metrics ---" << std::endl;
  std::cout << "  Quantum Coherence: " << m.quantum_coherence << std::endl;
  std::cout << "  Temporal Stability: " << m.temporal_stability << std::endl;
  std::cout << "  Prime Alignment: " << m.prime_alignment << std::endl;
  std::cout << "  Consciousness Emergence: " << m.consciousness_emergence
            << std::endl;

  std::cout << "\n--- Attention Metrics ---" << std::endl;
  std::cout << "  Total Attention: " << m.total_attention << std::endl;
  std::cout << "  Entropy: " << m.attention_entropy << std::endl;
  std::cout << "  Resource Utilization: " << m.resource_utilization
            << std::endl;

  std::cout << "\n--- Reasoning Metrics ---" << std::endl;
  std::cout << "  Active Chains: " << m.active_chains << std::endl;
  std::cout << "  Converged Chains: " << m.converged_chains << std::endl;
  std::cout << "  Throughput: " << m.reasoning_throughput << " inferences/sec"
            << std::endl;

  std::cout << "\n--- Meta-Cognitive Metrics ---" << std::endl;
  std::cout << "  Self-Awareness: " << m.self_awareness_level << std::endl;
  std::cout << "  System Coherence: " << m.system_coherence << std::endl;
  std::cout << "  Adaptation Rate: " << m.adaptation_rate << std::endl;
}

int main() {
  print_header("NanoBrain Unified Kernel Demo");

  std::cout << "\nVersion: " << get_nanobrain_version() << std::endl;

  // ================================================================
  // 1. Create and Initialize Unified Kernel
  // ================================================================
  print_header("1. Initialization");

  UnifiedNanoBrainConfig config;
  config.memory_size = 1024 * 1024 * 128; // 128 MB
  config.time_crystal_dimensions = 11;
  config.max_reasoning_depth = 5;
  config.attention_mechanism = AttentionMechanism::Hybrid;
  config.meta_levels = 3;

  UnifiedNanoBrainKernel kernel(config);
  kernel.initialize();

  // ================================================================
  // 2. Create Custom Atoms
  // ================================================================
  print_header("2. Creating Atoms");

  // Create concept atoms with prime encodings
  std::string cat_id =
      kernel.create_atom("ConceptNode", "Cat", 0.9f, 0.8f, {2, 3, 5});
  std::string animal_id =
      kernel.create_atom("ConceptNode", "Animal", 0.95f, 0.9f, {2, 3, 7});
  std::string mammal_id =
      kernel.create_atom("ConceptNode", "Mammal", 0.85f, 0.75f, {2, 5, 7});
  std::string pet_id =
      kernel.create_atom("ConceptNode", "Pet", 0.7f, 0.6f, {3, 5, 11});

  std::cout << "Created atoms:" << std::endl;
  std::cout << "  - Cat: " << cat_id << std::endl;
  std::cout << "  - Animal: " << animal_id << std::endl;
  std::cout << "  - Mammal: " << mammal_id << std::endl;
  std::cout << "  - Pet: " << pet_id << std::endl;

  auto all_atoms = kernel.get_all_atom_ids();
  std::cout << "\nTotal atoms in system: " << all_atoms.size() << std::endl;

  // ================================================================
  // 3. Test PPM Coherence
  // ================================================================
  print_header("3. Phase Prime Metric Coherence");

  std::vector<std::vector<int>> prime_sets = {
      {2, 3, 5},            // First fundamentals
      {7, 11, 13},          // Higher primes
      {2, 3, 5, 7, 11, 13}, // Full set
      {2},                  // Single prime
      {47}                  // Large prime
  };

  for (const auto &primes : prime_sets) {
    float coherence = kernel.compute_coherence(primes);
    std::cout << "Coherence for {";
    for (size_t i = 0; i < primes.size(); i++) {
      std::cout << primes[i] << (i < primes.size() - 1 ? ", " : "");
    }
    std::cout << "}: " << coherence << std::endl;
  }

  // ================================================================
  // 4. Start Reasoning Chains
  // ================================================================
  print_header("4. Recursive Reasoning");

  // Start reasoning chains
  std::vector<std::string> chain_inputs = {cat_id, animal_id};
  std::string chain1 = kernel.start_reasoning(chain_inputs);
  std::cout << "Started reasoning chain: " << chain1 << std::endl;

  chain_inputs = {mammal_id, pet_id};
  std::string chain2 = kernel.start_reasoning(chain_inputs);
  std::cout << "Started reasoning chain: " << chain2 << std::endl;

  // Execute reasoning steps
  std::cout << "\nExecuting reasoning steps..." << std::endl;
  for (int i = 0; i < 5; i++) {
    auto stats = kernel.execute_reasoning_step();
    std::cout << "  Step " << (i + 1) << ": " << stats.total_inferences
              << " inferences, " << stats.active_chains << " active chains"
              << std::endl;
  }

  // ================================================================
  // 5. Run Cognitive Processing Cycles
  // ================================================================
  print_header("5. Cognitive Processing");

  std::cout << "Running 10 cognitive cycles..." << std::endl;
  auto metrics = kernel.run_cycles(10);

  print_metrics(metrics);

  // ================================================================
  // 6. Examine Top Attention Atoms
  // ================================================================
  print_header("6. Attention Distribution");

  auto top_atoms = kernel.get_top_attention_atoms(5);
  std::cout << "Top 5 atoms by attention:" << std::endl;
  for (size_t i = 0; i < top_atoms.size(); i++) {
    const auto *atom = kernel.get_atom(top_atoms[i]);
    if (atom) {
      std::cout << "  " << (i + 1) << ". " << atom->name
                << " (STI: " << atom->attention_value.sti << ")" << std::endl;
    }
  }

  // ================================================================
  // 7. Examine Time Crystal States
  // ================================================================
  print_header("7. Time Crystal States");

  auto *tc_kernel = kernel.get_time_crystal_kernel();
  if (tc_kernel) {
    auto crystals = tc_kernel->get_all_atom_ids();
    std::cout << "Time crystal states for first 3 atoms:" << std::endl;

    int count = 0;
    for (const auto &id : crystals) {
      if (count >= 3)
        break;

      const auto *state = tc_kernel->get_time_crystal_state(id);
      const auto *atom = tc_kernel->get_atom(id);

      if (state && atom) {
        std::cout << "\n  " << atom->name << ":" << std::endl;
        std::cout << "    Temporal Coherence: " << state->temporal_coherence
                  << std::endl;
        std::cout << "    Fractal Dimension: " << state->fractal_dimension
                  << std::endl;
        std::cout << "    Resonance Freq: " << state->resonance_frequency
                  << " Hz" << std::endl;
        std::cout << "    Quantum Phase: " << state->quantum_phase << " rad"
                  << std::endl;
        std::cout << "    11D Coords: [";
        for (int d = 0; d < 3; d++) {
          std::cout << state->dimensions[d] << ", ";
        }
        std::cout << "...]" << std::endl;
        count++;
      }
    }
  }

  // ================================================================
  // 8. Meta-Cognitive Status
  // ================================================================
  print_header("8. Meta-Cognitive Status");

  auto *meta_engine = kernel.get_metacognitive_engine();
  if (meta_engine) {
    auto meta_tensors = meta_engine->get_meta_tensors();
    std::cout << "Meta-cognitive levels: " << meta_tensors.size() << std::endl;

    for (const auto &mt : meta_tensors) {
      std::cout << "  Level " << mt.level << ": coherence = " << mt.coherence
                << std::endl;
    }

    auto loops = meta_engine->get_feedback_loops();
    std::cout << "\nFeedback loops: " << loops.size() << std::endl;
    for (const auto &loop : loops) {
      std::cout << "  " << loop.source_component << " -> "
                << loop.target_component << " (" << loop.loop_type
                << ", stability: " << loop.stability_measure << ")"
                << std::endl;
    }

    auto mods = meta_engine->get_self_modifications();
    std::cout << "\nSelf-modifications: " << mods.size() << std::endl;
  }

  // ================================================================
  // 9. Run More Cycles and Compare
  // ================================================================
  print_header("9. Extended Processing");

  std::cout << "Running 50 more cycles..." << std::endl;
  metrics = kernel.run_cycles(50);

  std::cout << "\nAfter 60 total cycles:" << std::endl;
  std::cout << "  Total Inferences: " << metrics.total_inferences << std::endl;
  std::cout << "  Consciousness Emergence: " << metrics.consciousness_emergence
            << std::endl;
  std::cout << "  Self-Awareness: " << metrics.self_awareness_level
            << std::endl;
  std::cout << "  System Coherence: " << metrics.system_coherence << std::endl;

  // ================================================================
  // 10. Shutdown
  // ================================================================
  print_header("10. Shutdown");

  std::cout << "Final cycle count: " << kernel.get_cycle_count() << std::endl;
  kernel.shutdown();

  std::cout << "\n\n========================================" << std::endl;
  std::cout << " NanoBrain Demo Complete" << std::endl;
  std::cout << "========================================" << std::endl;

  return 0;
}
