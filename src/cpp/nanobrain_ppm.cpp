#include "nanobrain_ppm.h"
#include <algorithm>
#include <cmath>
#include <fstream>
#include <numeric>
#include <sstream>

// ================================================================
// Utility Functions
// ================================================================

bool is_prime(int n) {
  if (n <= 1)
    return false;
  if (n <= 3)
    return true;
  if (n % 2 == 0 || n % 3 == 0)
    return false;
  for (int i = 5; i * i <= n; i += 6) {
    if (n % i == 0 || n % (i + 2) == 0)
      return false;
  }
  return true;
}

int next_prime(int n) {
  int candidate = n + 1;
  while (!is_prime(candidate)) {
    candidate++;
  }
  return candidate;
}

std::vector<int> sieve_primes(int limit) {
  std::vector<bool> sieve(limit + 1, true);
  sieve[0] = sieve[1] = false;

  for (int i = 2; i * i <= limit; i++) {
    if (sieve[i]) {
      for (int j = i * i; j <= limit; j += i) {
        sieve[j] = false;
      }
    }
  }

  std::vector<int> primes;
  for (int i = 2; i <= limit; i++) {
    if (sieve[i])
      primes.push_back(i);
  }
  return primes;
}

int gcd(int a, int b) {
  while (b != 0) {
    int t = b;
    b = a % b;
    a = t;
  }
  return a;
}

bool are_coprime(int a, int b) { return gcd(a, b) == 1; }

std::unique_ptr<PPMMetric> create_ppm_metric(int index) {
  switch (index) {
  case 1:
    return std::make_unique<PPMMetric1_GeometricShape>();
  case 2:
    return std::make_unique<PPMMetric2_OrderedFactor>();
  case 3:
    return std::make_unique<PPMMetric3_PhasePath>();
  case 4:
    return std::make_unique<PPMMetric4_DomainLimit>();
  case 5:
    return std::make_unique<PPMMetric5_HighOrderedFactor>();
  case 6:
    return std::make_unique<PPMMetric6_HoleFinder>();
  case 7:
    return std::make_unique<PPMMetric7_PrimeStatistics>();
  case 8:
    return std::make_unique<PPMMetric8_PeriodicRipples>();
  case 9:
    return std::make_unique<PPMMetric9_PrimeLattice>();
  case 10:
    return std::make_unique<PPMMetric10_ImaginaryLayers>();
  default:
    return nullptr;
  }
}

// ================================================================
// PPMMetric1_GeometricShape
// ================================================================

GMLShape PPMMetric1_GeometricShape::number_to_shape(int n) {
  if (n <= 0)
    return GMLShape::Point;

  // Factorize and map to shapes
  int factor_count = 0;
  int temp = n;
  for (int p : PPM_15_PRIMES) {
    while (temp % p == 0) {
      factor_count++;
      temp /= p;
    }
    if (temp == 1)
      break;
  }

  // Map factor count to shape
  switch (factor_count % 15) {
  case 0:
    return GMLShape::Point;
  case 1:
    return GMLShape::Line;
  case 2:
    return GMLShape::Triangle;
  case 3:
    return GMLShape::Square;
  case 4:
    return GMLShape::Pentagon;
  case 5:
    return GMLShape::Hexagon;
  case 6:
    return GMLShape::Circle;
  case 7:
    return GMLShape::Tetrahedron;
  case 8:
    return GMLShape::Cube;
  case 9:
    return GMLShape::Octahedron;
  case 10:
    return GMLShape::Sphere;
  case 11:
    return GMLShape::Torus;
  case 12:
    return GMLShape::Dodecahedron;
  case 13:
    return GMLShape::Icosahedron;
  case 14:
    return GMLShape::Mobius;
  default:
    return GMLShape::Point;
  }
}

int PPMMetric1_GeometricShape::shape_complexity(GMLShape shape) {
  switch (shape) {
  case GMLShape::Point:
    return 0;
  case GMLShape::Line:
    return 1;
  case GMLShape::Triangle:
    return 3;
  case GMLShape::Square:
    return 4;
  case GMLShape::Pentagon:
    return 5;
  case GMLShape::Hexagon:
    return 6;
  case GMLShape::Circle:
    return 7;
  case GMLShape::Tetrahedron:
    return 8;
  case GMLShape::Cube:
    return 9;
  case GMLShape::Octahedron:
    return 10;
  case GMLShape::Sphere:
    return 11;
  case GMLShape::Torus:
    return 12;
  case GMLShape::Dodecahedron:
    return 13;
  case GMLShape::Icosahedron:
    return 14;
  case GMLShape::Mobius:
    return 15;
  default:
    return 0;
  }
}

