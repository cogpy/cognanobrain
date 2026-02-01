#include "nanobrain_npu.h"
#include <algorithm>
#include <chrono>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>

using namespace nanobrain_npu;

// ================================================================
// Constructor / Destructor
// ================================================================

NanoBrainCoprocessor::NanoBrainCoprocessor() : initialized_(false) {
  clear_registers();
}

NanoBrainCoprocessor::~NanoBrainCoprocessor() {
  if (initialized_) {
    shutdown();
  }
}

// ================================================================
// Lifecycle
// ================================================================

bool NanoBrainCoprocessor::initialize(const NPUConfig &config) {
  if (initialized_) {
    return true;
  }

  config_ = config;

  try {
    // Create core kernel
    NanoBrainConfig kernel_config;
    kernel_config.memory_size = 1024 * 1024 * 64; // 64 MB
    kernel_ = std::make_unique<NanoBrainKernel>(kernel_config);

    // Create unified kernel
    UnifiedNanoBrainConfig unified_config;
    unified_config.time_crystal_dimensions = config.tc_dimensions;
    unified_config.max_reasoning_depth = config.max_reasoning_depth;
    unified_config.attention_temperature = config.attention_temperature;

    unified_kernel_ = std::make_unique<UnifiedNanoBrainKernel>(unified_config);
    unified_kernel_->initialize();

    // Initialize telemetry
    telemetry_ = {};
    telemetry_.is_initialized = true;

    // Update registers
    initialized_ = true;
    update_status_registers();
    update_metric_registers();

    std::cout << "[NanoBrain NPU] Initialized @ 0x" << std::hex << REG_NB_BASE
              << std::dec << std::endl;
    return true;

  } catch (const std::exception &e) {
    std::cerr << "[NanoBrain NPU] Init failed: " << e.what() << std::endl;
    registers_[REG_NB_ERROR - REG_NB_BASE] = ERR_NOT_INITIALIZED;
    return false;
  }
}

void NanoBrainCoprocessor::shutdown() {
  if (unified_kernel_) {
    unified_kernel_->shutdown();
    unified_kernel_.reset();
  }
  kernel_.reset();
  initialized_ = false;
  clear_registers();
  std::cout << "[NanoBrain NPU] Shutdown complete" << std::endl;
}

void NanoBrainCoprocessor::reset() {
  NPUConfig saved_config = config_;
  shutdown();
  initialize(saved_config);
}

// ================================================================
// Register Access
// ================================================================

void NanoBrainCoprocessor::write_reg32(uint64_t addr, uint32_t value) {
  if (addr < REG_NB_BASE || addr >= REG_NB_BASE + registers_.size() * 4) {
    return;
  }

  size_t offset = (addr - REG_NB_BASE) / 4;

  // Handle command register specially
  if (addr == REG_NB_CMD) {
    execute_command(value);
    return;
  }

  // Handle config registers
  if (addr == REG_NB_CONFIG_FLAGS) {
    apply_config_flags(value);
  } else if (addr == REG_NB_TC_DIMENSIONS) {
    config_.tc_dimensions = static_cast<int>(value);
  } else if (addr == REG_NB_REASONING_DEPTH) {
    config_.max_reasoning_depth = static_cast<int>(value);
  }

  registers_[offset] = value;
}

uint32_t NanoBrainCoprocessor::read_reg32(uint64_t addr) const {
  if (addr < REG_NB_BASE || addr >= REG_NB_BASE + registers_.size() * 4) {
    return 0;
  }

  size_t offset = (addr - REG_NB_BASE) / 4;
  return registers_[offset];
}

float NanoBrainCoprocessor::read_metric(uint64_t addr) const {
  uint32_t raw = read_reg32(addr);
  return fixed_to_float(raw);
}

// ================================================================
// Command Interface
// ================================================================

bool NanoBrainCoprocessor::execute_command(uint32_t cmd) {
  // Set busy status
  registers_[(REG_NB_STATUS - REG_NB_BASE) / 4] |= STATUS_BUSY;
  registers_[(REG_NB_STATUS - REG_NB_BASE) / 4] &= ~STATUS_IDLE;

  bool success = true;

  if (cmd & CMD_RESET) {
    reset();
  }

  if (cmd & CMD_INIT) {
    success = initialize(config_);
  }

  if (cmd & CMD_SHUTDOWN) {
    shutdown();
  }

  if (cmd & CMD_RUN_CYCLE) {
    success = run_cycle();
  }

  // Clear busy, set idle
  registers_[(REG_NB_STATUS - REG_NB_BASE) / 4] &= ~STATUS_BUSY;
  registers_[(REG_NB_STATUS - REG_NB_BASE) / 4] |= STATUS_IDLE;

  // Clear command register
  registers_[(REG_NB_CMD - REG_NB_BASE) / 4] = 0;

  return success;
}

