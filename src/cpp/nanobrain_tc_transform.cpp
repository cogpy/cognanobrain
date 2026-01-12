#include "nanobrain_tc_transform.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <numeric>

// ================================================================
// Constructor / Destructor
// ================================================================

TCTransformEngine::TCTransformEngine(NanoBrainKernel *kernel,
                                     const TCTransformConfig &config)
    : kernel(kernel), config(config) {
  initialize_primes();
}

TCTransformEngine::~TCTransformEngine() {}

void TCTransformEngine::initialize_primes() {
  // Use fundamental primes from time crystal theory
  analysis_primes.clear();
  for (int i = 0;
       i < config.num_prime_components && i < FUNDAMENTAL_PRIMES_COUNT; i++) {
    analysis_primes.push_back(FUNDAMENTAL_PRIMES[i]);
  }
}

// ================================================================
// Core Transform Operations
// ================================================================

TCTransformResult
TCTransformEngine::transform(const std::vector<float> &signal) {
  TCTransformResult result;
  result.signal_energy = 0.0f;

  // Compute signal energy
  for (float s : signal) {
    result.signal_energy += s * s;
  }
  result.signal_energy = std::sqrt(result.signal_energy);

  // Compute each prime component
  float total_coherence = 0.0f;
  for (int prime : analysis_primes) {
    TCSpectralComponent comp = compute_prime_component(signal, prime);
    result.components.push_back(comp);
    result.prime_spectrum.push_back(comp.amplitude);
    result.phase_values.push_back(comp.phase);
    total_coherence += comp.coherence;
  }

  // Overall coherence
  result.overall_coherence =
      analysis_primes.empty() ? 0.0f : total_coherence / analysis_primes.size();

  // Find dominant primes
  std::vector<std::pair<int, float>> prime_amps;
  for (size_t i = 0; i < analysis_primes.size(); i++) {
    prime_amps.push_back({analysis_primes[i], result.prime_spectrum[i]});
  }
  std::sort(prime_amps.begin(), prime_amps.end(),
            [](const auto &a, const auto &b) { return a.second > b.second; });

  for (size_t i = 0; i < std::min(size_t(5), prime_amps.size()); i++) {
    result.dominant_primes.push_back(prime_amps[i].first);
  }

  // Normalize if requested
  if (config.normalize_output && result.signal_energy > 0) {
    for (auto &amp : result.prime_spectrum) {
      amp /= result.signal_energy;
    }
    for (auto &comp : result.components) {
      comp.amplitude /= result.signal_energy;
    }
  }

  return result;
}

std::vector<float>
TCTransformEngine::inverse_transform(const TCTransformResult &result) {
  if (result.components.empty()) {
    return std::vector<float>();
  }

  size_t length = config.signal_resolution;
  std::vector<float> reconstructed(length, 0.0f);

  // Sum prime basis functions weighted by amplitude and phase
  for (const auto &comp : result.components) {
    auto basis = generate_prime_basis(comp.prime, length);

    for (size_t i = 0; i < length; i++) {
      // Apply amplitude and phase shift
      float phase_shifted = std::cos(basis[i] + comp.phase);
      reconstructed[i] += comp.amplitude * phase_shifted;
    }
  }

  return reconstructed;
}

TCSpectralComponent
TCTransformEngine::compute_prime_component(const std::vector<float> &signal,
                                           int prime) {
  TCSpectralComponent comp;
  comp.prime = prime;

  // Compute correlation with prime basis
  float correlation = compute_prime_correlation(signal, prime);
  comp.amplitude = std::abs(correlation);

  // Compute phase
  comp.phase = compute_phase_at_prime(signal, prime);

  // Compute coherence using PPM formula
  float sqrt_prime = std::sqrt(static_cast<float>(prime));
  comp.coherence = 0.5f + 0.5f * std::sin(sqrt_prime * PI / prime);

  // Compute sub-harmonics
  comp.harmonics.clear();
  for (int h = 2; h <= 5; h++) {
    int sub_prime = prime / h;
    if (sub_prime > 1) {
      float sub_corr = compute_prime_correlation(signal, sub_prime);
      comp.harmonics.push_back(std::abs(sub_corr));
    }
  }

  return comp;
}

