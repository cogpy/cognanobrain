#ifndef NANOBRAIN_BRAIN_JELLY_H
#define NANOBRAIN_BRAIN_JELLY_H

/**
 * NanoBrain Brain Jelly & Humanoid Avatar Module
 *
 * Chapter 9 Implementation: Fractal condensation, programmable matter,
 * and bio-morphic device systems for the NanoBrain cognitive architecture.
 *
 * Features:
 * - 17 Bio-morphic device types with inter-device communication
 * - Fractal condensation engine for multi-point condensation
 * - Brain jelly simulator with EEG signal generation
 * - Cortical pen freezer for dynamics→time crystal conversion
 * - Humanoid avatar interface with 11D signal sensing
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
// Bio-Morphic Device Types (17 neuromorphic device categories)
// ================================================================

/**
 * 17 Bio-morphic device types representing different neuronal
 * and glial cell archetypes for programmable matter simulation
 */
enum class BioMorphicDeviceType {
  // Fundamental neuristors
  Neuristor, // Generic neuron device
  Memristor, // Memory resistor (synaptic plasticity)
  Synapstor, // Synaptic junction device
  Denristor, // Dendritic processing device
  Axonistor, // Axonal transmission device

  // Glial cell inspired devices
  Gliaistor,         // Generic glial support device
  Astrocystor,       // Astrocyte-inspired signaling
  Oligodendrocystor, // Myelination/insulation device
  Microglisator,     // Immune/cleanup device
  Ependymistor,      // CSF interface device

  // Specialized neuron types
  Purkinjistor,    // Cerebellar Purkinje cell device
  Pyramidistor,    // Cortical pyramidal cell device
  Stellatistor,    // Stellate cell device
  Granulistor,     // Granule cell device
  Basketistor,     // Basket cell (inhibitory) device
  Chandelieristor, // Chandelier cell device
  Martinottistor   // Martinotti cell device
};

/**
 * Get string name for device type
 */
inline const char *get_device_type_name(BioMorphicDeviceType type) {
  switch (type) {
  case BioMorphicDeviceType::Neuristor:
    return "Neuristor";
  case BioMorphicDeviceType::Memristor:
    return "Memristor";
  case BioMorphicDeviceType::Synapstor:
    return "Synapstor";
  case BioMorphicDeviceType::Denristor:
    return "Denristor";
  case BioMorphicDeviceType::Axonistor:
    return "Axonistor";
  case BioMorphicDeviceType::Gliaistor:
    return "Gliaistor";
  case BioMorphicDeviceType::Astrocystor:
    return "Astrocystor";
  case BioMorphicDeviceType::Oligodendrocystor:
    return "Oligodendrocystor";
  case BioMorphicDeviceType::Microglisator:
    return "Microglisator";
  case BioMorphicDeviceType::Ependymistor:
    return "Ependymistor";
  case BioMorphicDeviceType::Purkinjistor:
    return "Purkinjistor";
  case BioMorphicDeviceType::Pyramidistor:
    return "Pyramidistor";
  case BioMorphicDeviceType::Stellatistor:
    return "Stellatistor";
  case BioMorphicDeviceType::Granulistor:
    return "Granulistor";
  case BioMorphicDeviceType::Basketistor:
    return "Basketistor";
  case BioMorphicDeviceType::Chandelieristor:
    return "Chandelieristor";
  case BioMorphicDeviceType::Martinottistor:
    return "Martinottistor";
  default:
    return "Unknown";
  }
}

// ================================================================
// Bio-Morphic Device Structures
// ================================================================

/**
 * Individual bio-morphic device instance
 */
struct BioMorphicDevice {
  std::string id;
  BioMorphicDeviceType type;

  // Device state
  float activation_level;  // Current activation [0, 1]
  float threshold;         // Firing threshold
  float refractory_period; // Recovery time (ms)
  float last_fire_time;    // Timestamp of last firing

