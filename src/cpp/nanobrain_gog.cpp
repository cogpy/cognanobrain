#include "nanobrain_gog.h"
#include <algorithm>
#include <cmath>
#include <numeric>

// ================================================================
// GardenOfGardens Implementation
// ================================================================

GardenOfGardens::GardenOfGardens(NanoBrainKernel *k, const GOGConfig &cfg)
    : kernel(k), config(cfg), initialized(false), garden_counter(0),
      petal_counter(0), evolution_step(0) {}

GardenOfGardens::~GardenOfGardens() { reset(); }

void GardenOfGardens::initialize() {
  reset();
  initialized = true;
}

void GardenOfGardens::reset() {
  root_gardens.clear();
  all_gardens.clear();
  garden_counter = 0;
  petal_counter = 0;
  evolution_step = 0;
  initialized = false;
}

std::string GardenOfGardens::generate_garden_id() {
  return "gog_" + std::to_string(++garden_counter);
}

std::string GardenOfGardens::generate_petal_id() {
  return "petal_" + std::to_string(++petal_counter);
}

// ================================================================
// Garden Management
// ================================================================

std::string GardenOfGardens::create_garden(MeanderPattern pattern) {
  auto garden = std::make_shared<GOGNode>();
  garden->id = generate_garden_id();
  garden->depth = 0;
  garden->meander_phase = 0.0f;
  garden->pattern = pattern;
  garden->coherence = 1.0f;
  garden->resonance_frequency = 1.0f;

  // Create default petals
  for (int i = 0; i < config.petals_per_garden; i++) {
    GOGPetal petal;
    petal.id = generate_petal_id();
    petal.phase = calculate_petal_phase(i, config.petals_per_garden, pattern);
    petal.amplitude = 1.0f;
    petal.prime_signature = {FUNDAMENTAL_PRIMES[i % 15]};
    petal.geometry.shape = index_to_gml_shape(i);
    petal.geometry.dimensions = 3;
    petal.tensor = nullptr;
    garden->petals.push_back(petal);
  }

  root_gardens.push_back(garden);
  all_gardens[garden->id] = garden;

  return garden->id;
}

std::string GardenOfGardens::create_sub_garden(const std::string &parent_id) {
  auto parent_it = all_gardens.find(parent_id);
  if (parent_it == all_gardens.end())
    return "";

  auto parent = parent_it->second;
  if (parent->depth >= config.max_depth)
    return ""; // Max depth reached

  auto garden = std::make_shared<GOGNode>();
  garden->id = generate_garden_id();
  garden->depth = parent->depth + 1;
  garden->meander_phase = parent->meander_phase;
  garden->pattern = parent->pattern;
  garden->coherence = parent->coherence * 0.9f;
  garden->resonance_frequency = parent->resonance_frequency * GOG_PHI;

  // Create scaled petals
  int num_petals = std::max(3, config.petals_per_garden - garden->depth);
  for (int i = 0; i < num_petals; i++) {
    GOGPetal petal;
    petal.id = generate_petal_id();
    petal.phase = calculate_petal_phase(i, num_petals, garden->pattern);
    petal.amplitude = 1.0f / (garden->depth + 1);
    petal.prime_signature = {FUNDAMENTAL_PRIMES[(i + garden->depth) % 15]};
    petal.geometry.shape = index_to_gml_shape(i + garden->depth);
    petal.geometry.dimensions = 3;
    petal.tensor = nullptr;
    garden->petals.push_back(petal);
  }

  parent->sub_gardens.push_back(garden);
  all_gardens[garden->id] = garden;

  return garden->id;
}

std::string GardenOfGardens::add_petal(const std::string &garden_id,
                                       const std::vector<int> &primes) {
  auto garden = get_garden(garden_id);
  if (!garden)
    return "";

  GOGPetal petal;
  petal.id = generate_petal_id();
  petal.phase = calculate_petal_phase(
      static_cast<int>(garden->petals.size()),
      static_cast<int>(garden->petals.size()) + 1, garden->pattern);
  petal.amplitude = 1.0f;
  petal.prime_signature = primes;
  petal.geometry.shape =
      index_to_gml_shape(primes.empty() ? 0 : primes[0] % 19);
  petal.geometry.dimensions = 3;
  petal.tensor = nullptr;

  garden->petals.push_back(petal);
  return petal.id;
}

