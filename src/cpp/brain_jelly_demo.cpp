#include "nanobrain_brain_jelly.h"
#include "nanobrain_time_crystal.h"
#include "nanobrain_unified.h"
#include <iomanip>
#include <iostream>

/**
 * Brain Jelly Demo Program
 *
 * Demonstrates Chapter 9 features:
 * - Bio-morphic device registry with 17 device types
 * - Fractal condensation engine
 * - Brain jelly EEG signal generation
 * - Cortical pen freezer
 * - Humanoid avatar 11D sensing
 */

void print_separator(const std::string &title) {
  std::cout << "\n" << std::string(60, '=') << "\n";
  std::cout << "  " << title << "\n";
  std::cout << std::string(60, '=') << "\n";
}

void print_device_type_name(BioMorphicDeviceType type) {
  std::cout << get_device_type_name(type);
}

void demo_bio_morphic_devices(NanoBrainKernel *kernel) {
  print_separator("Bio-Morphic Device Registry Demo");

  BioMorphicRegistryConfig config;
  config.max_devices = 256;
  config.communication_rate = 0.15f;
  config.enable_collective_response = true;

  BioMorphicDeviceRegistry registry(kernel, config);

  std::cout << "\nInitializing all 17 bio-morphic device types...\n";
  registry.initialize_full_registry();

  std::cout << "Created " << registry.device_count() << " devices\n\n";

  // List all device types
  std::cout << "Device Types:\n";
  for (int i = 0; i < 17; ++i) {
    auto type = static_cast<BioMorphicDeviceType>(i);
    int count = registry.count_device_type(type);
    std::cout << "  [" << std::setw(2) << i << "] " << std::setw(18)
              << std::left << get_device_type_name(type)
              << " - Count: " << count << "\n";
  }

  // Simulate some processing cycles
  std::cout << "\nRunning 10 processing cycles...\n";
  for (int cycle = 0; cycle < 10; ++cycle) {
    registry.update_all_devices(0.001f); // 1ms timestep
    registry.propagate_signals();
  }

  // Compute collective response
  auto response = registry.compute_collective_response();
  std::cout << "\nCollective Response:\n";
  std::cout << "  Synchronization Index:   " << std::fixed
            << std::setprecision(4) << response.synchronization_index << "\n";
  std::cout << "  Collective Activation:   " << response.collective_activation
            << "\n";
  std::cout << "  Emergence Level:         " << response.emergence_level
            << "\n";

  // Show harmonic modes
  std::cout << "  Harmonic Modes:          [";
  for (size_t i = 0; i < response.harmonic_modes.size(); ++i) {
    std::cout << response.harmonic_modes[i];
    if (i < response.harmonic_modes.size() - 1)
      std::cout << ", ";
  }
  std::cout << "] Hz\n";
}

void demo_fractal_condensation(NanoBrainKernel *kernel) {
  print_separator("Fractal Condensation Demo");

  FractalCondensationConfig config;
  config.max_condensation_points = 32;
  config.condensation_order = 7;
  config.prime_threshold = 0.2f;

  FractalCondensation engine(kernel, config);

  std::cout << "\nSetting prime pattern: [2, 3, 5, 7, 11, 13]\n";
  engine.set_prime_pattern({2, 3, 5, 7, 11, 13});

  std::cout << "Running condense_everywhere()...\n";
  engine.condense_everywhere();

  auto points = engine.get_points();
  std::cout << "Generated " << points.size() << " condensation points\n";

  // Show first few points
  std::cout << "\nFirst 5 Condensation Points:\n";
  for (size_t i = 0; i < std::min(size_t(5), points.size()); ++i) {
    const auto &pt = points[i];
    std::cout << "  Point " << i << ":\n";
    std::cout << "    Position (11D): [";
    for (int d = 0; d < 3; ++d) { // Show first 3 dims
      std::cout << std::fixed << std::setprecision(3) << pt.position[d];
      if (d < 2)
        std::cout << ", ";
    }
    std::cout << ", ...]\n";
    std::cout << "    Strength:       " << pt.condensation_strength << "\n";
    std::cout << "    Resonance:      " << pt.resonance_frequency << " Hz\n";
    std::cout << "    Order:          " << pt.condensation_order << "\n";
  }

  // Compute field
  auto field = engine.compute_field();
  std::cout << "\nCondensation Field:\n";
  std::cout << "  Total Condensation: " << field.total_condensation << "\n";
  std::cout << "  Field Coherence:    " << field.field_coherence << "\n";
  std::cout << "  Pattern Coherence:  " << engine.compute_pattern_coherence()
            << "\n";
}