  // Communication channels
  std::vector<std::string> input_connections;  // IDs of input devices
  std::vector<std::string> output_connections; // IDs of output devices
  std::vector<float> connection_weights;       // Synaptic weights

  // PPM encoding for device state
  std::vector<int> prime_encoding;
  float ppm_coherence;

  // Tensor representation
  NanoBrainTensor *state_tensor;
};

/**
 * Configuration for bio-morphic device registry
 */
struct BioMorphicRegistryConfig {
  int max_devices = 1024;
  float default_threshold = 0.5f;
  float communication_rate = 0.1f;
  float plasticity_rate = 0.01f;
  bool enable_collective_response = true;
};

/**
 * Collective response metrics
 */
struct CollectiveResponse {
  float synchronization_index;       // How synchronized devices are
  float collective_activation;       // Overall network activation
  float emergence_level;             // Emergent behavior metric
  std::vector<float> harmonic_modes; // Dominant oscillation modes
};

// ================================================================
// Fractal Condensation Structures
// ================================================================

/**
 * A point in 11D space where condensation occurs
 */
struct CondensationPoint {
  std::array<float, 11> position; // 11D position vector
  std::vector<int> prime_pattern; // Prime signature at this point
  float condensation_strength;    // Strength of condensation [0, 1]
  float resonance_frequency;      // Resonance frequency (Hz)
  float temporal_phase;           // Phase in time crystal cycle
  int condensation_order;         // Order of condensation (1-15)
};

/**
 * Configuration for fractal condensation engine
 */
struct FractalCondensationConfig {
  int max_condensation_points = 256;
  int condensation_order = 7;       // Prime order for condensation
  float spatial_resolution = 0.01f; // Resolution in 11D space
  float prime_threshold = 0.3f;     // Minimum prime alignment
  bool enable_everywhere = true;    // Condense at all points
};

/**
 * Condensation field tensor
 */
struct CondensationField {
  NanoBrainTensor *field_tensor; // 11D field values
  std::vector<CondensationPoint> active_points;
  float total_condensation;
  float field_coherence;
};

// ================================================================
// Brain Jelly Structures
// ================================================================

/**
 * Megamer unit in the brain jelly resonance chain
 */
struct MegamerUnit {
  std::string id;
  int chain_position;
  float resonance_amplitude;
  float phase_offset;
  std::vector<int> prime_encoding;
  std::array<float, 11> quantum_state;
};

/**
 * EEG band information
 */
enum class EEGBand {
  Delta, // 0.5-4 Hz (deep sleep)
  Theta, // 4-8 Hz (drowsiness, meditation)
  Alpha, // 8-13 Hz (relaxed, eyes closed)
  Beta,  // 13-30 Hz (active thinking)
  Gamma  // 30-100 Hz (perception, consciousness)
};

/**
 * EEG signal sample
 */
struct EEGSignal {
  float timestamp;
  std::vector<float> channel_values; // Per-channel voltage (μV)
  EEGBand dominant_band;
  float band_power[5]; // Power in each band
  float consciousness_index;
};

/**
 * Brain jelly configuration
 */
struct BrainJellyConfig {
  int megamer_chain_length = 64;
  float base_frequency = 10.0f; // Alpha rhythm base
  int eeg_channels = 19;        // Standard 10-20 montage
  float sample_rate = 256.0f;   // Samples per second
  bool enable_prime_writing = true;
};

// ================================================================
// Cortical Pen Freezer Structures
// ================================================================

/**
 * Problem capsule containing frozen dynamics
 */
struct ProblemCapsule {
  std::string id;
  NanoBrainTensor *frozen_state;
  std::vector<int> prime_signature;
  float temporal_anchor;
  int complexity_order;
  std::vector<std::string> related_atoms;
};

/**
 * Cortical pen freezer configuration
 */