GOGNode *GardenOfGardens::get_garden(const std::string &id) {
  auto it = all_gardens.find(id);
  return (it != all_gardens.end()) ? it->second.get() : nullptr;
}

const GOGNode *GardenOfGardens::get_garden(const std::string &id) const {
  auto it = all_gardens.find(id);
  return (it != all_gardens.end()) ? it->second.get() : nullptr;
}

bool GardenOfGardens::remove_garden(const std::string &id) {
  auto it = all_gardens.find(id);
  if (it == all_gardens.end())
    return false;

  auto garden = it->second;

  // Remove from root if present
  root_gardens.erase(
      std::remove_if(root_gardens.begin(), root_gardens.end(),
                     [&id](const auto &g) { return g->id == id; }),
      root_gardens.end());

  // Recursively remove sub-gardens
  for (auto &sub : garden->sub_gardens) {
    remove_garden(sub->id);
  }

  all_gardens.erase(it);
  return true;
}

// ================================================================
// Meander Evolution
// ================================================================

float GardenOfGardens::calculate_petal_phase(int index, int total,
                                             MeanderPattern pattern) {
  float base_angle = 2.0f * M_PI * index / std::max(1, total);

  switch (pattern) {
  case MeanderPattern::Spiral:
    return base_angle + std::log(index + 1) * 0.5f;

  case MeanderPattern::Flower:
    return base_angle; // Uniform radial

  case MeanderPattern::Labyrinth:
    return base_angle + (index % 2) * M_PI; // Alternating

  case MeanderPattern::Fractal:
    return base_angle * GOG_PHI; // Golden angle

  case MeanderPattern::Nested:
    return base_angle + (index / 3) * M_PI / 6; // Grouped rings

  default:
    return base_angle;
  }
}

void GardenOfGardens::evolve_garden(GOGNode &garden) {
  // Update meander phase
  garden.meander_phase += config.meander_speed;
  if (garden.meander_phase > 2.0f * M_PI) {
    garden.meander_phase -= 2.0f * M_PI;
  }

  // Evolve petals
  for (auto &petal : garden.petals) {
    petal.phase += config.meander_speed * petal.amplitude;
    if (petal.phase > 2.0f * M_PI) {
      petal.phase -= 2.0f * M_PI;
    }
  }

  // Update coherence based on phase alignment
  garden.coherence = calculate_garden_coherence(garden.id);

  // Prune low-coherence gardens if enabled
  if (config.auto_prune &&
      garden.coherence < config.coherence_threshold * 0.5f) {
    // Mark for pruning (handled externally)
  }

  // Evolve sub-gardens
  for (auto &sub : garden.sub_gardens) {
    evolve_garden(*sub);
  }
}

void GardenOfGardens::evolve_step() {
  for (auto &garden : root_gardens) {
    evolve_garden(*garden);
  }
  evolution_step++;
}

void GardenOfGardens::evolve(int steps) {
  for (int i = 0; i < steps; i++) {
    evolve_step();
  }
}

void GardenOfGardens::synchronize_meanders() {
  if (all_gardens.empty())
    return;

  // Calculate average phase
  float sum_phase = 0.0f;
  for (const auto &[_, garden] : all_gardens) {
    sum_phase += garden->meander_phase;
  }
  float avg_phase = sum_phase / all_gardens.size();

  // Gently pull phases toward average
  for (auto &[_, garden] : all_gardens) {
    float diff = avg_phase - garden->meander_phase;
    garden->meander_phase += diff * 0.1f;
  }
}

// ================================================================
// Coherence & Resonance
// ================================================================

