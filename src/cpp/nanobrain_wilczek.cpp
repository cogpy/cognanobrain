#include "nanobrain_wilczek.h"
#include <algorithm>
#include <chrono>
#include <cmath>
#include <numeric>
#include <random>

// ================================================================
// WilczekTimeCrystal Implementation
// ================================================================

WilczekTimeCrystal::WilczekTimeCrystal(const WilczekConfig &cfg)
    : config(cfg), initialized(false), period_count(0), total_time(0.0f) {}

WilczekTimeCrystal::~WilczekTimeCrystal() = default;

void WilczekTimeCrystal::initialize() {
  spins.clear();
  spins.resize(config.num_spins);

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<float> dis(-1.0f, 1.0f);
  std::normal_distribution<float> disorder(0.0f, config.disorder_strength);

  for (int i = 0; i < config.num_spins; i++) {
    spins[i].index = i;
    spins[i].z_component =
        (i % 2 == 0) ? 1.0f : -1.0f; // Antiferromagnetic init
    spins[i].x_component = 0.0f;
    spins[i].coupling = config.interaction_strength;
    spins[i].local_field = disorder(gen);
  }

  magnetization_history.clear();
  period_count = 0;
  total_time = 0.0f;
  initialized = true;
}

void WilczekTimeCrystal::reset() {
  spins.clear();
  magnetization_history.clear();
  period_count = 0;
  total_time = 0.0f;
  initialized = false;
}

// ================================================================
// Time Evolution
// ================================================================

void WilczekTimeCrystal::apply_floquet_drive() {
  // Apply π pulse (flip all spins) - imperfect rotation
  float rotation_error = 0.03f; // 3% error

  for (auto &spin : spins) {
    // Rotation around X axis - nearly π
    float angle = M_PI * (1.0f - rotation_error);
    float z_new = spin.z_component * std::cos(angle);
    float y_new = spin.z_component * std::sin(angle);
    spin.z_component = z_new;
  }
}

void WilczekTimeCrystal::apply_interaction() {
  // Ising interaction: H = -J Σ σ_i^z σ_{i+1}^z
  std::vector<float> new_z(config.num_spins);

  for (int i = 0; i < config.num_spins; i++) {
    float field = 0.0f;

    // Nearest neighbor interactions
    if (i > 0) {
      field += spins[i].coupling * spins[i - 1].z_component;
    }
    if (i < config.num_spins - 1) {
      field += spins[i].coupling * spins[i + 1].z_component;
    }

    // Local field from disorder
    field += spins[i].local_field;

    // Update spin (simplified dynamics)
    float dt = 1.0f / config.driving_frequency;
    new_z[i] = spins[i].z_component + field * dt * 0.1f;
    new_z[i] = std::max(-1.0f, std::min(1.0f, new_z[i])); // Clamp
  }

  for (int i = 0; i < config.num_spins; i++) {
    spins[i].z_component = new_z[i];
  }
}

void WilczekTimeCrystal::apply_disorder() {
  if (!config.many_body_localized) {
    // Without MBL, system thermalizes
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<float> noise(0.0f, 0.01f);

    for (auto &spin : spins) {
      spin.z_component += noise(gen);
      spin.z_component = std::max(-1.0f, std::min(1.0f, spin.z_component));
    }
  }
}

void WilczekTimeCrystal::evolve_period() {
  if (!initialized)
    return;

  // Store current magnetization for history
  std::vector<float> current_config = get_spin_configuration();
  magnetization_history.push_back(current_config);
  if (magnetization_history.size() > static_cast<size_t>(history_size)) {
    magnetization_history.erase(magnetization_history.begin());
  }

  // Floquet evolution: alternating drive and interaction
  for (int t = 0; t < config.driving_period; t++) {
    if (t == 0) {
      apply_floquet_drive(); // π pulse at start
    }
    apply_interaction();
    apply_disorder();
  }

  period_count++;
  total_time += 1.0f / config.driving_frequency;
}

void WilczekTimeCrystal::evolve(int periods) {
  for (int i = 0; i < periods; i++) {
    evolve_period();
  }
}

// ================================================================
// Measurements
// ================================================================