struct CorticalPenConfig {
  int max_capsules = 128;
  float freeze_threshold = 0.7f;
  bool preserve_dynamics = true;
  int time_crystal_resolution = 11;
};

// ================================================================
// Humanoid Avatar Structures
// ================================================================

/**
 * 11D sensory signal from avatar
 */
struct Signal11D {
  std::array<float, 11> dimensions;
  float signal_strength;
  int64_t timestamp;
  std::string source_sensor;
};

/**
 * Sensor-Jelly-Muscle-Brain triad state
 */
struct SensorTriadState {
  // Sensor layer
  std::vector<Signal11D> sensory_inputs;
  float sensory_integration;

  // Jelly layer
  float jelly_resonance;
  std::vector<float> jelly_currents;

  // Muscle layer
  std::vector<float> motor_outputs;
  float motor_coordination;

  // Brain layer
  NanoBrainTensor *cognitive_state;
  float consciousness_level;
};

/**
 * Humanoid avatar configuration
 */
struct HumanoidAvatarConfig {
  int sensor_count = 64;
  int motor_count = 32;
  bool enable_11d_sensing = true;
  float response_latency = 0.01f; // seconds
  bool enable_time_crystal_net = true;
};

/**
 * Avatar metrics
 */
struct AvatarMetrics {
  float sensory_bandwidth;
  float motor_precision;
  float cognitive_load;
  float embodiment_index;
  float temporal_coherence;
};

// ================================================================
// Bio-Morphic Device Registry
// ================================================================

/**
 * Registry managing all bio-morphic devices with inter-device
 * communication and collective response computation
 */
class BioMorphicDeviceRegistry {
public:
  BioMorphicDeviceRegistry(NanoBrainKernel *kernel,
                           const BioMorphicRegistryConfig &config);
  ~BioMorphicDeviceRegistry();

  // Device management
  std::string create_device(BioMorphicDeviceType type,
                            const std::string &name = "");
  BioMorphicDevice *get_device(const std::string &id);
  const BioMorphicDevice *get_device(const std::string &id) const;
  bool remove_device(const std::string &id);
  std::vector<std::string> get_all_device_ids() const;
  size_t device_count() const { return devices.size(); }

  // Connections
  bool connect_devices(const std::string &source_id,
                       const std::string &target_id, float weight = 1.0f);
  bool disconnect_devices(const std::string &source_id,
                          const std::string &target_id);

  // Processing
  void update_all_devices(float delta_time);
  void propagate_signals();
  CollectiveResponse compute_collective_response();

  // Device type queries
  std::vector<std::string> get_devices_by_type(BioMorphicDeviceType type) const;
  int count_device_type(BioMorphicDeviceType type) const;

  // Initialize all 17 device types
  void initialize_full_registry();

private:
  NanoBrainKernel *kernel;
  BioMorphicRegistryConfig config;
  std::map<std::string, std::unique_ptr<BioMorphicDevice>> devices;
  int device_counter = 0;

  void update_device(BioMorphicDevice &device, float delta_time);
  float compute_input_sum(const BioMorphicDevice &device);
  void fire_device(BioMorphicDevice &device, float current_time);
};

// ================================================================
// Fractal Condensation Engine
// ================================================================

/**
 * Engine for multi-point fractal condensation in 11D space
 * using prime patterns for structure formation
 */
class FractalCondensation {
public:
  FractalCondensation(NanoBrainKernel *kernel,
                      const FractalCondensationConfig &config);
  ~FractalCondensation();

  // Core condensation operations
  void condense_everywhere();
  void apply_prime_pattern();
  std::vector<CondensationPoint> get_points() const;

  // Point management
  void add_condensation_point(const CondensationPoint &point);
  void clear_points();
  size_t point_count() const { return active_points.size(); }

  // Field operations
  CondensationField compute_field();
  float get_field_value(const std::array<float, 11> &position);