bool NanoBrainCoprocessor::run_cycle() {
  if (!initialized_ || !unified_kernel_) {
    registers_[(REG_NB_ERROR - REG_NB_BASE) / 4] = ERR_NOT_INITIALIZED;
    return false;
  }

  auto metrics = unified_kernel_->run_cycles(1);

  telemetry_.cycle_count++;
  telemetry_.ppm_coherence = metrics.average_coherence;
  telemetry_.consciousness_level = metrics.consciousness_emergence;

  update_metric_registers();
  return true;
}

bool NanoBrainCoprocessor::run_cycles(int count) {
  if (!initialized_ || !unified_kernel_) {
    return false;
  }

  auto start = std::chrono::steady_clock::now();

  auto metrics = unified_kernel_->run_cycles(count);

  auto end = std::chrono::steady_clock::now();
  auto duration = std::chrono::duration<float>(end - start).count();

  telemetry_.cycle_count += count;
  telemetry_.ppm_coherence = metrics.average_coherence;
  telemetry_.consciousness_level = metrics.consciousness_emergence;
  telemetry_.cycles_per_second = count / duration;

  update_metric_registers();
  return true;
}

// ================================================================
// Cognitive Functions
// ================================================================

std::string NanoBrainCoprocessor::create_atom(const std::string &type,
                                              const std::string &name,
                                              float strength,
                                              float confidence) {
  if (!initialized_ || !unified_kernel_) {
    return "";
  }

  std::string atom_id =
      unified_kernel_->create_atom(type, name, strength, confidence);
  telemetry_.atom_count++;

  registers_[(REG_NB_PERF_ATOMS - REG_NB_BASE) / 4] =
      static_cast<uint32_t>(telemetry_.atom_count);

  return atom_id;
}

std::string NanoBrainCoprocessor::start_reasoning(
    const std::vector<std::string> &premises) {
  if (!initialized_ || !unified_kernel_) {
    return "";
  }

  // Set reasoning status
  registers_[(REG_NB_STATUS - REG_NB_BASE) / 4] |= STATUS_REASONING;

  std::string chain_id = unified_kernel_->start_reasoning(premises);
  telemetry_.inference_count++;

  // Update registers
  registers_[(REG_NB_INFERENCE_COUNT - REG_NB_BASE) / 4] =
      static_cast<uint32_t>(telemetry_.inference_count);
  registers_[(REG_NB_STATUS - REG_NB_BASE) / 4] &= ~STATUS_REASONING;

  return chain_id;
}

float NanoBrainCoprocessor::compute_coherence(const std::vector<int> &primes) {
  if (!initialized_ || !unified_kernel_) {
    return 0.0f;
  }

  float coherence = unified_kernel_->compute_coherence(primes);
  telemetry_.ppm_coherence = coherence;

  registers_[(REG_NB_PPM_COHERENCE - REG_NB_BASE) / 4] =
      float_to_fixed(coherence);

  // Update coherence status
  if (coherence > 0.7f) {
    registers_[(REG_NB_STATUS - REG_NB_BASE) / 4] |= STATUS_COHERENT;
  } else {
    registers_[(REG_NB_STATUS - REG_NB_BASE) / 4] &= ~STATUS_COHERENT;
  }

  return coherence;
}

float NanoBrainCoprocessor::get_consciousness_level() const {
  return telemetry_.consciousness_level;
}

// ================================================================
// LLM Augmentation
// ================================================================

AugmentationResult NanoBrainCoprocessor::augment_inference(
    const std::string &prompt, const std::vector<std::string> &context) {
  AugmentationResult result;
  result.coherence_boost = 0.0f;
  result.attention_modifier = 1.0f;
  result.confidence = 0.0f;

  if (!initialized_ || !unified_kernel_) {
    return result;
  }

  // Run cognitive cycle to update state
  run_cycle();

  // Get PPM coherence boost
  result.coherence_boost = telemetry_.ppm_coherence * 0.1f;

  // Query relevant atoms
  result.injected_atoms = get_relevant_atoms(prompt, 5);

  // Calculate attention modifier based on consciousness
  result.attention_modifier = 1.0f + telemetry_.consciousness_level * 0.2f;

  // Get reasoning confidence
  result.confidence = telemetry_.reasoning_confidence;

  // Build reasoning chain summary
  std::ostringstream oss;
  oss << "Coherence: " << telemetry_.ppm_coherence
      << ", Consciousness: " << telemetry_.consciousness_level;
  result.reasoning_chain = oss.str();

  return result;
}

