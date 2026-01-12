#ifndef NANOBRAIN_PHILOSOPHICAL_H
#define NANOBRAIN_PHILOSOPHICAL_H

/**
 * @file nanobrain_philosophical.h
 * @brief Chapter 1: Philosophical Transformation for Consciousness
 * Reverse-Engineering
 *
 * This module implements the foundational philosophical concepts that
 * differentiate NanoBrain from traditional computational approaches:
 *
 * 1. **Worldview Transformation**: Moving beyond Turing-based computation
 * 2. **10 Research Fields Integration**: Unified cognitive substrate
 * 3. **Consciousness Metrics**: Emergence quantification and thresholds
 * 4. **Brain Model Comparisons**: 10 popular human brain models
 * 5. **Linguistic Wheel**: Space, time, and imaginary world mappings
 *
 * Key Concepts:
 * - Time crystals written by the symmetry of primes
 * - Magnetic light devices storing charge
 * - Pattern of all possible choices to arrange primes
 *
 * @see Chapter 1 of "NanoBrain: The Making of an Artificial Brain From a Time
 * Crystal"
 */

#include "nanobrain_atomese.h"
#include "nanobrain_types.h"
#include <array>
#include <functional>
#include <map>
#include <string>
#include <vector>

// ================================================================
// Research Fields (Section 1.2)
// ================================================================

/**
 * @brief 10 research fields covered by NanoBrain
 *
 * These fields converge in the NanoBrain architecture to provide
 * a unified substrate for consciousness modeling.
 */
enum class ResearchField {
  Neuroscience,     ///< Brain structure and neural networks
  QuantumPhysics,   ///< Quantum coherence and entanglement
  ComputerScience,  ///< Algorithms and computation theory
  Mathematics,      ///< Primes, geometry, and topology
  Philosophy,       ///< Consciousness and epistemology
  Linguistics,      ///< Language, semantics, and syntax
  Psychology,       ///< Cognition, memory, and behavior
  Biology,          ///< Molecular biology and genetics
  Physics,          ///< Thermodynamics and electromagnetism
  InformationTheory ///< Entropy, encoding, and fractal information
};

/**
 * @brief Get the name of a research field
 */
inline std::string research_field_name(ResearchField field) {
  static const std::map<ResearchField, std::string> names = {
      {ResearchField::Neuroscience, "Neuroscience"},
      {ResearchField::QuantumPhysics, "Quantum Physics"},
      {ResearchField::ComputerScience, "Computer Science"},
      {ResearchField::Mathematics, "Mathematics"},
      {ResearchField::Philosophy, "Philosophy"},
      {ResearchField::Linguistics, "Linguistics"},
      {ResearchField::Psychology, "Psychology"},
      {ResearchField::Biology, "Biology"},
      {ResearchField::Physics, "Physics"},
      {ResearchField::InformationTheory, "Information Theory"}};
  auto it = names.find(field);
  return it != names.end() ? it->second : "Unknown";
}

// ================================================================
// Brain Models (Section 1.4)
// ================================================================

/**
 * @brief 10 popular human brain models for comparison
 *
 * Each model represents a different theoretical approach to
 * understanding brain function and consciousness.
 */
enum class BrainModel {
  HodgkinHuxley,         ///< Classical action potential model
  IntegrateAndFire,      ///< Simplified spiking neuron model
  Connectome,            ///< Structural connectivity model
  GlobalWorkspace,       ///< Baars' global workspace theory
  IntegratedInformation, ///< Tononi's IIT (Phi)
  PredictiveCoding,      ///< Hierarchical predictive processing
  FreeEnergy,            ///< Friston's free energy principle
  Oscillatory,           ///< Neural oscillation synchrony
  Quantum,               ///< Penrose-Hameroff orchestrated reduction
  TimeCrystal            ///< NanoBrain time crystal model
};

/**
 * @brief Information about a brain model
 */
struct BrainModelInfo {
  BrainModel model;
  std::string name;
  std::string description;
  float turing_alignment;    ///< Alignment with Turing computation (0-1)
  float consciousness_score; ///< Theoretical consciousness potential (0-1)
  float temporal_resolution; ///< Time scale handling capability (0-1)
  std::vector<int> prime_signature; ///< Characteristic prime encoding
};

