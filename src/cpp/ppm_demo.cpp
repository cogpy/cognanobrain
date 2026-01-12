#include "nanobrain_ppm.h"
#include <iomanip>
#include <iostream>

/**
 * PPM Demo - Phase Prime Metric Demonstration
 *
 * Demonstrates all 10 PPM metric classes, operator chain, and coherence table.
 */

void print_separator(const std::string &title) {
  std::cout << "\n";
  std::cout << "========================================" << std::endl;
  std::cout << " " << title << std::endl;
  std::cout << "========================================" << std::endl;
}

void print_result(const PPMResult &result) {
  std::cout << std::fixed << std::setprecision(4);
  std::cout << "  Metric: " << result.metric_name << std::endl;
  std::cout << "  Value: " << result.value << std::endl;
  std::cout << "  Valid: " << (result.valid ? "Yes" : "No") << std::endl;

  if (!result.metadata.empty()) {
    std::cout << "  Metadata:" << std::endl;
    for (const auto &[key, value] : result.metadata) {
      std::cout << "    " << key << ": " << value << std::endl;
    }
  }

  if (!result.components.empty() && result.components.size() <= 10) {
    std::cout << "  Components: [";
    for (size_t i = 0; i < result.components.size(); i++) {
      std::cout << result.components[i];
      if (i < result.components.size() - 1)
        std::cout << ", ";
    }
    std::cout << "]" << std::endl;
  }
}