void demo_brain_jelly(NanoBrainKernel *kernel) {
  print_separator("Brain Jelly Simulator Demo");

  BrainJellyConfig config;
  config.megamer_chain_length = 32;
  config.base_frequency = 10.0f; // Alpha rhythm
  config.eeg_channels = 19;
  config.sample_rate = 256.0f;

  BrainJellySimulator simulator(kernel, config);

  std::cout << "\nMegamer Chain: " << simulator.get_chain().size()
            << " units\n";
  std::cout << "EEG Sample Rate: " << config.sample_rate << " Hz\n";
  std::cout << "Channels: " << config.eeg_channels << " (10-20 montage)\n";

  // Update chain
  std::cout << "\nUpdating chain for 100ms...\n";
  for (int i = 0; i < 100; ++i) {
    simulator.update_chain(0.001f);
  }

  std::cout << "Total Resonance: " << std::fixed << std::setprecision(4)
            << simulator.get_total_resonance() << "\n";
  std::cout << "Consciousness Index: " << simulator.get_consciousness_index()
            << "\n";

  // Generate EEG signal
  std::cout << "\nGenerating EEG signal...\n";
  auto eeg = simulator.generate_eeg_signal(0.1f);

  std::cout << "  Timestamp:         " << eeg.timestamp << "s\n";
  std::cout << "  Dominant Band:     ";
  switch (eeg.dominant_band) {
  case EEGBand::Delta:
    std::cout << "Delta";
    break;
  case EEGBand::Theta:
    std::cout << "Theta";
    break;
  case EEGBand::Alpha:
    std::cout << "Alpha";
    break;
  case EEGBand::Beta:
    std::cout << "Beta";
    break;
  case EEGBand::Gamma:
    std::cout << "Gamma";
    break;
  }
  std::cout << "\n";

  std::cout << "  Band Powers:\n";
  const char *band_names[] = {"Delta", "Theta", "Alpha", "Beta", "Gamma"};
  for (int b = 0; b < 5; ++b) {
    std::cout << "    " << std::setw(6) << band_names[b] << ": "
              << std::setprecision(4) << eeg.band_power[b] << "\n";
  }

  std::cout << "  Consciousness: " << eeg.consciousness_index << "\n";

  // Show first few channel values
  std::cout << "\n  Channel Values (first 5):\n";
  for (int ch = 0; ch < 5 && ch < static_cast<int>(eeg.channel_values.size());
       ++ch) {
    std::cout << "    Ch" << std::setw(2) << ch << ": " << std::setprecision(2)
              << eeg.channel_values[ch] << " μV\n";
  }

  // Prime pattern writing
  std::cout << "\nWriting prime pattern [2, 3, 5, 7, 11]...\n";
  simulator.write_prime_pattern({2, 3, 5, 7, 11});
  auto read_pattern = simulator.read_prime_pattern();
  std::cout << "Read back: [";
  for (size_t i = 0; i < read_pattern.size(); ++i) {
    std::cout << read_pattern[i];
    if (i < read_pattern.size() - 1)
      std::cout << ", ";
  }
  std::cout << "]\n";
}

void demo_cortical_pen_freezer(NanoBrainKernel *kernel,
                               TimeCrystalKernel *time_crystal) {
  print_separator("Cortical Pen Freezer Demo");

  CorticalPenConfig config;
  config.max_capsules = 64;
  config.freeze_threshold = 0.7f;
  config.time_crystal_resolution = 11;

  CorticalPenFreezer freezer(kernel, time_crystal, config);

  std::cout << "\nFreezing cognitive dynamics...\n";

  // Create some test capsules
  std::string cap1 =
      freezer.freeze_dynamics(kernel->create_tensor(16), {"atom_1", "atom_2"});
  std::string cap2 = freezer.freeze_dynamics(kernel->create_tensor(32),
                                             {"atom_3", "atom_4", "atom_5"});

  freezer.create_problem_capsule("test_problem");

  std::cout << "Created " << freezer.capsule_count() << " capsules\n";

  auto ids = freezer.get_all_capsule_ids();
  std::cout << "\nCapsule IDs:\n";
  for (const auto &id : ids) {
    auto *capsule = freezer.get_capsule(id);
    if (capsule) {
      std::cout << "  - " << id << "\n";
      std::cout << "    Complexity:   " << capsule->complexity_order << "\n";
      std::cout << "    Anchor:       " << capsule->temporal_anchor << "\n";

      auto sig = freezer.get_capsule_signature(id);
      std::cout << "    Signature:    [";
      for (size_t i = 0; i < std::min(size_t(5), sig.size()); ++i) {
        std::cout << sig[i];
        if (i < std::min(size_t(5), sig.size()) - 1)
          std::cout << ", ";
      }
      if (sig.size() > 5)
        std::cout << ", ...";
      std::cout << "]\n";
    }
  }

  // Thaw a capsule
  std::cout << "\nThawing capsule '" << cap1 << "'...\n";
  auto *thawed = freezer.thaw_capsule(cap1);
  std::cout << "Thaw result: " << (thawed ? "Success" : "Failed") << "\n";
}