float WilczekTimeCrystal::measure_magnetization() const {
  if (spins.empty())
    return 0.0f;

  float sum = 0.0f;
  for (const auto &spin : spins) {
    sum += spin.z_component;
  }
  return sum / static_cast<float>(spins.size());
}

float WilczekTimeCrystal::measure_correlation(int i, int j) const {
  if (i < 0 || i >= static_cast<int>(spins.size()) || j < 0 ||
      j >= static_cast<int>(spins.size())) {
    return 0.0f;
  }
  return spins[i].z_component * spins[j].z_component;
}

float WilczekTimeCrystal::measure_coherence() const {
  if (spins.empty())
    return 0.0f;

  // Coherence as average of nearest-neighbor correlations
  float sum = 0.0f;
  for (int i = 0; i < static_cast<int>(spins.size()) - 1; i++) {
    sum += std::abs(measure_correlation(i, i + 1));
  }
  return sum / static_cast<float>(spins.size() - 1);
}

float WilczekTimeCrystal::measure_period() const {
  if (magnetization_history.size() < 4)
    return 0.0f;

  // Look for period doubling - magnetization should oscillate with period 2T
  // where T is the driving period

  // Compare magnetizations separated by different periods
  int best_period = 1;
  float best_correlation = 0.0f;

  for (int p = 1; p <= static_cast<int>(magnetization_history.size()) / 2;
       p++) {
    float corr = 0.0f;
    int count = 0;

    for (size_t i = p; i < magnetization_history.size(); i++) {
      // Correlation between configurations separated by p periods
      for (size_t j = 0; j < magnetization_history[i].size(); j++) {
        corr += magnetization_history[i][j] * magnetization_history[i - p][j];
      }
      count++;
    }
    corr /= std::max(1, count);

    if (std::abs(corr) > best_correlation) {
      best_correlation = std::abs(corr);
      best_period = p;
    }
  }

  return static_cast<float>(best_period);
}

bool WilczekTimeCrystal::has_time_crystal_signature() const {
  // Time crystal signature: period doubling (oscillates with period 2T)
  float measured = measure_period();
  return std::abs(measured - 2.0f) < 0.5f;
}

WilczekPhase WilczekTimeCrystal::current_phase() const {
  float coherence = measure_coherence();
  bool has_tc = has_time_crystal_signature();

  if (coherence < 0.1f) {
    return WilczekPhase::Melted;
  } else if (has_tc && coherence > 0.5f) {
    return WilczekPhase::DiscreteTC;
  } else if (coherence > 0.3f) {
    return WilczekPhase::ContinuousTC;
  } else {
    return WilczekPhase::Normal;
  }
}

std::string WilczekTimeCrystal::phase_name() const {
  switch (current_phase()) {
  case WilczekPhase::Normal:
    return "Normal";
  case WilczekPhase::DiscreteTC:
    return "Discrete Time Crystal";
  case WilczekPhase::ContinuousTC:
    return "Continuous/Prethermal";
  case WilczekPhase::Melted:
    return "Melted/Thermalized";
  default:
    return "Unknown";
  }
}

// ================================================================
// Comparison
// ================================================================

TimeCrystalComparison WilczekTimeCrystal::compare_with_nanobrain(
    const TimeCrystalKernel &nb_tc) const {
  TimeCrystalComparison result;

  result.wilczek_coherence = measure_coherence();
  result.wilczek_period = measure_period();

  // Get NanoBrain metrics
  auto nb_metrics = nb_tc.get_metrics();
  result.nanobrain_coherence = nb_metrics.quantum_coherence;
  result.nanobrain_period = 1.0f; // NanoBrain uses continuous time

  // Correlation between approaches
  // Higher when both show similar coherence levels
  result.correlation =
      1.0f - std::abs(result.nanobrain_coherence - result.wilczek_coherence);

  // Phase alignment
  result.phase_alignment =
      std::min(result.nanobrain_coherence, result.wilczek_coherence);

  // Determine dominant approach
  if (result.nanobrain_coherence > result.wilczek_coherence) {
    result.dominant_approach = "NanoBrain";
  } else {
    result.dominant_approach = "Wilczek";
  }

  return result;
}

