#include "nanobrain_singularity.h"
#include <algorithm>
#include <chrono>
#include <cmath>
#include <iostream>
#include <numeric>
#include <random>
#include <sstream>

// ================================================================
// Utility Functions Implementation
// ================================================================

std::string tubulin_ring_to_string(const TubulinRing &ring) {
  std::stringstream ss;
  ss << "TubulinRing[" << ring.ring_id << "] " << (ring.is_alpha ? "α" : "β")
     << "-tubulin, "
     << "coherence=" << ring.ring_coherence << ", "
     << "primes=[";
  for (size_t i = 0; i < ring.prime_encoding.size(); i++) {
    ss << ring.prime_encoding[i];
    if (i < ring.prime_encoding.size() - 1)
      ss << ",";
  }
  ss << "]";
  return ss.str();
}

std::string triplet_resonance_to_string(const TripletResonance &resonance) {
  std::stringstream ss;
  ss << "TripletResonance[depth=" << resonance.depth << "] "
     << "bands=[" << resonance.inner_band[0] << "," << resonance.inner_band[1]
     << "," << resonance.inner_band[2] << "] "
     << "freq=" << resonance.resonance_frequency << "Hz";
  return ss.str();
}

std::string neuron_segment_to_string(const NeuronSegment &segment) {
  std::stringstream ss;
  ss << "NeuronSegment[" << segment.segment_id << "] "
     << "type=" << segment.segment_type << ", "
     << "scale=" << segment.scale_level << ", "
     << "bands=[" << segment.triplet_bands[0] << "," << segment.triplet_bands[1]
     << "," << segment.triplet_bands[2] << "]";
  return ss.str();
}

// ================================================================
// TripletResonance Implementation
// ================================================================

float TripletResonance::cross_resonance() const {
  // Calculate cross-band resonance using harmonic mean
  float sum = 0.0f;
  int count = 0;

  for (int i = 0; i < TRIPLET_INNER_BANDS; i++) {
    for (int j = i + 1; j < TRIPLET_INNER_BANDS; j++) {
      if (inner_band[i] > 0 && inner_band[j] > 0) {
        sum += 2.0f * inner_band[i] * inner_band[j] /
               (inner_band[i] + inner_band[j]);
        count++;
      }
    }
  }

  return count > 0 ? sum / count : 0.0f;
}

float TripletResonance::total_harmonic() const {
  float total = cross_resonance();

  // Add nested contributions (recursive)
  for (int i = 0; i < TRIPLET_INNER_BANDS; i++) {
    if (nested[i] != nullptr) {
      total += nested[i]->total_harmonic() * 0.5f; // Decay with depth
    }
  }

  return total;
}

// ================================================================
// TubulinPPMModel Implementation
// ================================================================

TubulinPPMModel::TubulinPPMModel(NanoBrainKernel *kernel,
                                 TimeCrystalKernel *tc_kernel)
    : kernel_(kernel), tc_kernel_(tc_kernel) {}

void TubulinPPMModel::initialize(int num_rings) {
  if (initialized_)
    return;

  rings_.clear();
  rings_.reserve(num_rings);

  // Create alternating α/β tubulin rings
  for (int i = 0; i < num_rings; i++) {
    bool is_alpha = (i % 2 == 0);
    TubulinRing ring = create_alpha_helix_ring(i, is_alpha);
    rings_.push_back(ring);
  }

  // Initialize water channel
  float length = num_rings * 8.0f; // ~8nm per ring
  water_channel_ = encode_water_channel(length);

  initialized_ = true;
  std::cout << "[TubulinPPMModel] Initialized with " << num_rings
            << " rings and water channel" << std::endl;
}

TubulinRing TubulinPPMModel::create_alpha_helix_ring(int ring_id,
                                                     bool is_alpha) {
  TubulinRing ring;
  ring.ring_id = ring_id;
  ring.is_alpha = is_alpha;
  ring.rotation_angle = calculate_spiral_angle(ring_id);

  // Generate protofilament phases (13 protofilaments)
  static std::mt19937 rng(42);
  static std::uniform_real_distribution<float> phase_dist(0.0f, 2.0f * PI);

  for (int i = 0; i < TUBULIN_PROTOFILAMENTS; i++) {
    // Phase based on position around cylinder + random variation
    float base_phase = (2.0f * PI * i) / TUBULIN_PROTOFILAMENTS;
    ring.protofilament_phases[i] = base_phase + phase_dist(rng) * 0.1f;
  }

  // Generate PPM for this ring
  ring.prime_encoding = generate_ring_primes(ring_id, is_alpha);

  // Calculate coherence using PPM
  ring.ring_coherence = tc_kernel_->compute_ppm_coherence(ring.prime_encoding);

  return ring;
}

const TubulinRing *TubulinPPMModel::get_ring(int ring_id) const {
  for (const auto &ring : rings_) {
    if (ring.ring_id == ring_id) {
      return &ring;
    }
  }
  return nullptr;
}

float TubulinPPMModel::calculate_ring_resonance(int ring1_id,
                                                int ring2_id) const {
  const auto *ring1 = get_ring(ring1_id);
  const auto *ring2 = get_ring(ring2_id);

  if (!ring1 || !ring2)
    return 0.0f;

  // Combine prime encodings
  std::vector<int> combined = ring1->prime_encoding;
  for (int p : ring2->prime_encoding) {
    if (std::find(combined.begin(), combined.end(), p) == combined.end()) {
      combined.push_back(p);
    }
  }

  // Calculate combined coherence
  float combined_coherence = tc_kernel_->compute_ppm_coherence(combined);

  // Factor in distance between rings
  float distance_factor = 1.0f / (1.0f + std::abs(ring1_id - ring2_id) * 0.1f);

  return combined_coherence * distance_factor;
}