void demo_humanoid_avatar(NanoBrainKernel *kernel,
                          TimeCrystalKernel *time_crystal,
                          BrainJellySimulator *brain_jelly) {
  print_separator("Humanoid Avatar Interface Demo");

  HumanoidAvatarConfig config;
  config.sensor_count = 32;
  config.motor_count = 16;
  config.enable_11d_sensing = true;
  config.enable_time_crystal_net = true;

  HumanoidAvatarInterface avatar(kernel, time_crystal, brain_jelly, config);

  std::cout << "\nSensor Count: " << config.sensor_count << "\n";
  std::cout << "Motor Count:  " << config.motor_count << "\n";

  // Create some 11D signals
  std::vector<Signal11D> signals;
  for (int i = 0; i < 5; ++i) {
    Signal11D sig;
    for (int d = 0; d < 11; ++d) {
      sig.dimensions[d] = static_cast<float>(i + d) * 0.1f;
    }
    sig.signal_strength = 0.5f + i * 0.1f;
    sig.timestamp = 1000 + i;
    sig.source_sensor = "sensor_" + std::to_string(i);
    signals.push_back(sig);
  }

  std::cout << "\nSensing " << signals.size() << " 11D signals...\n";
  avatar.sense_11d_signals(signals);

  // Process sensor triad
  avatar.process_sensor_triad();

  auto triad = avatar.get_triad_state();
  std::cout << "\nSensor Triad State:\n";
  std::cout << "  Sensory Integration: " << triad.sensory_integration << "\n";
  std::cout << "  Jelly Resonance:     " << triad.jelly_resonance << "\n";
  std::cout << "  Motor Coordination:  " << triad.motor_coordination << "\n";
  std::cout << "  Consciousness Level: " << triad.consciousness_level << "\n";

  // Get integrated signal
  auto fused = avatar.get_integrated_signal();
  std::cout << "\nFused 11D Signal:\n";
  std::cout << "  Dimensions (first 5): [";
  for (int d = 0; d < 5; ++d) {
    std::cout << std::fixed << std::setprecision(3) << fused.dimensions[d];
    if (d < 4)
      std::cout << ", ";
  }
  std::cout << ", ...]\n";
  std::cout << "  Signal Strength: " << fused.signal_strength << "\n";

  // Set motor targets and get commands
  std::vector<float> targets(16, 0.5f);
  targets[0] = 1.0f;
  targets[5] = 0.8f;
  avatar.set_motor_targets(targets);

  // Update neural state
  avatar.update_neural_state(0.01f);

  auto commands = avatar.get_motor_commands();
  std::cout << "\nMotor Commands (first 8):\n  [";
  for (int i = 0; i < 8 && i < static_cast<int>(commands.size()); ++i) {
    std::cout << std::fixed << std::setprecision(3) << commands[i];
    if (i < 7)
      std::cout << ", ";
  }
  std::cout << ", ...]\n";

  // Get metrics
  auto metrics = avatar.get_metrics();
  std::cout << "\nAvatar Metrics:\n";
  std::cout << "  Sensory Bandwidth:   " << metrics.sensory_bandwidth << "\n";
  std::cout << "  Motor Precision:     " << metrics.motor_precision << "\n";
  std::cout << "  Cognitive Load:      " << metrics.cognitive_load << "\n";
  std::cout << "  Embodiment Index:    " << metrics.embodiment_index << "\n";
  std::cout << "  Temporal Coherence:  " << metrics.temporal_coherence << "\n";
}

