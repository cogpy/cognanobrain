#ifndef NANOBRAIN_BRAIN_MODEL_H
#define NANOBRAIN_BRAIN_MODEL_H

#include "nanobrain_kernel.h"
#include "nanobrain_metacognitive.h"
#include "nanobrain_reasoning.h"
#include "nanobrain_time_crystal.h"
#include "nanobrain_types.h"
#include <array>
#include <map>
#include <memory>
#include <string>
#include <vector>

// ================================================================
// Chapter 7: Integrated Human Brain Model
// Complete time crystal brain architecture
// ================================================================

// ================================================================
// 7.1: Five Sensory Prime Mapping
// ================================================================

/**
 * Sensory modality enumeration
 */
enum class SensoryModality {
  Visual = 0,    // Primary: primes 2, 7, 13
  Auditory = 1,  // Primary: primes 3, 11, 17
  Tactile = 2,   // Primary: primes 5, 19, 29
  Olfactory = 3, // Primary: primes 23, 31, 37
  Gustatory = 4  // Primary: primes 41, 43, 47
};

/**
 * Sensory prime mapping structure
 */
struct SensoryPrimeMapping {
  SensoryModality modality;
  std::vector<int> prime_sequence;
  float cross_modal_weight;
  NanoBrainTensor *encoding_tensor;
  NanoBrainTensor *integration_tensor;
  float resonance_frequency;
  int gml_shape_index;
};

/**
 * Cross-modal integration result
 */
struct CrossModalResult {
  std::vector<float> integrated_features;
  float coherence_score;
  std::map<SensoryModality, float> modality_contributions;
};

/**
 * Sensory Prime Mapper
 * Maps each sense to prime sequences for time crystal encoding
 */
class SensoryPrimeMapper {
public:
  SensoryPrimeMapper(NanoBrainKernel *kernel);
  ~SensoryPrimeMapper();

  void initialize();

  // Map sensory input to prime-encoded tensor
  NanoBrainTensor *map_input(SensoryModality modality, NanoBrainTensor *input);

  // Get prime sequence for a modality
  std::vector<int> get_prime_sequence(SensoryModality modality) const;

  // Cross-modal integration
  CrossModalResult cross_modal_integrate(
      const std::map<SensoryModality, NanoBrainTensor *> &inputs);

  // Calculate resonance between two modalities
  float calculate_cross_resonance(SensoryModality a, SensoryModality b) const;

  // Get mapping for a modality
  const SensoryPrimeMapping *get_mapping(SensoryModality modality) const;

private:
  NanoBrainKernel *kernel;
  std::array<SensoryPrimeMapping, 5> mappings;

  void setup_prime_assignments();
  NanoBrainTensor *apply_prime_encoding(NanoBrainTensor *input,
                                        const std::vector<int> &primes);
};

// ================================================================
// 7.2: Twelve Memory Carrier Types
// ================================================================

/**
 * Memory carrier enumeration
 */
enum class MemoryCarrier {
  // Sensory memories
  Visual = 0,
  Auditory = 1,
  Tactile = 2,
  Olfactory = 3,
  Gustatory = 4,
  // Cognitive memories
  Episodic = 5,
  Semantic = 6,
  Procedural = 7,
  Working = 8,
  // Implicit memories
  Implicit = 9,
  Explicit = 10,
  Priming = 11
};

/**
 * Memory carrier state
 */
struct MemoryCarrierState {
  MemoryCarrier type;
  std::string id;
  NanoBrainTensor *content_tensor;
  float activation_level;
  float decay_rate;
  float consolidation_strength;
  int64_t creation_time;
  int64_t last_access;
  int access_count;
  std::vector<std::string> associated_atoms;
};

/**
 * Memory consolidation result
 */
struct ConsolidationResult {
  std::vector<std::string> consolidated_ids;
  float consolidation_strength;
  int memories_merged;
};

/**
 * Memory Carrier System
 * Manages 12 types of memory carriers with decay and consolidation
 */
class MemoryCarrierSystem {
public:
  MemoryCarrierSystem(NanoBrainKernel *kernel,
                      AttentionAllocationEngine *attention);
  ~MemoryCarrierSystem();

  void initialize();

  // Encode a memory
  std::string encode_memory(MemoryCarrier type, NanoBrainTensor *content,
                            const std::vector<std::string> &associated_atoms);

