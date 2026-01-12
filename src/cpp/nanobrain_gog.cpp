#include "nanobrain_gog.h"
#include <algorithm>
#include <chrono>
#include <cmath>
#include <iostream>
#include <numeric>
#include <random>
#include <sstream>

// ================================================================
// Constructor / Destructor
// ================================================================

GardenOfGardens::GardenOfGardens(NanoBrainKernel *kernel,
                                 const GOGConfig &config)
    : kernel(kernel), config(config), active(false), cycle_count(0),
      garden_counter(0) {}

GardenOfGardens::~GardenOfGardens() { shutdown(); }

// ================================================================
// Initialization
// ================================================================

void GardenOfGardens::initialize() {
  if (active)
    return;

  create_root_garden();
  active = true;

  std::cout << "[GardenOfGardens] Initialized with root garden: " << root_id
            << std::endl;
}

void GardenOfGardens::shutdown() {
  active = false;
  gardens.clear();
  root_id.clear();
  std::cout << "[GardenOfGardens] Shutdown after " << cycle_count << " cycles"
            << std::endl;
}

void GardenOfGardens::create_root_garden() {
  // Create root with fundamental primes
  std::vector<int> root_primes = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
  root_id = create_garden("", root_primes);
}

std::string GardenOfGardens::generate_garden_id() {
  std::stringstream ss;
  ss << "gog_" << garden_counter++;
  return ss.str();
}

int64_t GardenOfGardens::current_time_millis() const {
  return std::chrono::duration_cast<std::chrono::milliseconds>(
             std::chrono::system_clock::now().time_since_epoch())
      .count();
}

// ================================================================
// Garden Management
// ================================================================

std::string
GardenOfGardens::create_garden(const std::string &parent_id,
                               const std::vector<int> &prime_signature) {
  GOGNode node;
  node.id = generate_garden_id();
  node.parent_id = parent_id;
  node.meander_phase = 0.0f;
  node.meander_frequency = config.meander_frequency;
  node.coherence = 1.0f;
  node.resonance_score = 0.0f;
  node.last_updated = current_time_millis();

  // Determine depth
  if (parent_id.empty()) {
    node.depth = 0;
  } else {
    const auto *parent = get_garden(parent_id);
    if (parent) {
      node.depth = parent->depth + 1;
    } else {
      node.depth = 0;
    }
  }

  // Check depth limit
  if (node.depth > config.max_depth) {
    std::cerr << "[GardenOfGardens] Max depth exceeded, cannot create garden"
              << std::endl;
    return "";
  }

  // Initialize petals
  initialize_petals(node, prime_signature);

  // Register with parent
  if (!parent_id.empty()) {
    auto *parent = get_mutable_garden(parent_id);
    if (parent) {
      parent->sub_garden_ids.push_back(node.id);
    }
  }

  gardens[node.id] = node;
  return node.id;
}

const GOGNode *GardenOfGardens::get_garden(const std::string &id) const {
  auto it = gardens.find(id);
  if (it != gardens.end()) {
    return &it->second;
  }
  return nullptr;
}

GOGNode *GardenOfGardens::get_mutable_garden(const std::string &id) {
  auto it = gardens.find(id);
  if (it != gardens.end()) {
    return &it->second;
  }
  return nullptr;
}

std::vector<std::string> GardenOfGardens::get_all_garden_ids() const {
  std::vector<std::string> ids;
  ids.reserve(gardens.size());
  for (const auto &[id, _] : gardens) {
    ids.push_back(id);
  }
  return ids;
}

std::vector<std::string>
GardenOfGardens::get_gardens_at_depth(int depth) const {
  std::vector<std::string> result;
  for (const auto &[id, node] : gardens) {
    if (node.depth == depth) {
      result.push_back(id);
    }
  }
  return result;
}

bool GardenOfGardens::remove_garden(const std::string &id) {
  auto it = gardens.find(id);
  if (it == gardens.end()) {
    return false;
  }

  // Recursively remove sub-gardens
  std::vector<std::string> sub_ids = it->second.sub_garden_ids;
  for (const auto &sub_id : sub_ids) {
    remove_garden(sub_id);
  }

  // Remove from parent
  if (!it->second.parent_id.empty()) {
    auto *parent = get_mutable_garden(it->second.parent_id);
    if (parent) {
      auto &subs = parent->sub_garden_ids;
      subs.erase(std::remove(subs.begin(), subs.end(), id), subs.end());
    }
  }

  gardens.erase(it);
  return true;
}

// ================================================================
// Petal Operations
// ================================================================

