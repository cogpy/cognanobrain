#include "nanobrain_npu.h"
#include <iomanip>
#include <iostream>

using namespace nanobrain_npu;

/**
 * NPU Demo - NanoBrain Cognitive Coprocessor
 *
 * Demonstrates:
 * - Hardware-style register access
 * - Cognitive processing cycles
 * - LLM inference augmentation
 * - Telemetry and diagnostics
 */

void print_separator(const std::string &title) {
  std::cout << "\n========================================" << std::endl;
  std::cout << " " << title << std::endl;
  std::cout << "========================================" << std::endl;
}

int main() {
  print_separator("NanoBrain NPU Cognitive Coprocessor Demo");

  std::cout << std::fixed << std::setprecision(4);

  // ================================================================
  // Part 1: Register Map
  // ================================================================
  print_separator("1. Register Map");
  print_register_map();

  // ================================================================
  // Part 2: Initialize Coprocessor
  // ================================================================
  print_separator("2. Coprocessor Initialization");

  NanoBrainCoprocessor npu;

  NPUConfig config;
  config.tc_dimensions = 11;
  config.max_reasoning_depth = 5;
  config.enable_ppm = true;
  config.enable_time_crystal = true;
  config.enable_ecan = true;
  config.enable_pln = true;

  if (!npu.initialize(config)) {
    std::cerr << "Failed to initialize NPU!" << std::endl;
    return 1;
  }

  std::cout << npu.get_status_string() << std::endl;

  // ================================================================
  // Part 3: Register Access
  // ================================================================
  print_separator("3. Hardware-Style Register Access");

  // Read status register
  uint32_t status = npu.read_reg32(REG_NB_STATUS);
  std::cout << "STATUS register: 0x" << std::hex << status << std::dec
            << std::endl;

  if (status & STATUS_INITIALIZED)
    std::cout << "  - INITIALIZED" << std::endl;
  if (status & STATUS_IDLE)
    std::cout << "  - IDLE" << std::endl;

  // Write config register
  uint32_t config_flags = CFG_ENABLE_PPM | CFG_ENABLE_TC | CFG_ENABLE_ECAN;
  npu.write_reg32(REG_NB_CONFIG_FLAGS, config_flags);
  std::cout << "\nWrote CONFIG_FLAGS: 0x" << std::hex << config_flags
            << std::dec << std::endl;

  // Read back
  uint32_t read_back = npu.read_reg32(REG_NB_CONFIG_FLAGS);
  std::cout << "Read back CONFIG_FLAGS: 0x" << std::hex << read_back << std::dec
            << std::endl;

  // ================================================================
  // Part 4: Cognitive Processing
  // ================================================================
  print_separator("4. Cognitive Processing Cycles");

  // Run cycles via register
  std::cout << "Running 100 cognitive cycles..." << std::endl;
  npu.run_cycles(100);

  // Read metric registers
  float coherence = npu.read_metric(REG_NB_PPM_COHERENCE);
  float consciousness = npu.read_metric(REG_NB_CONSCIOUSNESS);
  uint32_t cycles = npu.read_reg32(REG_NB_CYCLE_COUNT);

  std::cout << "Metrics after 100 cycles:" << std::endl;
  std::cout << "  PPM Coherence: " << coherence << std::endl;
  std::cout << "  Consciousness: " << consciousness << std::endl;
  std::cout << "  Cycle count: " << cycles << std::endl;

  // ================================================================
  // Part 5: Atom Creation
  // ================================================================
  print_separator("5. Cognitive Atom Creation");

  auto cat = npu.create_atom("ConceptNode", "Cat", 0.9f, 0.85f);
  auto animal = npu.create_atom("ConceptNode", "Animal", 0.95f, 0.9f);
  auto mammal = npu.create_atom("ConceptNode", "Mammal", 0.9f, 0.88f);

  std::cout << "Created atoms:" << std::endl;
  std::cout << "  " << cat << std::endl;
  std::cout << "  " << animal << std::endl;
  std::cout << "  " << mammal << std::endl;

  uint32_t atom_count = npu.read_reg32(REG_NB_PERF_ATOMS);
  std::cout << "Atom count register: " << atom_count << std::endl;

  // ================================================================
  // Part 6: PPM Coherence
  // ================================================================
  print_separator("6. Phase Prime Metric Coherence");

  std::vector<std::vector<int>> prime_sets = {
      {2, 3, 5}, {2, 3, 5, 7, 11}, {2, 3, 5, 7, 11, 13, 17}};

  for (const auto &primes : prime_sets) {
    float coh = npu.compute_coherence(primes);
    std::cout << "Coherence [";
    for (size_t i = 0; i < primes.size(); i++) {
      std::cout << primes[i];
      if (i < primes.size() - 1)
        std::cout << ",";
    }
    std::cout << "]: " << coh << std::endl;
  }

  // Check coherence status bit
  status = npu.read_reg32(REG_NB_STATUS);
  std::cout << "Coherent status: "
            << ((status & STATUS_COHERENT) ? "Yes" : "No") << std::endl;

  // ================================================================
  // Part 7: LLM Augmentation
  // ================================================================
  print_separator("7. LLM Inference Augmentation");

  std::string prompt = "What is the relationship between cats and animals?";
  std::cout << "Prompt: \"" << prompt << "\"" << std::endl;

  auto augment = npu.augment_inference(prompt);
  std::cout << "\nAugmentation result:" << std::endl;
  std::cout << "  Coherence boost: " << augment.coherence_boost << std::endl;
  std::cout << "  Attention modifier: " << augment.attention_modifier
            << std::endl;
  std::cout << "  Confidence: " << augment.confidence << std::endl;
  std::cout << "  Reasoning: " << augment.reasoning_chain << std::endl;
  std::cout << "  Injected atoms: " << augment.injected_atoms.size()
            << std::endl;

  // Attention weights
  std::vector<std::string> tokens = {"What", "is", "the", "relationship"};
  auto weights = npu.get_attention_weights(tokens);
  std::cout << "\nAttention weights:" << std::endl;
  for (size_t i = 0; i < tokens.size(); i++) {
    std::cout << "  \"" << tokens[i] << "\": " << weights[i] << std::endl;
  }

  // ================================================================
  // Part 8: Telemetry & Diagnostics
  // ================================================================
  print_separator("8. Telemetry & Diagnostics");

  auto telemetry = npu.get_telemetry();
  std::cout << "Telemetry:" << std::endl;
  std::cout << "  Cycles: " << telemetry.cycle_count << std::endl;
  std::cout << "  Atoms: " << telemetry.atom_count << std::endl;
  std::cout << "  Inferences: " << telemetry.inference_count << std::endl;
  std::cout << "  Cycles/sec: " << telemetry.cycles_per_second << std::endl;

  std::cout << "\nFull diagnostics:" << std::endl;
  std::cout << npu.get_diagnostics();

  // ================================================================
  // Part 9: Command Interface
  // ================================================================
  print_separator("9. Command Interface");

  // Execute single cycle via command
  std::cout << "Executing CMD_RUN_CYCLE..." << std::endl;
  npu.execute_command(CMD_RUN_CYCLE);

  uint32_t new_cycles = npu.read_reg32(REG_NB_CYCLE_COUNT);
  std::cout << "Cycle count after command: " << new_cycles << std::endl;

  // ================================================================
  // Part 10: Shutdown
  // ================================================================
  print_separator("10. Shutdown");

  npu.execute_command(CMD_SHUTDOWN);
  std::cout << "NPU shutdown complete" << std::endl;
  std::cout << npu.get_status_string() << std::endl;

  // ================================================================
  // Summary
  // ================================================================
  print_separator("Summary");

  std::cout << "NanoBrain NPU Demo Complete:" << std::endl;
  std::cout << "  ✓ Register map display" << std::endl;
  std::cout << "  ✓ Hardware-style register access" << std::endl;
  std::cout << "  ✓ Cognitive processing cycles" << std::endl;
  std::cout << "  ✓ Atom creation via coprocessor" << std::endl;
  std::cout << "  ✓ PPM coherence calculation" << std::endl;
  std::cout << "  ✓ LLM inference augmentation" << std::endl;
  std::cout << "  ✓ Telemetry and diagnostics" << std::endl;
  std::cout << "  ✓ Command interface" << std::endl;

  std::cout << "\n========================================" << std::endl;
  std::cout << " NPU Cognitive Coprocessor Demo Complete" << std::endl;
  std::cout << "========================================" << std::endl;

  return 0;
}