PPMResult PPMMetric1_GeometricShape::compute(const std::vector<int> &primes) {
  PPMResult result;
  result.metric_name = name();
  result.valid = true;

  if (primes.empty()) {
    result.value = 0.0f;
    result.valid = false;
    return result;
  }

  // Compute product of primes
  int64_t product = 1;
  for (int p : primes) {
    product *= p;
    if (product > 1e12)
      break; // Prevent overflow
  }

  GMLShape shape = number_to_shape(static_cast<int>(product));
  int complexity = shape_complexity(shape);

  result.value = static_cast<float>(complexity) / 15.0f; // Normalize to [0,1]
  result.components.push_back(static_cast<float>(complexity));
  result.metadata["shape_index"] = static_cast<float>(static_cast<int>(shape));
  result.metadata["product"] = static_cast<float>(product);

  return result;
}

// ================================================================
// PPMMetric2_OrderedFactor
// ================================================================

OrderedFactor PPMMetric2_OrderedFactor::factorize(int64_t n) {
  OrderedFactor of;
  of.product = n;

  if (n <= 1) {
    of.ratio = 0.0f;
    return of;
  }

  int64_t temp = n;
  int64_t sum = 0;

  for (int p = 2; temp > 1; p = next_prime(p)) {
    int exp = 0;
    while (temp % p == 0) {
      temp /= p;
      exp++;
    }
    if (exp > 0) {
      of.primes.push_back(p);
      of.exponents.push_back(exp);
      sum += p * exp;
    }
  }

  of.ratio = sum > 0 ? static_cast<float>(n) / static_cast<float>(sum) : 0.0f;
  return of;
}

float PPMMetric2_OrderedFactor::compute_of_ratio(int64_t n) {
  auto of = factorize(n);
  return of.ratio;
}

PPMResult PPMMetric2_OrderedFactor::compute(const std::vector<int> &primes) {
  PPMResult result;
  result.metric_name = name();
  result.valid = true;

  if (primes.empty()) {
    result.value = 0.0f;
    result.valid = false;
    return result;
  }

  // Compute product of primes
  int64_t product = 1;
  for (int p : primes) {
    product *= p;
    if (product > 1e12)
      break;
  }

  OrderedFactor of = factorize(product);
  result.value = of.ratio;
  result.metadata["product"] = static_cast<float>(product);
  result.metadata["prime_count"] = static_cast<float>(of.primes.size());

  for (size_t i = 0; i < of.primes.size(); i++) {
    result.components.push_back(static_cast<float>(of.primes[i]));
    result.components.push_back(static_cast<float>(of.exponents[i]));
  }

  return result;
}

// ================================================================
// PPMMetric3_PhasePath
// ================================================================

float PPMMetric3_PhasePath::prime_phase_angle(int prime) {
  // Use golden ratio for phase distribution
  return fmod(prime * GOLDEN_RATIO * 2.0 * PI, 2.0 * PI);
}

PhasePath
PPMMetric3_PhasePath::compute_phase_path(const std::vector<int> &primes) {
  PhasePath path;
  path.total_rotation = 0.0f;

  float current_angle = 0.0f;
  for (int p : primes) {
    float step = prime_phase_angle(p);
    path.steps.push_back(step);
    current_angle = fmod(current_angle + step, 2.0f * static_cast<float>(PI));
    path.total_rotation += step;
  }

  path.phase_angle = current_angle;
  path.clockwise = path.total_rotation > 0;

  return path;
}