  // Prime pattern operations
  void set_prime_pattern(const std::vector<int> &primes);
  std::vector<int> get_current_pattern() const { return current_pattern; }
  float compute_pattern_coherence() const;

private:
  NanoBrainKernel *kernel;
  FractalCondensationConfig config;
  std::vector<CondensationPoint> active_points;
  std::vector<int> current_pattern;
  CondensationField field;

  CondensationPoint
  find_condensation_point(const std::array<float, 11> &seed_position);
  float compute_local_density(const std::array<float, 11> &position);
  void apply_prime_at_point(CondensationPoint &point);
};

// ================================================================
// Brain Jelly Simulator
// ================================================================

/**
 * Simulator for brain jelly mechanics including megamer chains,
 * EEG signal generation, and jelly-based prime writing
 */
class BrainJellySimulator {
public:
  BrainJellySimulator(NanoBrainKernel *kernel, const BrainJellyConfig &config);
  ~BrainJellySimulator();

  // Megamer chain operations
  void generate_megamer_chain();
  void update_chain(float delta_time);
  std::vector<MegamerUnit> &get_chain() { return megamer_chain; }
  const std::vector<MegamerUnit> &get_chain() const { return megamer_chain; }

  // EEG signal generation
  EEGSignal generate_eeg_signal(float timestamp);
  std::vector<EEGSignal> generate_eeg_epoch(float start_time, float duration);
  float get_band_power(EEGBand band) const;

  // Prime writing
  void write_prime_pattern(const std::vector<int> &primes);
  std::vector<int> read_prime_pattern() const;

  // State queries
  float get_total_resonance() const;
  float get_consciousness_index() const;

private:
  NanoBrainKernel *kernel;
  BrainJellyConfig config;
  std::vector<MegamerUnit> megamer_chain;
  std::vector<int> written_primes;
  float current_time = 0.0f;

  float compute_eeg_sample(int channel, float timestamp);
  float compute_band_power(EEGBand band, float timestamp);
  void propagate_resonance();
};

// ================================================================
// Cortical Pen Freezer
// ================================================================

/**
 * Freezes cognitive dynamics into time crystal structures,
 * creating problem capsules for later retrieval
 */
class CorticalPenFreezer {
public:
  CorticalPenFreezer(NanoBrainKernel *kernel, TimeCrystalKernel *time_crystal,
                     const CorticalPenConfig &config);
  ~CorticalPenFreezer();

  // Freezing operations
  std::string freeze_dynamics(NanoBrainTensor *current_state,
                              const std::vector<std::string> &related_atoms);
  ProblemCapsule *create_problem_capsule(const std::string &problem_id);

  // Capsule management
  ProblemCapsule *get_capsule(const std::string &id);
  const ProblemCapsule *get_capsule(const std::string &id) const;
  bool delete_capsule(const std::string &id);
  std::vector<std::string> get_all_capsule_ids() const;
  size_t capsule_count() const { return capsules.size(); }

  // Retrieval
  NanoBrainTensor *thaw_capsule(const std::string &id);
  std::vector<int> get_capsule_signature(const std::string &id) const;

private:
  NanoBrainKernel *kernel;
  TimeCrystalKernel *time_crystal;
  CorticalPenConfig config;
  std::map<std::string, std::unique_ptr<ProblemCapsule>> capsules;
  int capsule_counter = 0;

  std::vector<int> compute_signature(NanoBrainTensor *state);
  NanoBrainTensor *encode_to_time_crystal(NanoBrainTensor *dynamics);
};

// ================================================================
// Humanoid Avatar Interface
// ================================================================

/**
 * Interface for humanoid avatar with 11D signal sensing
 * and sensor-jelly-muscle-brain triad processing
 */
class HumanoidAvatarInterface {
public:
  HumanoidAvatarInterface(NanoBrainKernel *kernel,
                          TimeCrystalKernel *time_crystal,
                          BrainJellySimulator *brain_jelly,
                          const HumanoidAvatarConfig &config);
  ~HumanoidAvatarInterface();

