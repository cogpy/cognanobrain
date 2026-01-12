#include "nanobrain_dodecanion.h"
#include "nanobrain_fractal.h"
#include "nanobrain_kernel.h"
#include <iomanip>
#include <iostream>

/**
 * Fractal Mechanics Demo - Chapter 4
 *
 * Demonstrates:
 * - Dodecanion 12D algebra
 * - CFGA 13-operation engine
 * - Fractal Harmonic Oscillator
 * - Fractal Interference simulation
 * - Regulatory Equations
 */

void print_separator(const std::string &title) {
  std::cout << "\n========================================" << std::endl;
  std::cout << " " << title << std::endl;
  std::cout << "========================================" << std::endl;
}

int main() {
  print_separator("Chapter 4: Fractal Mechanics Demo");

  NanoBrainConfig config;
  config.memory_size = 1024 * 1024 * 64;
  NanoBrainKernel kernel(config);

  // ================================================================
  // Part 1: Dodecanion Algebra
  // ================================================================
  print_separator("1. Dodecanion Algebra (12D Hypercomplex)");

  std::cout << std::fixed << std::setprecision(4);

  // Create dodecanions
  Dodecanion d1 = Dodecanion::from_quaternion(1.0f, 0.0f, 0.0f, 0.0f);
  std::cout << "d1 = from_quaternion(1,0,0,0)" << std::endl;
  std::cout << "  Norm: " << d1.norm() << std::endl;

  std::array<float, 8> oct = {1.0f, 0.5f, 0.25f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
  Dodecanion d2 = Dodecanion::from_octonion(oct);
  std::cout << "d2 = from_octonion(1, 0.5, 0.25, ...)" << std::endl;
  std::cout << "  Norm: " << d2.norm() << std::endl;

  // Multiplication (non-commutative)
  Dodecanion d3 = d1 * d2;
  Dodecanion d4 = d2 * d1;
  std::cout << "\nMultiplication (non-commutative):" << std::endl;
  std::cout << "  d1 * d2 norm: " << d3.norm() << std::endl;
  std::cout << "  d2 * d1 norm: " << d4.norm() << std::endl;
  std::cout << "  Equal: " << (d3 == d4 ? "Yes" : "No") << std::endl;

  // Inverse
  Dodecanion d2_inv = d2.inverse();
  Dodecanion identity = d2 * d2_inv;
  std::cout << "\nInverse test:" << std::endl;
  std::cout << "  d2 * d2.inverse() real: " << identity.real() << std::endl;

  // 11D projection
  auto coords = d2.project_to_11d();
  std::cout << "\n11D manifold projection:" << std::endl;
  std::cout << "  [" << coords[0] << ", " << coords[1] << ", " << coords[2]
            << ", " << coords[3] << ", ...]" << std::endl;

  // Consciousness expressions
  DodecanionAlgebra dod_algebra(&kernel);
  auto expressions = dod_algebra.get_all_expressions();
  std::cout << "\n20 Consciousness Expressions (12 dodecanion + 8 octonion):"
            << std::endl;
  for (size_t i = 0; i < 5; i++) {
    std::cout << "  [" << i << "] " << expressions[i].name << std::endl;
  }
  std::cout << "  ... and " << expressions.size() - 5 << " more" << std::endl;

  // ================================================================
  // Part 2: CFGA 13-Operation Engine
  // ================================================================
  print_separator("2. CFGA 13-Operation Engine");

  CFGAOperator cfga(&kernel);
  std::cout << "Operations: add, sub, mul, div, diff, integrate," << std::endl;
  std::cout << "            partial_diff, rotate, scale, project," << std::endl;
  std::cout << "            reflect, compose, decompose" << std::endl;

  auto *input = kernel.create_tensor({10});
  std::vector<float> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  kernel.set_data(input, data);

  // Differentiation
  auto *diff = cfga.diff(input);
  kernel.compute(diff);
  std::cout << "\nDifferentiation:" << std::endl;
  std::cout << "  Input: [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]" << std::endl;
  std::cout << "  Output[0]: " << kernel.get_value(diff, 0) << std::endl;

  // Integration
  auto *integ = cfga.integrate(input);
  kernel.compute(integ);
  std::cout << "Integration:" << std::endl;
  std::cout << "  Output[9]: " << kernel.get_value(integ, 9) << std::endl;

  // Rotation
  auto *rotated = cfga.rotate(input, 0.5f);
  kernel.compute(rotated);
  std::cout << "Rotation (0.5 rad):" << std::endl;
  std::cout << "  Applied successfully" << std::endl;

  // ================================================================
  // Part 3: Fractal Harmonic Oscillator
  // ================================================================
  print_separator("3. Fractal Harmonic Oscillator");

  FractalHarmonicOscillator fho(&kernel, 11);
  std::cout << "Initialized 11D Fractal Harmonic Oscillator" << std::endl;

  // Initialize with position and velocity
  std::vector<float> pos(11, 0.0f);
  std::vector<float> vel(11, 0.0f);
  pos[0] = 1.0f; // Initial displacement in first dimension
  vel[1] = 0.5f; // Initial velocity in second dimension
  fho.initialize(pos, vel);

  std::cout << "\nEvolution (100 steps, dt=0.01):" << std::endl;
  for (int i = 0; i < 100; i++) {
    fho.evolve(0.01f);
  }

  auto state = fho.get_state();
  std::cout << "  Energy: " << state.energy << std::endl;
  std::cout << "  Phase: " << state.phase << std::endl;
  std::cout << "  Fractal dim: " << state.fractal_dimension << std::endl;

  // Harvest singularities
  auto singularities = fho.harvest_singularities();
  std::cout << "\nSingularity Harvesting:" << std::endl;
  std::cout << "  Found: " << singularities.size() << std::endl;
  std::cout << "  Total energy: " << fho.get_harvested_energy() << std::endl;

  // ================================================================
  // Part 4: Fractal Interference (Microtubule Model)
  // ================================================================
  print_separator("4. Fractal Interference Simulation");

  FractalInterference interference(&kernel);
  std::cout << "Simulating 100-tubulin microtubule..." << std::endl;

  interference.simulate_microtubule(100);
  auto tubulins = interference.get_tubulin_states();
  std::cout << "  Tubulin count: " << tubulins.size() << std::endl;

  // Model comparison
  std::cout << "\nQuantum vs Fractal comparison:" << std::endl;
  auto comparison = interference.compare_models("default");
  std::cout << "  Better model: " << comparison.better_model << std::endl;
  std::cout << "  Quantum coherence: " << comparison.quantum_coherence
            << std::endl;
  std::cout << "  Fractal coherence: " << comparison.fractal_coherence
            << std::endl;

  // ================================================================
  // Part 5: 12 Regulatory Equations
  // ================================================================
  print_separator("5. 12 Regulatory Equations");

  RegulatoryEquations reg(&kernel);
  std::cout << "Regulatory equations for future → present projection"
            << std::endl;

  // Projection from infinity
  auto proj = reg.project_from_infinity(input);
  std::cout << "\nProjection from infinity:" << std::endl;
  std::cout << "  Factor: " << proj.projection_factor << std::endl;
  std::cout << "  Converged: " << (proj.converged ? "Yes" : "No") << std::endl;

  // Full 12-equation regulation
  auto *regulated = reg.apply_full_regulation(input);
  kernel.compute(regulated);
  std::cout << "\nFull 12-equation regulation applied" << std::endl;
  std::cout << "  Output[0]: " << kernel.get_value(regulated, 0) << std::endl;

  // ================================================================
  // Summary
  // ================================================================
  print_separator("Summary");

  std::cout << "Chapter 4 Fractal Mechanics Complete:" << std::endl;
  std::cout << "  ✓ Dodecanion 12D algebra" << std::endl;
  std::cout << "  ✓ CFGA 13-operation engine" << std::endl;
  std::cout << "  ✓ Fractal Harmonic Oscillator" << std::endl;
  std::cout << "  ✓ Fractal Interference simulation" << std::endl;
  std::cout << "  ✓ 12 Regulatory Equations" << std::endl;

  std::cout << "\n========================================" << std::endl;
  std::cout << " Fractal Mechanics Demo Complete" << std::endl;
  std::cout << "========================================" << std::endl;

  return 0;
}
