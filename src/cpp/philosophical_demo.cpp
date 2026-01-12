/**
 * @file philosophical_demo.cpp
 * @brief Chapter 1 Demonstration: Philosophical Transformation Engine
 *
 * This demo showcases the NanoBrain philosophical transformation capabilities:
 * - Brain model comparison (10 models from Hodgkin-Huxley to Time Crystal)
 * - Consciousness emergence calculation with configurable thresholds
 * - Research field integration (10 fields)
 * - Linguistic wheel operations for space/time/imaginary mapping
 * - Extended philosophical knowledge base generation
 */

#include "nanobrain_philosophical.h"
#include <iomanip>
#include <iostream>

void print_separator(const std::string &title) {
  std::cout << "\n";
  std::cout << "════════════════════════════════════════════════════════════\n";
  std::cout << "  " << title << "\n";
  std::cout << "════════════════════════════════════════════════════════════\n";
}

void demo_brain_models() {
  print_separator("Brain Model Comparison");

  PhilosophicalTransformationEngine engine;
  auto models = engine.get_all_model_info();

  std::cout << "\n  Model                    | Conscious | Temporal | Turing | "
               "Primes\n";
  std::cout
      << "  "
         "─────────────────────────┼───────────┼──────────┼────────┼───────\n";

  for (const auto &m : models) {
    std::cout << "  " << std::left << std::setw(24) << m.name << " | "
              << std::fixed << std::setprecision(2) << m.consciousness_score
              << "     | " << m.temporal_resolution << "    | "
              << m.turing_alignment << "  | " << m.prime_signature.size()
              << "\n";
  }

  // Compare models
  std::cout << "\n  Model Comparisons (positive = first better):\n";
  std::cout << "    Time Crystal vs Hodgkin-Huxley: "
            << engine.compare_models(BrainModel::TimeCrystal,
                                     BrainModel::HodgkinHuxley)
            << "\n";
  std::cout << "    Time Crystal vs IIT:           "
            << engine.compare_models(BrainModel::TimeCrystal,
                                     BrainModel::IntegratedInformation)
            << "\n";
  std::cout << "    Time Crystal vs Quantum:       "
            << engine.compare_models(BrainModel::TimeCrystal,
                                     BrainModel::Quantum)
            << "\n";
}

void demo_consciousness_metrics() {
  print_separator("Consciousness Emergence Metrics");

  ConsciousnessConfig config;
  config.consciousness_threshold = 0.7f;
  config.self_awareness_threshold = 0.5f;
  config.prime_symmetry_depth = 15;
  config.active_model = BrainModel::TimeCrystal;

  PhilosophicalTransformationEngine engine(config);

  std::cout << "\n  Configuration:\n";
  std::cout << "    Consciousness threshold:  "
            << config.consciousness_threshold << "\n";
  std::cout << "    Self-awareness threshold: "
            << config.self_awareness_threshold << "\n";
  std::cout << "    Prime symmetry depth:     " << config.prime_symmetry_depth
            << "\n";
  std::cout << "    Active model:             Time Crystal\n";

  std::cout
      << "\n  Emergence Calculations (coherence, awareness, integration):\n";

  struct TestCase {
    float coherence, awareness, integration;
    std::string description;
  };

  std::vector<TestCase> cases = {
      {0.9f, 0.8f, 0.85f, "High all metrics"},
      {0.6f, 0.7f, 0.5f, "Medium-high"},
      {0.3f, 0.4f, 0.3f, "Low metrics"},
      {0.8f, 0.2f, 0.9f, "Low awareness"},
      {0.5f, 0.9f, 0.5f, "High awareness only"},
  };

  for (const auto &c : cases) {
    float emergence = engine.calculate_consciousness_emergence(
        c.coherence, c.awareness, c.integration);
    bool is_conscious = engine.is_conscious(emergence);
    bool is_aware = engine.is_self_aware(c.awareness);

    std::cout << "    " << std::left << std::setw(20) << c.description
              << " → Emergence: " << std::fixed << std::setprecision(3)
              << emergence << " ["
              << (is_conscious ? "CONSCIOUS" : "not conscious") << "]"
              << " [" << (is_aware ? "AWARE" : "not aware") << "]\n";
  }
}

void demo_research_fields() {
  print_separator("Research Field Integration");

  PhilosophicalTransformationEngine engine;

  std::cout << "\n  Research Field Weights:\n";
  for (int i = 0; i < 10; ++i) {
    ResearchField field = static_cast<ResearchField>(i);
    std::cout << "    " << std::left << std::setw(20)
              << research_field_name(field) << ": " << std::fixed
              << std::setprecision(2) << engine.get_field_weight(field) << "\n";
  }

  // Generate field integration KB
  auto kb = engine.generate_field_integration_kb();
  std::cout << "\n  Generated KB: " << kb.description << "\n";
  std::cout << "  Total expressions: " << kb.expressions.size() << "\n";
}