  // Retrieve a memory
  NanoBrainTensor *retrieve_memory(const std::string &memory_id);

  // Get memories by carrier type
  std::vector<std::string> get_memories_by_type(MemoryCarrier type) const;

  // Apply decay to all memories
  void apply_decay(float time_delta);

  // Consolidate memories (working -> long-term)
  ConsolidationResult consolidate_memories();

  // Get memory state
  const MemoryCarrierState *get_memory_state(const std::string &id) const;

  // Update activation based on attention
  void update_activations();

  // Get carrier statistics
  struct CarrierStats {
    size_t total_memories;
    std::array<size_t, 12> memories_per_type;
    float average_activation;
    float average_consolidation;
  };
  CarrierStats get_stats() const;

private:
  NanoBrainKernel *kernel;
  AttentionAllocationEngine *attention;
  std::map<std::string, MemoryCarrierState> memories;
  int memory_counter = 0;

  float get_decay_rate(MemoryCarrier type) const;
  std::string generate_memory_id(MemoryCarrier type);
  int64_t current_time_millis() const;
};

// ================================================================
// 7.3: H3 Decision Device
// ================================================================

/**
 * Decision layer in H3 device
 */
struct DecisionLayer {
  int layer_index;
  NanoBrainTensor *weights;
  NanoBrainTensor *bias;
  float confidence_threshold;
  NanoBrainTensor *activation;
};

/**
 * Decision result from H3 device
 */
struct H3DecisionResult {
  int selected_action;
  float confidence;
  std::vector<float> action_probabilities;
  std::array<float, 3> layer_contributions;
  bool consensus_reached;
};

/**
 * H3 Decision Device
 * Three-layer decision architecture with confidence-weighted voting
 */
class H3DecisionDevice {
public:
  H3DecisionDevice(NanoBrainKernel *kernel, int input_dim, int action_count);
  ~H3DecisionDevice();

  void initialize();

  // Forward pass through all layers
  H3DecisionResult decide(NanoBrainTensor *input);

  // Forward through single layer
  NanoBrainTensor *layer_forward(int layer_idx, NanoBrainTensor *input);

  // Confidence-weighted voting
  std::vector<float>
  vote(const std::array<NanoBrainTensor *, 3> &layer_outputs);

  // Update weights based on feedback
  void update_weights(int layer_idx, NanoBrainTensor *gradient,
                      float learning_rate);

  // Get layer state
  const DecisionLayer *get_layer(int idx) const;

  // Set confidence thresholds
  void set_thresholds(float l1, float l2, float l3);

private:
  NanoBrainKernel *kernel;
  int input_dim;
  int action_count;
  std::array<DecisionLayer, 3> layers;
  NanoBrainTensor *voting_weights;

  void init_layer(int idx, int in_dim, int out_dim);
};

// ================================================================
// 7.4: Brain Region Simulators
// ================================================================

/**
 * Brain region types
 */
enum class BrainRegion {
  Cerebellum,   // Motor coordination, timing
  Hippocampus,  // Memory consolidation, spatial
  Hypothalamus, // Homeostasis, regulation
  PrefrontalCortex,
  TemporalLobe,
  ParietalLobe
};

/**
 * Brain region state
 */
struct BrainRegionState {
  BrainRegion region;
  float activity_level;
  NanoBrainTensor *state_tensor;
  std::vector<std::string> active_atom_ids;
  float coherence;
};

/**
 * Abstract brain region simulator
 */
class BrainRegionSimulator {
public:
  BrainRegionSimulator(NanoBrainKernel *kernel, BrainRegion region);
  virtual ~BrainRegionSimulator();

  virtual void initialize() = 0;
  virtual void process_cycle(const std::vector<NodeTensor *> &inputs) = 0;
  virtual NanoBrainTensor *get_output() const = 0;

  BrainRegion get_region() const { return region; }
  const BrainRegionState &get_state() const { return state; }

protected:
  NanoBrainKernel *kernel;
  BrainRegion region;
  BrainRegionState state;
};

/**
 * Cerebellum simulator - motor coordination and timing
 */
class CerebellumSimulator : public BrainRegionSimulator {
public:
  CerebellumSimulator(NanoBrainKernel *kernel);
  void initialize() override;
  void process_cycle(const std::vector<NodeTensor *> &inputs) override;
  NanoBrainTensor *get_output() const override;

