#include "nanobrain_fractal_tape.h"
#include <algorithm>
#include <chrono>
#include <cmath>
#include <numeric>
#include <sstream>

// ================================================================
// FractalTape Implementation
// ================================================================

FractalTape::FractalTape(const FractalTapeConfig &cfg)
    : config(cfg), initialized(false), cell_counter(0), generation_counter(0) {
  assembly_state.generation = 0;
  assembly_state.active_rules = 0;
  assembly_state.total_growth = 0.0f;
}

FractalTape::~FractalTape() { reset(); }

void FractalTape::initialize() {
  reset();
  initialized = true;
}

void FractalTape::reset() {
  cells.clear();
  root_cells.clear();
  rules.clear();
  cell_counter = 0;
  generation_counter = 0;
  assembly_state = SelfAssemblyState{};
  initialized = false;
}

std::string FractalTape::generate_cell_id() {
  return "fc_" + std::to_string(++cell_counter);
}

float FractalTape::calculate_cell_scale(int depth) const {
  return config.base_scale * std::pow(config.scale_ratio, depth);
}

// ================================================================
// Cell Management
// ================================================================

std::string FractalTape::create_cell(const std::array<float, 11> &position,
                                     const std::vector<float> &data,
                                     const std::vector<int> &primes) {
  auto cell = std::make_shared<FractalCell>();
  cell->id = generate_cell_id();
  cell->depth = 0;
  cell->position = position;
  cell->data = data;
  cell->prime_encoding = primes;
  cell->scale = config.base_scale;

  // Set default geometry based on primes
  cell->geometry.shape =
      index_to_gml_shape(primes.empty() ? 0 : primes[0] % 19);
  cell->geometry.dimensions = 3;
  cell->geometry.prime_resonance = primes;
  cell->geometry.scale_factor = cell->scale;

  cells[cell->id] = cell;
  root_cells.push_back(cell);

  return cell->id;
}

std::string FractalTape::create_child_cell(const std::string &parent_id,
                                           const std::vector<float> &data) {
  auto parent_it = cells.find(parent_id);
  if (parent_it == cells.end())
    return "";

  auto parent = parent_it->second;

  if (parent->depth >= config.max_depth)
    return ""; // Max depth reached

  auto child = std::make_shared<FractalCell>();
  child->id = generate_cell_id();
  child->depth = parent->depth + 1;
  child->position = parent->position; // Start at parent position
  child->data = data;
  child->prime_encoding = parent->prime_encoding;
  child->scale = calculate_cell_scale(child->depth);
  child->parent = parent;

  // Offset child position slightly
  for (int i = 0; i < 11; i++) {
    child->position[i] += child->scale * (i % 2 == 0 ? 0.1f : -0.1f);
  }

  // Inherit and modify geometry
  child->geometry = parent->geometry;
  child->geometry.scale_factor = child->scale;

  parent->children.push_back(child);
  cells[child->id] = child;

  return child->id;
}

FractalCell *FractalTape::get_cell(const std::string &id) {
  auto it = cells.find(id);
  return (it != cells.end()) ? it->second.get() : nullptr;
}

const FractalCell *FractalTape::get_cell(const std::string &id) const {
  auto it = cells.find(id);
  return (it != cells.end()) ? it->second.get() : nullptr;
}

bool FractalTape::remove_cell(const std::string &id) {
  auto it = cells.find(id);
  if (it == cells.end())
    return false;

  auto cell = it->second;

  // Remove from parent's children
  if (auto parent = cell->parent.lock()) {
    auto &siblings = parent->children;
    siblings.erase(std::remove_if(siblings.begin(), siblings.end(),
                                  [&id](const auto &c) { return c->id == id; }),
                   siblings.end());
  } else {
    // It's a root cell
    root_cells.erase(
        std::remove_if(root_cells.begin(), root_cells.end(),
                       [&id](const auto &c) { return c->id == id; }),
        root_cells.end());
  }

  // Recursively remove children
  for (auto &child : cell->children) {
    remove_cell(child->id);
  }

  cells.erase(it);
  return true;
}

