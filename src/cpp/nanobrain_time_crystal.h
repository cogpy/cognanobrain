#ifndef NANOBRAIN_TIME_CRYSTAL_H
#define NANOBRAIN_TIME_CRYSTAL_H

#include "nanobrain_kernel.h"
#include <array>
#include <cmath>
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

// Constants from NanoBrain Time Crystal Theory
constexpr int TIME_CRYSTAL_DIMENSIONS = 11;
constexpr int FUNDAMENTAL_PRIMES_COUNT = 15;
constexpr double GOLDEN_RATIO = 1.618033988749895;
constexpr double PI = 3.141592653589793;

// Fundamental primes used in Phase Prime Metric calculations
static const std::array<int, FUNDAMENTAL_PRIMES_COUNT> FUNDAMENTAL_PRIMES = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47};

/**
 * Geometric Musical Language (GML) shape types
 */
enum class GMLShape {
  Sphere,
  Torus,
  Helix,
  Fractal,
  Hypercube,
  Simplex,
  Point,
  Line,
  Triangle,
  Square,
  Pentagon,
  Hexagon,
  Circle,
  Tetrahedron,
  Cube,
  Octahedron,
  Dodecahedron,
  Icosahedron,
  Mobius
};

/**
 * Musical notes for GML encoding
 */
enum class MusicalNote {
  C,
  CSharp,
  D,
  DSharp,
  E,
  F,
  FSharp,
  G,
  GSharp,
  A,
  ASharp,
  B
};

/**
 * Fractal Information Theory geometric pattern
 */
struct GeometricPattern {
  GMLShape shape;
  int dimensions;
  std::string symmetry_group;
  MusicalNote musical_note;
  std::vector<int> prime_resonance;
  float scale_factor;
};

/**
 * Time Crystal quantum state for 11D processing
 */
struct TimeCrystalQuantumState {
  std::array<float, TIME_CRYSTAL_DIMENSIONS> dimensions;
  std::vector<int> prime_signature;
  float temporal_coherence;
  float fractal_dimension;
  float resonance_frequency;
  float quantum_phase;
};

/**
 * Truth value structure (OpenCog standard)
 */
struct TruthValue {
  float strength;
  float confidence;
  float count;
};

/**
 * Attention value structure (OpenCog ECAN)
 */
struct AttentionValue {
  float sti;  // Short-term importance
  float lti;  // Long-term importance
  float vlti; // Very long-term importance
};

/**
 * Time Crystal enhanced atom (OpenCog AtomSpace compatible)
 */
struct TimeCrystalAtom {
  std::string id;
  std::string type; // ConceptNode, PredicateNode, NumberNode, SchemaNode,
                    // VariableNode
  std::string name;
  TruthValue truth_value;
  AttentionValue attention_value;
  TimeCrystalQuantumState time_crystal_state;
  std::vector<int> prime_encoding;
  GeometricPattern fractal_geometry;
};

/**
 * PLN inference rule types
 */
enum class InferenceRuleType {
  Inheritance,
  Similarity,
  Implication,
  Deduction,
  Induction,
  Abduction
};

/**
 * Time Crystal enhanced inference
 */
struct TimeCrystalInference {
  InferenceRuleType rule;
  std::vector<std::string> premise_ids;
  std::string conclusion_id;
  float temporal_flow;     // -1 to 1 (time direction)
  float prime_consistency; // PPM consistency score
  float fractal_convergence;
  float quantum_coherence;
};

/**
 * NanoBrain performance metrics
 */
struct NanoBrainMetrics {
  size_t total_atoms;
  size_t total_links;
  float average_attention;
  float quantum_coherence;
  float temporal_stability;
  float prime_alignment;
  float fractal_complexity;
  float inference_rate;
  float consciousness_emergence;
};

/**
 * OpenCog NanoBrain Time Crystal configuration
 */
struct TimeCrystalConfig {
  size_t memory_size = 1024 * 1024 * 128; // 128 MB
  int time_crystal_dimensions = TIME_CRYSTAL_DIMENSIONS;
  int fractal_resolution = 5;
  int geometric_shape_count = 15;
  float quantum_coherence_threshold = 0.5f;
  float temporal_processing_frequency = 100.0f; // Hz
  float attention_decay_rate = 0.02f;
  int pln_inference_depth = 7;
  float resource_budget = 1000.0f;
  float diffusion_strength = 0.1f;
  float rent_collection_rate = 0.01f;
  float wage_distribution_rate = 0.8f;
};

/**
 * OpenCog NanoBrain Time Crystal Kernel
 *
 * Implements the unified OpenCog ecosystem integrated with nanobrain
 * time crystal mechanics, combining:
 * - OpenCog AtomSpace hypergraph knowledge representation
 * - NanoBrain Phase Prime Metrics (PPM) for universal pattern recognition
 * - Time Crystal temporal quantum structures for 11D processing
 * - Fractal Information Theory (FIT) for geometric musical language encoding
 */
class TimeCrystalKernel {
public:
  TimeCrystalKernel(const TimeCrystalConfig &config);
  ~TimeCrystalKernel();

  // ================================================================
  // Core Lifecycle
  // ================================================================

  void initialize();
  void shutdown();
  bool is_active() const { return active; }

  // ================================================================
  // Atom Management
  // ================================================================

  // Create a new Time Crystal Atom
  std::string create_atom(const std::string &type, const std::string &name,
                          const TruthValue &tv, const AttentionValue &av,
                          const std::vector<int> &prime_encoding,
                          const GeometricPattern &geometry);

  // Get atom by ID
  const TimeCrystalAtom *get_atom(const std::string &id) const;

  // Get mutable atom by ID
  TimeCrystalAtom *get_mutable_atom(const std::string &id);