std::vector<float> NanoBrainCoprocessor::get_attention_weights(
    const std::vector<std::string> &tokens) {
  std::vector<float> weights(tokens.size(), 1.0f);

  if (!initialized_ || !unified_kernel_) {
    return weights;
  }

  // Simple attention based on token length and consciousness
  for (size_t i = 0; i < tokens.size(); i++) {
    float base_weight = 1.0f + tokens[i].length() * 0.01f;
    weights[i] = base_weight * (1.0f + telemetry_.consciousness_level * 0.1f);
  }

  return weights;
}

std::vector<std::string>
NanoBrainCoprocessor::get_relevant_atoms(const std::string &query,
                                         int max_results) {
  std::vector<std::string> atoms;

  if (!initialized_ || !unified_kernel_) {
    return atoms;
  }

  // Return atom IDs that match the query context
  // (simplified - in full implementation would search AtomSpace)
  atoms.push_back("concept_" +
                  query.substr(0, std::min(size_t(10), query.size())));

  return atoms;
}

// ================================================================
// Telemetry
// ================================================================

NPUTelemetry NanoBrainCoprocessor::get_telemetry() const { return telemetry_; }

std::string NanoBrainCoprocessor::get_status_string() const {
  uint32_t status = registers_[(REG_NB_STATUS - REG_NB_BASE) / 4];

  std::ostringstream oss;
  oss << "NanoBrain NPU Status: ";

  if (status & STATUS_IDLE)
    oss << "IDLE ";
  if (status & STATUS_BUSY)
    oss << "BUSY ";
  if (status & STATUS_INITIALIZED)
    oss << "INIT ";
  if (status & STATUS_ERROR)
    oss << "ERROR ";
  if (status & STATUS_REASONING)
    oss << "REASONING ";
  if (status & STATUS_COHERENT)
    oss << "COHERENT ";
  if (status & STATUS_CONSCIOUS)
    oss << "CONSCIOUS ";

  return oss.str();
}

std::string NanoBrainCoprocessor::get_diagnostics() const {
  std::ostringstream oss;
  oss << std::fixed << std::setprecision(4);

  oss << "=== NanoBrain NPU Diagnostics ===" << std::endl;
  oss << "Base Address: 0x" << std::hex << REG_NB_BASE << std::dec << std::endl;
  oss << "Initialized: " << (initialized_ ? "Yes" : "No") << std::endl;
  oss << std::endl;

  oss << "Cognitive Metrics:" << std::endl;
  oss << "  PPM Coherence: " << telemetry_.ppm_coherence << std::endl;
  oss << "  Consciousness: " << telemetry_.consciousness_level << std::endl;
  oss << "  Attention STI: " << telemetry_.attention_sti << std::endl;
  oss << "  TC Phase: " << telemetry_.tc_phase << std::endl;
  oss << std::endl;

  oss << "Counters:" << std::endl;
  oss << "  Cycles: " << telemetry_.cycle_count << std::endl;
  oss << "  Inferences: " << telemetry_.inference_count << std::endl;
  oss << "  Atoms: " << telemetry_.atom_count << std::endl;
  oss << std::endl;

  oss << "Performance:" << std::endl;
  oss << "  Cycles/sec: " << telemetry_.cycles_per_second << std::endl;

  return oss.str();
}

// ================================================================
// Configuration
// ================================================================

void NanoBrainCoprocessor::update_config(const NPUConfig &config) {
  config_ = config;
  apply_config_flags(get_config_flags());
}

// ================================================================
// Internal Helpers
// ================================================================

void NanoBrainCoprocessor::update_status_registers() {
  uint32_t status = 0;

  if (initialized_) {
    status |= STATUS_INITIALIZED;
    status |= STATUS_IDLE;
  }

  if (telemetry_.has_error) {
    status |= STATUS_ERROR;
  }

  if (telemetry_.ppm_coherence > 0.7f) {
    status |= STATUS_COHERENT;
  }

  if (telemetry_.consciousness_level > 0.5f) {
    status |= STATUS_CONSCIOUS;
  }

  registers_[(REG_NB_STATUS - REG_NB_BASE) / 4] = status;
}

void NanoBrainCoprocessor::update_metric_registers() {
  registers_[(REG_NB_PPM_COHERENCE - REG_NB_BASE) / 4] =
      float_to_fixed(telemetry_.ppm_coherence);
  registers_[(REG_NB_CONSCIOUSNESS - REG_NB_BASE) / 4] =
      float_to_fixed(telemetry_.consciousness_level);
  registers_[(REG_NB_ATTENTION_STI - REG_NB_BASE) / 4] =
      float_to_fixed(telemetry_.attention_sti);
  registers_[(REG_NB_ATTENTION_LTI - REG_NB_BASE) / 4] =
      float_to_fixed(telemetry_.attention_lti);
  registers_[(REG_NB_TC_PHASE - REG_NB_BASE) / 4] =
      float_to_fixed(telemetry_.tc_phase);
  registers_[(REG_NB_CYCLE_COUNT - REG_NB_BASE) / 4] =
      static_cast<uint32_t>(telemetry_.cycle_count);
  registers_[(REG_NB_PERF_CYCLES_SEC - REG_NB_BASE) / 4] =
      float_to_fixed(telemetry_.cycles_per_second);
}