std::vector<std::string> FractalTape::get_all_cell_ids() const {
  std::vector<std::string> ids;
  ids.reserve(cells.size());
  for (const auto &[id, _] : cells) {
    ids.push_back(id);
  }
  return ids;
}

std::vector<FractalCell *> FractalTape::get_cells_at_depth(int depth) {
  std::vector<FractalCell *> result;
  for (auto &[_, cell] : cells) {
    if (cell->depth == depth) {
      result.push_back(cell.get());
    }
  }
  return result;
}

// ================================================================
// Navigation
// ================================================================

float FractalTape::cell_distance(const FractalCell &a,
                                 const FractalCell &b) const {
  float sum = 0.0f;
  for (int i = 0; i < 11; i++) {
    float diff = a.position[i] - b.position[i];
    sum += diff * diff;
  }
  return std::sqrt(sum);
}

std::string
FractalTape::find_nearest_cell(const std::array<float, 11> &position) const {
  if (cells.empty())
    return "";

  std::string nearest_id;
  float min_dist = std::numeric_limits<float>::max();

  for (const auto &[id, cell] : cells) {
    float dist = 0.0f;
    for (int i = 0; i < 11; i++) {
      float diff = position[i] - cell->position[i];
      dist += diff * diff;
    }
    dist = std::sqrt(dist);

    if (dist < min_dist) {
      min_dist = dist;
      nearest_id = id;
    }
  }

  return nearest_id;
}

std::string FractalTape::navigate(const std::string &from_cell, int dimension,
                                  int direction) {
  auto cell = get_cell(from_cell);
  if (!cell || dimension < 0 || dimension >= 11)
    return from_cell;

  // Create target position
  std::array<float, 11> target = cell->position;
  target[dimension] += direction * cell->scale;

  return find_nearest_cell(target);
}

std::vector<std::string> FractalTape::find_path(const std::string &from,
                                                const std::string &to) const {
  // Simple path: just return start and end for now
  // Could implement A* for complex navigation
  std::vector<std::string> path;
  path.push_back(from);
  if (from != to) {
    path.push_back(to);
  }
  return path;
}

// ================================================================
// Sphere Surgery Operations
// ================================================================

SphereSurgeryResult FractalTape::perform_surgery(const std::string &cell_id,
                                                 SphereSurgeryType type,
                                                 float parameter) {
  switch (type) {
  case SphereSurgeryType::Inflation:
    return inflate_to_sphere(cell_id);
  case SphereSurgeryType::Deflation:
    return deflate_to_plane(cell_id);
  case SphereSurgeryType::Cutting:
    return cut_sphere(cell_id, parameter);
  default:
    return {false, {}, 0.0f, "Unsupported surgery type"};
  }
}

SphereSurgeryResult FractalTape::inflate_to_sphere(const std::string &cell_id) {
  SphereSurgeryResult result;
  auto cell = get_cell(cell_id);
  if (!cell) {
    result.success = false;
    result.transformation_log = "Cell not found";
    return result;
  }

  // Transform 2D plane representation to 3D sphere
  // Update geometry to sphere
  cell->geometry.shape = GMLShape::Sphere;
  cell->geometry.dimensions = 3;

  // Apply stereographic projection-like transformation to data
  if (cell->data.size() >= 2) {
    float x = cell->data[0];
    float y = cell->data[1];
    float r_sq = x * x + y * y;

    // Inverse stereographic projection
    if (cell->data.size() < 3) {
      cell->data.resize(3);
    }
    cell->data[0] = 2 * x / (1 + r_sq);
    cell->data[1] = 2 * y / (1 + r_sq);
    cell->data[2] = (r_sq - 1) / (1 + r_sq);
  }

  result.success = true;
  result.output_cells.push_back(*cell);
  result.topological_invariant = 2.0f; // Euler characteristic of sphere
  result.transformation_log = "Inflated 2D to 3D sphere";

  return result;
}

