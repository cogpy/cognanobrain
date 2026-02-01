#ifndef NANOBRAIN_TC_TRANSFORM_H
#define NANOBRAIN_TC_TRANSFORM_H

/**
 * Time Crystal Transform - Chapter 5, Task 5.2
 *
 * Signal analysis without Fourier - using time crystal synchronization
 * for frequency decomposition and pattern recognition.
 */

#include "nanobrain_kernel.h"
#include "nanobrain_time_crystal.h"
#include <complex>
#include <functional>
#include <vector>

// ================================================================
// Transform Modes
// ================================================================

enum class TCTransformMode {
  Signal1D, // 1D time series
  Image2D,  // 2D image data
  Sound,    // Audio analysis
  Video,    // 3D video frames
  Custom    // User-defined
};

enum class TCBasisType {
  Prime,     // Prime number basis
  Fibonacci, // Fibonacci sequence
  Golden,    // Golden ratio based
  Harmonic,  // Musical harmonics
  Fractal    // Self-similar basis
};

// ================================================================
// Transform Configuration
// ================================================================

struct TCTransformConfig {
  TCTransformMode mode = TCTransformMode::Signal1D;
  TCBasisType basis = TCBasisType::Prime;
  int num_crystals = 64;   // Number of TC oscillators
  int max_frequency = 100; // Max frequency to analyze
  float coherence_threshold = 0.5f;
  bool use_gpu = false;
};

// ================================================================
// Transform Results
// ================================================================

struct TCSpectralComponent {
  float frequency;
  float amplitude;
  float phase;
  int prime_index; // Which prime resonated
  float coherence;
};

struct TCTransformResult {
  std::vector<TCSpectralComponent> components;
  float total_energy;
  float reconstruction_error;
  int64_t compute_time_us;
};

struct TCBenchmarkResult {
  int signal_length;
  float tc_time_ms;
  float fft_time_ms;
  float tc_accuracy;
  float fft_accuracy;
  float speedup_ratio;
  std::string recommendation;
};

// ================================================================
// Time Crystal Transform Class
// ================================================================

class TimeCrystalTransform {
public:
  TimeCrystalTransform(NanoBrainKernel *kernel,
                       const TCTransformConfig &config = {});
  ~TimeCrystalTransform();

  // ================================================================
  // Initialization
  // ================================================================

  void initialize();
  void reset();
  bool is_initialized() const { return initialized; }

  // ================================================================
  // Forward Transform (Signal → Spectrum)
  // ================================================================

  // Transform 1D signal
  TCTransformResult forward(const std::vector<float> &signal);

  // Transform 2D image
  TCTransformResult forward_2d(const std::vector<std::vector<float>> &image);

  // Transform complex signal
  TCTransformResult
  forward_complex(const std::vector<std::complex<float>> &signal);

  // ================================================================
  // Inverse Transform (Spectrum → Signal)
  // ================================================================

  // Reconstruct 1D signal
  std::vector<float> inverse(const TCTransformResult &spectrum);

  // Reconstruct 2D image
  std::vector<std::vector<float>> inverse_2d(const TCTransformResult &spectrum,
                                             int width, int height);

  // ================================================================
  // Analysis Functions
  // ================================================================

  // Find dominant frequencies
  std::vector<float> find_dominant_frequencies(const std::vector<float> &signal,
                                               int top_k = 5);

  // Detect periodic patterns
  std::vector<int> detect_periods(const std::vector<float> &signal);

  // Calculate signal coherence
  float calculate_coherence(const std::vector<float> &signal);

  // ================================================================
  // Filtering
  // ================================================================

  // Low-pass filter using TC
  std::vector<float> lowpass(const std::vector<float> &signal, float cutoff);

  // High-pass filter using TC
  std::vector<float> highpass(const std::vector<float> &signal, float cutoff);

  // Band-pass filter using TC
  std::vector<float> bandpass(const std::vector<float> &signal, float low,
                              float high);

  // ================================================================
  // Benchmarking
  // ================================================================

  // Compare with FFT
  TCBenchmarkResult benchmark_vs_fft(const std::vector<float> &signal);

  // ================================================================
  // Configuration
  // ================================================================

  const TCTransformConfig &get_config() const { return config; }
  void update_config(const TCTransformConfig &new_config);

private:
  NanoBrainKernel *kernel;
  TCTransformConfig config;
  bool initialized = false;

  // Time crystal oscillators
  std::vector<float> oscillator_phases;
  std::vector<float> oscillator_frequencies;
  std::vector<int> oscillator_primes;

  // Private helpers
  void initialize_oscillators();
  float correlate_with_oscillator(const std::vector<float> &signal,
                                  int osc_idx);
  std::vector<float> generate_basis_function(int index, int length);
  int64_t current_time_us() const;
};

// ================================================================
// Utility Functions
// ================================================================

// Simple DFT for comparison (not optimized)
std::vector<std::complex<float>> simple_dft(const std::vector<float> &signal);

// Inverse DFT
std::vector<float> simple_idft(const std::vector<std::complex<float>> &spectrum,
                               int length);

#endif // NANOBRAIN_TC_TRANSFORM_H
