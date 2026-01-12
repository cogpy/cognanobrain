#ifndef NANOBRAIN_ONTOGENESIS_H
#define NANOBRAIN_ONTOGENESIS_H

/**
 * NanoBrain Ontogenesis
 *
 * Self-generation and evolutionary framework for NPU/NanoBrain systems.
 * Enables systems to:
 * - Generate offspring with inherited traits
 * - Self-optimize through iterative improvement
 * - Reproduce via genetic crossover
 * - Evolve populations toward higher fitness
 */

#include "nanobrain_npu_bridge.h"
#include <array>
#include <functional>
#include <memory>
#include <random>
#include <string>
#include <vector>

// ================================================================
// Genome Structure
// ================================================================

/**
 * Genetic configuration for NPU/NanoBrain system
 */
struct NPUGenome {
  std::string id;                   // Unique genome identifier
  int generation = 0;               // Generation number
  std::vector<std::string> lineage; // Ancestor IDs

  // Genetic trait vectors (each gene 0.0 - 1.0)
  struct GeneticTraits {
    // Ontological genes - structural configuration
    std::vector<float> ontological;

    // Teleological genes - purpose/goal orientation
    std::vector<float> teleological;

    // Cognitive genes - reasoning strategies
    std::vector<float> cognitive;

    // Integrative genes - coherence parameters
    std::vector<float> integrative;

    // Evolutionary genes - growth/adaptation rates
    std::vector<float> evolutionary;
  } genes;

  // Phenotypic expression values
  NPUEnhancementConfig expressed_config;

  // Fitness metrics
  float fitness = 0.0f;
  float age = 0.0f;
  float actualization_level = 0.0f;
};

/**
 * Evolution configuration
 */
struct EvolutionConfig {
  int population_size = 20;
  float mutation_rate = 0.15f;    // Probability of gene mutation
  float mutation_strength = 0.2f; // Magnitude of mutations
  float crossover_rate = 0.8f;    // Probability of crossover
  float elitism_rate = 0.1f;      // Top fraction preserved
  int max_generations = 100;
  float fitness_threshold = 0.9f; // Target fitness

  // Gene vector sizes
  int ontological_genes = 5;
  int teleological_genes = 5;
  int cognitive_genes = 5;
  int integrative_genes = 5;
  int evolutionary_genes = 5;
};

/**
 * Statistics for a generation
 */
struct GenerationStats {
  int generation;
  float best_fitness;
  float avg_fitness;
  float worst_fitness;
  float diversity; // Genetic diversity measure
  std::string best_genome_id;
};

/**
 * Fitness function type
 */
using FitnessFunction = std::function<float(const NPUGenome &)>;

// ================================================================
// Ontogenesis Engine
// ================================================================

/**
 * Engine for self-generation, reproduction, and evolution
 */
class OntogenesisEngine {
public:
  OntogenesisEngine(const EvolutionConfig &config = EvolutionConfig());
  ~OntogenesisEngine();

  // ================================================================
  // Genome Management
  // ================================================================

  /**
   * Create a random initial genome
   */
  NPUGenome create_random_genome();

  /**
   * Create genome from existing configuration
   */
  NPUGenome create_genome_from_config(const NPUEnhancementConfig &config);

  /**
   * Express genome as NPU configuration
   */
  NPUEnhancementConfig express_phenotype(const NPUGenome &genome);

  // ================================================================
  // Self-Generation
  // ================================================================

  /**
   * Generate offspring from single parent (asexual reproduction)
   * @param parent Parent genome
   * @return Child genome with mutations
   */
  NPUGenome self_generate(const NPUGenome &parent);

  /**
   * Self-optimize genome through hill climbing
   * @param genome Genome to optimize (modified in place)
   * @param fitness_fn Fitness evaluation function
   * @param iterations Number of optimization iterations
   * @return Final fitness score
   */
  float self_optimize(NPUGenome &genome, FitnessFunction fitness_fn,
                      int iterations = 10);

  // ================================================================
  // Reproduction
  // ================================================================