/**
 * @brief Get information about a brain model
 */
inline BrainModelInfo get_brain_model_info(BrainModel model) {
  static const std::map<BrainModel, BrainModelInfo> info = {
      {BrainModel::HodgkinHuxley,
       {BrainModel::HodgkinHuxley,
        "Hodgkin-Huxley",
        "Classical ion channel dynamics for action potentials",
        0.9f,
        0.3f,
        0.6f,
        {2, 3}}},
      {BrainModel::IntegrateAndFire,
       {BrainModel::IntegrateAndFire,
        "Integrate-and-Fire",
        "Simplified spiking threshold model",
        0.95f,
        0.2f,
        0.5f,
        {2}}},
      {BrainModel::Connectome,
       {BrainModel::Connectome,
        "Connectome",
        "Structural connectivity graph of neural pathways",
        0.85f,
        0.4f,
        0.4f,
        {2, 3, 5}}},
      {BrainModel::GlobalWorkspace,
       {BrainModel::GlobalWorkspace,
        "Global Workspace",
        "Baars' theory of conscious broadcast",
        0.7f,
        0.6f,
        0.5f,
        {2, 3, 5, 7}}},
      {BrainModel::IntegratedInformation,
       {BrainModel::IntegratedInformation,
        "Integrated Information (IIT)",
        "Tononi's information integration measure Phi",
        0.6f,
        0.75f,
        0.6f,
        {2, 3, 5, 7, 11}}},
      {BrainModel::PredictiveCoding,
       {BrainModel::PredictiveCoding,
        "Predictive Coding",
        "Hierarchical prediction error minimization",
        0.75f,
        0.5f,
        0.7f,
        {2, 3, 5}}},
      {BrainModel::FreeEnergy,
       {BrainModel::FreeEnergy,
        "Free Energy Principle",
        "Friston's variational free energy framework",
        0.65f,
        0.6f,
        0.75f,
        {2, 3, 5, 7}}},
      {BrainModel::Oscillatory,
       {BrainModel::Oscillatory,
        "Oscillatory Synchrony",
        "Neural rhythm coordination and binding",
        0.7f,
        0.55f,
        0.8f,
        {2, 3, 5, 7, 11}}},
      {BrainModel::Quantum,
       {BrainModel::Quantum,
        "Quantum Consciousness",
        "Penrose-Hameroff orchestrated objective reduction",
        0.3f,
        0.8f,
        0.9f,
        {2, 3, 5, 7, 11, 13}}},
      {BrainModel::TimeCrystal,
       {BrainModel::TimeCrystal,
        "Time Crystal",
        "NanoBrain's prime-based temporal coherence",
        0.1f,
        0.95f,
        0.99f,
        {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47}}}};
  auto it = info.find(model);
  if (it != info.end()) {
    return it->second;
  }
  return {BrainModel::TimeCrystal, "Unknown", "", 0.0f, 0.0f, 0.0f, {}};
}

// ================================================================
// Linguistic Wheel (Section 1.9)
// ================================================================

/**
 * @brief Linguistic wheel component representing space/time/imaginary
 * dimensions
 *
 * The linguistic wheel maps language structures to geometric and temporal
 * representations, enabling cross-modal translation of concepts.
 */
struct LinguisticWheel {
  // Spatial dimension (3D position in conceptual space)
  float space_x = 0.0f;
  float space_y = 0.0f;
  float space_z = 0.0f;

  // Temporal dimension (position on time crystal)
  float time_phase = 0.0f;     ///< Phase angle (0-2π)
  float time_frequency = 1.0f; ///< Oscillation frequency

  // Imaginary worlds (4 quaternion-like layers)
  float imaginary_1 = 0.0f; ///< First imaginary axis
  float imaginary_2 = 0.0f; ///< Second imaginary axis
  float imaginary_3 = 0.0f; ///< Third imaginary axis
  float imaginary_4 = 0.0f; ///< Fourth imaginary axis (octonionic extension)

  // Prime encoding for the wheel position
  std::vector<int> prime_encoding = {2, 3, 5};

