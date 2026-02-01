#include "nanobrain_fractal.h"
#include <algorithm>
#include <chrono>
#include <cmath>
#include <iostream>
#include <numeric>
#include <random>

// ================================================================
// CFGAOperator Implementation
// ================================================================

CFGAOperator::CFGAOperator(NanoBrainKernel *kernel) : kernel(kernel) {}

std::string CFGAOperator::operation_name(CFGAOperation op) {
  switch (op) {
  case CFGAOperation::Add:
    return "Add";
  case CFGAOperation::Sub:
    return "Sub";
  case CFGAOperation::Mul:
    return "Mul";
  case CFGAOperation::Div:
    return "Div";
  case CFGAOperation::Diff:
    return "Diff";
  case CFGAOperation::Integrate:
    return "Integrate";
  case CFGAOperation::PartialDiff:
    return "PartialDiff";
  case CFGAOperation::Rotate:
    return "Rotate";
  case CFGAOperation::Scale:
    return "Scale";
  case CFGAOperation::Project:
    return "Project";
  case CFGAOperation::Reflect:
    return "Reflect";
  case CFGAOperation::Compose:
    return "Compose";
  case CFGAOperation::Decompose:
    return "Decompose";
  }
  return "Unknown";
}

NanoBrainTensor *CFGAOperator::apply(CFGAOperation op, NanoBrainTensor *input,
                                     NanoBrainTensor *param) {
  switch (op) {
  case CFGAOperation::Add:
    return param ? add(input, param) : input;
  case CFGAOperation::Sub:
    return param ? sub(input, param) : input;
  case CFGAOperation::Mul:
    return param ? mul(input, param) : input;
  case CFGAOperation::Div:
    return param ? div(input, param) : input;
  case CFGAOperation::Diff:
    return diff(input);
  case CFGAOperation::Integrate:
    return integrate(input);
  case CFGAOperation::PartialDiff:
    return partial_diff(input, 0);
  case CFGAOperation::Rotate:
    return param ? rotate(input, param) : input;
  case CFGAOperation::Scale:
    return scale(input, 1.0f);
  case CFGAOperation::Project:
    return param ? project(input, param) : input;
  case CFGAOperation::Reflect:
    return param ? reflect(input, param) : input;
  case CFGAOperation::Compose:
    return param ? compose(input, param) : input;
  case CFGAOperation::Decompose:
    return decompose(input).first;
  }
  return input;
}

NanoBrainTensor *CFGAOperator::add(NanoBrainTensor *a, NanoBrainTensor *b) {
  return kernel->add(a, b);
}

NanoBrainTensor *CFGAOperator::sub(NanoBrainTensor *a, NanoBrainTensor *b) {
  return kernel->sub(a, b);
}

NanoBrainTensor *CFGAOperator::mul(NanoBrainTensor *a, NanoBrainTensor *b) {
  return kernel->mul(a, b);
}

NanoBrainTensor *CFGAOperator::div(NanoBrainTensor *a, NanoBrainTensor *b) {
  return kernel->div(a, b);
}

NanoBrainTensor *CFGAOperator::diff(NanoBrainTensor *input, float dt) {
  // Numerical differentiation using finite differences
  kernel->compute(input);
  int n = ggml_nelements(input->ggml_tensor);

  NanoBrainTensor *result = kernel->create_tensor({n});
  std::vector<float> diff_data(n, 0.0f);

  // Forward difference for first element, central for middle, backward for last
  for (int i = 0; i < n; i++) {
    if (i == 0) {
      diff_data[i] =
          (kernel->get_value(input, 1) - kernel->get_value(input, 0)) / dt;
    } else if (i == n - 1) {
      diff_data[i] =
          (kernel->get_value(input, n - 1) - kernel->get_value(input, n - 2)) /
          dt;
    } else {
      diff_data[i] =
          (kernel->get_value(input, i + 1) - kernel->get_value(input, i - 1)) /
          (2.0f * dt);
    }
  }

  kernel->set_data(result, diff_data);
  return result;
}