SphereSurgeryResult FractalTape::deflate_to_plane(const std::string &cell_id) {
  SphereSurgeryResult result;
  auto cell = get_cell(cell_id);
  if (!cell) {
    result.success = false;
    result.transformation_log = "Cell not found";
    return result;
  }

  // Transform 3D sphere to 2D plane
  cell->geometry.shape = GMLShape::Circle;
  cell->geometry.dimensions = 2;

  // Apply stereographic projection
  if (cell->data.size() >= 3) {
    float x = cell->data[0];
    float y = cell->data[1];
    float z = cell->data[2];

    // Stereographic projection from north pole
    float denom = 1.0f - z;
    if (std::abs(denom) > 0.001f) {
      cell->data[0] = x / denom;
      cell->data[1] = y / denom;
    }
    cell->data.resize(2);
  }

  result.success = true;
  result.output_cells.push_back(*cell);
  result.topological_invariant = 1.0f; // Euler characteristic of plane
  result.transformation_log = "Deflated 3D sphere to 2D plane";

  return result;
}

SphereSurgeryResult FractalTape::cut_sphere(const std::string &cell_id,
                                            float cut_plane_angle) {
  SphereSurgeryResult result;
  auto cell = get_cell(cell_id);
  if (!cell) {
    result.success = false;
    result.transformation_log = "Cell not found";
    return result;
  }

  // Create two hemisphere cells
  std::vector<float> data_upper = cell->data;
  std::vector<float> data_lower = cell->data;

  // Create upper hemisphere
  std::array<float, 11> pos_upper = cell->position;
  pos_upper[2] += cell->scale * 0.5f;
  std::string id_upper =
      create_cell(pos_upper, data_upper, cell->prime_encoding);

  // Create lower hemisphere
  std::array<float, 11> pos_lower = cell->position;
  pos_lower[2] -= cell->scale * 0.5f;
  std::string id_lower =
      create_cell(pos_lower, data_lower, cell->prime_encoding);

  // Update geometry to hemispheres
  auto *upper = get_cell(id_upper);
  auto *lower = get_cell(id_lower);
  if (upper)
    upper->geometry.shape = GMLShape::Sphere;
  if (lower)
    lower->geometry.shape = GMLShape::Sphere;

  result.success = true;
  if (upper)
    result.output_cells.push_back(*upper);
  if (lower)
    result.output_cells.push_back(*lower);
  result.topological_invariant = 1.0f; // Two disks
  result.transformation_log =
      "Cut sphere at angle " + std::to_string(cut_plane_angle);

  return result;
}

SphereSurgeryResult FractalTape::glue_cells(const std::string &cell_a,
                                            const std::string &cell_b) {
  SphereSurgeryResult result;
  auto a = get_cell(cell_a);
  auto b = get_cell(cell_b);

  if (!a || !b) {
    result.success = false;
    result.transformation_log = "One or both cells not found";
    return result;
  }

  // Create merged cell
  std::array<float, 11> merged_pos;
  for (int i = 0; i < 11; i++) {
    merged_pos[i] = (a->position[i] + b->position[i]) / 2.0f;
  }

  std::vector<float> merged_data;
  merged_data.reserve(a->data.size() + b->data.size());
  merged_data.insert(merged_data.end(), a->data.begin(), a->data.end());
  merged_data.insert(merged_data.end(), b->data.begin(), b->data.end());

  std::vector<int> merged_primes;
  merged_primes.insert(merged_primes.end(), a->prime_encoding.begin(),
                       a->prime_encoding.end());
  merged_primes.insert(merged_primes.end(), b->prime_encoding.begin(),
                       b->prime_encoding.end());

  std::string merged_id = create_cell(merged_pos, merged_data, merged_primes);

  result.success = true;
  result.output_cells.push_back(*get_cell(merged_id));
  result.topological_invariant = 2.0f; // Glued sphere
  result.transformation_log = "Glued cells " + cell_a + " and " + cell_b;

  return result;
}

// ================================================================
// Self-Assembly Operations
// ================================================================

void FractalTape::register_rule(const SelfAssemblyRule &rule) {
  rules[rule.id] = rule;
}

