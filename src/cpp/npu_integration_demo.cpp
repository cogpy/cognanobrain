#include "nanobrain_npu_bridge.h"
#include "nanobrain_ontogenesis.h"
#include <iomanip>
#include <iostream>

/**
 * NPU-NanoBrain Integration Demo
 *
 * Demonstrates:
 * - Token enhancement with PPM coherence
 * - Consciousness-aware generation
 * - Entelechy (vital actualization) metrics
 * - Genetic evolution of NPU configurations
 */

void print_header(const std::string &title) {
  std::cout << "\n" << std::string(60, '=') << "\n";
  std::cout << title << "\n";
  std::cout << std::string(60, '=') << "\n";
}

void print_section(const std::string &title) {
  std::cout << "\n--- " << title << " ---\n";
}

void demo_token_enhancement(NanoBrainNPUBridge &bridge) {
  print_header("Token Enhancement Demo");

  std::cout << "Enhancing tokens with NanoBrain cognitive metrics...\n\n";

  // Simulate token IDs (would come from LLM tokenizer)
  std::vector<int32_t> token_ids = {42, 100, 7, 11, 256, 1024, 3};
  std::vector<std::string> token_texts = {"the",   "world",  "is", "a",
                                          "dream", "within", "."};

  std::cout << std::setw(10) << "Token" << std::setw(8) << "ID" << std::setw(12)
            << "PPM Coh" << std::setw(12) << "Consc" << std::setw(12) << "Attn"
            << std::setw(12) << "Enhance"
            << "\n";
  std::cout << std::string(66, '-') << "\n";

  for (size_t i = 0; i < token_ids.size(); ++i) {
    EnhancedToken token = bridge.enhance_token(token_ids[i], token_texts[i]);

    std::cout << std::setw(10) << token.token_text << std::setw(8)
              << token.token_id << std::setw(12) << std::fixed
              << std::setprecision(3) << token.ppm_coherence << std::setw(12)
              << token.consciousness_boost << std::setw(12)
              << token.attention_score << std::setw(12)
              << token.enhancement_factor << "\n";

    // Simulate token generation
    bridge.process_generated_token(token);
  }

  // Show metrics
  print_section("Enhancement Metrics");
  auto metrics = bridge.get_metrics();
  std::cout << "Tokens Enhanced: " << metrics.tokens_enhanced << "\n";
  std::cout << "Tokens Boosted: " << metrics.tokens_boosted << "\n";
  std::cout << "Avg Enhancement Factor: " << std::fixed << std::setprecision(3)
            << metrics.avg_enhancement_factor << "\n";
  std::cout << "Avg PPM Coherence: " << metrics.avg_ppm_coherence << "\n";
}

void demo_logit_modulation(NanoBrainNPUBridge &bridge) {
  print_header("Logit Modulation Demo");

  std::cout << "Modulating LLM logits with consciousness/coherence...\n\n";

  // Simulate logits (would come from LLM)
  std::vector<float> logits = {-1.2f, 0.5f,  2.1f, -0.3f, 1.8f,
                               0.2f,  -0.5f, 3.2f, 0.1f,  -1.1f};

  std::cout << "Original Logits:\n  ";
  for (float l : logits) {
    std::cout << std::setw(8) << std::fixed << std::setprecision(2) << l;
  }
  std::cout << "\n";

  // Modulate
  auto modulated = bridge.modulate_logits(logits);

  std::cout << "Modulated Logits:\n  ";
  for (float l : modulated) {
    std::cout << std::setw(8) << std::fixed << std::setprecision(2) << l;
  }
  std::cout << "\n";

  // Show changes
  std::cout << "Change (%):\n  ";
  for (size_t i = 0; i < logits.size(); ++i) {
    float change =
        ((modulated[i] - logits[i]) / std::abs(logits[i] + 0.001f)) * 100.0f;
    std::cout << std::setw(8) << std::fixed << std::setprecision(1) << change;
  }
  std::cout << "\n";
}

void demo_entelechy_assessment(NanoBrainNPUBridge &bridge) {
  print_header("Entelechy (Vital Actualization) Assessment");

  auto entelechy = bridge.get_entelechy();
  float fitness = bridge.calculate_entelechy_fitness();

  std::cout << "Five Dimensions of Actualization:\n\n";

  auto print_bar = [](const std::string &name, float value) {
    int bars = static_cast<int>(value * 30);
    std::cout << "  " << std::setw(15) << name << " [";
    for (int i = 0; i < 30; ++i) {
      std::cout << (i < bars ? "█" : "░");
    }
    std::cout << "] " << std::fixed << std::setprecision(2) << (value * 100.0f)
              << "%\n";
  };

  print_bar("Ontological", entelechy.ontological);
  print_bar("Teleological", entelechy.teleological);
  print_bar("Cognitive", entelechy.cognitive);
  print_bar("Integrative", entelechy.integrative);
  print_bar("Evolutionary", entelechy.evolutionary);

  std::cout << "\n  Total Fitness: " << std::fixed << std::setprecision(2)
            << (fitness * 100.0f) << "%\n";
  std::cout << "  Development Stage: " << bridge.get_development_stage_string()
            << "\n";
}