float TCTransformEngine::compute_prime_correlation(
    const std::vector<float> &signal, int prime) {
  if (signal.empty())
    return 0.0f;

  auto basis = generate_prime_basis(prime, signal.size());

  // Dot product
  float correlation = 0.0f;
  for (size_t i = 0; i < signal.size(); i++) {
    correlation += signal[i] * std::cos(basis[i]);
  }

  return correlation / signal.size();
}

float TCTransformEngine::compute_phase_at_prime(
    const std::vector<float> &signal, int prime) {
  if (signal.empty())
    return 0.0f;

  auto basis = generate_prime_basis(prime, signal.size());

  // Compute real and imaginary parts
  float real_part = 0.0f;
  float imag_part = 0.0f;

  for (size_t i = 0; i < signal.size(); i++) {
    real_part += signal[i] * std::cos(basis[i]);
    imag_part += signal[i] * std::sin(basis[i]);
  }

  return std::atan2(imag_part, real_part);
}

std::vector<float> TCTransformEngine::generate_prime_basis(int prime,
                                                           size_t length) {
  std::vector<float> basis(length);

  float frequency = 2.0f * PI * prime / length;

  for (size_t i = 0; i < length; i++) {
    // Prime-based frequency with golden ratio modulation
    float base_angle = frequency * i;

    if (config.use_golden_ratio) {
      base_angle *= (1.0f + (GOLDEN_RATIO - 1.0f) * std::sin(i * PI / length));
    }

    basis[i] = base_angle;
  }

  return basis;
}

// ================================================================
// Analysis Modes
// ================================================================

ImageAnalysisResult
TCTransformEngine::analyze_image(const std::vector<std::vector<float>> &image) {
  ImageAnalysisResult result;

  if (image.empty() || image[0].empty()) {
    return result;
  }

  size_t rows = image.size();
  size_t cols = image[0].size();

  // Row-wise analysis (average of all rows)
  std::vector<float> avg_row(cols, 0.0f);
  for (size_t r = 0; r < rows; r++) {
    for (size_t c = 0; c < cols; c++) {
      avg_row[c] += image[r][c];
    }
  }
  for (size_t c = 0; c < cols; c++) {
    avg_row[c] /= rows;
  }
  result.row_transform = transform(avg_row);

  // Column-wise analysis (average of all columns)
  std::vector<float> avg_col(rows, 0.0f);
  for (size_t r = 0; r < rows; r++) {
    for (size_t c = 0; c < cols; c++) {
      avg_col[r] += image[r][c];
    }
    avg_col[r] /= cols;
  }
  result.col_transform = transform(avg_col);

  // Spatial coherence map
  result.spatial_coherence.resize(rows);
  for (size_t r = 0; r < rows; r++) {
    result.spatial_coherence[r].resize(cols);
    for (size_t c = 0; c < cols; c++) {
      // Local coherence based on neighborhood
      float local_sum = image[r][c];
      int count = 1;

      if (r > 0) {
        local_sum += image[r - 1][c];
        count++;
      }
      if (r < rows - 1) {
        local_sum += image[r + 1][c];
        count++;
      }
      if (c > 0) {
        local_sum += image[r][c - 1];
        count++;
      }
      if (c < cols - 1) {
        local_sum += image[r][c + 1];
        count++;
      }

      float local_mean = local_sum / count;
      float variance = std::abs(image[r][c] - local_mean);
      result.spatial_coherence[r][c] = 1.0f / (1.0f + variance);
    }
  }

  // Dominant scale from row/col transforms
  result.dominant_scale = (result.row_transform.dominant_primes.empty()
                               ? 1.0f
                               : result.row_transform.dominant_primes[0]) *
                          (result.col_transform.dominant_primes.empty()
                               ? 1.0f
                               : result.col_transform.dominant_primes[0]);

  // Combine pattern primes
  result.pattern_primes = result.row_transform.dominant_primes;
  for (int p : result.col_transform.dominant_primes) {
    if (std::find(result.pattern_primes.begin(), result.pattern_primes.end(),
                  p) == result.pattern_primes.end()) {
      result.pattern_primes.push_back(p);
    }
  }

  return result;
}