void GardenOfGardens::initialize_petals(GOGNode &node,
                                        const std::vector<int> &primes) {
  static std::mt19937 rng(std::random_device{}());
  static std::uniform_real_distribution<float> phase_dist(0.0f, 2.0f * PI);
  static std::uniform_real_distribution<float> coherence_dist(0.5f, 1.0f);

  node.petals.clear();
  node.petals.reserve(config.petals_per_node);

  for (int i = 0; i < config.petals_per_node; i++) {
    GOGPetal petal;
    petal.index = i;
    petal.phase_offset = (2.0f * PI * i) / config.petals_per_node;
    petal.activation = 0.5f;

    // Assign prime from input or use fundamental primes
    if (i < static_cast<int>(primes.size())) {
      petal.prime_signature = {primes[i]};
    } else {
      petal.prime_signature = {
          FUNDAMENTAL_PRIMES[i % FUNDAMENTAL_PRIMES_COUNT]};
    }

    // Initialize quantum state
    petal.quantum_state.prime_signature = petal.prime_signature;
    petal.quantum_state.quantum_phase = phase_dist(rng);
    petal.quantum_state.temporal_coherence = coherence_dist(rng);
    petal.quantum_state.fractal_dimension = 3.0f + (i % 8);
    petal.quantum_state.resonance_frequency =
        440.0f * std::pow(2.0f, petal.prime_signature[0] / 12.0f);

    // Initialize dimensions
    for (int d = 0; d < TIME_CRYSTAL_DIMENSIONS; d++) {
      petal.quantum_state.dimensions[d] =
          std::sin(petal.prime_signature[0] * PI / 180.0f) *
          std::cos(d * PI / 6.0f);
    }

    node.petals.push_back(petal);
  }
}

void GardenOfGardens::update_petal_states(GOGNode &node) {
  for (auto &petal : node.petals) {
    // Phase evolution
    petal.quantum_state.quantum_phase +=
        0.01f * petal.quantum_state.resonance_frequency / 440.0f;
    while (petal.quantum_state.quantum_phase >= 2.0f * PI) {
      petal.quantum_state.quantum_phase -= 2.0f * PI;
    }

    // Meander influence on activation
    float meander_effect = calculate_meander_influence(node, petal.index);
    petal.activation = 0.5f + 0.5f * meander_effect;

    // Coherence decay and regeneration
    petal.quantum_state.temporal_coherence *= 0.999f;
    if (petal.quantum_state.temporal_coherence < 0.3f) {
      // Regenerate based on prime harmony
      float prime_sum = 0.0f;
      for (int p : petal.prime_signature) {
        prime_sum += p;
      }
      petal.quantum_state.temporal_coherence =
          std::sin(prime_sum * PI / 100.0f) * 0.5f + 0.5f;
    }
  }
}

NanoBrainTensor *
GardenOfGardens::get_petal_activations(const std::string &garden_id) {
  const auto *node = get_garden(garden_id);
  if (!node || node->petals.empty()) {
    return nullptr;
  }

  auto *tensor =
      kernel->create_tensor({static_cast<int64_t>(node->petals.size())});
  std::vector<float> activations;
  activations.reserve(node->petals.size());

  for (const auto &petal : node->petals) {
    activations.push_back(petal.activation);
  }

  kernel->set_data(tensor, activations);
  return tensor;
}

// ================================================================
// Meander Dynamics
// ================================================================

void GardenOfGardens::update_meander_phases() {
  for (auto &[id, node] : gardens) {
    // Update meander phase
    node.meander_phase += node.meander_frequency;
    while (node.meander_phase >= 2.0f * PI) {
      node.meander_phase -= 2.0f * PI;
    }

    // Synchronize with parent if exists
    if (!node.parent_id.empty()) {
      auto *parent = get_mutable_garden(node.parent_id);
      if (parent) {
        synchronize_meander(*parent, node);
      }
    }

    node.last_updated = current_time_millis();
  }
}

float GardenOfGardens::calculate_meander_influence(const GOGNode &node,
                                                   int petal_index) {
  if (petal_index < 0 || petal_index >= static_cast<int>(node.petals.size())) {
    return 0.0f;
  }

  const auto &petal = node.petals[petal_index];

  // Meander creates oscillating influence based on petal position and phase
  float angular_pos = petal.phase_offset + node.meander_phase;
  float meander_wave =
      std::sin(angular_pos) * std::cos(angular_pos * GOLDEN_RATIO);

  // Modulate by depth (deeper gardens have gentler meanders)
  float depth_factor = 1.0f / (1.0f + node.depth * 0.2f);

  return meander_wave * depth_factor;
}

void GardenOfGardens::synchronize_meander(GOGNode &parent, GOGNode &child) {
  // Child meander is influenced by parent
  float phase_diff = parent.meander_phase - child.meander_phase;
  float sync_strength = 0.1f;

  child.meander_phase += phase_diff * sync_strength;

  // Keep phase in valid range
  while (child.meander_phase < 0) {
    child.meander_phase += 2.0f * PI;
  }
  while (child.meander_phase >= 2.0f * PI) {
    child.meander_phase -= 2.0f * PI;
  }
}