TimeCrystalBenchmark WilczekTimeCrystal::run_benchmark(TimeCrystalKernel &nb_tc,
                                                       int test_cycles) {
  TimeCrystalBenchmark result;
  result.cycles_tested = test_cycles;

  // Initial coherences
  float nb_initial = nb_tc.get_metrics().quantum_coherence;
  float w_initial = measure_coherence();

  // Time NanoBrain
  auto start_nb = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < test_cycles; i++) {
    nb_tc.process_cycle();
  }
  auto end_nb = std::chrono::high_resolution_clock::now();
  result.nanobrain_compute_time_ms =
      std::chrono::duration<float, std::milli>(end_nb - start_nb).count();

  // Time Wilczek
  auto start_w = std::chrono::high_resolution_clock::now();
  evolve(test_cycles);
  auto end_w = std::chrono::high_resolution_clock::now();
  result.wilczek_compute_time_ms =
      std::chrono::duration<float, std::milli>(end_w - start_w).count();

  // Final coherences
  float nb_final = nb_tc.get_metrics().quantum_coherence;
  float w_final = measure_coherence();

  // Stability = how well coherence is maintained
  result.nanobrain_stability = nb_final / std::max(0.001f, nb_initial);
  result.wilczek_stability = w_final / std::max(0.001f, w_initial);

  // Decay rates
  result.coherence_decay_rate_nb = (nb_initial - nb_final) / test_cycles;
  result.coherence_decay_rate_w = (w_initial - w_final) / test_cycles;

  // Recommendation
  float nb_score = result.nanobrain_stability /
                   std::max(0.001f, result.nanobrain_compute_time_ms);
  float w_score = result.wilczek_stability /
                  std::max(0.001f, result.wilczek_compute_time_ms);

  if (nb_score > w_score * 1.2f) {
    result.recommendation = "NanoBrain recommended for stability/performance";
  } else if (w_score > nb_score * 1.2f) {
    result.recommendation = "Wilczek recommended for this workload";
  } else {
    result.recommendation = "Both approaches perform similarly";
  }

  return result;
}

// ================================================================
// Spin Access
// ================================================================

const WilczekSpin *WilczekTimeCrystal::get_spin(int index) const {
  if (index < 0 || index >= static_cast<int>(spins.size()))
    return nullptr;
  return &spins[index];
}

std::vector<float> WilczekTimeCrystal::get_spin_configuration() const {
  std::vector<float> config;
  config.reserve(spins.size());
  for (const auto &spin : spins) {
    config.push_back(spin.z_component);
  }
  return config;
}

void WilczekTimeCrystal::set_spin_configuration(const std::vector<float> &cfg) {
  for (size_t i = 0; i < std::min(cfg.size(), spins.size()); i++) {
    spins[i].z_component = cfg[i];
  }
}

void WilczekTimeCrystal::update_config(const WilczekConfig &new_config) {
  config = new_config;
  // Reinitialize if size changed
  if (static_cast<int>(spins.size()) != config.num_spins) {
    initialize();
  }
}

float WilczekTimeCrystal::compute_hamiltonian() const {
  float energy = 0.0f;

  // Ising energy
  for (int i = 0; i < static_cast<int>(spins.size()) - 1; i++) {
    energy -=
        spins[i].coupling * spins[i].z_component * spins[i + 1].z_component;
  }

  // Disorder energy
  for (const auto &spin : spins) {
    energy -= spin.local_field * spin.z_component;
  }

  return energy;
}

// ================================================================
// Utility Functions
// ================================================================

WilczekTimeCrystal create_default_wilczek_tc() {
  WilczekConfig config;
  config.num_spins = 100;
  config.driving_frequency = 1.0f;
  config.interaction_strength = 0.5f;
  config.disorder_strength = 0.15f;
  config.driving_period = 10;
  config.many_body_localized = true;

  WilczekTimeCrystal tc(config);
  tc.initialize();
  return tc;
}

TimeCrystalComparison quick_compare(const TimeCrystalKernel &nanobrain,
                                    const WilczekTimeCrystal &wilczek) {
  return wilczek.compare_with_nanobrain(nanobrain);
}
