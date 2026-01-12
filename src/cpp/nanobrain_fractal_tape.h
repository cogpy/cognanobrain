#ifndef NANOBRAIN_FRACTAL_TAPE_H
#define NANOBRAIN_FRACTAL_TAPE_H

/**
 * FractalTape - Chapter 2: Fractal Information Theory
 *
 * Replaces traditional Turing tape with fractal-based data structures
 * supporting multi-dimensional navigation, sphere surgery transformations,
 * and self-assembly operators.
 */

#include "nanobrain_kernel.h"
#include "nanobrain_time_crystal.h"
#include <array>
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

// ================================================================
// Constants
// ================================================================

constexpr int FRACTAL_TAPE_DIMENSIONS = 11;
constexpr int FRACTAL_CELL_DEPTH = 7; // Nesting depth

// ================================================================
// FractalTape Configuration
// ================================================================

struct FractalTapeConfig {
  int dimensions = FRACTAL_TAPE_DIMENSIONS;
  int max_depth = FRACTAL_CELL_DEPTH;
  int cells_per_level = 8;
  float base_scale = 1.0f;
  float scale_ratio = 0.618f; // Golden ratio inverse
  bool enable_self_assembly = true;
  bool enable_sphere_surgery = true;
};

// ================================================================
// Fractal Cell Structure
// ================================================================

/**
 * FractalCell - A nested, self-similar data container
 */
struct FractalCell {
  std::string id;
  int depth;                       // Nesting level (0 = root)
  std::array<float, 11> position;  // 11D coordinates
  std::vector<float> data;         // Cell data content
  std::vector<int> prime_encoding; // Prime signature
  float scale;                     // Relative scale
  GeometricPattern geometry;       // GML shape
  std::vector<std::shared_ptr<FractalCell>> children; // Nested cells
  std::weak_ptr<FractalCell> parent;                  // Parent reference
};

/**
 * Sphere surgery transformation types
 */
enum class SphereSurgeryType {
  Inflation, // Expand 2D to 3D sphere
  Deflation, // Compress 3D to 2D
  Cutting,   // Split sphere
  Gluing,    // Merge spheres
  Twisting   // Apply twist transformation
};

/**
 * Sphere surgery operation result
 */
struct SphereSurgeryResult {
  bool success;
  std::vector<FractalCell> output_cells;
  float topological_invariant;
  std::string transformation_log;
};

// ================================================================
// Self-Assembly Operators
// ================================================================

/**
 * Self-assembly rule for fractal growth
 */
struct SelfAssemblyRule {
  std::string id;
  std::string condition;          // Condition for activation
  GMLShape output_shape;          // Shape to generate
  std::vector<int> prime_trigger; // Primes that trigger this rule
  float probability;              // Activation probability
  int max_applications;           // Maximum times rule can apply
};

/**
 * Self-assembly state
 */
struct SelfAssemblyState {
  int generation;
  int active_rules;
  float total_growth;
  std::vector<std::string> applied_rules;
};

// ================================================================
// Multidimensional Sensor Interface (Task 2.3)
// ================================================================

/**
 * Sensor channel type
 */
enum class SensorChannelType {
  Spatial,   // 3D position
  Temporal,  // Time dimension
  Frequency, // Frequency domain
  Phase,     // Phase information
  Amplitude, // Signal strength
  Coherence, // Coherence measure
  Prime      // Prime encoding channel
};

/**
 * Multidimensional sensor reading
 */
struct SensorReading {
  int64_t timestamp;
  std::array<float, 11> values; // 11D sensor values
  std::vector<int> detected_primes;
  float confidence;
  SensorChannelType primary_channel;
};

/**
 * MultidimensionalSensor interface
 */
class MultidimensionalSensor {
public:
  virtual ~MultidimensionalSensor() = default;

  virtual void initialize() = 0;
  virtual bool is_active() const = 0;
  virtual SensorReading read() = 0;
  virtual std::vector<SensorReading> read_batch(int count) = 0;
  virtual void calibrate() = 0;
  virtual std::array<float, 11> get_baseline() const = 0;
};

/**
 * Nerve bundle simulator for biological-inspired sensing
 */
struct NerveBundleConfig {
  int fiber_count = 1000;
  float conduction_velocity = 50.0f; // m/s
  float refractory_period = 1.0f;    // ms
  float threshold = 0.5f;
};

/**
 * Operational chart state for sensor state machine
 */
enum class OperationalChartState {
  Idle,
  Sensing,
  Processing,
  Transmitting,
  Adapting,
  Error
};

struct OperationalChartTransition {
  OperationalChartState from;
  OperationalChartState to;
  std::string trigger;
  float probability;
};

// ================================================================
// Fractal Information Metrics (Task 2.5)
// ================================================================

/**
 * Fractal dimension calculation result
 */
struct FractalDimensionResult {
  float box_counting_dimension;
  float hausdorff_dimension_approx;
  float information_dimension;
  float correlation_dimension;
  int scale_levels_used;
  float regression_r_squared;
};

/**
 * Self-similarity metrics
 */
struct SelfSimilarityMetrics {
  float overall_similarity;
  std::vector<float> level_similarities; // Similarity at each depth
  float lacunarity;                      // Texture/gap measure
  float succolarity;                     // Connectivity measure
};

// ================================================================
// FractalTape Class
// ================================================================

/**
 * FractalTape - Multi-dimensional fractal data structure
 *
 * Implements:
 * - Nested sphere surgery for 2D→3D mapping
 * - Self-assembly geometric operators
 * - Integration with TimeCrystalKernel
 */
class FractalTape {
public:
  FractalTape(const FractalTapeConfig &config = {});
  ~FractalTape();

  // ================================================================
  // Initialization
  // ================================================================