// ================================================================
// Coherence & Resonance
// ================================================================

float GardenOfGardens::compute_garden_coherence(const GOGNode &node) {
  if (node.petals.empty()) {
    return 0.0f;
  }

  // Compute PPM-style coherence from petal primes
  std::vector<int> all_primes;
  for (const auto &petal : node.petals) {
    all_primes.insert(all_primes.end(), petal.prime_signature.begin(),
                      petal.prime_signature.end());
  }

  // Product and sum of primes
  double product = 1.0;
  double sum = 0.0;
  for (int p : all_primes) {
    product *= p;
    sum += p;
  }

  // PPM coherence: 0.5 + 0.5 * sin(sqrt(prod) * PI / sum)
  double sqrt_prod = std::sqrt(product);
  double arg = sqrt_prod * PI / sum;
  float base_coherence = static_cast<float>(0.5 + 0.5 * std::sin(arg));

  // Factor in petal temporal coherence
  float petal_coherence = 0.0f;
  for (const auto &petal : node.petals) {
    petal_coherence += petal.quantum_state.temporal_coherence;
  }
  petal_coherence /= node.petals.size();

  return (base_coherence + petal_coherence) / 2.0f;
}

void GardenOfGardens::update_all_coherences() {
  for (auto &[id, node] : gardens) {
    node.coherence = compute_garden_coherence(node);
  }
}

float GardenOfGardens::calculate_resonance(const GOGNode &node1,
                                           const GOGNode &node2) {
  if (node1.petals.empty() || node2.petals.empty()) {
    return 0.0f;
  }

  // Phase alignment
  float phase_diff = std::abs(node1.meander_phase - node2.meander_phase);
  float phase_alignment = std::cos(phase_diff);

  // Prime overlap
  std::vector<int> primes1, primes2;
  for (const auto &p : node1.petals) {
    primes1.insert(primes1.end(), p.prime_signature.begin(),
                   p.prime_signature.end());
  }
  for (const auto &p : node2.petals) {
    primes2.insert(primes2.end(), p.prime_signature.begin(),
                   p.prime_signature.end());
  }

  int overlap = 0;
  for (int p : primes1) {
    if (std::find(primes2.begin(), primes2.end(), p) != primes2.end()) {
      overlap++;
    }
  }
  float prime_overlap =
      static_cast<float>(overlap) / std::max(primes1.size(), primes2.size());

  // Coherence product
  float coherence_factor = node1.coherence * node2.coherence;

  return (phase_alignment + prime_overlap + coherence_factor) / 3.0f;
}

std::vector<std::string>
GardenOfGardens::find_resonant_gardens(const std::string &garden_id,
                                       float threshold) {
  std::vector<std::string> result;

  const auto *source = get_garden(garden_id);
  if (!source) {
    return result;
  }

  for (const auto &[id, node] : gardens) {
    if (id == garden_id)
      continue;

    float resonance = calculate_resonance(*source, node);
    if (resonance >= threshold) {
      result.push_back(id);
    }
  }

  return result;
}

void GardenOfGardens::update_resonance_network() {
  for (auto &[id, node] : gardens) {
    // Calculate resonance with siblings
    float total_resonance = 0.0f;
    int count = 0;

    for (const auto &[other_id, other] : gardens) {
      if (other_id == id)
        continue;
      if (std::abs(other.depth - node.depth) <= 1) {
        total_resonance += calculate_resonance(node, other);
        count++;
      }
    }

    node.resonance_score = count > 0 ? total_resonance / count : 0.0f;
  }
}

// ================================================================
// Processing Cycle
// ================================================================

void GardenOfGardens::process_cycle() {
  if (!active)
    return;

  // 1. Update meander phases
  update_meander_phases();

  // 2. Update petal states
  for (auto &[id, node] : gardens) {
    update_petal_states(node);
  }

  // 3. Update coherences
  update_all_coherences();

  // 4. Update resonance network
  update_resonance_network();

  // 5. Propagate signals
  propagate_bottom_up();
  propagate_top_down();

  // 6. Pruning and growth (if enabled)
  if (config.enable_auto_pruning) {
    prune_weak_gardens(0.1f);
  }

  cycle_count++;
}

void GardenOfGardens::propagate_bottom_up() {
  // Find max depth
  int max_depth = 0;
  for (const auto &[id, node] : gardens) {
    max_depth = std::max(max_depth, node.depth);
  }

  // Propagate from deepest to root
  for (int depth = max_depth; depth > 0; depth--) {
    auto level_ids = get_gardens_at_depth(depth);
    for (const auto &id : level_ids) {
      const auto *child = get_garden(id);
      if (!child || child->parent_id.empty())
        continue;

      auto *parent = get_mutable_garden(child->parent_id);
      if (!parent)
        continue;

      // Transfer coherence influence
      parent->coherence = (parent->coherence + child->coherence * 0.1f) / 1.1f;
    }
  }
}

