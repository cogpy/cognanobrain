#ifndef NANOBRAIN_NPU_H
#define NANOBRAIN_NPU_H

/**
 * NanoBrain NPU - Neural Processing Unit Cognitive Coprocessor
 *
 * Hardware-style memory-mapped interface exposing NanoBrain cognitive
 * functions as a coprocessor for LLM inference augmentation.
 *
 * Register Map @ 0x40002000:
 * - CMD, STATUS: Control and status
 * - PPM_COHERENCE: Phase Prime Metric coherence level
 * - CONSCIOUSNESS: Emergence level
 * - ATTENTION_STI: Short-term importance
 * - TC_PHASE: Time crystal phase
 */

#include "nanobrain_unified.h"
#include <array>
#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <vector>

// ================================================================
// Register Map
// ================================================================

namespace nanobrain_npu {

// Base address in VirtualPCB peripheral space
static constexpr uint64_t REG_NB_BASE = 0x40002000;

// Control Registers
static constexpr uint64_t REG_NB_CMD = REG_NB_BASE + 0x00;
static constexpr uint64_t REG_NB_STATUS = REG_NB_BASE + 0x04;

// Cognitive Metrics Registers (read-only)
static constexpr uint64_t REG_NB_PPM_COHERENCE = REG_NB_BASE + 0x08;
static constexpr uint64_t REG_NB_CONSCIOUSNESS = REG_NB_BASE + 0x0C;
static constexpr uint64_t REG_NB_ATTENTION_STI = REG_NB_BASE + 0x10;
static constexpr uint64_t REG_NB_ATTENTION_LTI = REG_NB_BASE + 0x14;
static constexpr uint64_t REG_NB_TC_PHASE = REG_NB_BASE + 0x18;
static constexpr uint64_t REG_NB_CYCLE_COUNT = REG_NB_BASE + 0x1C;

// Reasoning Registers
static constexpr uint64_t REG_NB_REASONING_CONF = REG_NB_BASE + 0x20;
static constexpr uint64_t REG_NB_INFERENCE_COUNT = REG_NB_BASE + 0x24;

// Configuration Registers (read/write)
static constexpr uint64_t REG_NB_CONFIG_FLAGS = REG_NB_BASE + 0x28;
static constexpr uint64_t REG_NB_TC_DIMENSIONS = REG_NB_BASE + 0x2C;
static constexpr uint64_t REG_NB_REASONING_DEPTH = REG_NB_BASE + 0x30;

// Error Register
static constexpr uint64_t REG_NB_ERROR = REG_NB_BASE + 0x34;

// Performance Registers
static constexpr uint64_t REG_NB_PERF_CYCLES_SEC = REG_NB_BASE + 0x38;
static constexpr uint64_t REG_NB_PERF_ATOMS = REG_NB_BASE + 0x3C;

// Command Bits
static constexpr uint32_t CMD_RESET = 0x01;
static constexpr uint32_t CMD_INIT = 0x02;
static constexpr uint32_t CMD_RUN_CYCLE = 0x04;
static constexpr uint32_t CMD_SHUTDOWN = 0x08;
static constexpr uint32_t CMD_CREATE_ATOM = 0x10;
static constexpr uint32_t CMD_START_REASONING = 0x20;
static constexpr uint32_t CMD_AUGMENT_LLM = 0x40;

// Status Bits
static constexpr uint32_t STATUS_IDLE = 0x01;
static constexpr uint32_t STATUS_BUSY = 0x02;
static constexpr uint32_t STATUS_INITIALIZED = 0x04;
static constexpr uint32_t STATUS_ERROR = 0x08;
static constexpr uint32_t STATUS_REASONING = 0x10;
static constexpr uint32_t STATUS_COHERENT = 0x20;
static constexpr uint32_t STATUS_CONSCIOUS = 0x40;

// Config Flags
static constexpr uint32_t CFG_ENABLE_PPM = 0x01;
static constexpr uint32_t CFG_ENABLE_TC = 0x02;
static constexpr uint32_t CFG_ENABLE_ECAN = 0x04;
static constexpr uint32_t CFG_ENABLE_PLN = 0x08;
static constexpr uint32_t CFG_ENABLE_META = 0x10;

// Error Codes
static constexpr uint32_t ERR_NONE = 0x00;
static constexpr uint32_t ERR_NOT_INITIALIZED = 0x01;
static constexpr uint32_t ERR_INVALID_CMD = 0x02;
static constexpr uint32_t ERR_OUT_OF_MEMORY = 0x03;
static constexpr uint32_t ERR_REASONING_FAILED = 0x04;

} // namespace nanobrain_npu

// ================================================================
// Structures
// ================================================================

/**
 * Coprocessor configuration
 */
struct NPUConfig {
  // Enable flags
  bool enable_ppm = true;
  bool enable_time_crystal = true;
  bool enable_ecan = true;
  bool enable_pln = true;
  bool enable_metacognitive = true;

  // Time crystal settings
  int tc_dimensions = 11;
  int tc_primes = 15;

  // Reasoning settings
  int max_reasoning_depth = 5;
  float reasoning_threshold = 0.5f;

  // Attention settings
  float attention_temperature = 1.0f;
  float sti_rent = 0.1f;
  float lti_rent = 0.05f;

  // Performance
  int cycles_per_second = 100;
};