void NanoBrainCoprocessor::clear_registers() { registers_.fill(0); }

uint32_t NanoBrainCoprocessor::get_config_flags() const {
  uint32_t flags = 0;
  if (config_.enable_ppm)
    flags |= CFG_ENABLE_PPM;
  if (config_.enable_time_crystal)
    flags |= CFG_ENABLE_TC;
  if (config_.enable_ecan)
    flags |= CFG_ENABLE_ECAN;
  if (config_.enable_pln)
    flags |= CFG_ENABLE_PLN;
  if (config_.enable_metacognitive)
    flags |= CFG_ENABLE_META;
  return flags;
}

void NanoBrainCoprocessor::apply_config_flags(uint32_t flags) {
  config_.enable_ppm = (flags & CFG_ENABLE_PPM) != 0;
  config_.enable_time_crystal = (flags & CFG_ENABLE_TC) != 0;
  config_.enable_ecan = (flags & CFG_ENABLE_ECAN) != 0;
  config_.enable_pln = (flags & CFG_ENABLE_PLN) != 0;
  config_.enable_metacognitive = (flags & CFG_ENABLE_META) != 0;

  registers_[(REG_NB_CONFIG_FLAGS - REG_NB_BASE) / 4] = flags;
}

// ================================================================
// Utility Functions
// ================================================================

std::string get_register_name(uint64_t addr) {
  switch (addr) {
  case REG_NB_CMD:
    return "CMD";
  case REG_NB_STATUS:
    return "STATUS";
  case REG_NB_PPM_COHERENCE:
    return "PPM_COHERENCE";
  case REG_NB_CONSCIOUSNESS:
    return "CONSCIOUSNESS";
  case REG_NB_ATTENTION_STI:
    return "ATTENTION_STI";
  case REG_NB_ATTENTION_LTI:
    return "ATTENTION_LTI";
  case REG_NB_TC_PHASE:
    return "TC_PHASE";
  case REG_NB_CYCLE_COUNT:
    return "CYCLE_COUNT";
  case REG_NB_REASONING_CONF:
    return "REASONING_CONF";
  case REG_NB_INFERENCE_COUNT:
    return "INFERENCE_COUNT";
  case REG_NB_CONFIG_FLAGS:
    return "CONFIG_FLAGS";
  case REG_NB_TC_DIMENSIONS:
    return "TC_DIMENSIONS";
  case REG_NB_REASONING_DEPTH:
    return "REASONING_DEPTH";
  case REG_NB_ERROR:
    return "ERROR";
  case REG_NB_PERF_CYCLES_SEC:
    return "PERF_CYCLES_SEC";
  case REG_NB_PERF_ATOMS:
    return "PERF_ATOMS";
  default:
    return "UNKNOWN";
  }
}

void print_register_map() {
  std::cout << "=== NanoBrain NPU Register Map ===" << std::endl;
  std::cout << "Base: 0x" << std::hex << REG_NB_BASE << std::dec << std::endl;
  std::cout << std::endl;

  std::cout << "Control Registers:" << std::endl;
  std::cout << "  0x00: CMD            - Command register" << std::endl;
  std::cout << "  0x04: STATUS         - Status flags" << std::endl;
  std::cout << std::endl;

  std::cout << "Metric Registers (read-only):" << std::endl;
  std::cout << "  0x08: PPM_COHERENCE  - Phase Prime Metric" << std::endl;
  std::cout << "  0x0C: CONSCIOUSNESS  - Emergence level" << std::endl;
  std::cout << "  0x10: ATTENTION_STI  - Short-term importance" << std::endl;
  std::cout << "  0x14: ATTENTION_LTI  - Long-term importance" << std::endl;
  std::cout << "  0x18: TC_PHASE       - Time crystal phase" << std::endl;
  std::cout << "  0x1C: CYCLE_COUNT    - Processing cycles" << std::endl;
  std::cout << std::endl;

  std::cout << "Config Registers (read/write):" << std::endl;
  std::cout << "  0x28: CONFIG_FLAGS   - Enable flags" << std::endl;
  std::cout << "  0x2C: TC_DIMENSIONS  - Time crystal dims" << std::endl;
  std::cout << "  0x30: REASONING_DEPTH- Max reasoning depth" << std::endl;
}