void GardenOfGardens::propagate_top_down() {
  // Find max depth
  int max_depth = 0;
  for (const auto &[id, node] : gardens) {
    max_depth = std::max(max_depth, node.depth);
  }

  // Propagate from root to deepest
  for (int depth = 0; depth < max_depth; depth++) {
    auto level_ids = get_gardens_at_depth(depth);
    for (const auto &id : level_ids) {
      const auto *parent = get_garden(id);
      if (!parent)
        continue;

      for (const auto &child_id : parent->sub_garden_ids) {
        auto *child = get_mutable_garden(child_id);
        if (!child)
          continue;

        // Transfer meander frequency influence
        child->meander_frequency =
            0.9f * child->meander_frequency + 0.1f * parent->meander_frequency;
      }
    }
  }
}

std::vector<std::string>
GardenOfGardens::match_pattern(NanoBrainTensor *pattern) {
  std::vector<std::pair<std::string, float>> matches;

  // Simple activation matching
  float *pattern_data = static_cast<float *>(pattern->ggml_tensor->data);
  int64_t pattern_size = ggml_nelements(pattern->ggml_tensor);

  for (const auto &[id, node] : gardens) {
    if (node.petals.size() != static_cast<size_t>(pattern_size))
      continue;

    float similarity = 0.0f;
    for (size_t i = 0; i < node.petals.size(); i++) {
      float diff = node.petals[i].activation - pattern_data[i];
      similarity += 1.0f - std::abs(diff);
    }
    similarity /= node.petals.size();

    if (similarity > 0.5f) {
      matches.push_back({id, similarity});
    }
  }

  // Sort by similarity
  std::sort(matches.begin(), matches.end(),
            [](const auto &a, const auto &b) { return a.second > b.second; });

  std::vector<std::string> result;
  for (const auto &m : matches) {
    result.push_back(m.first);
  }

  return result;
}

// ================================================================
// Pruning & Growth
// ================================================================

void GardenOfGardens::prune_weak_gardens(float coherence_threshold) {
  std::vector<std::string> to_remove;

  for (const auto &[id, node] : gardens) {
    // Don't prune root
    if (id == root_id)
      continue;

    if (node.coherence < coherence_threshold && node.sub_garden_ids.empty()) {
      to_remove.push_back(id);
    }
  }

  for (const auto &id : to_remove) {
    remove_garden(id);
  }
}

void GardenOfGardens::grow_active_regions(float activation_threshold) {
  std::vector<std::string> to_grow;

  for (const auto &[id, node] : gardens) {
    // Check if garden has high activation
    float avg_activation = 0.0f;
    for (const auto &petal : node.petals) {
      avg_activation += petal.activation;
    }
    avg_activation /= node.petals.size();

    if (avg_activation > activation_threshold &&
        node.depth < config.max_depth && node.sub_garden_ids.size() < 3) {
      to_grow.push_back(id);
    }
  }

  for (const auto &id : to_grow) {
    const auto *parent = get_garden(id);
    if (!parent)
      continue;

    // Create child with inherited primes
    std::vector<int> child_primes;
    for (const auto &petal : parent->petals) {
      if (!petal.prime_signature.empty()) {
        child_primes.push_back(petal.prime_signature[0]);
      }
    }

    create_garden(id, child_primes);
  }
}

// ================================================================
// Metrics
// ================================================================

GOGMetrics GardenOfGardens::get_metrics() const {
  GOGMetrics metrics = {};
  metrics.total_gardens = gardens.size();

  int max_depth = 0;
  float total_coherence = 0.0f;
  float total_resonance = 0.0f;

  for (const auto &[id, node] : gardens) {
    metrics.total_petals += node.petals.size();
    max_depth = std::max(max_depth, node.depth);
    total_coherence += node.coherence;
    total_resonance += node.resonance_score;
  }

  metrics.max_achieved_depth = max_depth;
  metrics.average_coherence =
      gardens.empty() ? 0.0f : total_coherence / gardens.size();
  metrics.total_resonance = total_resonance;

  // Count resonant pairs
  for (const auto &[id, node] : gardens) {
    auto resonant = find_resonant_gardens(id, config.resonance_threshold);
    metrics.resonant_pairs += resonant.size();
  }
  metrics.resonant_pairs /= 2; // Each pair counted twice

  metrics.processing_efficiency =
      cycle_count > 0 ? metrics.average_coherence : 0.0f;

  return metrics;
}