  /**
   * Crossover two parent genomes (sexual reproduction)
   * @param parent1 First parent
   * @param parent2 Second parent
   * @return Child genome from crossover
   */
  NPUGenome crossover(const NPUGenome &parent1, const NPUGenome &parent2);

  /**
   * Mutate a genome in place
   * @param genome Genome to mutate
   */
  void mutate(NPUGenome &genome);

  // ================================================================
  // Population Evolution
  // ================================================================

  /**
   * Initialize a random population
   * @param size Population size
   * @return Vector of genomes
   */
  std::vector<NPUGenome> initialize_population(int size);

  /**
   * Evolve population for multiple generations
   * @param population Initial population
   * @param fitness_fn Fitness evaluation function
   * @param generations Number of generations
   * @return Generation statistics history
   */
  std::vector<GenerationStats>
  evolve_population(std::vector<NPUGenome> &population,
                    FitnessFunction fitness_fn, int generations);

  /**
   * Run single evolution generation
   * @param population Current population
   * @param fitness_fn Fitness evaluation function
   * @return Statistics for this generation
   */
  GenerationStats evolve_generation(std::vector<NPUGenome> &population,
                                    FitnessFunction fitness_fn);

  /**
   * Get best genome from population
   */
  NPUGenome get_best_genome(const std::vector<NPUGenome> &population) const;

  // ================================================================
  // Configuration
  // ================================================================

  void set_config(const EvolutionConfig &config) { this->config = config; }
  const EvolutionConfig &get_config() const { return config; }

  // ================================================================
  // Fitness Functions
  // ================================================================

  /**
   * Create entelechy-based fitness function
   */
  static FitnessFunction entelechy_fitness(UnifiedNanoBrainKernel *kernel);

  /**
   * Create coherence-based fitness function
   */
  static FitnessFunction coherence_fitness(UnifiedNanoBrainKernel *kernel);

  /**
   * Create consciousness-based fitness function
   */
  static FitnessFunction consciousness_fitness(UnifiedNanoBrainKernel *kernel);

private:
  EvolutionConfig config;
  std::mt19937 rng;
  int genome_counter = 0;

  // Helpers
  std::string generate_genome_id();
  void initialize_gene_vector(std::vector<float> &genes, int size);
  float mutate_gene(float gene);
  void crossover_genes(const std::vector<float> &parent1,
                       const std::vector<float> &parent2,
                       std::vector<float> &child);
  float calculate_diversity(const std::vector<NPUGenome> &population) const;
  std::vector<size_t> select_parents(const std::vector<NPUGenome> &population,
                                     const std::vector<float> &fitness_scores,
                                     int count);
};

// ================================================================
// Phenotype Expression
// ================================================================

/**
 * Convert genetic traits to NPU enhancement configuration
 */
class PhenotypeExpressor {
public:
  /**
   * Express full genome as configuration
   */
  static NPUEnhancementConfig express(const NPUGenome &genome);

  /**
   * Express specific gene to configuration field
   */
  static float express_coherence_weight(const std::vector<float> &genes);
  static float express_consciousness_weight(const std::vector<float> &genes);
  static float express_attention_weight(const std::vector<float> &genes);
  static float express_threshold(const std::vector<float> &genes);
};

// ================================================================
// Utility Functions
// ================================================================

/**
 * Create default evolution configuration
 */
inline EvolutionConfig default_evolution_config() { return EvolutionConfig(); }

/**
 * Create rapid evolution configuration (smaller population, more mutations)
 */
inline EvolutionConfig rapid_evolution_config() {
  EvolutionConfig cfg;
  cfg.population_size = 10;
  cfg.mutation_rate = 0.25f;
  cfg.mutation_strength = 0.3f;
  cfg.max_generations = 50;
  return cfg;
}

/**
 * Create stable evolution configuration (larger population, less mutation)
 */
inline EvolutionConfig stable_evolution_config() {
  EvolutionConfig cfg;
  cfg.population_size = 50;
  cfg.mutation_rate = 0.1f;
  cfg.mutation_strength = 0.1f;
  cfg.elitism_rate = 0.2f;
  cfg.max_generations = 200;
  return cfg;
}

#endif // NANOBRAIN_ONTOGENESIS_H
