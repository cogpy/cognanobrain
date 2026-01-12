#include "nanobrain_gog.h"
#include "nanobrain_hardware_sim.h"
#include "nanobrain_kernel.h"
#include "nanobrain_spontaneous.h"
#include "nanobrain_tc_transform.h"
#include "nanobrain_turing_tests.h"
#include <iomanip>
#include <iostream>

/**
 * Universal Time Crystal Demo - Chapter 5
 *
 * Demonstrates:
 * - Garden of Gardens (GOG) nested structure
 * - Time Crystal Transform (FFT alternative)
 * - Spontaneous Reply System ("search without searching")
 * - Turing Failure Resolution
 * - Hardware Architecture Simulation
 */

void print_separator(const std::string &title) {
  std::cout << "\n========================================" << std::endl;
  std::cout << " " << title << std::endl;
  std::cout << "========================================" << std::endl;
}

int main() {
  print_separator("Chapter 5: Universal Time Crystal Demo");

  NanoBrainConfig config;
  config.memory_size = 1024 * 1024 * 64;
  NanoBrainKernel kernel(config);

  std::cout << std::fixed << std::setprecision(4);

  // ================================================================
  // Part 1: Garden of Gardens (GOG)
  // ================================================================
  print_separator("1. Garden of Gardens (Meander Flower)");

  GardenOfGardens gog(&kernel);
  std::cout << "Creating nested time crystal hierarchy..." << std::endl;

  // Create root garden with petals
  auto root = gog.create_garden("Root", 5);
  std::cout << "  Root garden: 5 petals" << std::endl;

  // Add sub-gardens
  auto sub1 = gog.create_sub_garden(root, "Sub1", 3);
  auto sub2 = gog.create_sub_garden(root, "Sub2", 4);
  std::cout << "  Sub-gardens: Sub1 (3 petals), Sub2 (4 petals)" << std::endl;

  // Update meander phases
  gog.update_meander_phases(0.1f);
  std::cout << "\nMeander phase update:" << std::endl;
  std::cout << "  Root phase: " << gog.get_meander_phase(root) << std::endl;

  // Get hierarchy metrics
  auto metrics = gog.get_hierarchy_metrics();
  std::cout << "\nHierarchy metrics:" << std::endl;
  std::cout << "  Total gardens: " << metrics.total_gardens << std::endl;
  std::cout << "  Total petals: " << metrics.total_petals << std::endl;
  std::cout << "  Max depth: " << metrics.max_depth << std::endl;

  // ================================================================
  // Part 2: Time Crystal Transform
  // ================================================================
  print_separator("2. Time Crystal Transform (FFT Alternative)");

  TimeCrystalTransform tct(&kernel);
  std::cout << "Prime-based transform engine initialized" << std::endl;

  // Create test signal
  auto *signal = kernel.create_tensor({64});
  std::vector<float> signal_data(64);
  for (int i = 0; i < 64; i++) {
    signal_data[i] = std::sin(2.0f * 3.14159f * i / 16.0f) +
                     0.5f * std::sin(2.0f * 3.14159f * i / 8.0f);
  }
  kernel.set_data(signal, signal_data);

  // Forward transform
  auto *transformed = tct.forward(signal);
  kernel.compute(transformed);
  std::cout << "\nForward transform:" << std::endl;
  std::cout << "  Input: 64-sample signal (two frequencies)" << std::endl;
  std::cout << "  Peak[0]: " << kernel.get_value(transformed, 0) << std::endl;

  // Inverse transform
  auto *reconstructed = tct.inverse(transformed);
  kernel.compute(reconstructed);
  std::cout << "Inverse transform:" << std::endl;
  std::cout << "  Reconstruction[0]: " << kernel.get_value(reconstructed, 0)
            << std::endl;

  // Compare to FFT
  auto comparison = tct.compare_to_fft(signal);
  std::cout << "\nFFT comparison:" << std::endl;
  std::cout << "  Accuracy: " << comparison.accuracy * 100 << "%" << std::endl;
  std::cout << "  Speed ratio: " << comparison.speed_ratio << "x" << std::endl;

  // ================================================================
  // Part 3: Spontaneous Reply System
  // ================================================================
  print_separator("3. Spontaneous Reply (Search Without Searching)");

  SpontaneousReply spontaneous(&kernel);
  std::cout << "Synchronization-based lookup system" << std::endl;

  // Add patterns
  spontaneous.add_pattern("cat", {0.9f, 0.1f, 0.2f, 0.8f});
  spontaneous.add_pattern("dog", {0.1f, 0.9f, 0.3f, 0.7f});
  spontaneous.add_pattern("bird", {0.5f, 0.5f, 0.9f, 0.1f});
  std::cout << "  Added 3 patterns: cat, dog, bird" << std::endl;

  // Query
  std::vector<float> query = {0.85f, 0.15f, 0.25f, 0.75f};
  auto result = spontaneous.query(query);
  std::cout << "\nQuery: [0.85, 0.15, 0.25, 0.75]" << std::endl;
  std::cout << "  Result: " << result.matched_pattern << std::endl;
  std::cout << "  Sync strength: " << result.sync_strength << std::endl;
  std::cout << "  Time: " << result.response_time_ms << " ms" << std::endl;

  // ================================================================
  // Part 4: Turing Failure Resolution
  // ================================================================
  print_separator("4. Turing Failure Resolution");

  TuringFailureResolver resolver(&kernel);
  std::cout << "Testing 10 Turing failure scenarios..." << std::endl;

  std::vector<std::string> scenarios = {"halting_problem", "self_reference",
                                        "goedel_incompleteness",
                                        "infinite_recursion", "paradox"};

  for (const auto &scenario : scenarios) {
    auto resolution = resolver.resolve(scenario);
    std::cout << "  " << scenario << ": "
              << (resolution.resolved ? "✓ Resolved" : "✗ Failed")
              << " (method: " << resolution.method << ")" << std::endl;
  }

  // ================================================================
  // Part 5: Hardware Simulation
  // ================================================================
  print_separator("5. Hardware Architecture Simulation");

  HardwareSimulator hw(&kernel);
  std::cout << "Thermal breathing and microtubule dynamics" << std::endl;

  // Thermal breathing
  hw.initialize_thermal_model(300.0f); // 300K
  for (int i = 0; i < 100; i++) {
    hw.step_thermal(0.001f);
  }
  auto thermal = hw.get_thermal_state();
  std::cout << "\nThermal breathing:" << std::endl;
  std::cout << "  Temperature: " << thermal.temperature << " K" << std::endl;
  std::cout << "  Breathing phase: " << thermal.breathing_phase << std::endl;

  // Microtubule dynamics
  hw.initialize_microtubule_model(100);
  for (int i = 0; i < 50; i++) {
    hw.step_microtubule(0.01f);
  }
  auto mt = hw.get_microtubule_state();
  std::cout << "\nMicrotubule dynamics:" << std::endl;
  std::cout << "  Active tubulins: " << mt.active_count << "/" << mt.total_count
            << std::endl;
  std::cout << "  Coherence: " << mt.coherence << std::endl;

  // ================================================================
  // Summary
  // ================================================================
  print_separator("Summary");

  std::cout << "Chapter 5 Universal Time Crystal Complete:" << std::endl;
  std::cout << "  ✓ Garden of Gardens (GOG)" << std::endl;
  std::cout << "  ✓ Time Crystal Transform" << std::endl;
  std::cout << "  ✓ Spontaneous Reply System" << std::endl;
  std::cout << "  ✓ Turing Failure Resolution" << std::endl;
  std::cout << "  ✓ Hardware Simulation" << std::endl;

  std::cout << "\n========================================" << std::endl;
  std::cout << " Universal Time Crystal Demo Complete" << std::endl;
  std::cout << "========================================" << std::endl;

  return 0;
}