/**
 * Coprocessor telemetry
 */
struct NPUTelemetry {
  // Cognitive metrics
  float ppm_coherence;
  float consciousness_level;
  float attention_sti;
  float attention_lti;
  float tc_phase;

  // Counters
  uint64_t cycle_count;
  uint64_t inference_count;
  uint64_t atom_count;

  // Performance
  float cycles_per_second;
  float reasoning_confidence;

  // Status
  bool is_initialized;
  bool is_busy;
  bool has_error;
  uint32_t error_code;
};

/**
 * LLM augmentation result
 */
struct AugmentationResult {
  float coherence_boost;                   // Added coherence to LLM context
  float attention_modifier;                // Attention weight adjustment
  float confidence;                        // Reasoning confidence
  std::vector<std::string> injected_atoms; // Relevant atoms injected
  std::string reasoning_chain;             // Reasoning chain summary
};

// ================================================================
// NanoBrain Coprocessor Class
// ================================================================

/**
 * NanoBrain Cognitive Coprocessor
 *
 * Exposes NanoBrain cognitive engine through hardware-style
 * memory-mapped registers for integration with LLM inference.
 */
class NanoBrainCoprocessor {
public:
  NanoBrainCoprocessor();
  ~NanoBrainCoprocessor();

  // ================================================================
  // Lifecycle
  // ================================================================

  /**
   * Initialize the coprocessor with configuration
   */
  bool initialize(const NPUConfig &config = {});

  /**
   * Shutdown and cleanup
   */
  void shutdown();

  /**
   * Reset to initial state
   */
  void reset();

  /**
   * Check if initialized
   */
  bool is_initialized() const { return initialized_; }

  // ================================================================
  // Register Access (Hardware-style)
  // ================================================================

  /**
   * Write 32-bit value to register
   */
  void write_reg32(uint64_t addr, uint32_t value);

  /**
   * Read 32-bit value from register
   */
  uint32_t read_reg32(uint64_t addr) const;

  /**
   * Read floating-point metric register
   */
  float read_metric(uint64_t addr) const;

  // ================================================================
  // Command Interface
  // ================================================================

  /**
   * Execute command (via CMD register)
   */
  bool execute_command(uint32_t cmd);

  /**
   * Run cognitive processing cycle
   */
  bool run_cycle();

  /**
   * Run multiple cycles
   */
  bool run_cycles(int count);

  // ================================================================
  // Cognitive Functions
  // ================================================================

  /**
   * Create atom in cognitive space
   */
  std::string create_atom(const std::string &type, const std::string &name,
                          float strength = 1.0f, float confidence = 0.9f);

  /**
   * Start reasoning chain
   */
  std::string start_reasoning(const std::vector<std::string> &premises);

  /**
   * Get PPM coherence for prime sequence
   */
  float compute_coherence(const std::vector<int> &primes);

  /**
   * Get consciousness emergence level
   */
  float get_consciousness_level() const;

  // ================================================================
  // LLM Augmentation
  // ================================================================

  /**
   * Augment LLM inference with cognitive context
   * @param prompt The input prompt
   * @param context Additional context tokens
   * @return Augmentation result with cognitive enhancements
   */
  AugmentationResult
  augment_inference(const std::string &prompt,
                    const std::vector<std::string> &context = {});

  /**
   * Get attention weights for context
   */
  std::vector<float>
  get_attention_weights(const std::vector<std::string> &tokens);

  /**
   * Inject relevant atoms into LLM context
   */
  std::vector<std::string> get_relevant_atoms(const std::string &query,
                                              int max_results = 10);

  // ================================================================
  // Telemetry
  // ================================================================

  /**
   * Get current telemetry
   */
  NPUTelemetry get_telemetry() const;

  /**
   * Get status string
   */
  std::string get_status_string() const;

  /**
   * Get diagnostic info
   */
  std::string get_diagnostics() const;

  // ================================================================
  // Configuration
  // ================================================================

  /**
   * Update configuration
   */
  void update_config(const NPUConfig &config);

  /**
   * Get current configuration
   */
  const NPUConfig &get_config() const { return config_; }

private:
  // Core kernel
  std::unique_ptr<NanoBrainKernel> kernel_;
  std::unique_ptr<UnifiedNanoBrainKernel> unified_kernel_;

  // Configuration
  NPUConfig config_;
  bool initialized_;

  // Register storage
  std::array<uint32_t, 64> registers_;

  // Telemetry
  NPUTelemetry telemetry_;

  // Internal helpers
  void update_status_registers();
  void update_metric_registers();
  void clear_registers();
  uint32_t get_config_flags() const;
  void apply_config_flags(uint32_t flags);
};

// ================================================================
// Utility Functions
// ================================================================

/**
 * Convert float to fixed-point for register
 */
inline uint32_t float_to_fixed(float value, int fractional_bits = 16) {
  return static_cast<uint32_t>(value * (1 << fractional_bits));
}

/**
 * Convert fixed-point register to float
 */
inline float fixed_to_float(uint32_t value, int fractional_bits = 16) {
  return static_cast<float>(value) / (1 << fractional_bits);
}

/**
 * Get register name from address
 */
std::string get_register_name(uint64_t addr);

/**
 * Print register map
 */
void print_register_map();

#endif // NANOBRAIN_NPU_H
