#include "nanobrain_tc_transform.h"
#include <algorithm>
#include <chrono>
#include <cmath>
#include <numeric>

// ================================================================
// TimeCrystalTransform Implementation
// ================================================================

TimeCrystalTransform::TimeCrystalTransform(NanoBrainKernel *k,
                                           const TCTransformConfig &cfg)
    : kernel(k), config(cfg), initialized(false) {}

TimeCrystalTransform::~TimeCrystalTransform() = default;

void TimeCrystalTransform::initialize() {
  initialize_oscillators();
  initialized = true;
}

void TimeCrystalTransform::reset() {
  oscillator_phases.clear();
  oscillator_frequencies.clear();
  oscillator_primes.clear();
  initialized = false;
}

int64_t TimeCrystalTransform::current_time_us() const {
  return std::chrono::duration_cast<std::chrono::microseconds>(
             std::chrono::high_resolution_clock::now().time_since_epoch())
      .count();
}

void TimeCrystalTransform::initialize_oscillators() {
  oscillator_phases.resize(config.num_crystals);
  oscillator_frequencies.resize(config.num_crystals);
  oscillator_primes.resize(config.num_crystals);

  // Initialize based on basis type
  for (int i = 0; i < config.num_crystals; i++) {
    oscillator_phases[i] = 0.0f;

    switch (config.basis) {
    case TCBasisType::Prime:
      oscillator_primes[i] = FUNDAMENTAL_PRIMES[i % 15];
      oscillator_frequencies[i] = static_cast<float>(oscillator_primes[i]);
      break;

    case TCBasisType::Fibonacci: {
      int fib_n = (i < 2) ? 1 : 0;
      int fib_prev = 1, fib_curr = 1;
      for (int j = 2; j <= i; j++) {
        fib_n = fib_prev + fib_curr;
        fib_prev = fib_curr;
        fib_curr = fib_n;
      }
      oscillator_frequencies[i] = static_cast<float>(fib_n);
      oscillator_primes[i] = fib_n;
    } break;

    case TCBasisType::Golden:
      oscillator_frequencies[i] = std::pow(1.618033988749895f, i);
      oscillator_primes[i] = static_cast<int>(oscillator_frequencies[i]);
      break;

    case TCBasisType::Harmonic:
      oscillator_frequencies[i] = static_cast<float>(i + 1);
      oscillator_primes[i] = i + 1;
      break;

    case TCBasisType::Fractal:
      oscillator_frequencies[i] =
          std::pow(2.0f, static_cast<float>(i) / 12.0f); // Octave divisions
      oscillator_primes[i] = static_cast<int>(oscillator_frequencies[i] * 10);
      break;
    }
  }
}

std::vector<float> TimeCrystalTransform::generate_basis_function(int index,
                                                                 int length) {
  std::vector<float> basis(length);
  float freq = oscillator_frequencies[index];
  float phase = oscillator_phases[index];

  for (int i = 0; i < length; i++) {
    float t = static_cast<float>(i) / length;
    basis[i] = std::cos(2.0f * M_PI * freq * t + phase);
  }

  return basis;
}

float TimeCrystalTransform::correlate_with_oscillator(
    const std::vector<float> &signal, int osc_idx) {
  if (signal.empty())
    return 0.0f;

  auto basis =
      generate_basis_function(osc_idx, static_cast<int>(signal.size()));

  float sum = 0.0f;
  for (size_t i = 0; i < signal.size(); i++) {
    sum += signal[i] * basis[i];
  }

  return sum / signal.size();
}

// ================================================================
// Forward Transform
// ================================================================