WaterChannel TubulinPPMModel::encode_water_channel(float length_nm) {
  WaterChannel channel;
  channel.diameter_nm = WATER_CHANNEL_DIAMETER_NM;
  channel.length_nm = length_nm;

  // Create coherence zones along the channel
  int num_zones = static_cast<int>(length_nm / 4.0f); // One zone per 4nm
  channel.coherence_zones.reserve(num_zones);

  static std::mt19937 rng(123);
  static std::uniform_real_distribution<float> coherence_dist(0.6f, 1.0f);

  for (int i = 0; i < num_zones; i++) {
    channel.coherence_zones.push_back(coherence_dist(rng));
  }

  // Use water-specific primes (hydrogen bonding related)
  channel.prime_resonance = {2, 3, 5, 7}; // Simple primes for water

  // Overall quantum coherence
  channel.quantum_coherence =
      tc_kernel_->compute_ppm_coherence(channel.prime_resonance);

  return channel;
}

bool TubulinPPMModel::verify_hyperspace_symmetry() const {
  if (!initialized_ || rings_.empty())
    return false;

  // Check that all rings have valid PPM coherence
  for (const auto &ring : rings_) {
    if (ring.ring_coherence < 0.3f) {
      return false; // Coherence too low
    }
  }

  // Check 13-fold symmetry (protofilaments)
  float symmetry_score = 0.0f;
  for (const auto &ring : rings_) {
    // Check phase distribution
    float phase_sum = 0.0f;
    for (int i = 0; i < TUBULIN_PROTOFILAMENTS; i++) {
      phase_sum += ring.protofilament_phases[i];
    }
    float avg_phase = phase_sum / TUBULIN_PROTOFILAMENTS;

    // Variance from uniform distribution
    float variance = 0.0f;
    for (int i = 0; i < TUBULIN_PROTOFILAMENTS; i++) {
      float diff = ring.protofilament_phases[i] -
                   ((2.0f * PI * i) / TUBULIN_PROTOFILAMENTS + avg_phase - PI);
      variance += diff * diff;
    }
    variance /= TUBULIN_PROTOFILAMENTS;

    symmetry_score += 1.0f / (1.0f + variance);
  }

  symmetry_score /= rings_.size();
  return symmetry_score > 0.5f;
}

float TubulinPPMModel::calculate_structure_coherence() const {
  if (!initialized_ || rings_.empty())
    return 0.0f;

  // Average ring coherence
  float ring_coherence_sum = 0.0f;
  for (const auto &ring : rings_) {
    ring_coherence_sum += ring.ring_coherence;
  }
  float avg_ring_coherence = ring_coherence_sum / rings_.size();

  // Water channel coherence
  float water_coherence = water_channel_.quantum_coherence;

  // Combined structure coherence
  return 0.6f * avg_ring_coherence + 0.4f * water_coherence;
}

std::vector<int> TubulinPPMModel::get_structure_ppm() const {
  std::vector<int> combined;

  for (const auto &ring : rings_) {
    for (int p : ring.prime_encoding) {
      if (std::find(combined.begin(), combined.end(), p) == combined.end()) {
        combined.push_back(p);
      }
    }
  }

  for (int p : water_channel_.prime_resonance) {
    if (std::find(combined.begin(), combined.end(), p) == combined.end()) {
      combined.push_back(p);
    }
  }

  std::sort(combined.begin(), combined.end());
  return combined;
}

NanoBrainTensor *TubulinPPMModel::create_structure_tensor() {
  if (!initialized_)
    return nullptr;

  // Create tensor: [num_rings, protofilaments, 2(phase, coherence)]
  auto *tensor = kernel_->create_tensor(
      {static_cast<int64_t>(rings_.size()), TUBULIN_PROTOFILAMENTS, 2});

  std::vector<float> data;
  data.reserve(rings_.size() * TUBULIN_PROTOFILAMENTS * 2);

  for (const auto &ring : rings_) {
    for (int i = 0; i < TUBULIN_PROTOFILAMENTS; i++) {
      data.push_back(ring.protofilament_phases[i]);
      data.push_back(ring.ring_coherence);
    }
  }

  kernel_->set_data(tensor, data);
  return tensor;
}

NanoBrainTensor *TubulinPPMModel::get_protofilament_tensor(int ring_id) {
  const auto *ring = get_ring(ring_id);
  if (!ring)
    return nullptr;

  auto *tensor = kernel_->create_tensor({TUBULIN_PROTOFILAMENTS});
  std::vector<float> phases(ring->protofilament_phases.begin(),
                            ring->protofilament_phases.end());
  kernel_->set_data(tensor, phases);
  return tensor;
}