float GardenOfGardens::calculate_garden_coherence(
    const std::string &garden_id) const {
  auto garden = get_garden(garden_id);
  if (!garden || garden->petals.empty())
    return 0.0f;

  // Coherence based on phase alignment of petals
  float sum_cos = 0.0f, sum_sin = 0.0f;
  for (const auto &petal : garden->petals) {
    sum_cos += std::cos(petal.phase);
    sum_sin += std::sin(petal.phase);
  }

  float n = static_cast<float>(garden->petals.size());
  float r = std::sqrt(sum_cos * sum_cos + sum_sin * sum_sin) / n;

  return r;
}

float GardenOfGardens::calculate_global_coherence() const {
  if (all_gardens.empty())
    return 0.0f;

  float sum = 0.0f;
  for (const auto &[id, _] : all_gardens) {
    sum += calculate_garden_coherence(id);
  }

  return sum / all_gardens.size();
}

std::vector<std::pair<std::string, std::string>>
GardenOfGardens::find_resonant_pairs(float threshold) const {
  std::vector<std::pair<std::string, std::string>> pairs;

  std::vector<std::string> ids;
  for (const auto &[id, _] : all_gardens) {
    ids.push_back(id);
  }

  for (size_t i = 0; i < ids.size(); i++) {
    for (size_t j = i + 1; j < ids.size(); j++) {
      auto g1 = get_garden(ids[i]);
      auto g2 = get_garden(ids[j]);
      if (!g1 || !g2)
        continue;

      // Resonance based on frequency ratio
      float ratio = g1->resonance_frequency / g2->resonance_frequency;
      float resonance = 1.0f - std::abs(ratio - std::round(ratio));

      if (resonance > threshold) {
        pairs.push_back({ids[i], ids[j]});
      }
    }
  }

  return pairs;
}

// ================================================================
// Traversal
// ================================================================

void GardenOfGardens::traverse(std::function<void(GOGNode &)> callback) {
  for (auto &garden : root_gardens) {
    std::function<void(GOGNode &)> visit = [&](GOGNode &node) {
      callback(node);
      for (auto &sub : node.sub_gardens) {
        visit(*sub);
      }
    };
    visit(*garden);
  }
}

void GardenOfGardens::traverse_const(
    std::function<void(const GOGNode &)> callback) const {
  for (const auto &garden : root_gardens) {
    std::function<void(const GOGNode &)> visit = [&](const GOGNode &node) {
      callback(node);
      for (const auto &sub : node.sub_gardens) {
        visit(*sub);
      }
    };
    visit(*garden);
  }
}

std::vector<std::string>
GardenOfGardens::get_gardens_at_depth(int depth) const {
  std::vector<std::string> result;
  for (const auto &[id, garden] : all_gardens) {
    if (garden->depth == depth) {
      result.push_back(id);
    }
  }
  return result;
}

// ================================================================
// Metrics
// ================================================================

GOGMetrics GardenOfGardens::get_metrics() const {
  GOGMetrics m;
  m.total_gardens = all_gardens.size();
  m.total_petals = 0;
  m.max_depth_reached = 0;

  for (const auto &[_, garden] : all_gardens) {
    m.total_petals += garden->petals.size();
    m.max_depth_reached = std::max(m.max_depth_reached, garden->depth);
  }

  m.global_coherence = calculate_global_coherence();

  // Meander synchronization (variance of phases)
  if (!all_gardens.empty()) {
    float sum = 0.0f, sum_sq = 0.0f;
    for (const auto &[_, garden] : all_gardens) {
      sum += garden->meander_phase;
      sum_sq += garden->meander_phase * garden->meander_phase;
    }
    float n = static_cast<float>(all_gardens.size());
    float variance = (sum_sq / n) - (sum / n) * (sum / n);
    m.meander_synchronization = 1.0f / (1.0f + variance);
  } else {
    m.meander_synchronization = 0.0f;
  }

  // Fractal dimension approximation
  m.fractal_dimension =
      std::log(static_cast<float>(m.total_petals)) /
      std::log(static_cast<float>(m.max_depth_reached + 1) + 1);

  return m;
}