  // Timing prediction
  float predict_timing(NanoBrainTensor *motor_input);

  // Coordination calculation
  NanoBrainTensor *
  coordinate(const std::vector<NanoBrainTensor *> &motor_signals);

private:
  NanoBrainTensor *timing_model;
  NanoBrainTensor *coordination_weights;
  NanoBrainTensor *output_tensor;
};

/**
 * Hippocampus simulator - memory and spatial processing
 */
class HippocampusSimulator : public BrainRegionSimulator {
public:
  HippocampusSimulator(NanoBrainKernel *kernel);
  void initialize() override;
  void process_cycle(const std::vector<NodeTensor *> &inputs) override;
  NanoBrainTensor *get_output() const override;

  // Memory encoding
  NanoBrainTensor *encode_episodic(NanoBrainTensor *experience);

  // Spatial mapping
  NanoBrainTensor *spatial_map(NanoBrainTensor *sensory_input);

  // Memory replay
  std::vector<NanoBrainTensor *> replay_memories(int count);

private:
  NanoBrainTensor *place_cells;
  NanoBrainTensor *grid_cells;
  NanoBrainTensor *output_tensor;
  std::vector<NanoBrainTensor *> memory_buffer;
};

/**
 * Hypothalamus simulator - homeostasis and regulation
 */
class HypothalamusSimulator : public BrainRegionSimulator {
public:
  HypothalamusSimulator(NanoBrainKernel *kernel);
  void initialize() override;
  void process_cycle(const std::vector<NodeTensor *> &inputs) override;
  NanoBrainTensor *get_output() const override;

  // Homeostatic drive calculation
  NanoBrainTensor *calculate_drive(const std::string &drive_type);

  // Regulation signal
  float get_regulation_signal(const std::string &system);

private:
  std::map<std::string, float> homeostatic_setpoints;
  std::map<std::string, float> current_values;
  NanoBrainTensor *drive_tensor;
  NanoBrainTensor *output_tensor;
};

// ================================================================
// 7.5: Twenty Conscious Expressions
// ================================================================

/**
 * Conscious expression enumeration
 * 12 dodecanion states + 8 octonion states = 20 total
 */
enum class ConsciousExpression {
  // Dodecanion states (12)
  Focus = 0,
  Diffuse = 1,
  Creative = 2,
  Analytical = 3,
  Emotional = 4,
  Logical = 5,
  Intuitive = 6,
  Deliberate = 7,
  Receptive = 8,
  Projective = 9,
  Unified = 10,
  Fragmented = 11,
  // Octonion states (8)
  Alert = 12,
  Drowsy = 13,
  Euphoric = 14,
  Anxious = 15,
  Calm = 16,
  Agitated = 17,
  Present = 18,
  Distant = 19
};

/**
 * Expression classification result
 */
struct ExpressionClassification {
  ConsciousExpression primary_expression;
  ConsciousExpression secondary_expression;
  std::array<float, 20> expression_probabilities;
  float classification_confidence;
  bool is_dodecanion; // true if primary is in first 12
};

/**
 * Consciousness Expression Engine
 * Recognizes and generates conscious expression states
 */
class ConsciousnessExpressionEngine {
public:
  ConsciousnessExpressionEngine(NanoBrainKernel *kernel);
  ~ConsciousnessExpressionEngine();

  void initialize();

  // Classify current expression from system state
  ExpressionClassification
  classify_state(const SystemState &system_state,
                 const std::vector<MetaCognitiveTensor> &meta_tensors);

  // Generate expression tensor
  NanoBrainTensor *generate_expression(ConsciousExpression expression);

  // Get expression name
  static std::string get_expression_name(ConsciousExpression expression);

  // Check if expression is dodecanion-based
  static bool is_dodecanion_expression(ConsciousExpression expression);

  // Get opposite expression
  static ConsciousExpression get_opposite(ConsciousExpression expression);

  // Blend two expressions
  NanoBrainTensor *blend_expressions(ConsciousExpression a,
                                     ConsciousExpression b, float blend_factor);

private:
  NanoBrainKernel *kernel;
  std::array<NanoBrainTensor *, 20> expression_templates;
  NanoBrainTensor *classifier_weights;

