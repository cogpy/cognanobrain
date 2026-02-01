#ifndef NANOBRAIN_PPM_H
#define NANOBRAIN_PPM_H

#include "nanobrain_kernel.h"
#include "nanobrain_time_crystal.h"
#include <array>
#include <cmath>
#include <complex>
#include <map>
#include <memory>
#include <string>
#include <vector>

/**
 * Phase Prime Metric (PPM) Module
 *
 * Implements the 10 PPM metric classes for pattern recognition using
 * prime number harmonics and universal symmetries.
 */

// ================================================================
// Constants
// ================================================================

constexpr int PPM_15_PRIMES_COUNT = 15;
static const std::array<int, PPM_15_PRIMES_COUNT> PPM_15_PRIMES = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47};

// ================================================================
// Result Structures
// ================================================================

/**
 * Result from PPM metric computation
 */
struct PPMResult {
  std::string metric_name;
  float value;
  std::vector<float> components;
  std::map<std::string, float> metadata;
  bool valid;
};

/**
 * Ordered factor representation
 */
struct OrderedFactor {
  std::vector<int> primes;    // Prime factors in order
  std::vector<int> exponents; // Exponent for each prime
  int64_t product;            // Original number
  float ratio;                // Product / sum of primes
};

/**
 * Phase path structure for 360° calculations
 */
struct PhasePath {
  float phase_angle;        // Angle in radians (0 to 2π)
  bool clockwise;           // CW or CCW direction
  std::vector<float> steps; // Individual phase steps
  float total_rotation;     // Total angular rotation
};

/**
 * Prime lattice node for graph relationships
 */
struct PrimeLatticeNode {
  int prime;
  std::vector<int> twins;    // Twin primes (diff = 2)
  std::vector<int> cousins;  // Cousin primes (diff = 4)
  std::vector<int> sexy;     // Sexy primes (diff = 6)
  std::vector<int> coprimes; // Coprime relationships
};

/**
 * Hole in prime coverage
 */
struct PrimeHole {
  int start;
  int end;
  int gap_size;
  float density_before;
  float density_after;
};

/**
 * Prime statistics
 */
struct PrimeStats {
  int total_primes;
  int active_primes;
  int silent_primes;
  float active_ratio;
  std::vector<int> active_list;
  std::vector<int> silent_list;
};

/**
 * Periodic ripple structure
 */
struct PeriodicRipple {
  float frequency;
  float amplitude;
  float period;
  bool is_periodic;
  std::vector<float> ripple_values;
};

/**
 * Imaginary layer structure
 */
struct ImaginaryLayer {
  int layer_index;
  std::complex<float> value;
  std::vector<std::complex<float>> components;
};

// ================================================================
// Base PPM Metric Interface
// ================================================================

/**
 * Abstract base class for all PPM metrics
 */
class PPMMetric {
public:
  virtual ~PPMMetric() = default;

  // Compute the metric for given primes
  virtual PPMResult compute(const std::vector<int> &primes) = 0;

  // Get metric name
  virtual std::string name() const = 0;

  // Get metric index (1-10)
  virtual int index() const = 0;
};

// ================================================================
// 10 PPM Metric Classes
// ================================================================

/**
 * Metric 1: Geometric Shape
 * Maps integers to GML shapes based on prime factorization
 */
class PPMMetric1_GeometricShape : public PPMMetric {
public:
  PPMResult compute(const std::vector<int> &primes) override;
  std::string name() const override { return "GeometricShape"; }
  int index() const override { return 1; }

  // Get GML shape for a number
  GMLShape number_to_shape(int n);

  // Get shape complexity
  int shape_complexity(GMLShape shape);
};

/**
 * Metric 2: Ordered Factor
 * Computes prime decomposition with ordering
 */
class PPMMetric2_OrderedFactor : public PPMMetric {
public:
  PPMResult compute(const std::vector<int> &primes) override;
  std::string name() const override { return "OrderedFactor"; }
  int index() const override { return 2; }

  // Compute ordered factors for a number
  OrderedFactor factorize(int64_t n);

  // Compute OF metric (product/sum ratio)
  float compute_of_ratio(int64_t n);
};

/**
 * Metric 3: Phase Path
 * Calculates CW/CCW 360° phase using golden ratio
 */
class PPMMetric3_PhasePath : public PPMMetric {
public:
  PPMResult compute(const std::vector<int> &primes) override;
  std::string name() const override { return "PhasePath"; }
  int index() const override { return 3; }

  // Compute phase path for primes
  PhasePath compute_phase_path(const std::vector<int> &primes);

  // Get phase angle for single prime
  float prime_phase_angle(int prime);
};

/**
 * Metric 4: Domain Limit
 * Determines prime domain boundaries
 */
class PPMMetric4_DomainLimit : public PPMMetric {
public:
  PPMResult compute(const std::vector<int> &primes) override;
  std::string name() const override { return "DomainLimit"; }
  int index() const override { return 4; }

  // Get domain boundaries
  std::pair<int, int> get_domain_bounds(int prime);

  // Check if prime is within fundamental domain
  bool is_in_fundamental_domain(int prime);
};

/**
 * Metric 5: High Ordered Factor
 * Computes cases where OF >> original integer
 */
class PPMMetric5_HighOrderedFactor : public PPMMetric {
public:
  PPMResult compute(const std::vector<int> &primes) override;
  std::string name() const override { return "HighOrderedFactor"; }
  int index() const override { return 5; }

  // Find numbers with high OF ratio
  std::vector<int64_t> find_high_of_numbers(int64_t limit,
                                            float threshold = 2.0f);

  // Compute high OF metric
  float compute_high_of(int64_t n);
};

