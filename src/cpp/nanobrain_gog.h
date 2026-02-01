#ifndef NANOBRAIN_GOG_H
#define NANOBRAIN_GOG_H

/**
 * Garden of Gardens (GOG) - Chapter 5, Task 5.1
 *
 * Implements the nested time crystal hierarchy described as the
 * "meander flower architecture" for universal time crystal processing.
 */

#include "nanobrain_time_crystal.h"
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

// ================================================================
// GOG Constants
// ================================================================

constexpr int GOG_MAX_DEPTH = 12;
constexpr int GOG_DEFAULT_PETALS = 7;
constexpr float GOG_PHI = 1.618033988749895f; // Golden ratio

// ================================================================
// Meander Pattern Types
// ================================================================

enum class MeanderPattern {
  Spiral,    // Logarithmic spiral
  Flower,    // Radial flower pattern
  Labyrinth, // Greek key meander
  Fractal,   // Self-similar branching
  Nested     // Concentric rings
};

// ================================================================
// GOG Node Structure
// ================================================================

/**
 * GOGPetal - Individual time crystal in the garden
 */
struct GOGPetal {
  std::string id;
  float phase;                      // Phase angle (0-2π)
  float amplitude;                  // Oscillation amplitude
  std::vector<int> prime_signature; // Prime encoding
  GeometricPattern geometry;        // GML shape
  NanoBrainTensor *tensor;          // Tensor representation
};

/**
 * GOGNode - A garden containing petals and sub-gardens
 */
struct GOGNode {
  std::string id;
  int depth;                                         // Nesting level
  std::vector<GOGPetal> petals;                      // Time crystal petals
  std::vector<std::shared_ptr<GOGNode>> sub_gardens; // Nested gardens
  float meander_phase;                               // Current meander phase
  MeanderPattern pattern;                            // Meander pattern type
  float coherence;                                   // Garden coherence
  float resonance_frequency;                         // Collective resonance
};

/**
 * GOG Configuration
 */
struct GOGConfig {
  int max_depth = GOG_MAX_DEPTH;
  int petals_per_garden = GOG_DEFAULT_PETALS;
  MeanderPattern default_pattern = MeanderPattern::Flower;
  float coherence_threshold = 0.5f;
  float meander_speed = 0.1f;
  bool auto_prune = true;
};

/**
 * GOG Metrics
 */
struct GOGMetrics {
  size_t total_gardens;
  size_t total_petals;
  int max_depth_reached;
  float global_coherence;
  float meander_synchronization;
  float fractal_dimension;
};

// ================================================================
// Garden of Gardens Class
// ================================================================

class GardenOfGardens {
public:
  GardenOfGardens(NanoBrainKernel *kernel, const GOGConfig &config = {});
  ~GardenOfGardens();

  // ================================================================
  // Initialization
  // ================================================================

  void initialize();
  void reset();
  bool is_initialized() const { return initialized; }

  // ================================================================
  // Garden Management
  // ================================================================

  // Create root garden
  std::string create_garden(MeanderPattern pattern = MeanderPattern::Flower);

  // Create sub-garden
  std::string create_sub_garden(const std::string &parent_id);

  // Add petal to garden
  std::string add_petal(const std::string &garden_id,
                        const std::vector<int> &primes = {2, 3, 5});

  // Get garden by ID
  GOGNode *get_garden(const std::string &id);
  const GOGNode *get_garden(const std::string &id) const;

  // Remove garden
  bool remove_garden(const std::string &id);

  // ================================================================
  // Meander Evolution
  // ================================================================

  // Evolve all gardens one step
  void evolve_step();

  // Evolve for N steps
  void evolve(int steps);

  // Synchronize meander phases across gardens
  void synchronize_meanders();

  // ================================================================
  // Coherence & Resonance
  // ================================================================

  // Calculate coherence for a garden
  float calculate_garden_coherence(const std::string &garden_id) const;

  // Calculate global coherence
  float calculate_global_coherence() const;

  // Find resonant petals across gardens
  std::vector<std::pair<std::string, std::string>>
  find_resonant_pairs(float threshold = 0.8f) const;

  // ================================================================
  // Traversal
  // ================================================================

  // Traverse all gardens with callback
  void traverse(std::function<void(GOGNode &)> callback);
  void traverse_const(std::function<void(const GOGNode &)> callback) const;

  // Get all garden IDs at depth
  std::vector<std::string> get_gardens_at_depth(int depth) const;

  // ================================================================
  // Metrics
  // ================================================================

  GOGMetrics get_metrics() const;
  const GOGConfig &get_config() const { return config; }

private:
  NanoBrainKernel *kernel;
  GOGConfig config;
  bool initialized = false;

  // Root gardens
  std::vector<std::shared_ptr<GOGNode>> root_gardens;

  // All gardens indexed by ID
  std::map<std::string, std::shared_ptr<GOGNode>> all_gardens;

  // Counters
  int garden_counter = 0;
  int petal_counter = 0;
  int evolution_step = 0;

  // Private helpers
  std::string generate_garden_id();
  std::string generate_petal_id();
  void evolve_garden(GOGNode &garden);
  float calculate_petal_phase(int index, int total, MeanderPattern pattern);
};

#endif // NANOBRAIN_GOG_H
