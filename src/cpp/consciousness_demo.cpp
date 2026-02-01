#include "nanobrain_consciousness.h"
#include "nanobrain_unified.h"
#include <iomanip>
#include <iostream>

/**
 * Consciousness Demo Program
 *
 * Demonstrates the Chapter 10 consciousness uploading features:
 * - Consciousness state capture and restore
 * - 11D paper folding operations
 * - Brain evolution simulation
 * - Future machine interface usage
 * - Consciousness test suite execution
 */

void print_header(const std::string &title) {
  std::cout << "\n" << std::string(60, '=') << "\n";
  std::cout << title << "\n";
  std::cout << std::string(60, '=') << "\n";
}

void print_section(const std::string &title) {
  std::cout << "\n--- " << title << " ---\n";
}

void demo_consciousness_upload(TimeCrystalKernel *tc_kernel,
                               MetaCognitiveFeedbackEngine *meta_engine) {
  print_header("Consciousness Upload Protocol Demo");

  ConsciousnessUploader uploader(tc_kernel, meta_engine);

  // Check readiness
  print_section("Upload Readiness Check");
  std::cout << "Upload Ready: " << (uploader.is_upload_ready() ? "Yes" : "No")
            << "\n";
  std::cout << "Readiness Score: " << std::fixed << std::setprecision(4)
            << uploader.get_readiness_score() << "\n";

  // Capture state
  print_section("Capturing Consciousness State");
  ConsciousnessState state = uploader.capture_state();

  std::cout << "State ID: " << state.id << "\n";
  std::cout << "Timestamp: " << state.timestamp << "\n";
  std::cout << "Consciousness Level: " << state.consciousness_level << "\n";
  std::cout << "Self Awareness: " << state.self_awareness << "\n";
  std::cout << "Temporal Coherence: " << state.temporal_coherence << "\n";
  std::cout << "Meta Stability: " << state.meta_stability << "\n";
  std::cout << "Status: " << consciousness_status_to_string(state.status)
            << "\n";

  // Show 11D snapshot
  std::cout << "\n11D Time Crystal Snapshot:\n";
  for (int i = 0; i < CONSCIOUSNESS_DIMENSIONS; ++i) {
    std::cout << "  D" << std::setw(2) << i << ": " << std::setw(8)
              << state.time_crystal_snapshot[i] << "\n";
  }

  // Validate state
  print_section("State Validation");
  bool valid = uploader.validate_state(state);
  std::cout << "State Valid: " << (valid ? "Yes" : "No") << "\n";
  std::cout << "Checksum: " << state.checksum << "\n";

  // Serialize to time crystal
  print_section("Serializing to Time Crystal");
  uploader.serialize_to_time_crystal(state);
  std::cout << "Serialization complete\n";
  std::cout << "Status: "
            << consciousness_status_to_string(uploader.get_status()) << "\n";

  // Restore from state
  print_section("Restoring Consciousness");
  bool restored = uploader.restore_to_substrate(state);
  std::cout << "Restoration " << (restored ? "Successful" : "Failed") << "\n";
}

void demo_dimensional_folding(NanoBrainKernel *kernel) {
  print_header("11D Paper Folding Algorithm Demo");

  DimensionalFolder folder(kernel);

  // Create initial 11D state
  std::vector<float> initial_state(CONSCIOUSNESS_DIMENSIONS);
  for (int i = 0; i < CONSCIOUSNESS_DIMENSIONS; ++i) {
    initial_state[i] = std::sin(2.0f * M_PI * i / CONSCIOUSNESS_DIMENSIONS);
  }

  std::cout << "Initial 11D State:\n";
  for (int i = 0; i < CONSCIOUSNESS_DIMENSIONS; ++i) {
    std::cout << "  D" << std::setw(2) << i << ": " << std::setw(10)
              << std::fixed << std::setprecision(4) << initial_state[i] << "\n";
  }

  // Project to various dimensions
  print_section("Dimensional Projections");

  for (int target_dim = 3; target_dim <= 7; target_dim += 2) {
    auto projected = folder.project_to_dimension(initial_state, target_dim);
    std::cout << "\nProjected to " << target_dim << "D:\n";
    for (size_t i = 0; i < projected.size(); ++i) {
      std::cout << "  [" << i << "]: " << projected[i] << "\n";
    }
  }

  // Unfold back to 11D
  print_section("Unfolding to 11D");

  auto projected_5d = folder.project_to_dimension(initial_state, 5);
  auto unfolded = folder.unfold_to_11d(projected_5d);

  std::cout << "Unfolded from 5D:\n";
  for (int i = 0; i < CONSCIOUSNESS_DIMENSIONS; ++i) {
    float diff = std::abs(initial_state[i] - unfolded[i]);
    std::cout << "  D" << std::setw(2) << i << ": " << std::setw(10)
              << unfolded[i] << "  (diff: " << std::setw(10) << diff << ")\n";
  }

  // Time wiring
  print_section("Time-Based Wiring");

  std::vector<float> state_to_wire = initial_state;
  folder.apply_time_wiring(state_to_wire);

  std::cout << "After time wiring:\n";
  for (int i = 0; i < CONSCIOUSNESS_DIMENSIONS; ++i) {
    std::cout << "  D" << std::setw(2) << i << ": " << std::setw(10)
              << state_to_wire[i] << "\n";
  }

  std::cout << "\nDimensional Coherence: " << folder.get_coherence() << "\n";
}