  // Sensing
  void sense_11d_signals(const std::vector<Signal11D> &signals);
  Signal11D get_integrated_signal() const;

  // Triad processing
  void process_sensor_triad();
  SensorTriadState get_triad_state() const { return triad_state; }

  // Motor output
  std::vector<float> get_motor_commands() const;
  void set_motor_targets(const std::vector<float> &targets);

  // Neural network
  NanoBrainTensor *get_neural_output();
  void update_neural_state(float delta_time);

  // Metrics
  AvatarMetrics get_metrics() const;
  float get_embodiment_index() const;

private:
  NanoBrainKernel *kernel;
  TimeCrystalKernel *time_crystal;
  BrainJellySimulator *brain_jelly;
  HumanoidAvatarConfig config;
  SensorTriadState triad_state;
  std::vector<float> motor_targets;
  NanoBrainTensor *neural_state;

  void integrate_sensory_inputs();
  void process_jelly_layer();
  void compute_motor_outputs();
  void update_cognitive_state();
  Signal11D fuse_signals(const std::vector<Signal11D> &signals);
};

// ================================================================
// Integrated Brain Jelly System
// ================================================================

/**
 * Configuration for complete brain jelly system
 */
struct IntegratedBrainJellyConfig {
  BioMorphicRegistryConfig registry_config;
  FractalCondensationConfig condensation_config;
  BrainJellyConfig jelly_config;
  CorticalPenConfig pen_config;
  HumanoidAvatarConfig avatar_config;
};

/**
 * Complete brain jelly system metrics
 */
struct BrainJellyMetrics {
  // Device metrics
  size_t total_devices;
  float collective_activation;
  float synchronization_index;

  // Condensation metrics
  size_t condensation_points;
  float field_coherence;
  float prime_alignment;

  // Jelly metrics
  float total_resonance;
  float consciousness_index;
  float eeg_alpha_power;

  // Capsule metrics
  size_t frozen_capsules;
  float average_complexity;

  // Avatar metrics
  float embodiment_index;
  float sensory_bandwidth;
  float motor_precision;
};

/**
 * Integrated brain jelly cognitive system
 * Coordinates all Chapter 9 components
 */
class IntegratedBrainJellySystem {
public:
  IntegratedBrainJellySystem(NanoBrainKernel *kernel,
                             TimeCrystalKernel *time_crystal,
                             const IntegratedBrainJellyConfig &config);
  ~IntegratedBrainJellySystem();

  // Lifecycle
  void initialize();
  void shutdown();
  bool is_active() const { return active; }

  // Processing
  void process_cycle(float delta_time);

  // Component access
  BioMorphicDeviceRegistry *get_device_registry() {
    return device_registry.get();
  }
  FractalCondensation *get_condensation_engine() {
    return condensation_engine.get();
  }
  BrainJellySimulator *get_brain_jelly() { return brain_jelly.get(); }
  CorticalPenFreezer *get_pen_freezer() { return pen_freezer.get(); }
  HumanoidAvatarInterface *get_avatar_interface() {
    return avatar_interface.get();
  }

  // Metrics
  BrainJellyMetrics get_metrics() const;

private:
  NanoBrainKernel *kernel;
  TimeCrystalKernel *time_crystal;
  IntegratedBrainJellyConfig config;

  std::unique_ptr<BioMorphicDeviceRegistry> device_registry;
  std::unique_ptr<FractalCondensation> condensation_engine;
  std::unique_ptr<BrainJellySimulator> brain_jelly;
  std::unique_ptr<CorticalPenFreezer> pen_freezer;
  std::unique_ptr<HumanoidAvatarInterface> avatar_interface;

  bool active = false;
  float current_time = 0.0f;
};

#endif // NANOBRAIN_BRAIN_JELLY_H
