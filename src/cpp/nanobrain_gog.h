#ifndef NANOBRAIN_GOG_H
#define NANOBRAIN_GOG_H

/**
 * NanoBrain Garden of Gardens (GOG) Module
 *
 * Chapter 5: Universal Time Crystal & Big Data
 *
 * Implements the "Garden of Gardens" meander flower architecture,
 * a nested hierarchy of time crystals forming a fractal-like structure
 * for massively parallel pattern processing.
 *
 * Key concepts:
 * - Petals: Individual time crystal states in a flower-like arrangement
 * - Meander: The oscillating phase pattern connecting petals
 * - Sub-gardens: Nested GOG structures at deeper levels
 * - Resonance: Coherent synchronization between gardens
 */

#include "nanobrain_kernel.h"
#include "nanobrain_time_crystal.h"
#include <map>
#include <memory>
#include <string>
#include <vector>

// Maximum depth for GOG hierarchy
constexpr int GOG_MAX_DEPTH = 7;

// Default number of petals per garden node
constexpr int GOG_DEFAULT_PETALS = 12;

/**
 * GOG configuration
 */
struct GOGConfig {
  int max_depth = 5;                // Maximum nesting depth
  int petals_per_node = 12;         // Petals in each flower
  float meander_frequency = 0.1f;   // Meander oscillation rate
  float resonance_threshold = 0.7f; // Threshold for resonant coupling
  float coherence_decay = 0.01f;    // Coherence decay per cycle
  bool enable_auto_pruning = true;  // Automatically prune weak gardens
};

/**
 * Individual petal in a garden
 */
struct GOGPetal {
  int index;                             // Petal index (0-11 typically)
  TimeCrystalQuantumState quantum_state; // Time crystal state
  float phase_offset;                    // Phase offset from center
  float activation;                      // Current activation level
  std::vector<int> prime_signature;      // Prime encoding for this petal
};

/**
 * Garden of Gardens node
 *
 * Represents a single "flower" in the GOG hierarchy with:
 * - Multiple petals (time crystals arranged in a circle)
 * - Nested sub-gardens forming a fractal structure
 * - Meander phase for oscillatory dynamics
 */
struct GOGNode {
  std::string id;
  std::string parent_id;                   // Empty for root
  std::vector<GOGPetal> petals;            // Time crystal petals
  std::vector<std::string> sub_garden_ids; // IDs of child gardens
  float meander_phase;                     // Current meander phase
  float meander_frequency;                 // Oscillation rate
  int depth;                               // Nesting level (0 = root)
  float coherence;                         // Overall garden coherence
  float resonance_score;                   // Resonance with neighbors
  int64_t last_updated;                    // Last update timestamp
};

/**
 * GOG metrics for monitoring
 */
struct GOGMetrics {
  size_t total_gardens;
  size_t total_petals;
  int max_achieved_depth;
  float average_coherence;
  float total_resonance;
  size_t resonant_pairs;
  float processing_efficiency;
};

/**
 * Garden of Gardens Manager
 *
 * Manages the nested time crystal hierarchy for fractal pattern processing.
 */
class GardenOfGardens {
public:
  GardenOfGardens(NanoBrainKernel *kernel, const GOGConfig &config);
  ~GardenOfGardens();

  // ================================================================
  // Initialization
  // ================================================================

  /**
   * Initialize the GOG structure with root garden
   */
  void initialize();

  /**
   * Shutdown and cleanup
   */
  void shutdown();

  /**
   * Check if GOG is active
   */
  bool is_active() const { return active; }

  // ================================================================
  // Garden Management
  // ================================================================

  /**
   * Create a new garden node
   * @param parent_id Parent garden ID (empty for root)
   * @param prime_signature Prime encoding for this garden
   * @return Created garden ID
   */
  std::string create_garden(const std::string &parent_id,
                            const std::vector<int> &prime_signature);

  /**
   * Get garden by ID
   */
  const GOGNode *get_garden(const std::string &id) const;

  /**
   * Get mutable garden
   */
  GOGNode *get_mutable_garden(const std::string &id);

  /**
   * Get root garden ID
   */
  std::string get_root_id() const { return root_id; }

  /**
   * Get all garden IDs
   */
  std::vector<std::string> get_all_garden_ids() const;

  /**
   * Get gardens at specific depth
   */
  std::vector<std::string> get_gardens_at_depth(int depth) const;

  /**
   * Remove a garden and its descendants
   */
  bool remove_garden(const std::string &id);

  // ================================================================
  // Petal Operations
  // ================================================================

  /**
   * Initialize petals for a garden
   */
  void initialize_petals(GOGNode &node, const std::vector<int> &primes);

  /**
   * Update petal states based on meander phase
   */
  void update_petal_states(GOGNode &node);

  /**
   * Get petal activation pattern as tensor
   */
  NanoBrainTensor *get_petal_activations(const std::string &garden_id);

  // ================================================================
  // Meander Dynamics
  // ================================================================

  /**
   * Update meander phases for all gardens
   */
  void update_meander_phases();

  /**
   * Calculate meander pattern influence
   */
  float calculate_meander_influence(const GOGNode &node, int petal_index);

  /**
   * Synchronize meander between parent and child
   */
  void synchronize_meander(GOGNode &parent, GOGNode &child);

  // ================================================================
  // Coherence & Resonance
  // ================================================================

  /**
   * Compute coherence for a single garden
   */
  float compute_garden_coherence(const GOGNode &node);

  /**
   * Update coherence for all gardens
   */
  void update_all_coherences();

  /**
   * Calculate resonance between two gardens
   */
  float calculate_resonance(const GOGNode &node1, const GOGNode &node2);

  /**
   * Find all gardens resonant with given garden
   */
  std::vector<std::string> find_resonant_gardens(const std::string &garden_id,
                                                 float threshold);

  /**
   * Update resonance scores across hierarchy
   */
  void update_resonance_network();

  // ================================================================
  // Processing Cycle
  // ================================================================

  /**
   * Execute one GOG processing cycle
   */
  void process_cycle();

  /**
   * Propagate signals through hierarchy (bottom-up)
   */
  void propagate_bottom_up();

  /**
   * Propagate signals through hierarchy (top-down)
   */
  void propagate_top_down();

  /**
   * Apply pattern to GOG and find matching gardens
   */
  std::vector<std::string> match_pattern(NanoBrainTensor *pattern);

  // ================================================================
  // Pruning & Growth
  // ================================================================

  /**
   * Prune gardens with low coherence
   */
  void prune_weak_gardens(float coherence_threshold);

  /**
   * Grow new sub-gardens where activity is high
   */
  void grow_active_regions(float activation_threshold);

  // ================================================================
  // Metrics & Statistics
  // ================================================================

  /**
   * Get GOG metrics
   */
  GOGMetrics get_metrics() const;

  /**
   * Get cycle count
   */
  size_t get_cycle_count() const { return cycle_count; }

  /**
   * Get configuration
   */
  const GOGConfig &get_config() const { return config; }

private:
  NanoBrainKernel *kernel;
  GOGConfig config;

  std::map<std::string, GOGNode> gardens;
  std::string root_id;

  bool active = false;
  size_t cycle_count = 0;
  int garden_counter = 0;

  // Private helpers
  std::string generate_garden_id();
  void create_root_garden();
  int64_t current_time_millis() const;
};

#endif // NANOBRAIN_GOG_H