void demo_brain_evolution(TimeCrystalKernel *tc_kernel) {
  print_header("Brain Evolution Simulator Demo");

  BrainEvolutionSimulator evolution(tc_kernel);

  // Show initial state
  print_section("Initial Evolutionary State");
  EvolutionaryState state = evolution.get_current_state();
  std::cout << "Phase: " << evolutionary_phase_to_string(state.phase) << "\n";
  std::cout << "Fitness: " << state.fitness_score << "\n";
  std::cout << "Generation: " << state.generation << "\n";
  std::cout << "Wheel Position: " << state.wheel_position << "°\n";

  // Run evolution steps
  print_section("Running Evolution (20 steps)");

  for (int epoch = 0; epoch < 4; ++epoch) {
    float fitness = evolution.run_evolution(5);
    state = evolution.get_current_state();

    std::cout << "After " << std::setw(2) << (epoch + 1) * 5 << " steps: "
              << "Phase=" << std::setw(12)
              << evolutionary_phase_to_string(state.phase)
              << "  Fitness=" << std::setw(6) << std::fixed
              << std::setprecision(4) << fitness << "  Gen=" << std::setw(3)
              << state.generation << "  Wheel=" << std::setw(6)
              << state.wheel_position << "°\n";
  }

  // Numerology
  print_section("Consciousness Numerology");
  auto numbers = evolution.compute_consciousness_numbers();
  std::cout << "Consciousness Numbers: ";
  for (int num : numbers) {
    std::cout << num << " ";
  }
  std::cout << "\n";
  std::cout << "Numerological Coherence: "
            << evolution.get_numerological_coherence() << "\n";

  // Phase history
  print_section("Phase Transition History");
  state = evolution.get_current_state();
  std::cout << "Phase History: ";
  for (const auto &phase : state.phase_history) {
    std::cout << evolutionary_phase_to_string(phase) << " -> ";
  }
  std::cout << evolutionary_phase_to_string(state.phase) << " (current)\n";
}

void demo_future_interfaces(TimeCrystalKernel *tc_kernel,
                            ConsciousnessUploader *uploader) {
  print_header("Future Machine Interfaces Demo");

  // Diagnosis Interface
  print_section("Diagnosis Interface");
  DiagnosisInterface diagnosis(tc_kernel);

  std::cout << "Interface: " << diagnosis.get_name() << "\n";
  std::cout << "Ready: " << (diagnosis.is_ready() ? "Yes" : "No") << "\n";

  auto state = uploader->capture_state();
  std::string report = diagnosis.diagnose(state);
  std::cout << "\n" << report << "\n";

  // Decision Support Interface
  print_section("Decision Support Interface");
  DecisionSupportInterface decision(tc_kernel);

  std::vector<std::vector<float>> options = {
      {0.8f, 0.6f, 0.4f}, // Option A
      {0.5f, 0.9f, 0.3f}, // Option B
      {0.3f, 0.3f, 0.95f} // Option C
  };

  int recommended = decision.recommend_decision(options);
  std::cout << "Options presented: 3\n";
  std::cout << "Recommended option: " << (char)('A' + recommended) << "\n";
  std::cout << "Decision confidence: " << decision.get_decision_confidence()
            << "\n";

  // Creative Generation Interface
  print_section("Creative Generation Interface");
  CreativeGenerationInterface creative(tc_kernel);

  std::vector<float> seed(CONSCIOUSNESS_DIMENSIONS, 0.5f);

  std::cout << "Generating creative variations:\n";
  for (float creativity : {0.1f, 0.5f, 0.9f}) {
    auto output = creative.generate_creative(seed, creativity);
    float variance = 0.0f;
    for (float v : output) {
      variance += (v - 0.5f) * (v - 0.5f);
    }
    variance /= output.size();

    std::cout << "  Creativity=" << creativity << "  -> Variance=" << std::fixed
              << std::setprecision(4) << variance << "\n";
  }
}