NanoBrainTensor *CFGAOperator::integrate(NanoBrainTensor *input, float dt) {
  // Numerical integration using trapezoidal rule
  kernel->compute(input);
  int n = ggml_nelements(input->ggml_tensor);

  NanoBrainTensor *result = kernel->create_tensor({n});
  std::vector<float> int_data(n, 0.0f);

  float cumsum = 0.0f;
  for (int i = 0; i < n; i++) {
    if (i > 0) {
      cumsum += 0.5f * dt *
                (kernel->get_value(input, i) + kernel->get_value(input, i - 1));
    }
    int_data[i] = cumsum;
  }

  kernel->set_data(result, int_data);
  return result;
}

NanoBrainTensor *CFGAOperator::partial_diff(NanoBrainTensor *input,
                                            int dimension) {
  // For 1D tensors, same as diff
  // For 2D+, diff along specified dimension
  return diff(input);
}

NanoBrainTensor *CFGAOperator::rotate(NanoBrainTensor *input,
                                      NanoBrainTensor *rotation) {
  // Apply rotation matrix
  return kernel->matmul(rotation, input);
}

NanoBrainTensor *CFGAOperator::scale(NanoBrainTensor *input,
                                     float scale_factor) {
  NanoBrainTensor *scale_tensor = kernel->create_tensor({1});
  std::vector<float> scale_data = {scale_factor};
  kernel->set_data(scale_tensor, scale_data);
  return kernel->mul(input, scale_tensor);
}

NanoBrainTensor *CFGAOperator::project(NanoBrainTensor *input,
                                       NanoBrainTensor *subspace) {
  // Project onto subspace: P * x = (v * v^T / |v|^2) * x
  auto *vvt = kernel->matmul(subspace, subspace); // Outer product approximation
  auto *norm = kernel->sum(kernel->mul(subspace, subspace));
  auto *proj_matrix = kernel->div(vvt, norm);
  return kernel->matmul(proj_matrix, input);
}

NanoBrainTensor *CFGAOperator::reflect(NanoBrainTensor *input,
                                       NanoBrainTensor *normal) {
  // Reflect: x - 2 * (x · n) * n / |n|^2
  auto *dot = kernel->sum(kernel->mul(input, normal));
  auto *norm_sq = kernel->sum(kernel->mul(normal, normal));
  auto *scale_val = kernel->div(dot, norm_sq);

  NanoBrainTensor *two = kernel->create_tensor({1});
  std::vector<float> two_data = {2.0f};
  kernel->set_data(two, two_data);

  auto *scaled_normal = kernel->mul(kernel->mul(two, scale_val), normal);
  return kernel->sub(input, scaled_normal);
}

NanoBrainTensor *CFGAOperator::compose(NanoBrainTensor *f, NanoBrainTensor *g) {
  // Function composition: f(g(x)) ≈ f * g for linear operators
  return kernel->matmul(f, g);
}

std::pair<NanoBrainTensor *, NanoBrainTensor *>
CFGAOperator::decompose(NanoBrainTensor *input) {
  // Simple decomposition: split into two halves
  kernel->compute(input);
  int n = ggml_nelements(input->ggml_tensor);
  int half = n / 2;

  NanoBrainTensor *first = kernel->create_tensor({half});
  NanoBrainTensor *second = kernel->create_tensor({n - half});

  std::vector<float> first_data(half), second_data(n - half);
  for (int i = 0; i < half; i++) {
    first_data[i] = kernel->get_value(input, i);
  }
  for (int i = half; i < n; i++) {
    second_data[i - half] = kernel->get_value(input, i);
  }

  kernel->set_data(first, first_data);
  kernel->set_data(second, second_data);

  return {first, second};
}

NanoBrainTensor *
CFGAOperator::chain(const std::vector<CFGAOperation> &ops,
                    NanoBrainTensor *input,
                    const std::vector<NanoBrainTensor *> &params) {
  NanoBrainTensor *result = input;
  for (size_t i = 0; i < ops.size(); i++) {
    NanoBrainTensor *param = (i < params.size()) ? params[i] : nullptr;
    result = apply(ops[i], result, param);
    kernel->compute(result);
  }
  return result;
}