PPMResult PPMMetric3_PhasePath::compute(const std::vector<int> &primes) {
  PPMResult result;
  result.metric_name = name();
  result.valid = true;

  if (primes.empty()) {
    result.value = 0.0f;
    result.valid = false;
    return result;
  }

  PhasePath path = compute_phase_path(primes);

  // Normalize phase angle to [0, 1]
  result.value = path.phase_angle / (2.0f * static_cast<float>(PI));
  result.components = path.steps;
  result.metadata["total_rotation"] = path.total_rotation;
  result.metadata["clockwise"] = path.clockwise ? 1.0f : 0.0f;

  return result;
}

// ================================================================
// PPMMetric4_DomainLimit
// ================================================================

std::pair<int, int> PPMMetric4_DomainLimit::get_domain_bounds(int prime) {
  // Find index in fundamental primes
  int idx = -1;
  for (size_t i = 0; i < PPM_15_PRIMES.size(); i++) {
    if (PPM_15_PRIMES[i] == prime) {
      idx = static_cast<int>(i);
      break;
    }
  }

  if (idx < 0) {
    // Prime not in fundamental set, find closest
    for (size_t i = 0; i < PPM_15_PRIMES.size(); i++) {
      if (PPM_15_PRIMES[i] > prime) {
        idx = static_cast<int>(i);
        break;
      }
    }
    if (idx < 0)
      idx = PPM_15_PRIMES_COUNT - 1;
  }

  int lower = (idx > 0) ? PPM_15_PRIMES[idx - 1] : 1;
  int upper = PPM_15_PRIMES[idx];

  return {lower, upper};
}

bool PPMMetric4_DomainLimit::is_in_fundamental_domain(int prime) {
  for (int p : PPM_15_PRIMES) {
    if (p == prime)
      return true;
  }
  return false;
}

PPMResult PPMMetric4_DomainLimit::compute(const std::vector<int> &primes) {
  PPMResult result;
  result.metric_name = name();
  result.valid = true;

  if (primes.empty()) {
    result.value = 0.0f;
    result.valid = false;
    return result;
  }

  int in_domain = 0;
  for (int p : primes) {
    if (is_in_fundamental_domain(p)) {
      in_domain++;
    }
    auto bounds = get_domain_bounds(p);
    result.components.push_back(static_cast<float>(bounds.first));
    result.components.push_back(static_cast<float>(bounds.second));
  }

  result.value =
      static_cast<float>(in_domain) / static_cast<float>(primes.size());
  result.metadata["in_domain_count"] = static_cast<float>(in_domain);
  result.metadata["total_count"] = static_cast<float>(primes.size());

  return result;
}

// ================================================================
// PPMMetric5_HighOrderedFactor
// ================================================================

float PPMMetric5_HighOrderedFactor::compute_high_of(int64_t n) {
  PPMMetric2_OrderedFactor of_metric;
  return of_metric.compute_of_ratio(n);
}

std::vector<int64_t>
PPMMetric5_HighOrderedFactor::find_high_of_numbers(int64_t limit,
                                                   float threshold) {
  std::vector<int64_t> high_of_numbers;

  for (int64_t n = 2; n <= limit; n++) {
    float ratio = compute_high_of(n);
    if (ratio >= threshold) {
      high_of_numbers.push_back(n);
    }
  }

  return high_of_numbers;
}

PPMResult
PPMMetric5_HighOrderedFactor::compute(const std::vector<int> &primes) {
  PPMResult result;
  result.metric_name = name();
  result.valid = true;

  if (primes.empty()) {
    result.value = 0.0f;
    result.valid = false;
    return result;
  }

  // Compute product
  int64_t product = 1;
  for (int p : primes) {
    product *= p;
    if (product > 1e12)
      break;
  }

  float ratio = compute_high_of(product);

  // Check if this is a "high OF" case (ratio > 2)
  bool is_high_of = ratio > 2.0f;

  result.value = ratio;
  result.metadata["is_high_of"] = is_high_of ? 1.0f : 0.0f;
  result.metadata["product"] = static_cast<float>(product);

  return result;
}

// ================================================================
// PPMMetric6_HoleFinder
// ================================================================