TCTransformResult
TimeCrystalTransform::forward(const std::vector<float> &signal) {
  TCTransformResult result;
  auto start = current_time_us();

  result.components.reserve(config.num_crystals);
  result.total_energy = 0.0f;

  for (int i = 0; i < config.num_crystals; i++) {
    float correlation = correlate_with_oscillator(signal, i);

    // Also compute phase using shifted basis
    oscillator_phases[i] = M_PI / 2.0f;
    float sin_correlation = correlate_with_oscillator(signal, i);
    oscillator_phases[i] = 0.0f;

    float amplitude = std::sqrt(correlation * correlation +
                                sin_correlation * sin_correlation) *
                      2.0f;
    float phase = std::atan2(sin_correlation, correlation);

    if (amplitude > config.coherence_threshold * 0.1f) {
      TCSpectralComponent comp;
      comp.frequency = oscillator_frequencies[i];
      comp.amplitude = amplitude;
      comp.phase = phase;
      comp.prime_index = oscillator_primes[i];
      comp.coherence = amplitude;
      result.components.push_back(comp);
      result.total_energy += amplitude * amplitude;
    }
  }

  result.total_energy = std::sqrt(result.total_energy);
  result.compute_time_us = current_time_us() - start;

  // Calculate reconstruction error
  auto reconstructed = inverse(result);
  float error = 0.0f;
  for (size_t i = 0; i < std::min(signal.size(), reconstructed.size()); i++) {
    float diff = signal[i] - reconstructed[i];
    error += diff * diff;
  }
  result.reconstruction_error = std::sqrt(error / signal.size());

  return result;
}

TCTransformResult
TimeCrystalTransform::forward_2d(const std::vector<std::vector<float>> &image) {
  // Flatten 2D to 1D and transform
  std::vector<float> flat;
  for (const auto &row : image) {
    flat.insert(flat.end(), row.begin(), row.end());
  }
  return forward(flat);
}

TCTransformResult TimeCrystalTransform::forward_complex(
    const std::vector<std::complex<float>> &signal) {
  // Extract real part and transform
  std::vector<float> real_part;
  real_part.reserve(signal.size());
  for (const auto &c : signal) {
    real_part.push_back(c.real());
  }
  return forward(real_part);
}

// ================================================================
// Inverse Transform
// ================================================================

std::vector<float>
TimeCrystalTransform::inverse(const TCTransformResult &spectrum) {
  // Determine length from first call or default
  int length = 256;
  std::vector<float> result(length, 0.0f);

  for (const auto &comp : spectrum.components) {
    for (int i = 0; i < length; i++) {
      float t = static_cast<float>(i) / length;
      result[i] += comp.amplitude *
                   std::cos(2.0f * M_PI * comp.frequency * t + comp.phase);
    }
  }

  return result;
}

std::vector<std::vector<float>>
TimeCrystalTransform::inverse_2d(const TCTransformResult &spectrum, int width,
                                 int height) {
  auto flat = inverse(spectrum);
  flat.resize(width * height, 0.0f);

  std::vector<std::vector<float>> image(height, std::vector<float>(width));
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      image[y][x] = flat[y * width + x];
    }
  }

  return image;
}

// ================================================================
// Analysis Functions
// ================================================================

std::vector<float> TimeCrystalTransform::find_dominant_frequencies(
    const std::vector<float> &signal, int top_k) {
  auto result = forward(signal);

  // Sort by amplitude
  std::sort(
      result.components.begin(), result.components.end(),
      [](const auto &a, const auto &b) { return a.amplitude > b.amplitude; });

  std::vector<float> freqs;
  for (int i = 0;
       i < std::min(top_k, static_cast<int>(result.components.size())); i++) {
    freqs.push_back(result.components[i].frequency);
  }

  return freqs;
}

std::vector<int>
TimeCrystalTransform::detect_periods(const std::vector<float> &signal) {
  auto freqs = find_dominant_frequencies(signal, 5);

  std::vector<int> periods;
  for (float f : freqs) {
    if (f > 0.001f) {
      periods.push_back(static_cast<int>(1.0f / f * signal.size()));
    }
  }

  return periods;
}

float TimeCrystalTransform::calculate_coherence(
    const std::vector<float> &signal) {
  auto result = forward(signal);
  return result.total_energy /
         std::max(1.0f, static_cast<float>(signal.size()));
}

// ================================================================
// Filtering
// ================================================================