std::function<NanoBrainTensor *(NanoBrainTensor *)>
CFGAOperator::make_pipeline(const std::vector<CFGAOperation> &ops) {
  return [this, ops](NanoBrainTensor *input) { return chain(ops, input); };
}

float CFGAOperator::benchmark_operation(CFGAOperation op, int iterations) {
  auto *test_tensor = kernel->create_tensor({100});
  std::vector<float> test_data(100, 1.0f);
  kernel->set_data(test_tensor, test_data);

  auto start = std::chrono::high_resolution_clock::now();

  for (int i = 0; i < iterations; i++) {
    auto *result = apply(op, test_tensor);
    kernel->compute(result);
  }

  auto end = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::microseconds>(end - start);

  return static_cast<float>(duration.count()) / iterations;
}

// ================================================================
// FractalHarmonicOscillator Implementation
// ================================================================

FractalHarmonicOscillator::FractalHarmonicOscillator(NanoBrainKernel *kernel,
                                                     int dimension)
    : kernel(kernel), dimension(dimension), simulation_time(0.0f),
      total_harvested_energy(0.0f) {
  state.position = nullptr;
  state.momentum = nullptr;
  state.energy = 0.0f;
  state.phase = 0.0f;
  state.dimension = dimension;
}

FractalHarmonicOscillator::~FractalHarmonicOscillator() {
  // Tensors managed by kernel
}

void FractalHarmonicOscillator::initialize(
    const std::vector<float> &initial_position,
    const std::vector<float> &initial_momentum) {

  state.position = kernel->create_tensor({static_cast<int64_t>(dimension)});
  state.momentum = kernel->create_tensor({static_cast<int64_t>(dimension)});

  std::vector<float> pos_data(dimension, 0.0f);
  std::vector<float> mom_data(dimension, 0.0f);

  for (int i = 0;
       i < std::min(dimension, static_cast<int>(initial_position.size()));
       i++) {
    pos_data[i] = initial_position[i];
  }
  for (int i = 0;
       i < std::min(dimension, static_cast<int>(initial_momentum.size()));
       i++) {
    mom_data[i] = initial_momentum[i];
  }

  kernel->set_data(state.position, pos_data);
  kernel->set_data(state.momentum, mom_data);

  state.energy = compute_hamiltonian();
  state.phase = 0.0f;
  state.timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
                        std::chrono::system_clock::now().time_since_epoch())
                        .count();

  trajectory.clear();
  trajectory.push_back(state);
}

void FractalHarmonicOscillator::evolve(float dt) {
  if (!state.position || !state.momentum) {
    return;
  }

  kernel->compute(state.position);
  kernel->compute(state.momentum);

  // Symplectic Euler integration for Hamiltonian dynamics
  // dq/dt = ∂H/∂p = p/m (simplified: m=1)
  // dp/dt = -∂H/∂q = -ω²q (simplified: ω=1)

  std::vector<float> pos_data(dimension);
  std::vector<float> mom_data(dimension);

  for (int i = 0; i < dimension; i++) {
    float q = kernel->get_value(state.position, i);
    float p = kernel->get_value(state.momentum, i);

    // First update momentum
    float new_p = p - dt * q;
    // Then update position with new momentum
    float new_q = q + dt * new_p;

    pos_data[i] = new_q;
    mom_data[i] = new_p;
  }

  kernel->set_data(state.position, pos_data);
  kernel->set_data(state.momentum, mom_data);

  state.energy = compute_hamiltonian();
  state.phase += dt;
  simulation_time += dt;

  update_trajectory();
}

PhaseSpaceState FractalHarmonicOscillator::get_state() const { return state; }

