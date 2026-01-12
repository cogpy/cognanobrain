/**
 * NanoBrain Singularity Geometry Demo
 *
 * Chapter 6: Harvesting Singularity Geometry
 * Demonstrates Phase Prime Metric in tubulin proteins
 *
 * Features:
 * - Tubulin PPM structure modeling
 * - Triplet-of-triplet resonance cascade
 * - Cellular automaton parallel computing
 * - Neuron time crystal mapping
 * - Hidden filament communication
 */

#include "nanobrain_singularity.h"
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

void demo_tubulin_ppm(TimeCrystalKernel &tc_kernel, NanoBrainKernel *kernel) {
  print_separator("Task 6.1: Tubulin PPM Structure Demo");

  TubulinPPMModel tubulin(kernel, &tc_kernel);
  tubulin.initialize(8); // 8 rings (4 dimers)

  std::cout << std::fixed << std::setprecision(4);

  // Display ring structure
  std::cout << "  Tubulin Microtubule Structure:\n";
  std::cout << "    Protofilaments: " << TUBULIN_PROTOFILAMENTS << "\n";
  std::cout << "    Rings: 8 (α/β alternating)\n\n";

  std::cout << "  Ring Structure:\n";
  for (int i = 0; i < 4; i++) {
    const auto *ring = tubulin.get_ring(i);
    if (ring) {
      std::cout << "    " << tubulin_ring_to_string(*ring) << "\n";
    }
  }

  // Water channel
  const auto &channel = tubulin.get_water_channel();
  std::cout << "\n  Water Channel:\n";
  std::cout << "    Diameter: " << channel.diameter_nm << " nm\n";
  std::cout << "    Length: " << channel.length_nm << " nm\n";
  std::cout << "    Quantum Coherence: " << channel.quantum_coherence << "\n";
  std::cout << "    Coherence Zones: " << channel.coherence_zones.size()
            << "\n";

  // Ring resonance
  std::cout << "\n  Inter-ring Resonance:\n";
  for (int i = 0; i < 3; i++) {
    float resonance = tubulin.calculate_ring_resonance(i, i + 1);
    std::cout << "    Ring " << i << " <-> Ring " << (i + 1) << ": "
              << resonance << "\n";
  }

  // Hyperspace symmetry
  bool symmetry_valid = tubulin.verify_hyperspace_symmetry();
  std::cout << "\n  Hyperspace Symmetry: "
            << (symmetry_valid ? "VALID" : "INVALID") << "\n";
  std::cout << "  Structure Coherence: "
            << tubulin.calculate_structure_coherence() << "\n";

  // PPM signature
  auto ppm = tubulin.get_structure_ppm();
  std::cout << "  Structure PPM: [";
  for (size_t i = 0; i < ppm.size(); i++) {
    std::cout << ppm[i] << (i < ppm.size() - 1 ? ", " : "");
  }
  std::cout << "]\n";
}

void demo_triplet_resonance(NanoBrainKernel *kernel) {
  print_separator("Task 6.2: Triplet Resonance Cascade Demo");

  TripletResonanceCascade cascade(kernel);
  cascade.initialize(3); // Depth 3

  std::cout << std::fixed << std::setprecision(2);

  const auto *root = cascade.get_root();
  if (root) {
    std::cout << "  Root: " << triplet_resonance_to_string(*root) << "\n\n";
  }

  // Simulate input cascade
  std::cout << "  Cascading Input Signal:\n";
  std::array<float, 3> input = {440.0f, 550.0f, 660.0f}; // C major chord

  for (int step = 0; step < 5; step++) {
    cascade.cascade_update(input);
    float energy = cascade.calculate_cascade_energy();

    std::cout << "    Step " << (step + 1) << ": Energy=" << energy;

    auto harmonics = cascade.get_band_harmonics(0);
    if (harmonics.size() >= 3) {
      std::cout << " Bands=[" << harmonics[0] << ", " << harmonics[1] << ", "
                << harmonics[2] << "]";
    }
    std::cout << "\n";

    // Dampen input for next step
    for (auto &v : input) {
      v *= 0.9f;
    }
  }

  // Cross-level resonance
  std::cout << "\n  Cross-Level Resonance:\n";
  for (int l1 = 0; l1 < 2; l1++) {
    for (int l2 = l1 + 1; l2 < 3; l2++) {
      float resonance = cascade.calculate_cross_level_resonance(l1, l2);
      std::cout << "    Level " << l1 << " <-> Level " << l2 << ": "
                << resonance << "\n";
    }
  }

  // Apply PPM weighting
  cascade.apply_ppm_weighting({2, 3, 5, 7});
  std::cout << "\n  After PPM Weighting:\n";
  std::cout << "    Final Energy: " << cascade.calculate_cascade_energy()
            << "\n";
}