void FractalTape::unregister_rule(const std::string &rule_id) {
  rules.erase(rule_id);
}

void FractalTape::apply_rule(const SelfAssemblyRule &rule, FractalCell &cell) {
  // Apply rule to create child cell
  std::vector<float> new_data(8, 0.0f);
  for (size_t i = 0; i < std::min(cell.data.size(), size_t(8)); i++) {
    new_data[i] = cell.data[i] * rule.probability;
  }

  std::string child_id = create_child_cell(cell.id, new_data);
  if (!child_id.empty()) {
    auto *child = get_cell(child_id);
    if (child) {
      child->geometry.shape = rule.output_shape;
      child->prime_encoding = rule.prime_trigger;
    }
  }
}

SelfAssemblyState FractalTape::step_assembly() {
  if (!config.enable_self_assembly)
    return assembly_state;

  assembly_state.applied_rules.clear();
  int rules_applied = 0;

  // Get copy of cell IDs to avoid modifying during iteration
  auto cell_ids = get_all_cell_ids();

  for (const auto &cell_id : cell_ids) {
    auto *cell = get_cell(cell_id);
    if (!cell)
      continue;

    for (auto &[rule_id, rule] : rules) {
      // Check if rule can apply to this cell
      bool primes_match = false;
      for (int p : rule.prime_trigger) {
        for (int cp : cell->prime_encoding) {
          if (p == cp) {
            primes_match = true;
            break;
          }
        }
        if (primes_match)
          break;
      }

      if (primes_match) {
        // Apply with probability
        float r = static_cast<float>(rand()) / RAND_MAX;
        if (r < rule.probability) {
          apply_rule(rule, *cell);
          assembly_state.applied_rules.push_back(rule_id);
          rules_applied++;
        }
      }
    }
  }

  assembly_state.generation++;
  assembly_state.active_rules = static_cast<int>(rules.size());
  assembly_state.total_growth +=
      static_cast<float>(rules_applied) /
      std::max(1.0f, static_cast<float>(cell_ids.size()));
  generation_counter++;

  return assembly_state;
}

SelfAssemblyState FractalTape::run_assembly(int generations) {
  for (int i = 0; i < generations; i++) {
    step_assembly();
  }
  return assembly_state;
}

SelfAssemblyState FractalTape::get_assembly_state() const {
  return assembly_state;
}

// ================================================================
// Time Crystal Integration
// ================================================================

void FractalTape::link_time_crystal(TimeCrystalKernel *kernel) {
  time_crystal = kernel;
}

void FractalTape::sync_with_time_crystals() {
  if (!time_crystal)
    return;

  for (auto &[id, cell] : cells) {
    // Update cell prime encoding from time crystal states
    // This synchronizes fractal tape with time crystal manifold
    float coherence = time_crystal->compute_ppm_coherence(cell->prime_encoding);
    cell->geometry.scale_factor = coherence;
  }
}

std::string FractalTape::cell_to_atom(const std::string &cell_id) {
  if (!time_crystal)
    return "";

  auto cell = get_cell(cell_id);
  if (!cell)
    return "";

  // Create atom from cell
  return time_crystal->create_atom("FractalCell", cell->id, 0.8f, 0.9f,
                                   cell->prime_encoding);
}

// ================================================================
// Fractal Metrics (Task 2.5)
// ================================================================