std::vector<PrimeHole>
PPMMetric6_HoleFinder::find_holes(const std::vector<int> &primes) {
  std::vector<PrimeHole> holes;

  if (primes.size() < 2) {
    return holes;
  }

  // Sort primes
  std::vector<int> sorted_primes = primes;
  std::sort(sorted_primes.begin(), sorted_primes.end());

  for (size_t i = 1; i < sorted_primes.size(); i++) {
    int gap = sorted_primes[i] - sorted_primes[i - 1];
    if (gap > 2) { // Minimum gap for twin primes is 2
      PrimeHole hole;
      hole.start = sorted_primes[i - 1];
      hole.end = sorted_primes[i];
      hole.gap_size = gap;

      // Calculate density around hole
      hole.density_before = (i > 0) ? 1.0f / hole.start : 0.0f;
      hole.density_after = 1.0f / hole.end;

      holes.push_back(hole);
    }
  }

  return holes;
}

PrimeHole PPMMetric6_HoleFinder::largest_hole(const std::vector<int> &primes) {
  auto holes = find_holes(primes);

  if (holes.empty()) {
    PrimeHole empty;
    empty.gap_size = 0;
    return empty;
  }

  return *std::max_element(holes.begin(), holes.end(),
                           [](const PrimeHole &a, const PrimeHole &b) {
                             return a.gap_size < b.gap_size;
                           });
}

PPMResult PPMMetric6_HoleFinder::compute(const std::vector<int> &primes) {
  PPMResult result;
  result.metric_name = name();
  result.valid = true;

  if (primes.size() < 2) {
    result.value = 0.0f;
    result.valid = false;
    return result;
  }

  auto holes = find_holes(primes);
  PrimeHole largest = largest_hole(primes);

  result.value = static_cast<float>(holes.size());
  result.metadata["largest_gap"] = static_cast<float>(largest.gap_size);
  result.metadata["total_holes"] = static_cast<float>(holes.size());

  for (const auto &hole : holes) {
    result.components.push_back(static_cast<float>(hole.gap_size));
  }

  return result;
}

// ================================================================
// PPMMetric7_PrimeStatistics
// ================================================================

PrimeStats
PPMMetric7_PrimeStatistics::compute_stats(const std::vector<int> &primes,
                                          int range_limit) {
  PrimeStats stats;
  stats.total_primes = 0;
  stats.active_primes = 0;
  stats.silent_primes = 0;

  // Generate all primes up to range_limit
  auto all_primes = sieve_primes(range_limit);
  stats.total_primes = static_cast<int>(all_primes.size());

  // Convert input to set for O(1) lookup
  std::set<int> active_set(primes.begin(), primes.end());

  for (int p : all_primes) {
    if (active_set.count(p) > 0) {
      stats.active_primes++;
      stats.active_list.push_back(p);
    } else {
      stats.silent_primes++;
      stats.silent_list.push_back(p);
    }
  }

  stats.active_ratio = stats.total_primes > 0
                           ? static_cast<float>(stats.active_primes) /
                                 static_cast<float>(stats.total_primes)
                           : 0.0f;

  return stats;
}

float PPMMetric7_PrimeStatistics::active_ratio(const std::vector<int> &primes) {
  if (primes.empty())
    return 0.0f;

  int max_prime = *std::max_element(primes.begin(), primes.end());
  auto stats = compute_stats(primes, max_prime + 10);
  return stats.active_ratio;
}

PPMResult PPMMetric7_PrimeStatistics::compute(const std::vector<int> &primes) {
  PPMResult result;
  result.metric_name = name();
  result.valid = true;

  if (primes.empty()) {
    result.value = 0.0f;
    result.valid = false;
    return result;
  }

  int max_prime = *std::max_element(primes.begin(), primes.end());
  PrimeStats stats = compute_stats(primes, max_prime + 10);

  result.value = stats.active_ratio;
  result.metadata["active_count"] = static_cast<float>(stats.active_primes);
  result.metadata["silent_count"] = static_cast<float>(stats.silent_primes);
  result.metadata["total_count"] = static_cast<float>(stats.total_primes);

  return result;
}

// ================================================================
// PPMMetric8_PeriodicRipples
// ================================================================