void demo_ontogenesis_evolution(UnifiedNanoBrainKernel *kernel) {
  print_header("Ontogenesis - Genetic Evolution Demo");

  std::cout << "Evolving NPU configurations for optimal consciousness...\n\n";

  // Create ontogenesis engine
  EvolutionConfig evo_config = rapid_evolution_config();
  evo_config.population_size = 8;
  evo_config.max_generations = 10;
  OntogenesisEngine engine(evo_config);

  // Initialize population
  auto population = engine.initialize_population(evo_config.population_size);
  std::cout << "Initial Population: " << population.size() << " genomes\n\n";

  // Create fitness function
  auto fitness_fn = OntogenesisEngine::entelechy_fitness(kernel);

  // Show initial best
  for (auto &genome : population) {
    genome.fitness = fitness_fn(genome);
  }
  auto initial_best = engine.get_best_genome(population);
  std::cout << "Initial Best Fitness: " << std::fixed << std::setprecision(4)
            << initial_best.fitness << "\n\n";

  // Evolve
  std::cout << "Evolving...\n";
  std::cout << std::setw(5) << "Gen" << std::setw(12) << "Best" << std::setw(12)
            << "Avg" << std::setw(12) << "Diversity"
            << "\n";
  std::cout << std::string(41, '-') << "\n";

  auto history = engine.evolve_population(population, fitness_fn,
                                          evo_config.max_generations);

  for (const auto &stats : history) {
    std::cout << std::setw(5) << stats.generation << std::setw(12) << std::fixed
              << std::setprecision(4) << stats.best_fitness << std::setw(12)
              << stats.avg_fitness << std::setw(12) << stats.diversity << "\n";
  }

  // Final best
  auto final_best = engine.get_best_genome(population);
  std::cout << "\nFinal Best Genome: " << final_best.id << "\n";
  std::cout << "Final Fitness: " << std::fixed << std::setprecision(4)
            << final_best.fitness << "\n";
  std::cout << "Generation: " << final_best.generation << "\n";

  if (!final_best.lineage.empty()) {
    std::cout << "Lineage: ";
    for (size_t i = 0; i < std::min(size_t(3), final_best.lineage.size());
         ++i) {
      std::cout << final_best.lineage[i] << " -> ";
    }
    std::cout << final_best.id << "\n";
  }

  // Show expressed configuration
  print_section("Evolved Configuration");
  const auto &cfg = final_best.expressed_config;
  std::cout << "Enable PPM Coherence: "
            << (cfg.enable_ppm_coherence ? "Yes" : "No") << "\n";
  std::cout << "Enable Consciousness: "
            << (cfg.enable_consciousness ? "Yes" : "No") << "\n";
  std::cout << "Coherence Weight: " << cfg.coherence_weight << "\n";
  std::cout << "Consciousness Weight: " << cfg.consciousness_weight << "\n";
  std::cout << "Consciousness Threshold: " << cfg.consciousness_threshold
            << "\n";
}

void demo_self_optimization(OntogenesisEngine &engine,
                            UnifiedNanoBrainKernel *kernel) {
  print_header("Self-Optimization Demo");

  std::cout << "Creating and self-optimizing a genome...\n\n";

  // Create initial genome
  NPUGenome genome = engine.create_random_genome();
  auto fitness_fn = OntogenesisEngine::consciousness_fitness(kernel);

  std::cout << "Initial Fitness: " << std::fixed << std::setprecision(4)
            << fitness_fn(genome) << "\n";

  // Self-optimize
  float final_fitness = engine.self_optimize(genome, fitness_fn, 20);

  std::cout << "Final Fitness: " << std::fixed << std::setprecision(4)
            << final_fitness << "\n";
  std::cout << "Improvement: " << std::fixed << std::setprecision(2)
            << ((final_fitness - 0.0f) * 100.0f) << "%\n";
}

int main() {
  std::cout << "NanoBrain-NPU Integration Demo\n";
  std::cout << "Consciousness-Enhanced LLM Inference\n";
  std::cout << std::string(60, '=') << "\n\n";

  // Initialize NanoBrain
  std::cout << "Initializing NanoBrain...\n";
  UnifiedNanoBrainConfig config;
  config.time_crystal_dimensions = 11;
  config.max_reasoning_depth = 5;
  config.attention_mechanism = AttentionMechanism::Hybrid;

  UnifiedNanoBrainKernel kernel(config);
  kernel.initialize();

  // Create initial knowledge
  std::cout << "Creating knowledge base...\n";
  for (int i = 0; i < 15; ++i) {
    kernel.create_atom("ConceptNode", "Concept_" + std::to_string(i),
                       0.6f + 0.03f * i, 0.8f, {2, 3, 5, 7, 11});
  }

  // Run processing cycles
  kernel.run_cycles(20);
  std::cout << "Ran " << kernel.get_cycle_count() << " cognitive cycles\n";

  // Create NPU Bridge
  NPUEnhancementConfig bridge_config = consciousness_focused_config();
  NanoBrainNPUBridge bridge(bridge_config);
  bridge.attach_nanobrain(&kernel);

  std::cout << "Bridge attached: " << (bridge.is_attached() ? "Yes" : "No")
            << "\n";

  // Run demos
  demo_token_enhancement(bridge);
  demo_logit_modulation(bridge);
  demo_entelechy_assessment(bridge);
  demo_ontogenesis_evolution(&kernel);

  OntogenesisEngine engine;
  demo_self_optimization(engine, &kernel);

  // Final summary
  print_header("Summary");
  auto metrics = kernel.get_metrics();
  std::cout << "Total Atoms: " << metrics.total_atoms << "\n";
  std::cout << "Consciousness Emergence: " << std::fixed << std::setprecision(3)
            << metrics.consciousness_emergence << "\n";
  std::cout << "System Coherence: " << metrics.system_coherence << "\n";

  auto bridge_metrics = bridge.get_metrics();
  std::cout << "NPU Tokens Enhanced: " << bridge_metrics.tokens_enhanced
            << "\n";
  std::cout << "Current Consciousness: " << bridge_metrics.current_consciousness
            << "\n";

  // Shutdown
  std::cout << "\nShutting down...\n";
  kernel.shutdown();

  std::cout << "\nNPU-NanoBrain Integration Demo Complete!\n";

  return 0;
}