std::vector<int> TubulinPPMModel::generate_ring_primes(int ring_id,
                                                       bool is_alpha) {
  // Select primes based on ring position and type
  std::vector<int> primes;

  // Base primes depend on α/β type
  if (is_alpha) {
    primes = {2, 3, 5}; // α-tubulin: first 3 primes
  } else {
    primes = {7, 11, 13}; // β-tubulin: next 3 primes
  }

  // Add ring-specific prime
  int ring_prime = FUNDAMENTAL_PRIMES[ring_id % FUNDAMENTAL_PRIMES_COUNT];
  if (std::find(primes.begin(), primes.end(), ring_prime) == primes.end()) {
    primes.push_back(ring_prime);
  }

  return primes;
}

float TubulinPPMModel::calculate_spiral_angle(int ring_id) const {
  // Microtubule spiral: ~3 subunits start per 8nm
  // This gives a helical rise
  return (ring_id * 2.0f * PI * 3.0f) / TUBULIN_PROTOFILAMENTS;
}

// ================================================================
// TripletResonanceCascade Implementation
// ================================================================

TripletResonanceCascade::TripletResonanceCascade(NanoBrainKernel *kernel)
    : kernel_(kernel) {}

TripletResonanceCascade::~TripletResonanceCascade() {
  // Unique pointers will clean up automatically
}

void TripletResonanceCascade::initialize(int max_depth) {
  if (initialized_)
    return;

  max_depth_ = max_depth;

  // Create root triplet
  root_ = create_triplet(0, {2, 3, 5});

  // Build nested structure
  build_nested_triplets(root_, 0);

  initialized_ = true;
  std::cout << "[TripletResonanceCascade] Initialized with depth " << max_depth
            << std::endl;
}

TripletResonance *TripletResonanceCascade::create_triplet(
    int depth, const std::vector<int> &prime_signature) {
  auto triplet = std::make_unique<TripletResonance>();
  triplet->depth = depth;
  triplet->prime_signature = prime_signature;

  // Initialize inner bands with PPM-derived values
  float base_freq = 440.0f; // A4
  for (int i = 0; i < TRIPLET_INNER_BANDS; i++) {
    // Use prime ratios for harmonic relationship
    int prime = (i < static_cast<int>(prime_signature.size()))
                    ? prime_signature[i]
                    : (i + 2);
    triplet->inner_band[i] = base_freq * (prime / 2.0f);
    triplet->nested[i] = nullptr;
  }

  triplet->resonance_frequency = base_freq * std::pow(2.0f, depth / 12.0f);

  TripletResonance *ptr = triplet.get();
  triplet_pool_.push_back(std::move(triplet));
  return ptr;
}

void TripletResonanceCascade::build_nested_triplets(TripletResonance *parent,
                                                    int current_depth) {
  if (current_depth >= max_depth_ - 1 || !parent)
    return;

  for (int i = 0; i < TRIPLET_INNER_BANDS; i++) {
    // Create child with shifted primes
    std::vector<int> child_primes = parent->prime_signature;
    if (!child_primes.empty()) {
      // Rotate primes for variety
      std::rotate(child_primes.begin(), child_primes.begin() + 1,
                  child_primes.end());
    }

    parent->nested[i] = create_triplet(current_depth + 1, child_primes);
    build_nested_triplets(parent->nested[i], current_depth + 1);
  }
}

void TripletResonanceCascade::cascade_update(
    const std::array<float, TRIPLET_INNER_BANDS> &input) {
  if (!initialized_ || !root_)
    return;

  update_recursive(root_, input, 0);
}

void TripletResonanceCascade::update_recursive(
    TripletResonance *node, const std::array<float, TRIPLET_INNER_BANDS> &input,
    int depth) {
  if (!node)
    return;

  // Update this node's bands based on input
  float decay = std::pow(0.8f, depth);
  for (int i = 0; i < TRIPLET_INNER_BANDS; i++) {
    // Blend current value with input
    node->inner_band[i] = 0.7f * node->inner_band[i] + 0.3f * input[i] * decay;
  }

  // Update resonance frequency based on cross-resonance
  node->resonance_frequency =
      440.0f * (1.0f + node->cross_resonance() / 1000.0f);

  // Propagate to children with dampened input
  std::array<float, TRIPLET_INNER_BANDS> child_input;
  for (int i = 0; i < TRIPLET_INNER_BANDS; i++) {
    child_input[i] = input[i] * 0.5f;
  }

  for (int i = 0; i < TRIPLET_INNER_BANDS; i++) {
    if (node->nested[i]) {
      update_recursive(node->nested[i], child_input, depth + 1);
    }
  }
}

std::vector<float>
TripletResonanceCascade::get_band_harmonics(int depth) const {
  std::vector<float> harmonics;
  if (!root_)
    return harmonics;

  // Traverse to requested depth
  std::function<void(const TripletResonance *, int)> collect_harmonics =
      [&](const TripletResonance *node, int current_depth) {
        if (!node)
          return;

        if (current_depth == depth) {
          for (int i = 0; i < TRIPLET_INNER_BANDS; i++) {
            harmonics.push_back(node->inner_band[i]);
          }
        } else if (current_depth < depth) {
          for (int i = 0; i < TRIPLET_INNER_BANDS; i++) {
            collect_harmonics(node->nested[i], current_depth + 1);
          }
        }
      };

  collect_harmonics(root_, 0);
  return harmonics;
}

float TripletResonanceCascade::calculate_cascade_energy() const {
  if (!root_)
    return 0.0f;
  return calculate_energy_recursive(root_);
}