FractalDimensionResult
FractalTape::calculate_fractal_dimension(int max_scales) {
  FractalDimensionResult result;
  result.box_counting_dimension = 0.0f;
  result.hausdorff_dimension_approx = 0.0f;
  result.information_dimension = 0.0f;
  result.correlation_dimension = 0.0f;
  result.scale_levels_used = 0;
  result.regression_r_squared = 0.0f;

  if (cells.empty())
    return result;

  // Box-counting method
  std::vector<float> scale_values;
  std::vector<float> box_counts;

  for (int s = 1; s <= max_scales; s++) {
    float scale = config.base_scale / std::pow(2.0f, s);
    int count = 0;

    // Count boxes needed to cover all cells at this scale
    std::set<std::tuple<int, int, int>> occupied_boxes;
    for (const auto &[_, cell] : cells) {
      int bx = static_cast<int>(cell->position[0] / scale);
      int by = static_cast<int>(cell->position[1] / scale);
      int bz = static_cast<int>(cell->position[2] / scale);
      occupied_boxes.insert({bx, by, bz});
    }
    count = static_cast<int>(occupied_boxes.size());

    scale_values.push_back(std::log(1.0f / scale));
    box_counts.push_back(std::log(static_cast<float>(count)));
  }

  result.scale_levels_used = max_scales;

  // Linear regression for dimension
  if (scale_values.size() >= 2) {
    float sum_x = 0, sum_y = 0, sum_xy = 0, sum_xx = 0;
    int n = static_cast<int>(scale_values.size());
    for (int i = 0; i < n; i++) {
      sum_x += scale_values[i];
      sum_y += box_counts[i];
      sum_xy += scale_values[i] * box_counts[i];
      sum_xx += scale_values[i] * scale_values[i];
    }
    float slope = (n * sum_xy - sum_x * sum_y) / (n * sum_xx - sum_x * sum_x);
    result.box_counting_dimension = slope;

    // Approximate Hausdorff as slightly higher
    result.hausdorff_dimension_approx = slope * 1.05f;
  }

  return result;
}

SelfSimilarityMetrics FractalTape::calculate_self_similarity() {
  SelfSimilarityMetrics metrics;
  metrics.overall_similarity = 0.0f;
  metrics.lacunarity = 0.0f;
  metrics.succolarity = 0.0f;

  int max_depth = static_cast<int>(max_depth_used());
  metrics.level_similarities.resize(max_depth + 1, 0.0f);

  if (cells.size() < 2)
    return metrics;

  // Calculate similarity between levels
  for (int d = 0; d < max_depth; d++) {
    auto level_cells = get_cells_at_depth(d);
    auto next_level = get_cells_at_depth(d + 1);

    if (!level_cells.empty() && !next_level.empty()) {
      // Compare structure between levels
      float similarity = static_cast<float>(next_level.size()) /
                         (level_cells.size() * config.cells_per_level);
      metrics.level_similarities[d] = std::min(1.0f, similarity);
    }
  }

  // Overall similarity as average
  float sum = 0.0f;
  int count = 0;
  for (float s : metrics.level_similarities) {
    if (s > 0) {
      sum += s;
      count++;
    }
  }
  metrics.overall_similarity = (count > 0) ? sum / count : 0.0f;

  // Lacunarity from cell distribution
  metrics.lacunarity = 1.0f - metrics.overall_similarity;

  // Succolarity from connectivity
  float connected_ratio = 0.0f;
  for (const auto &[_, cell] : cells) {
    if (!cell->children.empty() || !cell->parent.expired()) {
      connected_ratio += 1.0f;
    }
  }
  metrics.succolarity =
      connected_ratio / std::max(1.0f, static_cast<float>(cells.size()));

  return metrics;
}

std::string FractalTape::export_to_json() const {
  std::ostringstream ss;
  ss << "{\n  \"cells\": [\n";
  bool first = true;
  for (const auto &[id, cell] : cells) {
    if (!first)
      ss << ",\n";
    ss << "    {\"id\": \"" << id << "\", \"depth\": " << cell->depth
       << ", \"scale\": " << cell->scale << "}";
    first = false;
  }
  ss << "\n  ],\n  \"total\": " << cells.size() << "\n}";
  return ss.str();
}

std::string FractalTape::export_to_csv() const {
  std::ostringstream ss;
  ss << "id,depth,scale,x,y,z\n";
  for (const auto &[id, cell] : cells) {
    ss << id << "," << cell->depth << "," << cell->scale << ","
       << cell->position[0] << "," << cell->position[1] << ","
       << cell->position[2] << "\n";
  }
  return ss.str();
}

// ================================================================
// Statistics
// ================================================================

size_t FractalTape::total_cells() const { return cells.size(); }