/**
 * Metric 6: Hole Finder
 * Detects gaps in prime coverage
 */
class PPMMetric6_HoleFinder : public PPMMetric {
public:
  PPMResult compute(const std::vector<int> &primes) override;
  std::string name() const override { return "HoleFinder"; }
  int index() const override { return 6; }

  // Find holes in prime sequence
  std::vector<PrimeHole> find_holes(const std::vector<int> &primes);

  // Get largest hole
  PrimeHole largest_hole(const std::vector<int> &primes);
};

/**
 * Metric 7: Prime Statistics
 * Tracks silent/active prime ratios
 */
class PPMMetric7_PrimeStatistics : public PPMMetric {
public:
  PPMResult compute(const std::vector<int> &primes) override;
  std::string name() const override { return "PrimeStatistics"; }
  int index() const override { return 7; }

  // Compute prime statistics
  PrimeStats compute_stats(const std::vector<int> &primes, int range_limit);

  // Get active ratio for a set
  float active_ratio(const std::vector<int> &primes);
};

/**
 * Metric 8: Periodic Ripples
 * Computes normalized periodic patterns
 */
class PPMMetric8_PeriodicRipples : public PPMMetric {
public:
  PPMResult compute(const std::vector<int> &primes) override;
  std::string name() const override { return "PeriodicRipples"; }
  int index() const override { return 8; }

  // Detect periodicity in primes
  PeriodicRipple detect_periodicity(const std::vector<int> &primes);

  // Compute ripple strength
  float ripple_strength(const std::vector<int> &primes);
};

/**
 * Metric 9: Prime Lattice
 * Builds twin/cousin/co-prime graph
 */
class PPMMetric9_PrimeLattice : public PPMMetric {
public:
  PPMResult compute(const std::vector<int> &primes) override;
  std::string name() const override { return "PrimeLattice"; }
  int index() const override { return 9; }

  // Build lattice for primes
  std::vector<PrimeLatticeNode> build_lattice(const std::vector<int> &primes);

  // Count twin prime pairs
  int count_twins(const std::vector<int> &primes);

  // Count cousin prime pairs
  int count_cousins(const std::vector<int> &primes);
};

/**
 * Metric 10: Imaginary Layers
 * Multi-layer complex operations
 */
class PPMMetric10_ImaginaryLayers : public PPMMetric {
public:
  PPMResult compute(const std::vector<int> &primes) override;
  std::string name() const override { return "ImaginaryLayers"; }
  int index() const override { return 10; }

  // Compute imaginary layers
  std::vector<ImaginaryLayer> compute_layers(const std::vector<int> &primes,
                                             int num_layers = 3);

  // Get total complex magnitude
  float total_magnitude(const std::vector<int> &primes);
};

// ================================================================
// PPM Operator Chain
// ================================================================

/**
 * Chains multiple PPM metrics together
 */
class PPMOperatorChain {
public:
  PPMOperatorChain();
  ~PPMOperatorChain() = default;

  // Add a metric to the chain
  void add(std::unique_ptr<PPMMetric> metric);

  // Add metric by index (1-10)
  void add_by_index(int index);

  // Execute chain on primes
  std::vector<PPMResult> execute(const std::vector<int> &primes);

  // Get combined metric value
  float combined_value(const std::vector<int> &primes);

  // Get chain length
  size_t length() const { return chain.size(); }

  // Clear chain
  void clear() { chain.clear(); }

private:
  std::vector<std::unique_ptr<PPMMetric>> chain;
};

// ================================================================
// PPM Coherence Table
// ================================================================

/**
 * Precomputed coherence lookup for 15 fundamental primes
 */
class PPMCoherenceTable {
public:
  PPMCoherenceTable();
  ~PPMCoherenceTable() = default;

  // Initialize table with precomputed values
  void initialize();

  // Lookup coherence for prime subset
  float lookup(const std::vector<int> &primes);

  // Get coherence for prime pair
  float pair_coherence(int p1, int p2);

  // Get table size
  size_t size() const { return coherence_cache.size(); }

private:
  // Key: bitmask of which primes are included
  std::map<uint16_t, float> coherence_cache;

  // Convert primes to bitmask
  uint16_t primes_to_bitmask(const std::vector<int> &primes);

  // Compute coherence for bitmask
  float compute_coherence(uint16_t mask);
};

// ================================================================
// PPM Evolution for Time Crystals
// ================================================================

/**
 * PPM-driven time crystal evolution
 */
class PPMEvolution {
public:
  PPMEvolution(NanoBrainKernel *kernel);
  ~PPMEvolution() = default;

  // Evolve a time crystal state using PPM
  TimeCrystalQuantumState evolve(const TimeCrystalQuantumState &state,
                                 const PPMOperatorChain &chain,
                                 float dt = 0.01f);

  // Compute PPM metrics for a state
  std::vector<PPMResult> compute_metrics(const TimeCrystalQuantumState &state);

  // Export phase plot to CSV
  bool export_phase_plot(const std::string &filename,
                         const std::vector<int> &primes, int num_points = 360);

private:
  NanoBrainKernel *kernel;
  PPMCoherenceTable coherence_table;
  std::unique_ptr<PPMOperatorChain> default_chain;
};

// ================================================================
// Utility Functions
// ================================================================

// Check if number is prime
bool is_prime(int n);

// Get next prime after n
int next_prime(int n);

// Sieve of Eratosthenes for primes up to limit
std::vector<int> sieve_primes(int limit);

// Compute GCD
int gcd(int a, int b);

// Check if coprime
bool are_coprime(int a, int b);

// Create PPM metric by index
std::unique_ptr<PPMMetric> create_ppm_metric(int index);

#endif // NANOBRAIN_PPM_H