  void init_expression_templates();
};

// ================================================================
// Integrated Brain Model
// ================================================================

/**
 * Integrated brain model configuration
 */
struct IntegratedBrainConfig {
  int sensory_dim = 128;
  int memory_capacity = 1000;
  int action_count = 10;
  float decay_rate = 0.01f;
  float consolidation_threshold = 0.7f;
  bool enable_consciousness_tracking = true;
};

/**
 * Integrated brain model metrics
 */
struct IntegratedBrainMetrics {
  // Sensory processing
  std::array<float, 5> sensory_activations;
  float cross_modal_coherence;

  // Memory
  size_t total_memories;
  float average_memory_activation;
  float consolidation_rate;

  // Decision
  float decision_confidence;
  int decisions_made;

  // Brain regions
  std::map<BrainRegion, float> region_activities;
  float overall_brain_coherence;

  // Consciousness
  ConsciousExpression current_expression;
  float expression_stability;
};

/**
 * Integrated Brain Model
 * Complete time crystal brain architecture combining all Chapter 7 components
 */
class IntegratedBrainModel {
public:
  IntegratedBrainModel(NanoBrainKernel *kernel,
                       AttentionAllocationEngine *attention,
                       RecursiveReasoningEngine *reasoning,
                       MetaCognitiveFeedbackEngine *metacognitive,
                       const IntegratedBrainConfig &config);
  ~IntegratedBrainModel();

  void initialize();
  void shutdown();

  // ================================================================
  // Sensory Processing
  // ================================================================

  // Process sensory input
  NanoBrainTensor *process_sensory_input(SensoryModality modality,
                                         NanoBrainTensor *input);

  // Process multiple sensory inputs with cross-modal integration
  CrossModalResult process_multimodal(
      const std::map<SensoryModality, NanoBrainTensor *> &inputs);

  // ================================================================
  // Memory Operations
  // ================================================================

  // Store memory
  std::string store_memory(MemoryCarrier type, NanoBrainTensor *content);

  // Recall memory
  NanoBrainTensor *recall_memory(const std::string &memory_id);

  // Consolidate working memory to long-term
  void consolidate();

  // ================================================================
  // Decision Making
  // ================================================================

  // Make a decision based on current state
  H3DecisionResult make_decision(NanoBrainTensor *situation);

  // ================================================================
  // Brain Region Processing
  // ================================================================

  // Run all brain regions for one cycle
  void process_brain_regions(const std::vector<NodeTensor *> &node_tensors);

  // Get output from specific region
  NanoBrainTensor *get_region_output(BrainRegion region);

  // ================================================================
  // Consciousness
  // ================================================================

  // Get current conscious expression
  ExpressionClassification get_current_expression();

  // Set target expression
  void set_target_expression(ConsciousExpression target);

  // ================================================================
  // Processing Cycle
  // ================================================================

  // Run one complete brain processing cycle
  IntegratedBrainMetrics
  process_cycle(const std::vector<NodeTensor *> &node_tensors,
                const std::vector<LinkTensor *> &link_tensors);

  // ================================================================
  // Metrics and State
  // ================================================================

  IntegratedBrainMetrics get_metrics() const;
  const IntegratedBrainConfig &get_config() const { return config; }

private:
  NanoBrainKernel *kernel;
  AttentionAllocationEngine *attention;
  RecursiveReasoningEngine *reasoning;
  MetaCognitiveFeedbackEngine *metacognitive;
  IntegratedBrainConfig config;
  bool active = false;

  // Subsystems
  std::unique_ptr<SensoryPrimeMapper> sensory_mapper;
  std::unique_ptr<MemoryCarrierSystem> memory_system;
  std::unique_ptr<H3DecisionDevice> decision_device;
  std::unique_ptr<ConsciousnessExpressionEngine> expression_engine;

  // Brain regions
  std::unique_ptr<CerebellumSimulator> cerebellum;
  std::unique_ptr<HippocampusSimulator> hippocampus;
  std::unique_ptr<HypothalamusSimulator> hypothalamus;

  // State
  ConsciousExpression target_expression = ConsciousExpression::Focus;
  int cycle_count = 0;
  int64_t start_time = 0;

  int64_t current_time_millis() const;
};

#endif // NANOBRAIN_BRAIN_MODEL_H