PeriodicRipple
PPMMetric8_PeriodicRipples::detect_periodicity(const std::vector<int> &primes) {
  PeriodicRipple ripple;
  ripple.is_periodic = false;
  ripple.frequency = 0.0f;
  ripple.amplitude = 0.0f;
  ripple.period = 0.0f;

  if (primes.size() < 3) {
    return ripple;
  }

  // Compute differences between consecutive primes
  std::vector<int> diffs;
  for (size_t i = 1; i < primes.size(); i++) {
    diffs.push_back(primes[i] - primes[i - 1]);
  }

  // Check for periodicity in differences
  for (size_t period = 1; period <= diffs.size() / 2; period++) {
    bool periodic = true;
    for (size_t i = period; i < diffs.size(); i++) {
      if (diffs[i] != diffs[i % period]) {
        periodic = false;
        break;
      }
    }
    if (periodic) {
      ripple.is_periodic = true;
      ripple.period = static_cast<float>(period);
      ripple.frequency = 1.0f / ripple.period;
      break;
    }
  }

  // Compute amplitude as variance of differences
  float mean = 0.0f;
  for (int d : diffs) {
    mean += d;
    ripple.ripple_values.push_back(static_cast<float>(d));
  }
  mean /= diffs.size();

  float variance = 0.0f;
  for (int d : diffs) {
    variance += (d - mean) * (d - mean);
  }
  variance /= diffs.size();
  ripple.amplitude = std::sqrt(variance);

  return ripple;
}

float PPMMetric8_PeriodicRipples::ripple_strength(
    const std::vector<int> &primes) {
  auto ripple = detect_periodicity(primes);
  return ripple.amplitude * (ripple.is_periodic ? 1.0f : 0.5f);
}

PPMResult PPMMetric8_PeriodicRipples::compute(const std::vector<int> &primes) {
  PPMResult result;
  result.metric_name = name();
  result.valid = true;

  if (primes.size() < 3) {
    result.value = 0.0f;
    result.valid = false;
    return result;
  }

  PeriodicRipple ripple = detect_periodicity(primes);

  result.value = ripple_strength(primes);
  result.components = ripple.ripple_values;
  result.metadata["is_periodic"] = ripple.is_periodic ? 1.0f : 0.0f;
  result.metadata["period"] = ripple.period;
  result.metadata["frequency"] = ripple.frequency;
  result.metadata["amplitude"] = ripple.amplitude;

  return result;
}

// ================================================================
// PPMMetric9_PrimeLattice
// ================================================================

std::vector<PrimeLatticeNode>
PPMMetric9_PrimeLattice::build_lattice(const std::vector<int> &primes) {
  std::vector<PrimeLatticeNode> lattice;

  for (int p : primes) {
    PrimeLatticeNode node;
    node.prime = p;

    for (int q : primes) {
      if (q == p)
        continue;

      int diff = std::abs(p - q);

      if (diff == 2) {
        node.twins.push_back(q);
      } else if (diff == 4) {
        node.cousins.push_back(q);
      } else if (diff == 6) {
        node.sexy.push_back(q);
      }

      if (are_coprime(p, q)) {
        node.coprimes.push_back(q);
      }
    }

    lattice.push_back(node);
  }

  return lattice;
}

int PPMMetric9_PrimeLattice::count_twins(const std::vector<int> &primes) {
  int count = 0;
  for (size_t i = 0; i < primes.size(); i++) {
    for (size_t j = i + 1; j < primes.size(); j++) {
      if (std::abs(primes[i] - primes[j]) == 2) {
        count++;
      }
    }
  }
  return count;
}

int PPMMetric9_PrimeLattice::count_cousins(const std::vector<int> &primes) {
  int count = 0;
  for (size_t i = 0; i < primes.size(); i++) {
    for (size_t j = i + 1; j < primes.size(); j++) {
      if (std::abs(primes[i] - primes[j]) == 4) {
        count++;
      }
    }
  }
  return count;
}

PPMResult PPMMetric9_PrimeLattice::compute(const std::vector<int> &primes) {
  PPMResult result;
  result.metric_name = name();
  result.valid = true;

  if (primes.empty()) {
    result.value = 0.0f;
    result.valid = false;
    return result;
  }

  auto lattice = build_lattice(primes);

  int twin_pairs = count_twins(primes);
  int cousin_pairs = count_cousins(primes);

  // Compute lattice density
  int total_connections = 0;
  for (const auto &node : lattice) {
    total_connections += static_cast<int>(
        node.twins.size() + node.cousins.size() + node.sexy.size());
  }

  int max_connections = static_cast<int>(primes.size() * (primes.size() - 1));
  float density = max_connections > 0 ? static_cast<float>(total_connections) /
                                            static_cast<float>(max_connections)
                                      : 0.0f;

  result.value = density;
  result.metadata["twin_pairs"] = static_cast<float>(twin_pairs);
  result.metadata["cousin_pairs"] = static_cast<float>(cousin_pairs);
  result.metadata["total_connections"] = static_cast<float>(total_connections);

  return result;
}