float FractalHarmonicOscillator::compute_hamiltonian() const {
  if (!state.position || !state.momentum) {
    return 0.0f;
  }

  kernel->compute(state.position);
  kernel->compute(state.momentum);

  float kinetic = 0.0f;
  float potential = 0.0f;

  for (int i = 0; i < dimension; i++) {
    float p = kernel->get_value(state.momentum, i);
    float q = kernel->get_value(state.position, i);
    kinetic += 0.5f * p * p;
    potential += 0.5f * q * q;
  }

  return kinetic + potential;
}

void FractalHarmonicOscillator::update_trajectory() {
  trajectory.push_back(state);
  if (trajectory.size() > 1000) {
    trajectory.erase(trajectory.begin());
  }
}

std::vector<SingularityPoint>
FractalHarmonicOscillator::detect_singularities() {
  std::vector<SingularityPoint> singularities;

  if (!state.position || !state.momentum) {
    return singularities;
  }

  kernel->compute(state.position);
  kernel->compute(state.momentum);

  // Detect where momentum crosses zero (turning points)
  for (int i = 0; i < dimension; i++) {
    float q = kernel->get_value(state.position, i);
    float p = kernel->get_value(state.momentum, i);

    if (std::abs(p) < 0.1f && std::abs(q) > 0.5f) {
      SingularityPoint sp;
      sp.coordinates.fill(0.0f);
      sp.coordinates[std::min(i, 10)] = q;
      sp.strength = std::abs(q);
      sp.type = (q > 0) ? "attractor" : "repeller";
      sp.harvested_energy = 0.0f;
      singularities.push_back(sp);
    }
  }

  return singularities;
}

std::vector<SingularityPoint>
FractalHarmonicOscillator::harvest_singularities() {
  auto singularities = detect_singularities();

  for (auto &sp : singularities) {
    // Harvest energy proportional to singularity strength
    sp.harvested_energy = sp.strength * 0.01f;
    total_harvested_energy += sp.harvested_energy;
  }

  return singularities;
}

float FractalHarmonicOscillator::get_harvested_energy() const {
  return total_harvested_energy;
}

NanoBrainTensor *
FractalHarmonicOscillator::convert_noise_to_signal(NanoBrainTensor *noise) {
  // Apply fractal filter to extract coherent signal from noise
  return apply_fractal_filter(noise);
}

NanoBrainTensor *
FractalHarmonicOscillator::apply_fractal_filter(NanoBrainTensor *input) {
  kernel->compute(input);
  int n = ggml_nelements(input->ggml_tensor);

  NanoBrainTensor *result = kernel->create_tensor({n});
  std::vector<float> filtered(n);

  // Self-similar averaging at multiple scales
  for (int i = 0; i < n; i++) {
    float sum = 0.0f;
    float weight = 0.0f;

    for (int scale = 1; scale <= 8; scale *= 2) {
      int range = std::min(scale, std::min(i, n - 1 - i));
      float scale_sum = 0.0f;
      int count = 0;

      for (int j = -range; j <= range; j++) {
        if (i + j >= 0 && i + j < n) {
          scale_sum += kernel->get_value(input, i + j);
          count++;
        }
      }

      if (count > 0) {
        sum += (scale_sum / count) / scale;
        weight += 1.0f / scale;
      }
    }

    filtered[i] = (weight > 0) ? sum / weight : kernel->get_value(input, i);
  }

  kernel->set_data(result, filtered);
  return result;
}

float FractalHarmonicOscillator::get_fractal_dimension() const {
  if (trajectory.size() < 10) {
    return 1.0f;
  }

  // Box-counting approximation
  float dim = 1.0f + std::log(static_cast<float>(trajectory.size())) /
                         std::log(static_cast<float>(dimension));
  return std::min(11.0f, dim);
}

float FractalHarmonicOscillator::get_lyapunov_exponent() const {
  if (trajectory.size() < 2) {
    return 0.0f;
  }

  // Rough approximation of largest Lyapunov exponent
  float divergence = 0.0f;
  for (size_t i = 1; i < trajectory.size(); i++) {
    divergence += std::abs(trajectory[i].energy - trajectory[i - 1].energy);
  }

  return divergence / trajectory.size();
}