int main() {
  print_separator("Phase Prime Metric (PPM) Demo");

  std::cout << "\nFundamental 15 primes: {";
  for (size_t i = 0; i < PPM_15_PRIMES.size(); i++) {
    std::cout << PPM_15_PRIMES[i];
    if (i < PPM_15_PRIMES.size() - 1)
      std::cout << ", ";
  }
  std::cout << "}" << std::endl;

  // Test primes
  std::vector<int> test_primes = {2, 3, 5, 7, 11};

  std::cout << "\nTest primes: {";
  for (size_t i = 0; i < test_primes.size(); i++) {
    std::cout << test_primes[i];
    if (i < test_primes.size() - 1)
      std::cout << ", ";
  }
  std::cout << "}" << std::endl;

  // ================================================================
  // Test each metric individually
  // ================================================================
  print_separator("1. Individual Metrics");

  for (int i = 1; i <= 10; i++) {
    auto metric = create_ppm_metric(i);
    if (metric) {
      std::cout << "\n--- Metric " << i << " ---" << std::endl;
      PPMResult result = metric->compute(test_primes);
      print_result(result);
    }
  }

  // ================================================================
  // Test Operator Chain
  // ================================================================
  print_separator("2. Operator Chain");

  PPMOperatorChain chain;
  chain.add_by_index(1); // Geometric Shape
  chain.add_by_index(2); // Ordered Factor
  chain.add_by_index(3); // Phase Path

  std::cout << "Chain length: " << chain.length() << std::endl;

  auto chain_results = chain.execute(test_primes);
  std::cout << "\nChain execution results:" << std::endl;
  for (const auto &result : chain_results) {
    std::cout << "  " << result.metric_name << ": " << result.value
              << std::endl;
  }

  float combined = chain.combined_value(test_primes);
  std::cout << "\nCombined value: " << combined << std::endl;

  // ================================================================
  // Test Coherence Table
  // ================================================================
  print_separator("3. Coherence Table");

  PPMCoherenceTable table;
  std::cout << "Table size: " << table.size() << " precomputed entries"
            << std::endl;

  std::vector<std::vector<int>> coherence_tests = {
      {2, 3},
      {2, 3, 5},
      {2, 3, 5, 7},
      {2, 3, 5, 7, 11},
      {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47} // All 15
  };

  std::cout << "\nCoherence lookups:" << std::endl;
  for (const auto &primes : coherence_tests) {
    float coherence = table.lookup(primes);
    std::cout << "  {";
    for (size_t i = 0; i < primes.size(); i++) {
      std::cout << primes[i];
      if (i < primes.size() - 1)
        std::cout << ",";
    }
    std::cout << "} → " << coherence << std::endl;
  }

  // ================================================================
  // Test Specific Metrics in Detail
  // ================================================================
  print_separator("4. Detailed Metric Tests");

  // Test Prime Lattice
  std::cout << "\n--- Prime Lattice (twins/cousins) ---" << std::endl;
  PPMMetric9_PrimeLattice lattice_metric;
  int twin_count = lattice_metric.count_twins(test_primes);
  int cousin_count = lattice_metric.count_cousins(test_primes);
  std::cout << "  Twin pairs: " << twin_count << std::endl;
  std::cout << "  Cousin pairs: " << cousin_count << std::endl;

  // Test Hole Finder
  std::cout << "\n--- Hole Finder ---" << std::endl;
  PPMMetric6_HoleFinder hole_metric;
  auto holes = hole_metric.find_holes(test_primes);
  std::cout << "  Total holes: " << holes.size() << std::endl;
  for (const auto &hole : holes) {
    std::cout << "    Gap " << hole.start << "-" << hole.end << ": size "
              << hole.gap_size << std::endl;
  }

  // Test Ordered Factor
  std::cout << "\n--- Ordered Factor ---" << std::endl;
  PPMMetric2_OrderedFactor of_metric;
  std::vector<int64_t> test_numbers = {60, 200, 1001};
  for (int64_t n : test_numbers) {
    auto of = of_metric.factorize(n);
    std::cout << "  " << n << " = ";
    for (size_t i = 0; i < of.primes.size(); i++) {
      std::cout << of.primes[i];
      if (of.exponents[i] > 1)
        std::cout << "^" << of.exponents[i];
      if (i < of.primes.size() - 1)
        std::cout << " × ";
    }
    std::cout << " (ratio: " << of.ratio << ")" << std::endl;
  }

  // ================================================================
  // Test PPM Evolution
  // ================================================================
  print_separator("5. PPM Evolution");

  NanoBrainConfig kernel_config;
  kernel_config.memory_size = 1024 * 1024 * 64;
  NanoBrainKernel kernel(kernel_config);

  PPMEvolution evolution(&kernel);

  // Create initial state
  TimeCrystalQuantumState state;
  state.prime_signature = {2, 3, 5, 7, 11};
  state.temporal_coherence = 0.5f;
  state.fractal_dimension = 1.5f;
  state.resonance_frequency = 1.0f;
  state.quantum_phase = 0.0f;

  std::cout << "Initial state:" << std::endl;
  std::cout << "  Coherence: " << state.temporal_coherence << std::endl;
  std::cout << "  Fractal dim: " << state.fractal_dimension << std::endl;
  std::cout << "  Phase: " << state.quantum_phase << std::endl;

  // Evolve state
  PPMOperatorChain evolution_chain;
  for (int i = 1; i <= 10; i++) {
    evolution_chain.add_by_index(i);
  }

  for (int step = 0; step < 10; step++) {
    state = evolution.evolve(state, evolution_chain, 0.1f);
  }

  std::cout << "\nAfter 10 evolution steps:" << std::endl;
  std::cout << "  Coherence: " << state.temporal_coherence << std::endl;
  std::cout << "  Fractal dim: " << state.fractal_dimension << std::endl;
  std::cout << "  Phase: " << state.quantum_phase << std::endl;

  // Export phase plot
  bool exported = evolution.export_phase_plot("phase_plot.csv", test_primes);
  std::cout << "\nPhase plot export: " << (exported ? "Success" : "Failed")
            << std::endl;

  // ================================================================
  // Summary
  // ================================================================
  print_separator("6. Summary");

  std::cout << "\nPPM Implementation Complete:" << std::endl;
  std::cout << "  ✓ 10 PPM Metric Classes" << std::endl;
  std::cout << "  ✓ Operator Chain Composition" << std::endl;
  std::cout << "  ✓ Coherence Table (32K entries)" << std::endl;
  std::cout << "  ✓ Time Crystal Evolution" << std::endl;
  std::cout << "  ✓ Phase Plot Export" << std::endl;

  std::cout << "\n========================================" << std::endl;
  std::cout << " PPM Demo Complete" << std::endl;
  std::cout << "========================================" << std::endl;

  return 0;
}