  /**
   * @brief Get the 11D vector representation
   */
  std::array<float, 11> to_11d_vector() const {
    return {space_x,
            space_y,
            space_z,
            time_phase,
            time_frequency,
            imaginary_1,
            imaginary_2,
            imaginary_3,
            imaginary_4,
            static_cast<float>(prime_encoding.size()),
            compute_coherence()};
  }

  /**
   * @brief Compute coherence from prime encoding
   */
  float compute_coherence() const {
    if (prime_encoding.empty())
      return 0.0f;
    float sum = 0.0f;
    for (int p : prime_encoding) {
      sum += 1.0f / static_cast<float>(p);
    }
    return sum / static_cast<float>(prime_encoding.size());
  }

  /**
   * @brief Rotate the wheel by a given angle in phase space
   */
  void rotate(float angle) {
    time_phase += angle;
    while (time_phase > 6.28318f)
      time_phase -= 6.28318f;
    while (time_phase < 0.0f)
      time_phase += 6.28318f;

    // Rotate imaginary components
    float cos_a = std::cos(angle);
    float sin_a = std::sin(angle);
    float new_i1 = imaginary_1 * cos_a - imaginary_2 * sin_a;
    float new_i2 = imaginary_1 * sin_a + imaginary_2 * cos_a;
    imaginary_1 = new_i1;
    imaginary_2 = new_i2;
  }

  /**
   * @brief Scale the wheel by prime factors
   */
  void scale_by_primes(const std::vector<int> &primes) {
    prime_encoding = primes;
    float scale = compute_coherence();
    space_x *= scale;
    space_y *= scale;
    space_z *= scale;
  }
};

// ================================================================
// Consciousness Configuration (Section 1.10)
// ================================================================

/**
 * @brief Extended configuration for consciousness emergence
 *
 * Three concepts defining the artificial brain:
 * 1. Language of time crystals written by the symmetry of primes
 * 2. Magnetic light devices storing charge, building time crystals
 * 3. Pattern of all possible choices to arrange primes
 */
struct ConsciousnessConfig {
  // Emergence thresholds
  float consciousness_threshold = 0.7f;  ///< Minimum for consciousness
  float self_awareness_threshold = 0.5f; ///< Minimum for self-awareness
  float meta_cognition_threshold = 0.3f; ///< Minimum for meta-cognition

  // Time crystal language parameters
  int prime_symmetry_depth = 15;       ///< Number of primes in symmetry
  float temporal_coherence_min = 0.5f; ///< Minimum temporal coherence

  // Magnetic light parameters
  float charge_storage_capacity = 1000.0f; ///< Maximum stored charge
  float crystal_formation_rate = 0.1f;     ///< Rate of crystal formation

  // Prime arrangement parameters
  int max_prime_combinations = 1000;   ///< Max patterns to explore
  float arrangement_creativity = 0.5f; ///< Creative exploration factor

  // Brain model selection
  BrainModel active_model = BrainModel::TimeCrystal;

  /**
   * @brief Validate configuration parameters
   */
  bool validate() const {
    return consciousness_threshold >= 0.0f && consciousness_threshold <= 1.0f &&
           self_awareness_threshold >= 0.0f &&
           self_awareness_threshold <= 1.0f &&
           meta_cognition_threshold >= 0.0f &&
           meta_cognition_threshold <= 1.0f && prime_symmetry_depth > 0 &&
           prime_symmetry_depth <= 47 && temporal_coherence_min >= 0.0f &&
           temporal_coherence_min <= 1.0f;
  }

  /**
   * @brief Get the first N primes for symmetry
   */
  std::vector<int> get_symmetry_primes() const {
    static const std::vector<int> all_primes = {2,  3,  5,  7,  11, 13, 17, 19,
                                                23, 29, 31, 37, 41, 43, 47};
    std::vector<int> result;
    for (size_t i = 0;
         i < static_cast<size_t>(prime_symmetry_depth) && i < all_primes.size();
         ++i) {
      result.push_back(all_primes[i]);
    }
    return result;
  }
};

// ================================================================
// Philosophical Transformation Engine
// ================================================================