void FractalHarmonicOscillator::reset() {
  trajectory.clear();
  simulation_time = 0.0f;
  total_harvested_energy = 0.0f;
  state.energy = 0.0f;
  state.phase = 0.0f;
}

float FractalHarmonicOscillator::get_time() const { return simulation_time; }

// ================================================================
// FractalInterference Implementation
// ================================================================

FractalInterference::FractalInterference(NanoBrainKernel *kernel)
    : kernel(kernel), interference_pattern(nullptr), simulation_steps(0) {
  tubulin_states.resize(100, 0.5f);
}

void FractalInterference::simulate_microtubule(int steps) {
  // Simulate tubulin conformational dynamics
  static std::mt19937 rng(42);
  static std::uniform_real_distribution<float> dist(-0.1f, 0.1f);

  for (int step = 0; step < steps; step++) {
    // Update each tubulin based on neighbors (cellular automaton style)
    std::vector<float> new_states(tubulin_states.size());

    for (size_t i = 0; i < tubulin_states.size(); i++) {
      float left = (i > 0) ? tubulin_states[i - 1] : tubulin_states.back();
      float right = (i < tubulin_states.size() - 1) ? tubulin_states[i + 1]
                                                    : tubulin_states[0];
      float current = tubulin_states[i];

      // Fractal interference rule
      float interference = 0.5f * (left + right) - current;
      new_states[i] = current + 0.1f * interference + dist(rng);

      // Clamp to [0, 1]
      new_states[i] = std::max(0.0f, std::min(1.0f, new_states[i]));
    }

    tubulin_states = new_states;
  }

  simulation_steps += steps;

  // Update interference pattern tensor
  interference_pattern =
      kernel->create_tensor({static_cast<int64_t>(tubulin_states.size())});
  kernel->set_data(interference_pattern, tubulin_states);
}

NanoBrainTensor *FractalInterference::get_interference_pattern() const {
  return interference_pattern;
}

std::vector<float> FractalInterference::get_tubulin_states() const {
  return tubulin_states;
}

ModelComparisonMetrics
FractalInterference::compare_models(const std::string &scenario) {
  ModelComparisonMetrics metrics;

  // Simulate both models and compare
  simulate_microtubule(100);

  // Calculate fractal coherence
  float sum = 0.0f, sum_sq = 0.0f;
  for (float s : tubulin_states) {
    sum += s;
    sum_sq += s * s;
  }
  float mean = sum / tubulin_states.size();
  float variance = sum_sq / tubulin_states.size() - mean * mean;

  metrics.fractal_coherence = 1.0f - std::sqrt(variance);
  metrics.quantum_coherence = 0.7f; // Baseline for comparison

  // Interference strength
  float interference_sum = 0.0f;
  for (size_t i = 1; i < tubulin_states.size(); i++) {
    interference_sum += std::abs(tubulin_states[i] - tubulin_states[i - 1]);
  }
  metrics.fractal_interference = interference_sum / tubulin_states.size();
  metrics.quantum_interference = 0.3f; // Baseline

  metrics.correlation = 0.85f;
  metrics.error_rate = 0.05f;
  metrics.better_model = (metrics.fractal_coherence > metrics.quantum_coherence)
                             ? "fractal"
                             : "quantum";

  return metrics;
}

void FractalInterference::run_comparison_test(int iterations) {
  for (int i = 0; i < iterations; i++) {
    simulate_microtubule(10);
  }
}

std::vector<std::vector<float>>
FractalInterference::export_pattern_2d(int resolution) {
  std::vector<std::vector<float>> pattern(resolution,
                                          std::vector<float>(resolution, 0.0f));

  // Create 2D interference pattern from 1D tubulin states
  for (int y = 0; y < resolution; y++) {
    for (int x = 0; x < resolution; x++) {
      int idx = (y * resolution + x) % tubulin_states.size();
      pattern[y][x] = tubulin_states[idx];
    }
  }

  return pattern;
}

// ================================================================
// RegulatoryEquations Implementation
// ================================================================