void demo_linguistic_wheel() {
  print_separator("Linguistic Wheel Operations");

  PhilosophicalTransformationEngine engine;

  // Create wheels from concepts
  LinguisticWheel consciousness_wheel =
      engine.create_wheel_from_concept("Consciousness");
  LinguisticWheel cognition_wheel =
      engine.create_wheel_from_concept("Cognition");

  std::cout << "\n  'Consciousness' Wheel:\n";
  std::cout << "    Space:     (" << consciousness_wheel.space_x << ", "
            << consciousness_wheel.space_y << ", "
            << consciousness_wheel.space_z << ")\n";
  std::cout << "    Time:      phase=" << consciousness_wheel.time_phase
            << " freq=" << consciousness_wheel.time_frequency << "\n";
  std::cout << "    Imaginary: (" << consciousness_wheel.imaginary_1 << ", "
            << consciousness_wheel.imaginary_2 << ", "
            << consciousness_wheel.imaginary_3 << ", "
            << consciousness_wheel.imaginary_4 << ")\n";
  std::cout << "    Primes:    [";
  for (size_t i = 0; i < consciousness_wheel.prime_encoding.size(); ++i) {
    if (i > 0)
      std::cout << ", ";
    std::cout << consciousness_wheel.prime_encoding[i];
  }
  std::cout << "]\n";
  std::cout << "    Coherence: " << consciousness_wheel.compute_coherence()
            << "\n";

  // 11D vector
  auto vec = consciousness_wheel.to_11d_vector();
  std::cout << "    11D Vector: [";
  for (size_t i = 0; i < 11; ++i) {
    if (i > 0)
      std::cout << ", ";
    std::cout << std::fixed << std::setprecision(2) << vec[i];
  }
  std::cout << "]\n";

  // Translate between wheels
  std::cout << "\n  Wheel Translation (50% interpolation):\n";
  LinguisticWheel mid_wheel =
      engine.translate_wheel(consciousness_wheel, cognition_wheel, 0.5f);
  std::cout << "    Midpoint Space: (" << mid_wheel.space_x << ", "
            << mid_wheel.space_y << ", " << mid_wheel.space_z << ")\n";

  // Compose wheels
  std::vector<LinguisticWheel> wheels = {consciousness_wheel, cognition_wheel};
  LinguisticWheel composed = engine.compose_wheels(wheels);
  std::cout << "\n  Composed Wheel (2 inputs):\n";
  std::cout << "    Space: (" << composed.space_x << ", " << composed.space_y
            << ", " << composed.space_z << ")\n";
  std::cout << "    Prime encoding: [";
  for (size_t i = 0; i < composed.prime_encoding.size(); ++i) {
    if (i > 0)
      std::cout << ", ";
    std::cout << composed.prime_encoding[i];
  }
  std::cout << "]\n";
}

void demo_knowledge_base_generation() {
  print_separator("Extended Philosophical Knowledge Base");

  ConsciousnessConfig config;
  config.consciousness_threshold = 0.7f;
  config.prime_symmetry_depth = 15;
  config.active_model = BrainModel::TimeCrystal;

  PhilosophicalTransformationEngine engine(config);
  AtomeseParser parser;

  auto kb = engine.generate_extended_philosophical_kb();

  std::cout << "\n  " << kb.description << "\n";
  std::cout << "  Total expressions: " << kb.expressions.size() << "\n\n";

  // Show first few expressions
  std::cout << "  Sample expressions (first 5):\n";
  int count = 0;
  for (const auto &expr : kb.expressions) {
    if (count >= 5)
      break;
    std::cout << "    " << parser.to_sexpr_string(*expr) << "\n";
    count++;
  }
  std::cout << "    ...\n";
}

void demo_config_persistence() {
  print_separator("Configuration Persistence");

  ConsciousnessConfig config;
  config.consciousness_threshold = 0.75f;
  config.self_awareness_threshold = 0.55f;
  config.meta_cognition_threshold = 0.35f;
  config.prime_symmetry_depth = 12;
  config.arrangement_creativity = 0.7f;
  config.active_model = BrainModel::TimeCrystal;

  PhilosophicalTransformationEngine engine(config);

  // Save to file
  std::string filename = "philosophical_config.json";
  if (engine.save_config_to_json(filename)) {
    std::cout << "\n  Configuration saved to: " << filename << "\n";

    // Load it back
    PhilosophicalTransformationEngine engine2;
    if (engine2.load_config_from_json(filename)) {
      std::cout << "  Configuration loaded successfully!\n";
      const auto &loaded = engine2.get_config();
      std::cout << "  Loaded consciousness_threshold: "
                << loaded.consciousness_threshold << "\n";
      std::cout << "  Loaded prime_symmetry_depth: "
                << loaded.prime_symmetry_depth << "\n";
    }
  } else {
    std::cout << "\n  (Skipping file persistence demo)\n";
  }
}

int main() {
  std::cout << "\n";
  std::cout
      << "╔════════════════════════════════════════════════════════════╗\n";
  std::cout
      << "║    NANOBRAIN CHAPTER 1: PHILOSOPHICAL TRANSFORMATION       ║\n";
  std::cout
      << "║    Reverse Engineering Consciousness                       ║\n";
  std::cout
      << "╚════════════════════════════════════════════════════════════╝\n";

  demo_brain_models();
  demo_consciousness_metrics();
  demo_research_fields();
  demo_linguistic_wheel();
  demo_knowledge_base_generation();
  demo_config_persistence();

  print_separator("Demo Complete");
  std::cout << "\n  Chapter 1 philosophical foundations implemented!\n";
  std::cout << "  Key concepts:\n";
  std::cout << "    • 10 brain models compared (Time Crystal optimal)\n";
  std::cout << "    • Consciousness emergence quantified with thresholds\n";
  std::cout << "    • 10 research fields integrated in knowledge base\n";
  std::cout << "    • Linguistic wheel maps concepts to 11D space\n";
  std::cout << "\n  Ready for Chapter 2: Fractal Tape & GML\n\n";

  return 0;
}