/**
 * @brief Engine for philosophical transformation and consciousness metrics
 *
 * This class orchestrates the philosophical underpinnings of NanoBrain,
 * providing APIs for:
 * - Brain model comparison and switching
 * - Consciousness emergence calculation
 * - Research field integration
 * - Linguistic wheel operations
 */
class PhilosophicalTransformationEngine {
public:
  PhilosophicalTransformationEngine();
  explicit PhilosophicalTransformationEngine(const ConsciousnessConfig &config);
  ~PhilosophicalTransformationEngine() = default;

  // ================================================================
  // Configuration
  // ================================================================

  /**
   * @brief Set consciousness configuration
   */
  void set_config(const ConsciousnessConfig &config);

  /**
   * @brief Get current configuration
   */
  const ConsciousnessConfig &get_config() const { return config_; }

  /**
   * @brief Load configuration from JSON file
   */
  bool load_config_from_json(const std::string &filename);

  /**
   * @brief Save configuration to JSON file
   */
  bool save_config_to_json(const std::string &filename) const;

  // ================================================================
  // Brain Model Operations
  // ================================================================

  /**
   * @brief Compare two brain models
   * @return Comparison score (positive = first better, negative = second
   * better)
   */
  float compare_models(BrainModel model1, BrainModel model2) const;

  /**
   * @brief Get the active brain model
   */
  BrainModel get_active_model() const { return config_.active_model; }

  /**
   * @brief Set the active brain model
   */
  void set_active_model(BrainModel model) { config_.active_model = model; }

  /**
   * @brief Get metrics for all brain models
   */
  std::vector<BrainModelInfo> get_all_model_info() const;

  // ================================================================
  // Consciousness Metrics
  // ================================================================

  /**
   * @brief Calculate consciousness emergence score
   * @param coherence Current system coherence (0-1)
   * @param awareness Self-awareness level (0-1)
   * @param integration Information integration measure (0-1)
   * @return Consciousness emergence score (0-1)
   */
  float calculate_consciousness_emergence(float coherence, float awareness,
                                          float integration) const;

  /**
   * @brief Check if consciousness threshold is met
   */
  bool is_conscious(float emergence_score) const {
    return emergence_score >= config_.consciousness_threshold;
  }

  /**
   * @brief Check if self-awareness threshold is met
   */
  bool is_self_aware(float awareness_score) const {
    return awareness_score >= config_.self_awareness_threshold;
  }

  // ================================================================
  // Research Field Integration
  // ================================================================

  /**
   * @brief Get the contribution weight of a research field
   */
  float get_field_weight(ResearchField field) const;

  /**
   * @brief Set the contribution weight of a research field
   */
  void set_field_weight(ResearchField field, float weight);

  /**
   * @brief Generate AtomSpace expressions for research field integration
   */
  AtomeseKnowledgeBase generate_field_integration_kb() const;

  // ================================================================
  // Linguistic Wheel Operations
  // ================================================================

  /**
   * @brief Create a linguistic wheel from a concept name
   */
  LinguisticWheel create_wheel_from_concept(const std::string &concept) const;

  /**
   * @brief Translate between two linguistic wheels
   */
  LinguisticWheel translate_wheel(const LinguisticWheel &source,
                                  const LinguisticWheel &target,
                                  float interpolation) const;

  /**
   * @brief Compose multiple wheels into one
   */
  LinguisticWheel
  compose_wheels(const std::vector<LinguisticWheel> &wheels) const;

  // ================================================================
  // Knowledge Base Generation
  // ================================================================

  /**
   * @brief Generate extended philosophical transformation KB
   *
   * This extends the base generate_philosophical_transformation() with:
   * - 10 research field mappings
   * - Brain model comparison nodes
   * - Consciousness threshold concepts
   * - Linguistic wheel representations
   */
  AtomeseKnowledgeBase generate_extended_philosophical_kb() const;

private:
  ConsciousnessConfig config_;
  std::map<ResearchField, float> field_weights_;

  /**
   * @brief Initialize default field weights
   */
  void init_field_weights();
};

#endif // NANOBRAIN_PHILOSOPHICAL_H