void demo_cellular_automaton(NanoBrainKernel *kernel) {
  print_separator("Task 6.3: Cellular Automaton Demo");

  CellularAutomatonEngine ca(kernel);
  ca.initialize(64, 32); // Smaller for demo

  std::cout << std::fixed << std::setprecision(4);
  std::cout << "  Grid: 64x32 cells\n\n";

  // Test different rules
  std::vector<int> rules = {30, 110, 150};

  for (int rule : rules) {
    ca.set_rule(rule);
    ca.randomize(0.5f);

    std::cout << "  Rule " << rule << ":\n";
    std::cout << "    Initial Density: " << ca.calculate_density() << "\n";

    ca.run(50); // 50 generations

    std::cout << "    After 50 generations:\n";
    std::cout << "      Density: " << ca.calculate_density() << "\n";
    std::cout << "      Entropy: " << ca.calculate_entropy() << "\n";

    auto patterns = ca.detect_patterns();
    std::cout << "      Patterns found: " << patterns.size() << "\n\n";
  }

  // PPM rules
  std::cout << "  PPM-Derived Rules:\n";
  ca.apply_ppm_rules({2, 3, 5, 7}); // PPM from first 4 primes
  std::cout << "    Rule from PPM [2,3,5,7]: " << ca.get_state().rule_name
            << "\n";
  ca.randomize(0.5f);
  ca.run(50);
  std::cout << "    Density after 50 gen: " << ca.calculate_density() << "\n";
  std::cout << "    Entropy: " << ca.calculate_entropy() << "\n";
}

void demo_neuron_mapping(TimeCrystalKernel &tc_kernel,
                         NanoBrainKernel *kernel) {
  print_separator("Task 6.4: Neuron Time Crystal Map Demo");

  NeuronTimeCrystalMapper mapper(kernel, &tc_kernel);

  // Create a neuron
  std::string neuron_id = mapper.create_neuron("PyramidalCell_1");

  // Build full hierarchy
  mapper.create_neuron_crystal_hierarchy(neuron_id);

  const auto *neuron = mapper.get_neuron(neuron_id);
  if (neuron) {
    std::cout << std::fixed << std::setprecision(4);

    std::cout << "  Neuron: " << neuron_id << "\n\n";

    // Soma
    std::cout << "  Soma:\n";
    std::cout << "    " << neuron_segment_to_string(neuron->soma) << "\n";
    std::cout << "    Coherence: "
              << neuron->soma.time_crystal_state.temporal_coherence << "\n\n";

    // Axon summary
    std::cout << "  Axon Segments: " << neuron->axon_segments.size() << "\n";
    if (!neuron->axon_segments.empty()) {
      std::cout << "    First: "
                << neuron_segment_to_string(neuron->axon_segments[0]) << "\n";
      std::cout << "    Last: "
                << neuron_segment_to_string(neuron->axon_segments.back())
                << "\n";
    }

    // Dendrite summary
    std::cout << "\n  Dendrite Segments: " << neuron->dendrite_segments.size()
              << "\n";
    if (!neuron->dendrite_segments.empty()) {
      std::cout << "    First: "
                << neuron_segment_to_string(neuron->dendrite_segments[0])
                << "\n";
    }

    // Scale hierarchy
    std::cout << "\n  Scale Hierarchy:\n";
    for (const auto &[level, segments] : neuron->scale_hierarchy) {
      std::cout << "    Level " << level << ": " << segments.size()
                << " segments\n";
    }

    // Sample triplet bands
    std::cout << "\n  Sample Triplet Bands:\n";
    int sample_count = 0;
    for (const auto &seg : neuron->axon_segments) {
      if (sample_count++ >= 3)
        break;
      auto bands = mapper.get_segment_bands(seg.segment_id);
      std::cout << "    " << seg.segment_id << ": [" << bands[0] << ", "
                << bands[1] << ", " << bands[2] << "] Hz\n";
    }
  }
}