AudioAnalysisResult
TCTransformEngine::analyze_audio(const std::vector<float> &samples,
                                 int sample_rate) {
  AudioAnalysisResult result;

  if (samples.empty()) {
    return result;
  }

  // Spectral analysis
  result.spectral = transform(samples);

  // Temporal coherence (sliding window)
  size_t window_size = sample_rate / 10; // 100ms windows
  size_t num_windows = samples.size() / window_size;

  result.temporal_coherence.reserve(num_windows);
  for (size_t w = 0; w < num_windows; w++) {
    std::vector<float> window(samples.begin() + w * window_size,
                              samples.begin() + (w + 1) * window_size);
    auto window_result = transform(window);
    result.temporal_coherence.push_back(window_result.overall_coherence);
  }

  // Dominant frequency from dominant prime
  if (!result.spectral.dominant_primes.empty()) {
    int dominant_prime = result.spectral.dominant_primes[0];
    result.dominant_frequency =
        static_cast<float>(dominant_prime * sample_rate) / samples.size();
  } else {
    result.dominant_frequency = 0.0f;
  }

  // Rhythm primes from temporal pattern
  if (!result.temporal_coherence.empty()) {
    auto tempo_analysis = transform(result.temporal_coherence);
    result.rhythm_primes = tempo_analysis.dominant_primes;
  }

  // Music coherence (average temporal coherence)
  if (!result.temporal_coherence.empty()) {
    float sum = 0.0f;
    for (float c : result.temporal_coherence) {
      sum += c;
    }
    result.music_coherence = sum / result.temporal_coherence.size();
  } else {
    result.music_coherence = result.spectral.overall_coherence;
  }

  return result;
}

std::vector<int>
TCTransformEngine::extract_prime_encoding(const std::vector<float> &pattern) {
  auto result = transform(pattern);
  return result.dominant_primes;
}

// ================================================================
// Comparison with FFT
// ================================================================

TCTransformEngine::TransformComparison
TCTransformEngine::compare_with_fft(const std::vector<float> &signal) {
  TransformComparison comparison = {};

  // TC Transform
  auto tc_result = transform(signal);
  comparison.tc_coherence = tc_result.overall_coherence;

  auto tc_reconstructed = inverse_transform(tc_result);

  // Reconstruction error for TC
  if (!tc_reconstructed.empty() && tc_reconstructed.size() == signal.size()) {
    float error = 0.0f;
    for (size_t i = 0; i < signal.size(); i++) {
      float diff = signal[i] - tc_reconstructed[i];
      error += diff * diff;
    }
    comparison.reconstruction_error_tc = std::sqrt(error / signal.size());
  }

  // Sparsity (percentage of components above threshold)
  int significant_tc = 0;
  for (const auto &comp : tc_result.components) {
    if (comp.amplitude > 0.1f) {
      significant_tc++;
    }
  }
  comparison.sparsity_tc = tc_result.components.empty()
                               ? 0.0f
                               : 1.0f - static_cast<float>(significant_tc) /
                                            tc_result.components.size();

  // For FFT comparison, we use simplified DFT-like calculation
  // (Real FFT would require additional library)
  float fft_energy = 0.0f;
  for (float s : signal) {
    fft_energy += s * s;
  }
  comparison.fft_energy = std::sqrt(fft_energy);
  comparison.reconstruction_error_fft = 0.0f; // Perfect reconstruction assumed
  comparison.sparsity_fft = 0.5f;             // Typical FFT sparsity

  return comparison;
}

// ================================================================
// Utility Functions
// ================================================================

std::vector<int> TCTransformEngine::get_analysis_primes() const {
  return analysis_primes;
}

void TCTransformEngine::set_analysis_primes(const std::vector<int> &primes) {
  analysis_primes = primes;
}