  // Remove atom
  bool remove_atom(const std::string &id);

  // Get all atom IDs
  std::vector<std::string> get_all_atom_ids() const;

  // ================================================================
  // Phase Prime Metric (PPM) Functions
  // ================================================================

  // Calculate PPM coherence: 0.5 + 0.5 * sin(sqrt(prod) * PI / sum)
  float compute_ppm_coherence(const std::vector<int> &primes);

  // Calculate prime importance (smaller primes = more fundamental)
  float calculate_prime_importance(const std::vector<int> &primes);

  // Calculate resonance frequency from prime encoding
  float calculate_resonance_frequency(const std::vector<int> &primes);

  // Check if a number is in fundamental primes
  bool is_fundamental_prime(int prime) const;

  // Get fundamental index (0-14) or -1 if not fundamental
  int get_fundamental_index(int prime) const;

  // ================================================================
  // Time Crystal Quantum State Functions
  // ================================================================

  // Generate 11D quantum manifold coordinates
  std::array<float, TIME_CRYSTAL_DIMENSIONS> generate_quantum_coordinates();

  // Update quantum states for all time crystals (one processing cycle)
  void update_time_crystal_states();

  // Get time crystal state for an atom
  const TimeCrystalQuantumState *
  get_time_crystal_state(const std::string &atom_id) const;

  // Calculate temporal flow direction between two states
  float calculate_temporal_flow(const TimeCrystalQuantumState &state1,
                                const TimeCrystalQuantumState &state2);

  // ================================================================
  // Geometric Musical Language (GML) Functions
  // ================================================================

  // Calculate geometric resonance between two patterns
  float calculate_geometric_resonance(const GeometricPattern &pattern1,
                                      const GeometricPattern &pattern2);

  // Calculate musical harmony between two notes
  float calculate_musical_harmony(MusicalNote note1, MusicalNote note2);

  // Generate prime geometry for a given prime number
  GeometricPattern generate_prime_geometry(int prime, int index);

  // Combine two geometric patterns
  GeometricPattern combine_patterns(const GeometricPattern &p1,
                                    const GeometricPattern &p2);

  // Update GML resonances across all atoms
  void update_gml_resonances();

  // ================================================================
  // ECAN Attention Allocation (PPM-enhanced)
  // ================================================================

  // Perform attention allocation cycle
  void perform_attention_allocation();

  // Apply attention decay
  void apply_attention_decay();

  // Spread attention through links
  void spread_attention(const std::vector<std::string> &link_ids);

  // Get top-k atoms by attention
  std::vector<std::string> get_top_attention_atoms(size_t k) const;

  // ================================================================
  // PLN Reasoning (Time Crystal Enhanced)
  // ================================================================

  // Create inference from two atoms
  std::string create_inference(const std::string &atom1_id,
                               const std::string &atom2_id,
                               InferenceRuleType rule);

  // Perform one cycle of enhanced PLN reasoning
  void perform_pln_reasoning();

  // Get all inference IDs
  std::vector<std::string> get_all_inference_ids() const;

  // Get inference by ID
  const TimeCrystalInference *get_inference(const std::string &id) const;

  // Calculate prime consistency between two encodings
  float calculate_prime_consistency(const std::vector<int> &primes1,
                                    const std::vector<int> &primes2);

  // ================================================================
  // Tensor Operations (via NanoBrainKernel)
  // ================================================================

  // Get underlying tensor kernel
  NanoBrainKernel *get_tensor_kernel() { return kernel.get(); }

  // Encode atom to tensor representation
  NanoBrainTensor *encode_atom_to_tensor(const std::string &atom_id);

  // Compute tensor coherence
  NanoBrainTensor *compute_tensor_coherence(NanoBrainTensor *primes_tensor);

  // ================================================================
  // Metrics and Statistics
  // ================================================================

  // Get comprehensive system metrics
  NanoBrainMetrics get_metrics() const;

  // Calculate overall prime alignment
  float calculate_overall_prime_alignment() const;

  // Calculate fractal complexity
  float calculate_fractal_complexity() const;

  // Calculate temporal stability
  float calculate_temporal_stability() const;

  // Calculate consciousness emergence metric
  float calculate_consciousness_emergence() const;

  // Get cycle count
  size_t get_cycle_count() const { return cycle_count; }

  // ================================================================
  // Processing Cycle
  // ================================================================

  // Execute one full processing cycle
  void process_cycle();

private:
  // Configuration
  TimeCrystalConfig config;

  // Tensor kernel
  std::unique_ptr<NanoBrainKernel> kernel;

  // AtomSpace
  std::map<std::string, TimeCrystalAtom> atom_space;

  // Time crystal states
  std::map<std::string, TimeCrystalQuantumState> time_crystals;

  // Inference links
  std::map<std::string, TimeCrystalInference> link_space;

  // State
  bool active = false;
  size_t cycle_count = 0;
  int64_t start_time = 0;
  int atom_counter = 0;

  // Private helper methods
  void initialize_fundamental_atoms();
  void initialize_gml_atoms();
  std::string generate_atom_id();
  int64_t current_time_millis() const;
};

// ================================================================
// Utility Functions
// ================================================================

// Convert GML shape to string
std::string gml_shape_to_string(GMLShape shape);

// Convert musical note to string
std::string musical_note_to_string(MusicalNote note);

// Convert musical note to semitone index (0-11)
int musical_note_to_index(MusicalNote note);

// Convert semitone index to musical note
MusicalNote index_to_musical_note(int index);

// Get shape name for index
GMLShape index_to_gml_shape(int index);

#endif // NANOBRAIN_TIME_CRYSTAL_H
