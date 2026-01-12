#include "nanobrain_attention.h"
#include "nanobrain_dodecanion.h"
#include "nanobrain_encoder.h"
#include "nanobrain_fractal.h"
#include "nanobrain_hinductor.h"
#include "nanobrain_kernel.h"
#include "nanobrain_metacognitive.h"
#include "nanobrain_reasoning.h"
#include "nanobrain_time_crystal.h"
#include "nanobrain_unified.h"
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

  // ================================================================
  // Part 8: Meta-Cognitive Feedback Engine
  // ================================================================
  std::cout << "\n--- Part 8: Meta-Cognitive Feedback Engine ---" << std::endl;

  MetaCognitiveConfig meta_config;
  meta_config.meta_levels = 3;
  meta_config.adaptation_learning_rate = 0.01f;
  meta_config.feedback_damping = 0.9f;
  meta_config.membrane_permeability = 0.5f;

  MetaCognitiveFeedbackEngine metacog(&kernel, &attention, &reasoning,
                                      meta_config);
  metacog.initialize();

  std::cout << "Meta-Cognitive Engine Initialized:" << std::endl;
  std::cout << "  Meta Levels: " << meta_config.meta_levels << std::endl;
  std::cout << "  Learning Rate: " << meta_config.adaptation_learning_rate
            << std::endl;
  std::cout << "  Feedback Damping: " << meta_config.feedback_damping
            << std::endl;

  // Update meta-cognitive state
  MetaCognitiveMetrics meta_metrics = metacog.update_meta_cognitive(
      node_tensors, link_tensors, attention_stats, stats);

  std::cout << "\nMeta-Cognitive Metrics:" << std::endl;
  std::cout << "  Self-Awareness Level: " << meta_metrics.self_awareness_level
            << std::endl;
  std::cout << "  Adaptation Rate: " << meta_metrics.adaptation_rate
            << std::endl;
  std::cout << "  System Coherence: " << meta_metrics.system_coherence
            << std::endl;
  std::cout << "  Feedback Stability: " << meta_metrics.feedback_loop_stability
            << std::endl;
  std::cout << "  Convergence Rate: " << meta_metrics.convergence_rate
            << std::endl;

  // Get feedback loops
  auto feedback_loops = metacog.get_feedback_loops();
  std::cout << "\nFeedback Loops: " << feedback_loops.size() << std::endl;
  for (const auto &loop : feedback_loops) {
    std::cout << "  " << loop.source_component << " -> "
              << loop.target_component << " (" << loop.loop_type
              << ", stability: " << loop.stability_measure << ")" << std::endl;
  }

  // Get meta-cognitive tensors
  auto meta_tensors = metacog.get_meta_tensors();
  std::cout << "\nMeta-Cognitive Tensors: " << meta_tensors.size() << " levels"
            << std::endl;
  for (const auto &mt : meta_tensors) {
    std::cout << "  Level " << mt.level << ": coherence = " << mt.coherence
              << std::endl;
  }

  // ================================================================
  // Part 9: Unified NanoBrain Kernel
  // ================================================================
  std::cout << "\n--- Part 9: Unified NanoBrain Kernel ---" << std::endl;

  UnifiedNanoBrainConfig unified_config;
  unified_config.memory_size = 1024 * 1024 * 64; // 64 MB
  unified_config.time_crystal_dimensions = 11;
  unified_config.max_reasoning_depth = 5;
  unified_config.attention_mechanism = AttentionMechanism::Hybrid;
  unified_config.meta_levels = 3;

  UnifiedNanoBrainKernel unified_kernel(unified_config);
  unified_kernel.initialize();

  std::cout << "Unified NanoBrain Kernel Initialized" << std::endl;
  std::cout << "  Version: " << get_nanobrain_version() << std::endl;
  std::cout << "  Active: " << (unified_kernel.is_active() ? "Yes" : "No")
            << std::endl;

  // Create some atoms
  std::string cat_id =
      unified_kernel.create_atom("ConceptNode", "Cat", 0.9f, 0.8f, {2, 3, 5});
  std::string dog_id =
      unified_kernel.create_atom("ConceptNode", "Dog", 0.85f, 0.75f, {2, 3, 7});
  std::string pet_id =
      unified_kernel.create_atom("ConceptNode", "Pet", 0.8f, 0.7f, {3, 5, 7});

  std::cout << "\nCreated atoms:" << std::endl;
  std::cout << "  Cat: " << cat_id << std::endl;
  std::cout << "  Dog: " << dog_id << std::endl;
  std::cout << "  Pet: " << pet_id << std::endl;

  // Run processing cycles
  std::cout << "\nRunning 5 cognitive cycles..." << std::endl;
  UnifiedNanoBrainMetrics unified_metrics = unified_kernel.run_cycles(5);

  std::cout << "Unified Kernel Metrics:" << std::endl;
  std::cout << "  Total Atoms: " << unified_metrics.total_atoms << std::endl;
  std::cout << "  Total Links: " << unified_metrics.total_links << std::endl;
  std::cout << "  Quantum Coherence: " << unified_metrics.quantum_coherence
            << std::endl;
  std::cout << "  Temporal Stability: " << unified_metrics.temporal_stability
            << std::endl;
  std::cout << "  Prime Alignment: " << unified_metrics.prime_alignment
            << std::endl;
  std::cout << "  Consciousness: " << unified_metrics.consciousness_emergence
            << std::endl;
  std::cout << "  Self-Awareness: " << unified_metrics.self_awareness_level
            << std::endl;

  // Test coherence computation
  float coherence = unified_kernel.compute_coherence({2, 3, 5, 7, 11});
  std::cout << "\nPPM Coherence {2,3,5,7,11}: " << coherence << std::endl;

  // Shutdown
  unified_kernel.shutdown();
  std::cout << "Unified kernel shutdown complete." << std::endl;

  // ================================================================
  // Part 8: Hinductor & Magnetic Light (Chapter 8)
  // ================================================================
  std::cout << "\n--- Part 8: Hinductor & Magnetic Light ---" << std::endl;

  // Test Hinductor vortex resistance
  HinductorConfig hinductor_config;
  hinductor_config.resistance_base = 1000.0f;
  hinductor_config.oscillation_frequency = 1.0e9f;
  hinductor_config.enable_hysteresis = true;

  Hinductor hinductor(&kernel, hinductor_config);

  std::cout << "Hinductor initialized with base resistance: "
            << hinductor_config.resistance_base << " Ω" << std::endl;

  // Test vortex resistance at different flux values
  for (float flux = 0.0f; flux <= 2.0f; flux += 0.5f) {
    float resistance = hinductor.vortex_resistance(flux);
    std::cout << "  Flux=" << flux << " → Resistance=" << resistance << " Ω"
              << std::endl;
  }

  // Test capacitance oscillation
  hinductor.oscillate_capacitance(0.001f);
  std::cout << "  Oscillating capacitance: " << hinductor.get_capacitance()
            << " F" << std::endl;

  // Test magnetic light generation
  hinductor.generate_magnetic_light();
  std::cout << "  Light intensity: " << hinductor.get_state().light_intensity
            << std::endl;

  // Test Magnetic Knot Generator
  std::cout << "\n  Testing Magnetic Knot Generator..." << std::endl;

  KnotGeneratorConfig knot_config;
  knot_config.resolution = 128;
  knot_config.compute_field = false; // Skip field computation for speed

  MagneticKnotGenerator knot_gen(&kernel, knot_config);

  // Generate different knot types
  auto trefoil = knot_gen.generate_spin_knot(KnotType::Trefoil);
  auto figure8 = knot_gen.generate_spin_knot(KnotType::FigureEight);
  auto cinquefoil = knot_gen.generate_spin_knot(KnotType::Cinquefoil);

  std::cout << "    " << trefoil.name
            << ": crossings=" << trefoil.crossing_number
            << ", length=" << trefoil.rope_length << std::endl;
  std::cout << "    " << figure8.name
            << ": crossings=" << figure8.crossing_number
            << ", writhe=" << figure8.writhe << std::endl;
  std::cout << "    " << cinquefoil.name
            << ": crossings=" << cinquefoil.crossing_number << std::endl;

  // Create vortex atom from knot
  VortexAtom vortex = knot_gen.create_vortex_atom(trefoil);
  std::cout << "    Vortex atom: circulation=" << vortex.circulation
            << ", tc_coherence=" << vortex.tc_coherence << std::endl;

  // Test super-coil synthesis
  auto super_coil =
      knot_gen.synthesize_super_coil({trefoil, figure8, cinquefoil});
  std::cout << "    " << super_coil.name
            << ": total crossings=" << super_coil.crossing_number << std::endl;

  // Test Tomasch Oscillator
  std::cout << "\n  Testing Tomasch Oscillator..." << std::endl;

  TomaschConfig tomasch_config;
  tomasch_config.harvest_efficiency = 0.3f;

  TomaschOscillator tomasch(&kernel, tomasch_config);

  // Harvest noise energy
  float noise_levels[] = {0.1f, 0.5f, 1.0f, 2.0f};
  for (float noise : noise_levels) {
    float harvested = tomasch.harvest_noise_energy(noise);
    std::cout << "    Noise=" << noise << " → Harvested=" << harvested
              << ", SNR=" << tomasch.get_signal_to_noise() << std::endl;
  }

  // Process thermal waves
  tomasch.process_thermal_waves(300.0f); // Room temperature
  std::cout << "    Total harvested energy: " << tomasch.get_harvested_energy()
            << std::endl;

  // Test Phase Space Dynamics
  std::cout << "\n  Testing Phase Space Dynamics (12 holes)..." << std::endl;

  PhaseSpaceConfig phase_config;
  phase_config.coupling_strength = 0.1f;
  phase_config.damping = 0.01f;

  PhaseSpaceDynamics phase_space(&kernel, phase_config);

  std::cout << "    Initial state: " << phase_space.visualize_state()
            << std::endl;
  std::cout << "    Active holes: " << phase_space.count_active_holes()
            << ", Resonant: " << (phase_space.is_resonant() ? "Yes" : "No")
            << std::endl;

  // Evolve phase space
  for (int step = 0; step < 5; step++) {
    phase_space.update_hole_states(0.01f);
  }

  std::cout << "    After evolution: " << phase_space.visualize_state()
            << std::endl;
  std::cout << "    Oscillation phase: " << phase_space.get_oscillation_phase()
            << std::endl;
  std::cout << "    Pattern coherence: "
            << phase_space.compute_pattern_coherence() << std::endl;

  // Test Magnonic Bridge
  std::cout << "\n  Testing Magnonic Bridge..." << std::endl;

  MagnonicBridgeConfig magnon_config;
  magnon_config.transition_efficiency = 0.85f;
  magnon_config.carrier_frequency = 10.0e9f;

  MagnonicBridge magnon_bridge(&kernel, magnon_config);

  // Test electron to magnon transition
  MagnonPacket magnon = magnon_bridge.transition_electron_to_magnon(1.5f, 0.8f);
  std::cout << "    Electron→Magnon: type="
            << magnon_type_to_string(magnon.type)
            << ", freq=" << magnon.frequency / 1.0e12f << " THz"
            << ", amp=" << magnon.amplitude << std::endl;

  // Test reverse transition
  float electron_energy = magnon_bridge.transition_magnon_to_electron(magnon);
  std::cout << "    Magnon→Electron: energy=" << electron_energy
            << ", efficiency=" << magnon_bridge.get_transition_efficiency()
            << std::endl;

  // Create and split magnons
  auto acoustic_magnon =
      magnon_bridge.create_magnon(MagnonType::Acoustic, 1.0e9f, 1.0f);
  auto split_magnons = magnon_bridge.split_magnon(acoustic_magnon, 4);
  std::cout << "    Split magnon into " << split_magnons.size() << " parts"
            << std::endl;

  // Setup wireless (just activates)
  magnon_bridge.setup_wireless_h_device();
  std::cout << "    Wireless H-device active: "
            << (magnon_bridge.is_wireless_active() ? "Yes" : "No") << std::endl;

  std::cout << "\n  Hinductor & Magnetic Light tests complete!" << std::endl;

  // ================================================================
  // Part 10: Dodecanion Algebra (Chapter 4)
  // ================================================================
  std::cout << "\n--- Part 10: Dodecanion Algebra ---" << std::endl;

  // Create unit dodecanion from quaternion
  Dodecanion d1 = Dodecanion::from_quaternion(1.0f, 0.0f, 0.0f, 0.0f);
  std::cout << "  d1 = Quaternion(1,0,0,0): " << d1.to_string() << std::endl;
  std::cout << "  Norm: " << d1.norm() << std::endl;

  // Create from octonion
  std::array<float, 8> oct = {1.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
  Dodecanion d2 = Dodecanion::from_octonion(oct);
  std::cout << "  d2 = Octonion(1,0.5,...): Norm = " << d2.norm() << std::endl;

  // Dodecanion multiplication
  Dodecanion d3 = d1 * d2;
  std::cout << "  d1 * d2: Norm = " << d3.norm() << std::endl;

  // Test conjugate
  Dodecanion d1_conj = d1.conjugate();
  std::cout << "  d1.conjugate(): " << d1_conj.to_string() << std::endl;

  // Test inverse
  Dodecanion d2_inv = d2.inverse();
  Dodecanion identity = d2 * d2_inv;
  std::cout << "  d2 * d2.inverse() ≈ 1: Real part = " << identity.real()
            << std::endl;

  // Project to 11D manifold
  auto coords_11d = d2.project_to_11d();
  std::cout << "  11D projection: [" << coords_11d[0] << ", " << coords_11d[1]
            << ", " << coords_11d[2] << ", ...]" << std::endl;

  // Convert to tensor
  DodecanionAlgebra dod_algebra(&kernel);
  std::vector<Dodecanion> batch = {d1, d2, d3};
  auto *batch_tensor = dod_algebra.to_batch_tensor(batch);
  std::cout << "  Batch tensor created for 3 dodecanions" << std::endl;

  std::cout << "\n  Dodecanion tests complete!" << std::endl;

  // ================================================================
  // Part 11: Fractal Mechanics (Chapter 4)
  // ================================================================
  std::cout << "\n--- Part 11: Fractal Mechanics ---" << std::endl;

  // Test CFGA 13-operation engine
  CFGAOperator cfga(&kernel);
  std::cout << "  CFGA Operator initialized with 13 operations" << std::endl;

  auto *test_input = kernel.create_tensor({10});
  std::vector<float> test_data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  kernel.set_data(test_input, test_data);

  auto *diff_result = cfga.diff(test_input);
  kernel.compute(diff_result);
  std::cout << "  Diff operation: first value = "
            << kernel.get_value(diff_result, 0) << std::endl;

  auto *int_result = cfga.integrate(test_input);
  kernel.compute(int_result);
  std::cout << "  Integrate operation: last value = "
            << kernel.get_value(int_result, 9) << std::endl;

  // Test Fractal Harmonic Oscillator
  FractalHarmonicOscillator fho(&kernel, 11);
  fho.initialize(
      {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f},
      {0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f});
  std::cout << "  FractalHarmonicOscillator initialized (11D)" << std::endl;

  // Evolve for several steps
  for (int i = 0; i < 10; i++) {
    fho.evolve(0.1f);
  }
  auto fho_state = fho.get_state();
  std::cout << "  After 10 steps: Energy = " << fho_state.energy
            << ", Phase = " << fho_state.phase << std::endl;

  // Harvest singularities
  auto singularities = fho.harvest_singularities();
  std::cout << "  Singularities harvested: " << singularities.size()
            << " (total energy: " << fho.get_harvested_energy() << ")"
            << std::endl;

  // Test Fractal Interference
  FractalInterference interference(&kernel);
  interference.simulate_microtubule(100);
  auto tubulin = interference.get_tubulin_states();
  std::cout << "  Microtubule simulation: " << tubulin.size() << " tubulins"
            << std::endl;

  auto comparison = interference.compare_models("default");
  std::cout << "  Model comparison - Better: " << comparison.better_model
            << " (fractal coherence: " << comparison.fractal_coherence << ")"
            << std::endl;

  // Test Regulatory Equations
  RegulatoryEquations reg(&kernel);
  auto proj_result = reg.project_from_infinity(test_input);
  std::cout << "  Projection from infinity: factor = "
            << proj_result.projection_factor << std::endl;

  auto *full_reg = reg.apply_full_regulation(test_input);
  kernel.compute(full_reg);
  std::cout << "  Full 12-equation regulation applied" << std::endl;

  std::cout << "\n  Fractal Mechanics tests complete!" << std::endl;

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
