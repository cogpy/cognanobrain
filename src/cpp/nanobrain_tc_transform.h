#ifndef NANOBRAIN_TC_TRANSFORM_H
#define NANOBRAIN_TC_TRANSFORM_H

/**
 * NanoBrain Time Crystal Transform Engine
 *
 * Chapter 5: Universal Time Crystal & Big Data
 *
 * Implements Time Crystal Transform (TCT) as an alternative to FFT,
 * using prime-based harmonic analysis instead of Fourier decomposition.
 *
 * Key advantages:
 * - Pattern recognition without explicit frequency domain
 * - Prime-coherent signal decomposition
 * - Native support for fractal signals
 */

#include "nanobrain_kernel.h"
#include "nanobrain_time_crystal.h"
#include <string>
#include <vector>

/**
 * Time Crystal Transform configuration
 */
struct TCTransformConfig {
  int num_prime_components = 15;    // Number of prime frequency bins
  int signal_resolution = 1024;     // Default signal length
  float coherence_threshold = 0.5f; // Minimum coherence for component
  bool normalize_output = true;     // Normalize spectrum to [0, 1]
  bool use_golden_ratio = true;     // Apply golden ratio modulation
};

/**
 * Single component in the TC spectrum
 */
struct TCSpectralComponent {
  int prime;                    // Prime number for this component
  float amplitude;              // Component amplitude
  float phase;                  // Phase angle
  float coherence;              // Coherence with signal
  std::vector<float> harmonics; // Sub-harmonics
};

/**
 * Time Crystal Transform result
 */
struct TCTransformResult {
  std::vector<TCSpectralComponent> components; // Prime spectral components
  std::vector<float> prime_spectrum;           // Amplitude by prime index
  std::vector<float> phase_values;             // Phase by prime index
  float overall_coherence;                     // Global coherence score
  float signal_energy;                         // Total signal energy
  std::vector<int> dominant_primes;            // Most significant primes
};

/**
 * Image analysis result
 */
struct ImageAnalysisResult {
  TCTransformResult row_transform;                   // Row-wise analysis
  TCTransformResult col_transform;                   // Column-wise analysis
  std::vector<std::vector<float>> spatial_coherence; // 2D coherence map
  float dominant_scale;            // Dominant spatial frequency
  std::vector<int> pattern_primes; // Primes describing pattern
};

/**
 * Audio analysis result
 */
struct AudioAnalysisResult {
  TCTransformResult spectral;            // Spectral analysis
  std::vector<float> temporal_coherence; // Coherence over time
  float dominant_frequency;              // Dominant frequency in Hz
  std::vector<int> rhythm_primes;        // Primes describing rhythm
  float music_coherence;                 // Musical coherence score
};

/**
 * Time Crystal Transform Engine
 *
 * Provides signal analysis using prime-based decomposition
 * instead of traditional Fourier analysis.
 */
class TCTransformEngine {
public:
  TCTransformEngine(NanoBrainKernel *kernel, const TCTransformConfig &config);
  ~TCTransformEngine();

  // ================================================================
  // Core Transform Operations
  // ================================================================

  /**
   * Forward Time Crystal Transform
   * @param signal Input signal
   * @return Transform result with prime spectrum
   */
  TCTransformResult transform(const std::vector<float> &signal);

  /**
   * Inverse Time Crystal Transform
   * @param result Transform result
   * @return Reconstructed signal
   */
  std::vector<float> inverse_transform(const TCTransformResult &result);

  /**
   * Compute single prime component
   * @param signal Input signal
   * @param prime Prime number for analysis
   * @return Spectral component
   */
  TCSpectralComponent compute_prime_component(const std::vector<float> &signal,
                                              int prime);

  // ================================================================
  // Analysis Modes
  // ================================================================

  /**
   * Analyze image using TC transform
   * @param image 2D image data (row-major)
   * @return Image analysis result
   */
  ImageAnalysisResult
  analyze_image(const std::vector<std::vector<float>> &image);

  /**
   * Analyze audio signal
   * @param samples Audio samples
   * @param sample_rate Sample rate in Hz
   * @return Audio analysis result
   */
  AudioAnalysisResult analyze_audio(const std::vector<float> &samples,
                                    int sample_rate);

  /**
   * Analyze pattern for prime encoding
   * @param pattern Input pattern
   * @return Vector of dominant primes describing pattern
   */
  std::vector<int> extract_prime_encoding(const std::vector<float> &pattern);

  // ================================================================
  // Comparison with FFT
  // ================================================================

  /**
   * Compare TC Transform with FFT (for benchmarking)
   * @param signal Input signal
   * @return Comparison metrics
   */
  struct TransformComparison {
    float tc_coherence;
    float fft_energy;
    float reconstruction_error_tc;
    float reconstruction_error_fft;
    float sparsity_tc;
    float sparsity_fft;
  };
  TransformComparison compare_with_fft(const std::vector<float> &signal);

  // ================================================================
  // Utility Functions
  // ================================================================

  /**
   * Get prime frequencies for analysis
   */
  std::vector<int> get_analysis_primes() const;

  /**
   * Set custom prime set
   */
  void set_analysis_primes(const std::vector<int> &primes);

  /**
   * Get configuration
   */
  const TCTransformConfig &get_config() const { return config; }

private:
  NanoBrainKernel *kernel;
  TCTransformConfig config;
  std::vector<int> analysis_primes;

  // Internal helpers
  void initialize_primes();
  float compute_prime_correlation(const std::vector<float> &signal, int prime);
  float compute_phase_at_prime(const std::vector<float> &signal, int prime);
  std::vector<float> generate_prime_basis(int prime, size_t length);
};

#endif // NANOBRAIN_TC_TRANSFORM_H