float TripletResonanceCascade::calculate_energy_recursive(
    const TripletResonance *node) const {
  if (!node)
    return 0.0f;

  float energy = 0.0f;
  for (int i = 0; i < TRIPLET_INNER_BANDS; i++) {
    energy += node->inner_band[i] * node->inner_band[i];
  }

  for (int i = 0; i < TRIPLET_INNER_BANDS; i++) {
    energy += calculate_energy_recursive(node->nested[i]);
  }

  return energy;
}

float TripletResonanceCascade::calculate_cross_level_resonance(
    int level1, int level2) const {
  auto harmonics1 = get_band_harmonics(level1);
  auto harmonics2 = get_band_harmonics(level2);

  if (harmonics1.empty() || harmonics2.empty())
    return 0.0f;

  // Calculate correlation
  float sum1 = 0.0f, sum2 = 0.0f, dot = 0.0f;
  size_t min_size = std::min(harmonics1.size(), harmonics2.size());

  for (size_t i = 0; i < min_size; i++) {
    dot += harmonics1[i] * harmonics2[i];
    sum1 += harmonics1[i] * harmonics1[i];
    sum2 += harmonics2[i] * harmonics2[i];
  }

  if (sum1 <= 0 || sum2 <= 0)
    return 0.0f;
  return dot / (std::sqrt(sum1) * std::sqrt(sum2));
}

void TripletResonanceCascade::apply_ppm_weighting(
    const std::vector<int> &primes) {
  if (!root_ || primes.empty())
    return;

  // Calculate PPM coherence
  float coherence = 0.5f;
  if (primes.size() >= 2) {
    double product = 1.0;
    double sum = 0.0;
    for (int p : primes) {
      product *= p;
      sum += p;
    }
    coherence =
        static_cast<float>(0.5 + 0.5 * std::sin(std::sqrt(product) * PI / sum));
  }

  // Apply weighting to all nodes
  std::function<void(TripletResonance *)> apply_weight =
      [&](TripletResonance *node) {
        if (!node)
          return;

        for (int i = 0; i < TRIPLET_INNER_BANDS; i++) {
          node->inner_band[i] *= coherence;
        }

        for (int i = 0; i < TRIPLET_INNER_BANDS; i++) {
          apply_weight(node->nested[i]);
        }
      };

  apply_weight(root_);
}

std::vector<int> TripletResonanceCascade::get_effective_signature() const {
  if (!root_)
    return {};
  return root_->prime_signature;
}

// ================================================================
// CellularAutomatonEngine Implementation
// ================================================================

CellularAutomatonEngine::CellularAutomatonEngine(NanoBrainKernel *kernel)
    : kernel_(kernel) {}

void CellularAutomatonEngine::initialize(int width, int height) {
  state_.width = width;
  state_.height = height;
  state_.generation = 0;
  state_.rule_name = "Rule110";
  state_.rule_number = 110;

  state_.grid.resize(height);
  for (int y = 0; y < height; y++) {
    state_.grid[y].resize(width, 0);
  }

  initialized_ = true;
  std::cout << "[CellularAutomatonEngine] Initialized " << width << "x"
            << height << " grid" << std::endl;
}

void CellularAutomatonEngine::set_initial_state(
    const std::vector<std::vector<int>> &state) {
  if (!initialized_)
    return;

  int h = std::min(static_cast<int>(state.size()), state_.height);
  for (int y = 0; y < h; y++) {
    int w = std::min(static_cast<int>(state[y].size()), state_.width);
    for (int x = 0; x < w; x++) {
      state_.grid[y][x] = state[y][x] != 0 ? 1 : 0;
    }
  }
}

void CellularAutomatonEngine::randomize(float density) {
  if (!initialized_)
    return;

  static std::mt19937 rng(std::random_device{}());
  std::uniform_real_distribution<float> dist(0.0f, 1.0f);

  for (int y = 0; y < state_.height; y++) {
    for (int x = 0; x < state_.width; x++) {
      state_.grid[y][x] = (dist(rng) < density) ? 1 : 0;
    }
  }
}

void CellularAutomatonEngine::set_rule(int rule_number) {
  rule_number_ = rule_number % 256;
  state_.rule_number = rule_number_;
  state_.rule_name = "Rule" + std::to_string(rule_number_);
}

void CellularAutomatonEngine::set_custom_rule(RuleFunction rule) {
  custom_rule_ = rule;
  state_.rule_name = "Custom";
}

void CellularAutomatonEngine::apply_ppm_rules(const std::vector<int> &primes) {
  if (primes.empty())
    return;

  // Use product of primes modulo 256 as rule number
  int product = 1;
  for (int p : primes) {
    product = (product * p) % 256;
  }

  set_rule(product);
  state_.rule_name = "PPM_Rule" + std::to_string(product);
}

std::vector<int>
CellularAutomatonEngine::explore_rule_space(int num_generations) {
  std::vector<int> interesting_rules;

  // Test some known interesting rules
  std::vector<int> candidates = {30, 54, 60, 90, 110, 150, 182};

  for (int rule : candidates) {
    set_rule(rule);
    randomize(0.5f);

    // Run and measure complexity
    for (int g = 0; g < num_generations; g++) {
      update_parallel();
    }

    float entropy = calculate_entropy();
    if (entropy > 0.3f && entropy < 0.7f) {
      // Interesting complexity range
      interesting_rules.push_back(rule);
    }
  }

  return interesting_rules;
}