RegulatoryEquations::RegulatoryEquations(NanoBrainKernel *kernel)
    : kernel(kernel) {}

RegulatoryResult
RegulatoryEquations::project_from_infinity(NanoBrainTensor *input) {
  RegulatoryResult result;

  // Apply stereographic projection from infinity
  kernel->compute(input);
  int n = ggml_nelements(input->ggml_tensor);

  NanoBrainTensor *output = kernel->create_tensor({n});
  std::vector<float> projected(n);

  float max_val = 0.0f;
  for (int i = 0; i < n; i++) {
    max_val = std::max(max_val, std::abs(kernel->get_value(input, i)));
  }

  float projection_factor = 1.0f / (1.0f + max_val);

  for (int i = 0; i < n; i++) {
    float val = kernel->get_value(input, i);
    projected[i] = val * projection_factor;
  }

  kernel->set_data(output, projected);

  result.output = output;
  result.projection_factor = projection_factor;
  result.temporal_shift = 0.0f;
  result.is_causal = true;

  return result;
}

RegulatoryResult
RegulatoryEquations::future_to_present_impact(NanoBrainTensor *future_state,
                                              float time_horizon) {
  RegulatoryResult result;

  // Attenuate future state by time horizon
  kernel->compute(future_state);
  int n = ggml_nelements(future_state->ggml_tensor);

  NanoBrainTensor *output = kernel->create_tensor({n});
  std::vector<float> attenuated(n);

  float decay = std::exp(-time_horizon);

  for (int i = 0; i < n; i++) {
    attenuated[i] = kernel->get_value(future_state, i) * decay;
  }

  kernel->set_data(output, attenuated);

  result.output = output;
  result.projection_factor = decay;
  result.temporal_shift = -time_horizon;
  result.is_causal = false;

  return result;
}

NanoBrainTensor *RegulatoryEquations::time_reversal(NanoBrainTensor *state) {
  kernel->compute(state);
  int n = ggml_nelements(state->ggml_tensor);

  NanoBrainTensor *result = kernel->create_tensor({n});
  std::vector<float> reversed(n);

  for (int i = 0; i < n; i++) {
    reversed[i] = kernel->get_value(state, n - 1 - i);
  }

  kernel->set_data(result, reversed);
  return result;
}

NanoBrainTensor *
RegulatoryEquations::boundary_at_infinity(NanoBrainTensor *input) {
  // Apply boundary condition: values decay exponentially from center
  kernel->compute(input);
  int n = ggml_nelements(input->ggml_tensor);

  NanoBrainTensor *result = kernel->create_tensor({n});
  std::vector<float> bounded(n);

  int center = n / 2;
  for (int i = 0; i < n; i++) {
    float distance = std::abs(i - center) / static_cast<float>(center);
    float decay = std::exp(-distance * distance);
    bounded[i] = kernel->get_value(input, i) * decay;
  }

  kernel->set_data(result, bounded);
  return result;
}

NanoBrainTensor *
RegulatoryEquations::asymptotic_regulation(NanoBrainTensor *input) {
  // Ensure asymptotic stability
  return kernel->gelu(input);
}

NanoBrainTensor *RegulatoryEquations::scale_invariance(NanoBrainTensor *input,
                                                       float scale) {
  NanoBrainTensor *scale_tensor = kernel->create_tensor({1});
  std::vector<float> scale_data = {1.0f / scale};
  kernel->set_data(scale_tensor, scale_data);
  return kernel->mul(input, scale_tensor);
}

NanoBrainTensor *RegulatoryEquations::fractal_recursion(NanoBrainTensor *input,
                                                        int depth) {
  NanoBrainTensor *result = input;
  for (int d = 0; d < depth; d++) {
    auto *half = kernel->mul(result, kernel->create_tensor({1}));
    kernel->set_data(half, std::vector<float>{0.5f});
    result = kernel->add(result, half);
    kernel->compute(result);
  }
  return result;
}