void demo_filament_communication(TimeCrystalKernel &tc_kernel,
                                 NanoBrainKernel *kernel) {
  print_separator("Task 6.5: Hidden Filament Communication Demo");

  // Create attention engine for integration
  AttentionAllocationConfig att_config;
  att_config.mechanism = AttentionMechanism::Hybrid;
  AttentionAllocationEngine attention(kernel, att_config);
  attention.initialize(100);

  FilamentCommunicator filament(kernel, &attention);
  filament.initialize(500);

  std::cout << std::fixed << std::setprecision(4);

  // Create some test nodes
  std::vector<NodeTensor> node_storage(5);
  std::vector<NodeTensor *> nodes;

  for (int i = 0; i < 5; i++) {
    node_storage[i].id = "node_" + std::to_string(i);
    node_storage[i].atom_id = "filament_" + std::to_string(i);
    node_storage[i].embedding = kernel->create_tensor({128});
    node_storage[i].attention_weights = kernel->create_tensor({1});

    // Simulate increasing attention (pre-spike pattern)
    std::vector<float> att_data = {50.0f + i * 30.0f};
    kernel->set_data(node_storage[i].attention_weights, att_data);

    nodes.push_back(&node_storage[i]);
  }

  std::cout << "  Simulating filament network with 5 nodes\n\n";

  // Encode hidden signals
  std::cout << "  Encoding Hidden Signals:\n";
  for (int i = 0; i < 3; i++) {
    float strength = 0.3f + i * 0.3f;
    auto signal = filament.encode_hidden_signal(
        "source_" + std::to_string(i), "target_" + std::to_string(i), strength);

    std::cout << "    Signal " << i << ": strength=" << signal.signal_strength
              << " primes=[";
    for (size_t j = 0; j < signal.prime_encoding.size(); j++) {
      std::cout << signal.prime_encoding[j]
                << (j < signal.prime_encoding.size() - 1 ? "," : "");
    }
    std::cout << "]\n";

    float decoded = filament.decode_hidden_signal(signal);
    std::cout << "      Decoded: " << decoded << " (PPM modulated)\n";
  }

  // Pre-spike detection simulation
  std::cout << "\n  Pre-Spike Detection:\n";
  for (int i = 0; i < 5; i++) {
    // Increase attention to trigger pre-spike
    kernel->compute(nodes[i]->attention_weights);
    float current = kernel->get_value(nodes[i]->attention_weights, 0);

    // Simulate by directly encoding (since we can't change attention easily)
    float sim_strength = current / 100.0f;
    if (sim_strength > 0.7f) {
      std::cout << "    Node " << nodes[i]->atom_id
                << ": PRE-SPIKE detected (strength=" << sim_strength << ")\n";

      float timing = filament.predict_spike_timing(nodes[i]->atom_id);
      std::cout << "      Predicted timing: " << timing << " ms\n";
    } else {
      std::cout << "    Node " << nodes[i]->atom_id
                << ": No spike (strength=" << sim_strength << ")\n";
    }
  }

  // Statistics
  std::cout << "\n  Signal Statistics:\n";
  std::cout << "    Mean Strength: " << filament.get_mean_signal_strength()
            << "\n";
  std::cout << "    Variance: " << filament.get_signal_variance() << "\n";
}

int main() {
  std::cout << R"(
  ╔══════════════════════════════════════════════════════════════════════════╗
  ║                                                                          ║
  ║       NanoBrain Singularity Geometry Demo - Chapter 6                    ║
  ║                                                                          ║
  ║    Harvesting Singularity Geometry:                                      ║
  ║    Phase Prime Metric in Tubulin Proteins                                ║
  ║                                                                          ║
  ╚══════════════════════════════════════════════════════════════════════════╝
)" << std::endl;

  // Initialize Time Crystal Kernel
  TimeCrystalConfig config;
  config.memory_size = 1024 * 1024 * 128; // 128 MB
  config.pln_inference_depth = 5;
  config.temporal_processing_frequency = 60.0f;

  std::cout << "  Initializing Time Crystal Kernel...\n";
  TimeCrystalKernel tc_kernel(config);
  tc_kernel.initialize();

  NanoBrainKernel *kernel = tc_kernel.get_tensor_kernel();

  // Run all demos
  demo_tubulin_ppm(tc_kernel, kernel);
  demo_triplet_resonance(kernel);
  demo_cellular_automaton(kernel);
  demo_neuron_mapping(tc_kernel, kernel);
  demo_filament_communication(tc_kernel, kernel);

  print_separator("Demo Complete");
  std::cout << "\n  All Chapter 6 features demonstrated successfully!\n";
  std::cout << "  Singularity geometry models tubulin proteins with PPM.\n\n";

  tc_kernel.shutdown();
  return 0;
}