void demo_test_suite(TimeCrystalKernel *tc_kernel,
                     ConsciousnessUploader *uploader,
                     BrainEvolutionSimulator *evolution) {
  print_header("Consciousness Test Suite Demo");

  ConsciousnessTestSuite test_suite(tc_kernel, uploader, evolution);

  // Run all tests
  print_section("Running All Tests");
  bool all_passed = test_suite.run_all_tests();

  auto metrics = test_suite.get_metrics();

  std::cout << "Total Tests: " << metrics.total_tests << "\n";
  std::cout << "Passed: " << metrics.passed_tests << "\n";
  std::cout << "Failed: " << metrics.failed_tests << "\n";
  std::cout << "Overall Score: " << std::fixed << std::setprecision(2)
            << (metrics.overall_score * 100.0f) << "%\n";
  std::cout << "All Critical Tests Passed: " << (all_passed ? "Yes" : "No")
            << "\n";

  // Reverse-engineering guidelines
  print_section("Reverse-Engineering Guideline Test Results");

  for (const auto &result : metrics.guideline_results) {
    std::cout << "  " << std::setw(2) << result.guideline_number << ". "
              << std::setw(22) << std::left << result.guideline_name
              << std::right << "  [" << (result.passed ? "PASS" : "FAIL") << "]"
              << "  Score: " << std::setw(5) << std::fixed
              << std::setprecision(2) << result.score << "\n";
  }

  // Special tests
  print_section("Special Tests");
  std::cout << "Paradox Resolution: "
            << (metrics.paradox_resolution_passed ? "PASS" : "FAIL") << "\n";
  std::cout << "Emergence Validation: "
            << (metrics.emergence_validation_passed ? "PASS" : "FAIL") << "\n";

  std::cout << "\nFinal Pass Rate: " << std::fixed << std::setprecision(1)
            << (test_suite.get_pass_rate() * 100.0f) << "%\n";
}

int main() {
  std::cout << "NanoBrain Consciousness Demo\n";
  std::cout << "Chapter 10: Uploading Consciousness\n";
  std::cout << std::string(60, '=') << "\n\n";

  // Initialize unified kernel
  std::cout << "Initializing NanoBrain...\n";

  UnifiedNanoBrainConfig config;
  config.time_crystal_dimensions = CONSCIOUSNESS_DIMENSIONS;
  config.max_reasoning_depth = 5;
  config.attention_mechanism = AttentionMechanism::Hybrid;

  UnifiedNanoBrainKernel kernel(config);
  kernel.initialize();

  // Create some initial atoms for testing
  std::cout << "Creating initial knowledge base...\n";
  for (int i = 0; i < 10; ++i) {
    kernel.create_atom("ConceptNode", "Concept_" + std::to_string(i),
                       0.7f + 0.03f * i, 0.8f, {2, 3, 5, 7, 11});
  }

  // Run a few processing cycles
  std::cout << "Running initial processing cycles...\n";
  kernel.run_cycles(10);

  // Get subsystems
  TimeCrystalKernel *tc_kernel = kernel.get_time_crystal_kernel();
  MetaCognitiveFeedbackEngine *meta_engine = kernel.get_metacognitive_engine();

  // Create consciousness components
  ConsciousnessUploader uploader(tc_kernel, meta_engine);
  BrainEvolutionSimulator evolution(tc_kernel);

  // Run demos
  demo_consciousness_upload(tc_kernel, meta_engine);
  demo_dimensional_folding(tc_kernel ? tc_kernel->get_tensor_kernel()
                                     : nullptr);
  demo_brain_evolution(tc_kernel);
  demo_future_interfaces(tc_kernel, &uploader);
  demo_test_suite(tc_kernel, &uploader, &evolution);

  // Final summary
  print_header("Summary");

  auto metrics = kernel.get_metrics();
  std::cout << "Total Atoms: " << metrics.total_atoms << "\n";
  std::cout << "Consciousness Emergence: " << metrics.consciousness_emergence
            << "\n";
  std::cout << "System Coherence: " << metrics.system_coherence << "\n";
  std::cout << "Cycles Run: " << kernel.get_cycle_count() << "\n";

  // Shutdown
  std::cout << "\nShutting down...\n";
  kernel.shutdown();

  std::cout << "\nConsciousness Demo Complete!\n";

  return 0;
}