// ================================================================
// PPMMetric10_ImaginaryLayers
// ================================================================

std::vector<ImaginaryLayer>
PPMMetric10_ImaginaryLayers::compute_layers(const std::vector<int> &primes,
                                            int num_layers) {
  std::vector<ImaginaryLayer> layers;

  for (int layer = 0; layer < num_layers; layer++) {
    ImaginaryLayer il;
    il.layer_index = layer;

    std::complex<float> layer_sum(0.0f, 0.0f);

    for (int p : primes) {
      // Phase based on prime and layer
      float phase = static_cast<float>(p) * static_cast<float>(layer + 1) *
                    static_cast<float>(GOLDEN_RATIO);
      float magnitude = 1.0f / std::sqrt(static_cast<float>(p));

      std::complex<float> component(magnitude * std::cos(phase),
                                    magnitude * std::sin(phase));
      il.components.push_back(component);
      layer_sum += component;
    }

    il.value = layer_sum / static_cast<float>(primes.size());
    layers.push_back(il);
  }

  return layers;
}

float PPMMetric10_ImaginaryLayers::total_magnitude(
    const std::vector<int> &primes) {
  auto layers = compute_layers(primes, 3);

  float total = 0.0f;
  for (const auto &layer : layers) {
    total += std::abs(layer.value);
  }
  return total;
}

PPMResult PPMMetric10_ImaginaryLayers::compute(const std::vector<int> &primes) {
  PPMResult result;
  result.metric_name = name();
  result.valid = true;

  if (primes.empty()) {
    result.value = 0.0f;
    result.valid = false;
    return result;
  }

  auto layers = compute_layers(primes, 3);

  result.value = total_magnitude(primes);

  for (const auto &layer : layers) {
    result.components.push_back(std::abs(layer.value));
    result.components.push_back(std::arg(layer.value));
  }

  result.metadata["num_layers"] = static_cast<float>(layers.size());

  return result;
}

// ================================================================
// PPMOperatorChain
// ================================================================

PPMOperatorChain::PPMOperatorChain() {}

void PPMOperatorChain::add(std::unique_ptr<PPMMetric> metric) {
  chain.push_back(std::move(metric));
}

void PPMOperatorChain::add_by_index(int index) {
  auto metric = create_ppm_metric(index);
  if (metric) {
    chain.push_back(std::move(metric));
  }
}

std::vector<PPMResult>
PPMOperatorChain::execute(const std::vector<int> &primes) {
  std::vector<PPMResult> results;

  for (auto &metric : chain) {
    results.push_back(metric->compute(primes));
  }

  return results;
}

float PPMOperatorChain::combined_value(const std::vector<int> &primes) {
  auto results = execute(primes);

  float sum = 0.0f;
  int valid_count = 0;

  for (const auto &r : results) {
    if (r.valid) {
      sum += r.value;
      valid_count++;
    }
  }

  return valid_count > 0 ? sum / static_cast<float>(valid_count) : 0.0f;
}

// ================================================================
// PPMCoherenceTable
// ================================================================

PPMCoherenceTable::PPMCoherenceTable() { initialize(); }

uint16_t PPMCoherenceTable::primes_to_bitmask(const std::vector<int> &primes) {
  uint16_t mask = 0;

  for (int p : primes) {
    for (size_t i = 0; i < PPM_15_PRIMES.size(); i++) {
      if (PPM_15_PRIMES[i] == p) {
        mask |= (1 << i);
        break;
      }
    }
  }

  return mask;
}