void CellularAutomatonEngine::update_parallel() {
  if (!initialized_)
    return;

  // Create new grid
  std::vector<std::vector<int>> new_grid(state_.height);
  for (int y = 0; y < state_.height; y++) {
    new_grid[y].resize(state_.width);
  }

  // Parallel update (simulated - actual parallelism would use std::thread)
  for (int y = 0; y < state_.height; y++) {
    for (int x = 0; x < state_.width; x++) {
      if (custom_rule_) {
        new_grid[y][x] = custom_rule_(x, y, state_);
      } else {
        // Use top row as 1D CA neighborhood
        int left = state_.grid[y][(x - 1 + state_.width) % state_.width];
        int center = state_.grid[y][x];
        int right = state_.grid[y][(x + 1) % state_.width];
        new_grid[y][x] = apply_elementary_rule(left, center, right);
      }
    }
  }

  state_.grid = std::move(new_grid);
  state_.generation++;
}

void CellularAutomatonEngine::run(int generations) {
  for (int g = 0; g < generations; g++) {
    update_parallel();
  }
}

int CellularAutomatonEngine::get_cell(int x, int y) const {
  if (!initialized_ || x < 0 || x >= state_.width || y < 0 ||
      y >= state_.height)
    return 0;
  return state_.grid[y][x];
}

float CellularAutomatonEngine::calculate_density() const {
  if (!initialized_)
    return 0.0f;

  int alive = 0;
  for (int y = 0; y < state_.height; y++) {
    for (int x = 0; x < state_.width; x++) {
      alive += state_.grid[y][x];
    }
  }

  return static_cast<float>(alive) / (state_.width * state_.height);
}

float CellularAutomatonEngine::calculate_entropy() const {
  if (!initialized_)
    return 0.0f;

  float density = calculate_density();
  if (density <= 0.0f || density >= 1.0f)
    return 0.0f;

  // Binary entropy
  return -density * std::log2(density) -
         (1.0f - density) * std::log2(1.0f - density);
}

std::vector<std::pair<int, int>>
CellularAutomatonEngine::detect_patterns() const {
  std::vector<std::pair<int, int>> patterns;

  if (!initialized_)
    return patterns;

  // Simple pattern detection: find clusters of alive cells
  for (int y = 0; y < state_.height; y++) {
    for (int x = 0; x < state_.width; x++) {
      if (state_.grid[y][x] == 1) {
        // Check if center of 3x3 pattern
        int count = 0;
        for (int dy = -1; dy <= 1; dy++) {
          for (int dx = -1; dx <= 1; dx++) {
            int ny = (y + dy + state_.height) % state_.height;
            int nx = (x + dx + state_.width) % state_.width;
            count += state_.grid[ny][nx];
          }
        }
        if (count >= 5) { // At least 5 alive in 3x3
          patterns.push_back({x, y});
        }
      }
    }
  }

  return patterns;
}

int CellularAutomatonEngine::apply_elementary_rule(int left, int center,
                                                   int right) const {
  int pattern = (left << 2) | (center << 1) | right;
  return (rule_number_ >> pattern) & 1;
}

std::array<int, 9> CellularAutomatonEngine::get_neighborhood(int x,
                                                             int y) const {
  std::array<int, 9> neighborhood;
  int idx = 0;

  for (int dy = -1; dy <= 1; dy++) {
    for (int dx = -1; dx <= 1; dx++) {
      int ny = (y + dy + state_.height) % state_.height;
      int nx = (x + dx + state_.width) % state_.width;
      neighborhood[idx++] = state_.grid[ny][nx];
    }
  }

  return neighborhood;
}

// ================================================================
// NeuronTimeCrystalMapper Implementation
// ================================================================

NeuronTimeCrystalMapper::NeuronTimeCrystalMapper(NanoBrainKernel *kernel,
                                                 TimeCrystalKernel *tc_kernel)
    : kernel_(kernel), tc_kernel_(tc_kernel) {}

std::string NeuronTimeCrystalMapper::create_neuron(const std::string &name) {
  std::string id = generate_neuron_id();

  NeuronTimeCrystalMap neuron;
  neuron.neuron_id = id;

  // Create soma
  neuron.soma.segment_id = generate_segment_id();
  neuron.soma.segment_type = "soma";
  neuron.soma.scale_level = 0;
  neuron.soma.prime_encoding = generate_segment_primes("soma", 0);
  neuron.soma.triplet_bands = {440.0f, 550.0f, 660.0f}; // C, E, G

  // Generate time crystal state
  neuron.soma.time_crystal_state.dimensions =
      std::array<float, TIME_CRYSTAL_DIMENSIONS>{};
  for (int i = 0; i < TIME_CRYSTAL_DIMENSIONS; i++) {
    neuron.soma.time_crystal_state.dimensions[i] =
        std::sin(i * PI / TIME_CRYSTAL_DIMENSIONS);
  }
  neuron.soma.time_crystal_state.prime_signature = neuron.soma.prime_encoding;
  neuron.soma.time_crystal_state.temporal_coherence = 0.9f;
  neuron.soma.time_crystal_state.fractal_dimension = 2.5f;
  neuron.soma.time_crystal_state.resonance_frequency =
      tc_kernel_->calculate_resonance_frequency(neuron.soma.prime_encoding);
  neuron.soma.time_crystal_state.quantum_phase = 0.0f;

  all_segments_[neuron.soma.segment_id] = neuron.soma;
  neurons_[id] = neuron;

  std::cout << "[NeuronTimeCrystalMapper] Created neuron " << id << " (" << name
            << ")" << std::endl;

  return id;
}