  void initialize();
  void reset();
  bool is_initialized() const { return initialized; }

  // ================================================================
  // Cell Management
  // ================================================================

  // Create a new cell at specified position
  std::string create_cell(const std::array<float, 11> &position,
                          const std::vector<float> &data,
                          const std::vector<int> &primes = {2, 3, 5});

  // Create nested child cell
  std::string create_child_cell(const std::string &parent_id,
                                const std::vector<float> &data);

  // Get cell by ID
  FractalCell *get_cell(const std::string &id);
  const FractalCell *get_cell(const std::string &id) const;

  // Remove cell
  bool remove_cell(const std::string &id);

  // Get all cell IDs
  std::vector<std::string> get_all_cell_ids() const;

  // Get cells at depth level
  std::vector<FractalCell *> get_cells_at_depth(int depth);

  // ================================================================
  // Navigation
  // ================================================================

  // Move to adjacent cell in direction
  std::string navigate(const std::string &from_cell, int dimension,
                       int direction);

  // Find nearest cell to position
  std::string find_nearest_cell(const std::array<float, 11> &position) const;

  // Get path between two cells
  std::vector<std::string> find_path(const std::string &from,
                                     const std::string &to) const;

  // ================================================================
  // Sphere Surgery Operations
  // ================================================================

  // Perform sphere surgery transformation
  SphereSurgeryResult perform_surgery(const std::string &cell_id,
                                      SphereSurgeryType type,
                                      float parameter = 1.0f);

  // Inflate 2D cell to 3D sphere
  SphereSurgeryResult inflate_to_sphere(const std::string &cell_id);

  // Deflate 3D cell to 2D
  SphereSurgeryResult deflate_to_plane(const std::string &cell_id);

  // Cut sphere into two cells
  SphereSurgeryResult cut_sphere(const std::string &cell_id,
                                 float cut_plane_angle);

  // Glue two cells together
  SphereSurgeryResult glue_cells(const std::string &cell_a,
                                 const std::string &cell_b);

  // ================================================================
  // Self-Assembly Operations
  // ================================================================

  // Register a self-assembly rule
  void register_rule(const SelfAssemblyRule &rule);

  // Remove a rule
  void unregister_rule(const std::string &rule_id);

  // Execute one generation of self-assembly
  SelfAssemblyState step_assembly();

  // Run assembly for N generations
  SelfAssemblyState run_assembly(int generations);

  // Get current assembly state
  SelfAssemblyState get_assembly_state() const;

  // ================================================================
  // Time Crystal Integration
  // ================================================================

  // Link to time crystal kernel
  void link_time_crystal(TimeCrystalKernel *kernel);

  // Synchronize cell states with time crystals
  void sync_with_time_crystals();

  // Convert cell to time crystal atom
  std::string cell_to_atom(const std::string &cell_id);

  // ================================================================
  // Fractal Metrics (Task 2.5)
  // ================================================================

  // Calculate fractal dimension
  FractalDimensionResult calculate_fractal_dimension(int max_scales = 10);

  // Calculate self-similarity metrics
  SelfSimilarityMetrics calculate_self_similarity();

  // Export for visualization
  std::string export_to_json() const;
  std::string export_to_csv() const;

  // ================================================================
  // Statistics
  // ================================================================

  size_t total_cells() const;
  size_t max_depth_used() const;
  float average_cell_density() const;

  // Get configuration
  const FractalTapeConfig &get_config() const { return config; }

private:
  FractalTapeConfig config;
  bool initialized = false;

  // Cell storage
  std::map<std::string, std::shared_ptr<FractalCell>> cells;
  std::vector<std::shared_ptr<FractalCell>> root_cells;

  // Self-assembly
  std::map<std::string, SelfAssemblyRule> rules;
  SelfAssemblyState assembly_state;

  // Time crystal integration
  TimeCrystalKernel *time_crystal = nullptr;

  // Counters
  int cell_counter = 0;
  int generation_counter = 0;

  // Private helpers
  std::string generate_cell_id();
  float calculate_cell_scale(int depth) const;
  void apply_rule(const SelfAssemblyRule &rule, FractalCell &cell);
  float cell_distance(const FractalCell &a, const FractalCell &b) const;
};

// ================================================================
// Nerve Bundle Simulator (Task 2.3)
// ================================================================

/**
 * Simulated nerve bundle for biological sensor modeling
 */
class NerveBundleSimulator : public MultidimensionalSensor {
public:
  NerveBundleSimulator(const NerveBundleConfig &config = {});
  ~NerveBundleSimulator() override;

  void initialize() override;
  bool is_active() const override { return active; }
  SensorReading read() override;
  std::vector<SensorReading> read_batch(int count) override;
  void calibrate() override;
  std::array<float, 11> get_baseline() const override { return baseline; }

  // Nerve-specific methods
  void stimulate(const std::array<float, 11> &input);
  float get_firing_rate() const;
  void set_threshold(float threshold);

private:
  NerveBundleConfig config;
  bool active = false;
  std::array<float, 11> baseline;
  std::array<float, 11> current_state;
  int64_t last_spike_time = 0;
  float firing_rate = 0.0f;
};

// ================================================================
// Operational Chart State Machine (Task 2.3)
// ================================================================

/**
 * State machine for sensor operational modes
 */
class OperationalChart {
public:
  OperationalChart();

  void add_transition(const OperationalChartTransition &transition);
  bool trigger(const std::string &trigger_name);
  OperationalChartState current_state() const { return state; }
  std::string state_name() const;
  std::vector<std::string> available_triggers() const;

private:
  OperationalChartState state = OperationalChartState::Idle;
  std::vector<OperationalChartTransition> transitions;
};

#endif // NANOBRAIN_FRACTAL_TAPE_H