float PPMCoherenceTable::compute_coherence(uint16_t mask) {
  if (mask == 0)
    return 0.0f;

  // Extract primes from mask
  std::vector<int> primes;
  for (size_t i = 0; i < PPM_15_PRIMES.size(); i++) {
    if (mask & (1 << i)) {
      primes.push_back(PPM_15_PRIMES[i]);
    }
  }

  if (primes.empty())
    return 0.0f;

  // Golden ratio coherence formula
  float sum = 0.0f;
  for (size_t i = 0; i < primes.size(); i++) {
    for (size_t j = i + 1; j < primes.size(); j++) {
      float ratio =
          static_cast<float>(primes[j]) / static_cast<float>(primes[i]);
      float phi_diff = std::abs(ratio - static_cast<float>(GOLDEN_RATIO));
      sum += 1.0f / (1.0f + phi_diff);
    }
  }

  int pairs = static_cast<int>(primes.size() * (primes.size() - 1) / 2);
  return pairs > 0 ? sum / static_cast<float>(pairs) : 1.0f;
}

void PPMCoherenceTable::initialize() {
  // Precompute coherence for all subsets of 15 primes
  // 2^15 = 32768 combinations
  for (uint16_t mask = 1; mask < (1 << PPM_15_PRIMES_COUNT); mask++) {
    coherence_cache[mask] = compute_coherence(mask);
  }
}

float PPMCoherenceTable::lookup(const std::vector<int> &primes) {
  uint16_t mask = primes_to_bitmask(primes);

  auto it = coherence_cache.find(mask);
  if (it != coherence_cache.end()) {
    return it->second;
  }

  // Fallback to computation if not in table
  return compute_coherence(mask);
}

float PPMCoherenceTable::pair_coherence(int p1, int p2) {
  return lookup({p1, p2});
}

// ================================================================
// PPMEvolution
// ================================================================

PPMEvolution::PPMEvolution(NanoBrainKernel *kernel) : kernel(kernel) {
  default_chain = std::make_unique<PPMOperatorChain>();

  // Add all 10 metrics to default chain
  for (int i = 1; i <= 10; i++) {
    default_chain->add_by_index(i);
  }
}

TimeCrystalQuantumState
PPMEvolution::evolve(const TimeCrystalQuantumState &state,
                     const PPMOperatorChain &chain, float dt) {
  TimeCrystalQuantumState new_state = state;

  // Compute metrics for current state
  auto results = chain.execute(state.prime_signature);

  // Update quantum phase based on phase path metric
  for (const auto &r : results) {
    if (r.metric_name == "PhasePath" && r.valid) {
      new_state.quantum_phase = fmod(state.quantum_phase + r.value * dt,
                                     2.0f * static_cast<float>(PI));
    }
  }

  // Update temporal coherence based on coherence table
  float coherence = coherence_table.lookup(state.prime_signature);
  new_state.temporal_coherence =
      state.temporal_coherence * 0.9f + coherence * 0.1f;

  // Update fractal dimension based on lattice metric
  for (const auto &r : results) {
    if (r.metric_name == "PrimeLattice" && r.valid) {
      new_state.fractal_dimension = 1.0f + r.value;
    }
  }

  // Update resonance frequency based on periodic ripples
  for (const auto &r : results) {
    if (r.metric_name == "PeriodicRipples" && r.valid) {
      auto it = r.metadata.find("frequency");
      if (it != r.metadata.end() && it->second > 0) {
        new_state.resonance_frequency = it->second;
      }
    }
  }

  return new_state;
}

std::vector<PPMResult>
PPMEvolution::compute_metrics(const TimeCrystalQuantumState &state) {
  return default_chain->execute(state.prime_signature);
}

bool PPMEvolution::export_phase_plot(const std::string &filename,
                                     const std::vector<int> &primes,
                                     int num_points) {
  std::ofstream file(filename);
  if (!file.is_open()) {
    return false;
  }

  file << "angle,coherence,phase_x,phase_y\n";

  PPMMetric3_PhasePath phase_metric;

  for (int i = 0; i < num_points; i++) {
    float angle = 2.0f * static_cast<float>(PI) * i / num_points;

    // Compute coherence at this angle
    float coherence = coherence_table.lookup(primes);

    // Compute phase components
    float phase_x = coherence * std::cos(angle);
    float phase_y = coherence * std::sin(angle);

    file << angle << "," << coherence << "," << phase_x << "," << phase_y
         << "\n";
  }

  file.close();
  return true;
}