std::string
NeuronTimeCrystalMapper::add_axon_segment(const std::string &neuron_id,
                                          int scale_level) {
  auto it = neurons_.find(neuron_id);
  if (it == neurons_.end())
    return "";

  NeuronSegment segment;
  segment.segment_id = generate_segment_id();
  segment.segment_type = "axon";
  segment.scale_level = scale_level;
  segment.prime_encoding = generate_segment_primes("axon", scale_level);

  // Triplet bands based on scale level
  float base_freq = 440.0f * std::pow(2.0f, scale_level / 3.0f);
  segment.triplet_bands = {base_freq, base_freq * 1.25f, base_freq * 1.5f};

  // Time crystal state
  segment.time_crystal_state.dimensions =
      std::array<float, TIME_CRYSTAL_DIMENSIONS>{};
  for (int i = 0; i < TIME_CRYSTAL_DIMENSIONS; i++) {
    segment.time_crystal_state.dimensions[i] =
        std::sin((i + scale_level) * PI / TIME_CRYSTAL_DIMENSIONS);
  }
  segment.time_crystal_state.prime_signature = segment.prime_encoding;
  segment.time_crystal_state.temporal_coherence = 0.85f - scale_level * 0.1f;
  segment.time_crystal_state.fractal_dimension = 1.8f + scale_level * 0.2f;
  segment.time_crystal_state.resonance_frequency =
      tc_kernel_->calculate_resonance_frequency(segment.prime_encoding);
  segment.time_crystal_state.quantum_phase = scale_level * 0.5f;

  all_segments_[segment.segment_id] = segment;
  it->second.axon_segments.push_back(segment);
  it->second.scale_hierarchy[scale_level].push_back(segment.segment_id);

  return segment.segment_id;
}

std::string
NeuronTimeCrystalMapper::add_dendrite_segment(const std::string &neuron_id,
                                              int scale_level) {
  auto it = neurons_.find(neuron_id);
  if (it == neurons_.end())
    return "";

  NeuronSegment segment;
  segment.segment_id = generate_segment_id();
  segment.segment_type = "dendrite";
  segment.scale_level = scale_level;
  segment.prime_encoding = generate_segment_primes("dendrite", scale_level);

  // Different triplet bands for dendrites
  float base_freq = 220.0f * std::pow(2.0f, scale_level / 4.0f);
  segment.triplet_bands = {base_freq, base_freq * 1.33f, base_freq * 1.67f};

  // Time crystal state
  segment.time_crystal_state.dimensions =
      std::array<float, TIME_CRYSTAL_DIMENSIONS>{};
  for (int i = 0; i < TIME_CRYSTAL_DIMENSIONS; i++) {
    segment.time_crystal_state.dimensions[i] =
        std::cos((i + scale_level * 2) * PI / TIME_CRYSTAL_DIMENSIONS);
  }
  segment.time_crystal_state.prime_signature = segment.prime_encoding;
  segment.time_crystal_state.temporal_coherence = 0.8f - scale_level * 0.05f;
  segment.time_crystal_state.fractal_dimension = 2.0f + scale_level * 0.15f;
  segment.time_crystal_state.resonance_frequency =
      tc_kernel_->calculate_resonance_frequency(segment.prime_encoding);
  segment.time_crystal_state.quantum_phase = scale_level * 0.3f;

  all_segments_[segment.segment_id] = segment;
  it->second.dendrite_segments.push_back(segment);
  it->second.scale_hierarchy[scale_level].push_back(segment.segment_id);

  return segment.segment_id;
}

void NeuronTimeCrystalMapper::map_axon_triplet_bands(
    const std::string &neuron_id) {
  auto it = neurons_.find(neuron_id);
  if (it == neurons_.end())
    return;

  // Create triplet band mapping based on segment positions
  int segment_idx = 0;
  for (auto &segment : it->second.axon_segments) {
    // Three bands: proximal, middle, distal
    float position_factor =
        static_cast<float>(segment_idx) / (it->second.axon_segments.size() + 1);

    segment.triplet_bands[0] = 440.0f * (1.0f + position_factor * 0.5f);
    segment.triplet_bands[1] = 550.0f * (1.0f + position_factor * 0.4f);
    segment.triplet_bands[2] = 660.0f * (1.0f + position_factor * 0.3f);

    // Update in all_segments_
    all_segments_[segment.segment_id] = segment;
    segment_idx++;
  }
}

void NeuronTimeCrystalMapper::add_scale_free_transitions(
    const std::string &neuron_id) {
  auto it = neurons_.find(neuron_id);
  if (it == neurons_.end())
    return;

  // Create scale-free transitions between hierarchy levels
  for (int level = 0; level < AXON_SCALE_LEVELS - 1; level++) {
    auto &current_level = it->second.scale_hierarchy[level];
    auto &next_level = it->second.scale_hierarchy[level + 1];

    // Each segment connects to 2-3 segments at next level
    for (const auto &seg_id : current_level) {
      auto seg_it = all_segments_.find(seg_id);
      if (seg_it == all_segments_.end())
        continue;

      // Update temporal coherence for scale-free behavior
      seg_it->second.time_crystal_state.temporal_coherence *=
          std::pow(0.9f, level);
    }
  }
}