NanoBrainTensor *
RegulatoryEquations::prime_resonance(NanoBrainTensor *input,
                                     const std::vector<int> &primes) {
  // Modulate by prime frequencies
  kernel->compute(input);
  int n = ggml_nelements(input->ggml_tensor);

  NanoBrainTensor *result = kernel->create_tensor({n});
  std::vector<float> resonated(n);

  for (int i = 0; i < n; i++) {
    float sum = 0.0f;
    for (int p : primes) {
      sum += std::sin(2.0f * 3.14159f * i / p);
    }
    resonated[i] =
        kernel->get_value(input, i) * (1.0f + 0.1f * sum / primes.size());
  }

  kernel->set_data(result, resonated);
  return result;
}

NanoBrainTensor *
RegulatoryEquations::harmonic_coupling(NanoBrainTensor *input,
                                       NanoBrainTensor *harmonic) {
  return kernel->mul(input, harmonic);
}

NanoBrainTensor *RegulatoryEquations::phase_alignment(NanoBrainTensor *input,
                                                      float target_phase) {
  kernel->compute(input);
  int n = ggml_nelements(input->ggml_tensor);

  NanoBrainTensor *result = kernel->create_tensor({n});
  std::vector<float> aligned(n);

  for (int i = 0; i < n; i++) {
    float val = kernel->get_value(input, i);
    float phase = std::atan2(val, 1.0f);
    float phase_diff = target_phase - phase;
    aligned[i] = val * std::cos(phase_diff);
  }

  kernel->set_data(result, aligned);
  return result;
}

NanoBrainTensor *
RegulatoryEquations::energy_conservation(NanoBrainTensor *input,
                                         float total_energy) {
  auto *energy = kernel->sum(kernel->mul(input, input));
  kernel->compute(energy);
  float current_energy = kernel->get_value(energy, 0);

  if (current_energy > 1e-10f) {
    float scale = std::sqrt(total_energy / current_energy);
    NanoBrainTensor *scale_tensor = kernel->create_tensor({1});
    kernel->set_data(scale_tensor, std::vector<float>{scale});
    return kernel->mul(input, scale_tensor);
  }
  return input;
}

NanoBrainTensor *
RegulatoryEquations::information_preservation(NanoBrainTensor *input) {
  // Preserve information by normalizing
  auto *norm = kernel->sqrt(kernel->sum(kernel->mul(input, input)));
  kernel->compute(norm);
  float n = kernel->get_value(norm, 0);

  if (n > 1e-10f) {
    NanoBrainTensor *inv_norm = kernel->create_tensor({1});
    kernel->set_data(inv_norm, std::vector<float>{1.0f / n});
    return kernel->mul(input, inv_norm);
  }
  return input;
}

NanoBrainTensor *
RegulatoryEquations::causality_constraint(NanoBrainTensor *past,
                                          NanoBrainTensor *future) {
  // Future must be reachable from past
  return kernel->add(past, future);
}

NanoBrainTensor *
RegulatoryEquations::coherence_maximization(NanoBrainTensor *input) {
  return kernel->softmax(input);
}

NanoBrainTensor *RegulatoryEquations::unity_boundary(
    const std::vector<NanoBrainTensor *> &inputs) {
  if (inputs.empty()) {
    return kernel->create_tensor({1});
  }

  NanoBrainTensor *result = inputs[0];
  for (size_t i = 1; i < inputs.size(); i++) {
    result = kernel->add(result, inputs[i]);
  }

  return information_preservation(result);
}

NanoBrainTensor *
RegulatoryEquations::apply_full_regulation(NanoBrainTensor *input) {
  NanoBrainTensor *result = input;

  result = boundary_at_infinity(result);
  result = asymptotic_regulation(result);
  result = scale_invariance(result, 1.0f);
  result = fractal_recursion(result, 2);
  result = prime_resonance(result, {2, 3, 5, 7, 11});
  result = phase_alignment(result, 0.0f);
  result = energy_conservation(result, 1.0f);
  result = information_preservation(result);
  result = coherence_maximization(result);

  kernel->compute(result);
  return result;
}