std::vector<float>
TimeCrystalTransform::lowpass(const std::vector<float> &signal, float cutoff) {
  auto spectrum = forward(signal);

  // Remove high frequency components
  spectrum.components.erase(
      std::remove_if(spectrum.components.begin(), spectrum.components.end(),
                     [cutoff](const auto &c) { return c.frequency > cutoff; }),
      spectrum.components.end());

  auto result = inverse(spectrum);
  result.resize(signal.size());
  return result;
}

std::vector<float>
TimeCrystalTransform::highpass(const std::vector<float> &signal, float cutoff) {
  auto spectrum = forward(signal);

  spectrum.components.erase(
      std::remove_if(spectrum.components.begin(), spectrum.components.end(),
                     [cutoff](const auto &c) { return c.frequency < cutoff; }),
      spectrum.components.end());

  auto result = inverse(spectrum);
  result.resize(signal.size());
  return result;
}

std::vector<float>
TimeCrystalTransform::bandpass(const std::vector<float> &signal, float low,
                               float high) {
  auto spectrum = forward(signal);

  spectrum.components.erase(
      std::remove_if(spectrum.components.begin(), spectrum.components.end(),
                     [low, high](const auto &c) {
                       return c.frequency < low || c.frequency > high;
                     }),
      spectrum.components.end());

  auto result = inverse(spectrum);
  result.resize(signal.size());
  return result;
}

// ================================================================
// Benchmarking
// ================================================================

TCBenchmarkResult
TimeCrystalTransform::benchmark_vs_fft(const std::vector<float> &signal) {
  TCBenchmarkResult result;
  result.signal_length = static_cast<int>(signal.size());

  // Time TC transform
  auto start_tc = current_time_us();
  auto tc_result = forward(signal);
  result.tc_time_ms =
      static_cast<float>(current_time_us() - start_tc) / 1000.0f;
  result.tc_accuracy = 1.0f - tc_result.reconstruction_error;

  // Time simple DFT
  auto start_fft = current_time_us();
  auto dft_result = simple_dft(signal);
  auto reconstructed = simple_idft(dft_result, static_cast<int>(signal.size()));
  result.fft_time_ms =
      static_cast<float>(current_time_us() - start_fft) / 1000.0f;

  // Calculate FFT accuracy
  float fft_error = 0.0f;
  for (size_t i = 0; i < signal.size(); i++) {
    float diff = signal[i] - reconstructed[i];
    fft_error += diff * diff;
  }
  result.fft_accuracy = 1.0f - std::sqrt(fft_error / signal.size());

  result.speedup_ratio =
      result.fft_time_ms / std::max(0.001f, result.tc_time_ms);

  if (result.speedup_ratio > 1.5f && result.tc_accuracy > 0.8f) {
    result.recommendation = "TC Transform recommended";
  } else if (result.fft_accuracy > result.tc_accuracy * 1.1f) {
    result.recommendation = "FFT recommended for accuracy";
  } else {
    result.recommendation = "Both perform similarly";
  }

  return result;
}

void TimeCrystalTransform::update_config(const TCTransformConfig &new_config) {
  config = new_config;
  if (initialized) {
    initialize_oscillators();
  }
}

// ================================================================
// Utility Functions
// ================================================================

std::vector<std::complex<float>> simple_dft(const std::vector<float> &signal) {
  int N = static_cast<int>(signal.size());
  std::vector<std::complex<float>> result(N);

  for (int k = 0; k < N; k++) {
    std::complex<float> sum(0.0f, 0.0f);
    for (int n = 0; n < N; n++) {
      float angle = -2.0f * M_PI * k * n / N;
      sum += signal[n] * std::complex<float>(std::cos(angle), std::sin(angle));
    }
    result[k] = sum;
  }

  return result;
}

std::vector<float> simple_idft(const std::vector<std::complex<float>> &spectrum,
                               int length) {
  int N = length;
  std::vector<float> result(N);

  for (int n = 0; n < N; n++) {
    float sum = 0.0f;
    for (size_t k = 0; k < spectrum.size(); k++) {
      float angle = 2.0f * M_PI * k * n / N;
      sum += spectrum[k].real() * std::cos(angle) -
             spectrum[k].imag() * std::sin(angle);
    }
    result[n] = sum / N;
  }

  return result;
}