size_t FractalTape::max_depth_used() const {
  size_t max_d = 0;
  for (const auto &[_, cell] : cells) {
    max_d = std::max(max_d, static_cast<size_t>(cell->depth));
  }
  return max_d;
}

float FractalTape::average_cell_density() const {
  if (cells.empty())
    return 0.0f;

  // Calculate bounding volume and cell count
  float volume = std::pow(config.base_scale * 2.0f, 11.0f); // 11D volume
  return static_cast<float>(cells.size()) / volume;
}

// ================================================================
// NerveBundleSimulator Implementation
// ================================================================

NerveBundleSimulator::NerveBundleSimulator(const NerveBundleConfig &cfg)
    : config(cfg), active(false), firing_rate(0.0f) {
  baseline.fill(0.0f);
  current_state.fill(0.0f);
}

NerveBundleSimulator::~NerveBundleSimulator() = default;

void NerveBundleSimulator::initialize() {
  baseline.fill(0.0f);
  current_state.fill(0.0f);
  firing_rate = 0.0f;
  active = true;
}

SensorReading NerveBundleSimulator::read() {
  SensorReading reading;
  reading.timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
                          std::chrono::system_clock::now().time_since_epoch())
                          .count();
  reading.values = current_state;
  reading.confidence = 0.9f;
  reading.primary_channel = SensorChannelType::Amplitude;

  // Detect primes from signal patterns
  for (int i = 0; i < 11; i++) {
    float v = std::abs(current_state[i]);
    if (v > config.threshold) {
      int prime_candidate = static_cast<int>(v * 47) + 2;
      // Simple primality check
      bool is_prime = true;
      for (int d = 2; d * d <= prime_candidate; d++) {
        if (prime_candidate % d == 0) {
          is_prime = false;
          break;
        }
      }
      if (is_prime) {
        reading.detected_primes.push_back(prime_candidate);
      }
    }
  }

  return reading;
}

std::vector<SensorReading> NerveBundleSimulator::read_batch(int count) {
  std::vector<SensorReading> readings;
  readings.reserve(count);
  for (int i = 0; i < count; i++) {
    readings.push_back(read());
  }
  return readings;
}

void NerveBundleSimulator::calibrate() { baseline = current_state; }

void NerveBundleSimulator::stimulate(const std::array<float, 11> &input) {
  // Integrate input with decay
  for (int i = 0; i < 11; i++) {
    current_state[i] = current_state[i] * 0.9f + input[i] * 0.1f;
  }

  // Check for spike threshold
  float total = 0.0f;
  for (float v : current_state) {
    total += std::abs(v);
  }

  if (total > config.threshold * 11) {
    firing_rate = config.fiber_count * total / 11.0f;
  } else {
    firing_rate *= 0.95f; // Decay
  }
}

float NerveBundleSimulator::get_firing_rate() const { return firing_rate; }

void NerveBundleSimulator::set_threshold(float threshold) {
  config.threshold = threshold;
}

// ================================================================
// OperationalChart Implementation
// ================================================================

OperationalChart::OperationalChart() : state(OperationalChartState::Idle) {}

void OperationalChart::add_transition(
    const OperationalChartTransition &transition) {
  transitions.push_back(transition);
}

bool OperationalChart::trigger(const std::string &trigger_name) {
  for (const auto &t : transitions) {
    if (t.from == state && t.trigger == trigger_name) {
      state = t.to;
      return true;
    }
  }
  return false;
}

std::string OperationalChart::state_name() const {
  switch (state) {
  case OperationalChartState::Idle:
    return "Idle";
  case OperationalChartState::Sensing:
    return "Sensing";
  case OperationalChartState::Processing:
    return "Processing";
  case OperationalChartState::Transmitting:
    return "Transmitting";
  case OperationalChartState::Adapting:
    return "Adapting";
  case OperationalChartState::Error:
    return "Error";
  default:
    return "Unknown";
  }
}

std::vector<std::string> OperationalChart::available_triggers() const {
  std::vector<std::string> triggers;
  for (const auto &t : transitions) {
    if (t.from == state) {
      triggers.push_back(t.trigger);
    }
  }
  return triggers;
}