void demo_integrated_system(NanoBrainKernel *kernel,
                            TimeCrystalKernel *time_crystal) {
  print_separator("Integrated Brain Jelly System Demo");

  IntegratedBrainJellyConfig config;
  config.registry_config.max_devices = 128;
  config.condensation_config.max_condensation_points = 64;
  config.jelly_config.megamer_chain_length = 32;
  config.pen_config.max_capsules = 32;
  config.avatar_config.sensor_count = 16;

  IntegratedBrainJellySystem system(kernel, time_crystal, config);

  std::cout << "\nInitializing integrated system...\n";
  system.initialize();
  std::cout << "System active: " << (system.is_active() ? "Yes" : "No") << "\n";

  // Run some processing cycles
  std::cout << "\nRunning 50 processing cycles...\n";
  for (int i = 0; i < 50; ++i) {
    system.process_cycle(0.001f);
  }

  // Get comprehensive metrics
  auto metrics = system.get_metrics();

  std::cout << "\n" << std::string(40, '-') << "\n";
  std::cout << "Integrated System Metrics:\n";
  std::cout << std::string(40, '-') << "\n";

  std::cout << "\nDevice Metrics:\n";
  std::cout << "  Total Devices:         " << metrics.total_devices << "\n";
  std::cout << "  Collective Activation: " << std::fixed << std::setprecision(4)
            << metrics.collective_activation << "\n";
  std::cout << "  Synchronization Index: " << metrics.synchronization_index
            << "\n";

  std::cout << "\nCondensation Metrics:\n";
  std::cout << "  Condensation Points:   " << metrics.condensation_points
            << "\n";
  std::cout << "  Field Coherence:       " << metrics.field_coherence << "\n";
  std::cout << "  Prime Alignment:       " << metrics.prime_alignment << "\n";

  std::cout << "\nBrain Jelly Metrics:\n";
  std::cout << "  Total Resonance:       " << metrics.total_resonance << "\n";
  std::cout << "  Consciousness Index:   " << metrics.consciousness_index
            << "\n";
  std::cout << "  EEG Alpha Power:       " << metrics.eeg_alpha_power << "\n";

  std::cout << "\nFreezer Metrics:\n";
  std::cout << "  Frozen Capsules:       " << metrics.frozen_capsules << "\n";
  std::cout << "  Average Complexity:    " << metrics.average_complexity
            << "\n";

  std::cout << "\nAvatar Metrics:\n";
  std::cout << "  Embodiment Index:      " << metrics.embodiment_index << "\n";
  std::cout << "  Sensory Bandwidth:     " << metrics.sensory_bandwidth << "\n";
  std::cout << "  Motor Precision:       " << metrics.motor_precision << "\n";

  std::cout << "\nShutting down...\n";
  system.shutdown();
  std::cout << "System active: " << (system.is_active() ? "Yes" : "No") << "\n";
}

int main() {
  std::cout << "\n";
  std::cout << "╔══════════════════════════════════════════════════════════╗\n";
  std::cout << "║     NanoBrain Brain Jelly & Humanoid Avatar Demo         ║\n";
  std::cout << "║                    Chapter 9 Features                    ║\n";
  std::cout << "╚══════════════════════════════════════════════════════════╝\n";

  // Create kernel
  std::cout << "\nInitializing NanoBrain Kernel...\n";
  NanoBrainKernel kernel(1024 * 1024 * 64); // 64MB

  // Create Time Crystal kernel
  TimeCrystalConfig tc_config;
  tc_config.time_crystal_dimensions = 11;
  TimeCrystalKernel time_crystal(tc_config);
  time_crystal.initialize();

  // Create Brain Jelly simulator for avatar demo
  BrainJellyConfig jelly_config;
  jelly_config.megamer_chain_length = 32;
  BrainJellySimulator brain_jelly(&kernel, jelly_config);

  try {
    // Run all demos
    demo_bio_morphic_devices(&kernel);
    demo_fractal_condensation(&kernel);
    demo_brain_jelly(&kernel);
    demo_cortical_pen_freezer(&kernel, &time_crystal);
    demo_humanoid_avatar(&kernel, &time_crystal, &brain_jelly);
    demo_integrated_system(&kernel, &time_crystal);

    print_separator("Demo Complete");
    std::cout << "\nAll Chapter 9 features demonstrated successfully!\n\n";
    std::cout << "Summary:\n";
    std::cout << "  ✓ 17 Bio-morphic device types\n";
    std::cout << "  ✓ Fractal condensation engine\n";
    std::cout << "  ✓ Brain jelly EEG generation\n";
    std::cout << "  ✓ Cortical pen freezer\n";
    std::cout << "  ✓ Humanoid avatar 11D sensing\n";
    std::cout << "  ✓ Integrated brain jelly system\n";
    std::cout << "\n";

  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }

  time_crystal.shutdown();
  return 0;
}