std::array<float, TRIPLET_INNER_BANDS>
NeuronTimeCrystalMapper::get_segment_bands(
    const std::string &segment_id) const {
  auto it = all_segments_.find(segment_id);
  if (it != all_segments_.end()) {
    return it->second.triplet_bands;
  }
  return {0.0f, 0.0f, 0.0f};
}

void NeuronTimeCrystalMapper::create_neuron_crystal_hierarchy(
    const std::string &neuron_id) {
  auto it = neurons_.find(neuron_id);
  if (it == neurons_.end())
    return;

  // Add segments at each scale level
  for (int level = 0; level < AXON_SCALE_LEVELS; level++) {
    // More segments at finer scales
    int num_axon = (AXON_SCALE_LEVELS - level) * 2;
    int num_dendrite = (AXON_SCALE_LEVELS - level) * 3;

    for (int i = 0; i < num_axon; i++) {
      add_axon_segment(neuron_id, level);
    }

    for (int i = 0; i < num_dendrite; i++) {
      add_dendrite_segment(neuron_id, level);
    }
  }

  // Map triplet bands and transitions
  map_axon_triplet_bands(neuron_id);
  add_scale_free_transitions(neuron_id);
}

void NeuronTimeCrystalMapper::update_crystal_states() {
  for (auto &[seg_id, segment] : all_segments_) {
    // Phase evolution
    segment.time_crystal_state.quantum_phase += 0.1f;
    if (segment.time_crystal_state.quantum_phase >= 2.0f * PI) {
      segment.time_crystal_state.quantum_phase -= 2.0f * PI;
    }

    // Coherence decay and restoration
    segment.time_crystal_state.temporal_coherence *= 0.999f;
    if (segment.time_crystal_state.temporal_coherence < 0.3f) {
      segment.time_crystal_state.temporal_coherence =
          tc_kernel_->compute_ppm_coherence(segment.prime_encoding);
    }
  }
}

const TimeCrystalQuantumState *NeuronTimeCrystalMapper::get_segment_crystal(
    const std::string &segment_id) const {
  auto it = all_segments_.find(segment_id);
  if (it != all_segments_.end()) {
    return &it->second.time_crystal_state;
  }
  return nullptr;
}

const NeuronTimeCrystalMap *
NeuronTimeCrystalMapper::get_neuron(const std::string &neuron_id) const {
  auto it = neurons_.find(neuron_id);
  if (it != neurons_.end()) {
    return &it->second;
  }
  return nullptr;
}

std::vector<std::string> NeuronTimeCrystalMapper::get_all_neuron_ids() const {
  std::vector<std::string> ids;
  for (const auto &[id, _] : neurons_) {
    ids.push_back(id);
  }
  return ids;
}

const NeuronSegment *
NeuronTimeCrystalMapper::get_segment(const std::string &segment_id) const {
  auto it = all_segments_.find(segment_id);
  if (it != all_segments_.end()) {
    return &it->second;
  }
  return nullptr;
}

std::string NeuronTimeCrystalMapper::generate_neuron_id() {
  return "neuron_" + std::to_string(neuron_counter_++);
}

std::string NeuronTimeCrystalMapper::generate_segment_id() {
  return "segment_" + std::to_string(segment_counter_++);
}

std::vector<int> NeuronTimeCrystalMapper::generate_segment_primes(
    const std::string &segment_type, int scale_level) {
  std::vector<int> primes;

  if (segment_type == "soma") {
    primes = {2, 3, 5, 7}; // Core primes
  } else if (segment_type == "axon") {
    primes = {11, 13, 17}; // Transmission primes
  } else if (segment_type == "dendrite") {
    primes = {19, 23, 29}; // Reception primes
  }

  // Add scale-specific prime
  if (scale_level >= 0 && scale_level < FUNDAMENTAL_PRIMES_COUNT) {
    primes.push_back(FUNDAMENTAL_PRIMES[scale_level]);
  }

  return primes;
}

// ================================================================
// FilamentCommunicator Implementation
// ================================================================

FilamentCommunicator::FilamentCommunicator(
    NanoBrainKernel *kernel, AttentionAllocationEngine *attention_engine)
    : kernel_(kernel), attention_engine_(attention_engine) {}

void FilamentCommunicator::initialize(size_t history_size) {
  max_history_size_ = history_size;
  signal_history_.clear();
  initialized_ = true;
  std::cout << "[FilamentCommunicator] Initialized with history size "
            << history_size << std::endl;
}

bool FilamentCommunicator::detect_pre_spike_signal(
    const std::string &source_id, const std::vector<NodeTensor *> &nodes) {
  if (!initialized_)
    return false;

  // Calculate attention derivative (rate of change)
  float derivative = calculate_attention_derivative(source_id, nodes);

  // Pre-spike detection: rapid increase in attention
  bool is_pre_spike = derivative > pre_spike_threshold_;

  if (is_pre_spike) {
    // Create signal record
    FilamentSignal signal;
    signal.source_id = source_id;
    signal.target_id = ""; // Will be determined by signal propagation
    signal.timestamp = current_time_millis();
    signal.signal_strength = derivative;
    signal.prime_encoding = generate_signal_primes(derivative);
    signal.is_pre_spike = true;
    signal.predicted_timing = predict_spike_timing(source_id);

    signal_history_.push_back(signal);

    // Limit history size
    while (signal_history_.size() > max_history_size_) {
      signal_history_.erase(signal_history_.begin());
    }
  }

  return is_pre_spike;
}

const FilamentSignal *FilamentCommunicator::get_last_signal() const {
  if (signal_history_.empty())
    return nullptr;
  return &signal_history_.back();
}

std::vector<FilamentSignal> FilamentCommunicator::get_signals_for_source(
    const std::string &source_id) const {
  std::vector<FilamentSignal> signals;
  for (const auto &signal : signal_history_) {
    if (signal.source_id == source_id) {
      signals.push_back(signal);
    }
  }
  return signals;
}

float FilamentCommunicator::predict_spike_timing(const std::string &source_id) {
  // Get recent signals for this source
  auto signals = get_signals_for_source(source_id);

  if (signals.size() < 2) {
    return 10.0f; // Default: 10ms
  }

  // Calculate average interval between signals
  float avg_interval = 0.0f;
  for (size_t i = 1; i < signals.size(); i++) {
    avg_interval += signals[i].timestamp - signals[i - 1].timestamp;
  }
  avg_interval /= (signals.size() - 1);

  // Predict next spike based on current signal strength
  const auto *last = get_last_signal();
  if (last && last->signal_strength > 0) {
    // Stronger signal = faster spike
    return avg_interval / last->signal_strength;
  }

  return avg_interval;
}

float FilamentCommunicator::get_prediction_confidence(
    const std::string &source_id) const {
  auto signals = get_signals_for_source(source_id);

  if (signals.size() < 3)
    return 0.3f; // Low confidence with few samples

  // Calculate variance in inter-signal intervals
  std::vector<float> intervals;
  for (size_t i = 1; i < signals.size(); i++) {
    intervals.push_back(
        static_cast<float>(signals[i].timestamp - signals[i - 1].timestamp));
  }

  float mean = std::accumulate(intervals.begin(), intervals.end(), 0.0f) /
               intervals.size();
  float variance = 0.0f;
  for (float interval : intervals) {
    variance += (interval - mean) * (interval - mean);
  }
  variance /= intervals.size();

  // Lower variance = higher confidence
  float cv = std::sqrt(variance) / (mean + 1.0f); // Coefficient of variation
  return std::max(0.1f, 1.0f - cv);
}

FilamentSignal
FilamentCommunicator::encode_hidden_signal(const std::string &source_id,
                                           const std::string &target_id,
                                           float signal_strength) {
  FilamentSignal signal;
  signal.source_id = source_id;
  signal.target_id = target_id;
  signal.timestamp = current_time_millis();
  signal.signal_strength = signal_strength;
  signal.prime_encoding = generate_signal_primes(signal_strength);
  signal.is_pre_spike = signal_strength > pre_spike_threshold_;
  signal.predicted_timing = predict_spike_timing(source_id);

  signal_history_.push_back(signal);

  return signal;
}

float FilamentCommunicator::decode_hidden_signal(const FilamentSignal &signal) {
  // Decode using PPM coherence
  if (signal.prime_encoding.empty())
    return signal.signal_strength;

  double product = 1.0;
  double sum = 0.0;
  for (int p : signal.prime_encoding) {
    product *= p;
    sum += p;
  }

  float coherence =
      static_cast<float>(0.5 + 0.5 * std::sin(std::sqrt(product) * PI / sum));

  return signal.signal_strength * coherence;
}

float FilamentCommunicator::get_mean_signal_strength() const {
  if (signal_history_.empty())
    return 0.0f;

  float sum = 0.0f;
  for (const auto &signal : signal_history_) {
    sum += signal.signal_strength;
  }
  return sum / signal_history_.size();
}

float FilamentCommunicator::get_signal_variance() const {
  if (signal_history_.size() < 2)
    return 0.0f;

  float mean = get_mean_signal_strength();
  float variance = 0.0f;

  for (const auto &signal : signal_history_) {
    float diff = signal.signal_strength - mean;
    variance += diff * diff;
  }

  return variance / signal_history_.size();
}

void FilamentCommunicator::clear_history() { signal_history_.clear(); }

float FilamentCommunicator::calculate_attention_derivative(
    const std::string &source_id, const std::vector<NodeTensor *> &nodes) {
  // Find node with matching ID
  for (const auto *node : nodes) {
    if (node && node->atom_id == source_id && node->attention_weights) {
      kernel_->compute(node->attention_weights);
      float current = kernel_->get_value(node->attention_weights, 0);

      // Estimate derivative from current value
      // In practice, would compare with previous value
      return current / 100.0f; // Normalized derivative estimate
    }
  }

  return 0.0f;
}

std::vector<int> FilamentCommunicator::generate_signal_primes(float strength) {
  std::vector<int> primes;

  // Select primes based on strength level
  if (strength < 0.3f) {
    primes = {2, 3}; // Low strength: simple primes
  } else if (strength < 0.6f) {
    primes = {2, 3, 5, 7}; // Medium strength
  } else if (strength < 0.9f) {
    primes = {2, 3, 5, 7, 11, 13}; // High strength
  } else {
    primes = {2, 3, 5, 7, 11, 13, 17, 19}; // Very high strength
  }

  return primes;
}

int64_t FilamentCommunicator::current_time_millis() const {
  return std::chrono::duration_cast<std::chrono::milliseconds>(
             std::chrono::system_clock::now().time_since_epoch())
      .count();
}
