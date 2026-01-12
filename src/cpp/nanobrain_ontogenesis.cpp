#include "nanobrain_ontogenesis.h"
#include <algorithm>
#include <chrono>
#include <numeric>
#include <sstream>

// ================================================================
// OntogenesisEngine Implementation
// ================================================================

OntogenesisEngine::OntogenesisEngine(const EvolutionConfig &cfg) : config(cfg) {
  // Seed random number generator
  auto seed =
      std::chrono::high_resolution_clock::now().time_since_epoch().count();
  rng.seed(static_cast<unsigned int>(seed));
}

OntogenesisEngine::~OntogenesisEngine() {}

// ================================================================
// Genome Management
// ================================================================

NPUGenome OntogenesisEngine::create_random_genome() {
  NPUGenome genome;
  genome.id = generate_genome_id();
  genome.generation = 0;

  // Initialize gene vectors with random values
  initialize_gene_vector(genome.genes.ontological, config.ontological_genes);
  initialize_gene_vector(genome.genes.teleological, config.teleological_genes);
  initialize_gene_vector(genome.genes.cognitive, config.cognitive_genes);
  initialize_gene_vector(genome.genes.integrative, config.integrative_genes);
  initialize_gene_vector(genome.genes.evolutionary, config.evolutionary_genes);

  // Express phenotype
  genome.expressed_config = express_phenotype(genome);

  return genome;
}

NPUGenome OntogenesisEngine::create_genome_from_config(
    const NPUEnhancementConfig &config) {

  NPUGenome genome;
  genome.id = generate_genome_id();
  genome.generation = 0;
  genome.expressed_config = config;

  // Derive genes from config (reverse phenotype expression)
  // Ontological genes
  genome.genes.ontological.resize(this->config.ontological_genes);
  genome.genes.ontological[0] = config.enable_ppm_coherence ? 0.8f : 0.2f;
  genome.genes.ontological[1] = config.enable_consciousness ? 0.8f : 0.2f;
  genome.genes.ontological[2] = config.enable_entelechy ? 0.8f : 0.2f;
  genome.genes.ontological[3] = config.enable_attention_boost ? 0.8f : 0.2f;
  genome.genes.ontological[4] = 0.5f; // Reserved

  // Teleological genes - purpose weights
  genome.genes.teleological.resize(this->config.teleological_genes);
  genome.genes.teleological[0] = config.coherence_weight;
  genome.genes.teleological[1] = config.consciousness_weight;
  genome.genes.teleological[2] = config.attention_weight;
  genome.genes.teleological[3] = config.consciousness_threshold;
  genome.genes.teleological[4] = config.coherence_threshold;

  // Other gene vectors with defaults
  initialize_gene_vector(genome.genes.cognitive, this->config.cognitive_genes);
  initialize_gene_vector(genome.genes.integrative,
                         this->config.integrative_genes);
  initialize_gene_vector(genome.genes.evolutionary,
                         this->config.evolutionary_genes);

  return genome;
}

NPUEnhancementConfig
OntogenesisEngine::express_phenotype(const NPUGenome &genome) {
  return PhenotypeExpressor::express(genome);
}

// ================================================================
// Self-Generation
// ================================================================

NPUGenome OntogenesisEngine::self_generate(const NPUGenome &parent) {
  NPUGenome child;
  child.id = generate_genome_id();
  child.generation = parent.generation + 1;
  child.lineage = parent.lineage;
  child.lineage.push_back(parent.id);

  // Copy genes from parent
  child.genes = parent.genes;

  // Apply mutations
  mutate(child);

  // Express phenotype
  child.expressed_config = express_phenotype(child);

  return child;
}

float OntogenesisEngine::self_optimize(NPUGenome &genome,
                                       FitnessFunction fitness_fn,
                                       int iterations) {
  float current_fitness = fitness_fn(genome);

  for (int i = 0; i < iterations; ++i) {
    // Create mutated copy
    NPUGenome mutated = genome;
    mutate(mutated);
    mutated.expressed_config = express_phenotype(mutated);

    // Evaluate fitness
    float new_fitness = fitness_fn(mutated);

    // Accept if better
    if (new_fitness > current_fitness) {
      genome = mutated;
      current_fitness = new_fitness;
    }
  }

  genome.fitness = current_fitness;
  return current_fitness;
}

// ================================================================
// Reproduction
// ================================================================

NPUGenome OntogenesisEngine::crossover(const NPUGenome &parent1,
                                       const NPUGenome &parent2) {
  NPUGenome child;
  child.id = generate_genome_id();
  child.generation = std::max(parent1.generation, parent2.generation) + 1;
  child.lineage = {parent1.id, parent2.id};

  // Crossover each gene vector
  crossover_genes(parent1.genes.ontological, parent2.genes.ontological,
                  child.genes.ontological);
  crossover_genes(parent1.genes.teleological, parent2.genes.teleological,
                  child.genes.teleological);
  crossover_genes(parent1.genes.cognitive, parent2.genes.cognitive,
                  child.genes.cognitive);
  crossover_genes(parent1.genes.integrative, parent2.genes.integrative,
                  child.genes.integrative);
  crossover_genes(parent1.genes.evolutionary, parent2.genes.evolutionary,
                  child.genes.evolutionary);

  // Apply mutation with crossover rate
  std::uniform_real_distribution<float> dist(0.0f, 1.0f);
  if (dist(rng) < config.mutation_rate) {
    mutate(child);
  }

  // Express phenotype
  child.expressed_config = express_phenotype(child);

  return child;
}

void OntogenesisEngine::mutate(NPUGenome &genome) {
  // Mutate each gene vector
  auto mutate_vector = [this](std::vector<float> &genes) {
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    for (float &gene : genes) {
      if (dist(rng) < config.mutation_rate) {
        gene = mutate_gene(gene);
      }
    }
  };

  mutate_vector(genome.genes.ontological);
  mutate_vector(genome.genes.teleological);
  mutate_vector(genome.genes.cognitive);
  mutate_vector(genome.genes.integrative);
  mutate_vector(genome.genes.evolutionary);
}

// ================================================================
// Population Evolution
// ================================================================

std::vector<NPUGenome> OntogenesisEngine::initialize_population(int size) {
  std::vector<NPUGenome> population;
  population.reserve(size);

  for (int i = 0; i < size; ++i) {
    population.push_back(create_random_genome());
  }

  return population;
}

std::vector<GenerationStats>
OntogenesisEngine::evolve_population(std::vector<NPUGenome> &population,
                                     FitnessFunction fitness_fn,
                                     int generations) {

  std::vector<GenerationStats> history;
  history.reserve(generations);

  for (int gen = 0; gen < generations; ++gen) {
    GenerationStats stats = evolve_generation(population, fitness_fn);
    stats.generation = gen;
    history.push_back(stats);

    // Check termination condition
    if (stats.best_fitness >= config.fitness_threshold) {
      break;
    }
  }

  return history;
}

GenerationStats
OntogenesisEngine::evolve_generation(std::vector<NPUGenome> &population,
                                     FitnessFunction fitness_fn) {

  // Evaluate fitness for all
  std::vector<float> fitness_scores;
  fitness_scores.reserve(population.size());

  for (auto &genome : population) {
    float fitness = fitness_fn(genome);
    genome.fitness = fitness;
    fitness_scores.push_back(fitness);
  }

  // Calculate statistics
  GenerationStats stats;
  stats.best_fitness =
      *std::max_element(fitness_scores.begin(), fitness_scores.end());
  stats.worst_fitness =
      *std::min_element(fitness_scores.begin(), fitness_scores.end());
  stats.avg_fitness =
      std::accumulate(fitness_scores.begin(), fitness_scores.end(), 0.0f) /
      fitness_scores.size();
  stats.diversity = calculate_diversity(population);

  // Find best genome
  auto best_it = std::max_element(population.begin(), population.end(),
                                  [](const NPUGenome &a, const NPUGenome &b) {
                                    return a.fitness < b.fitness;
                                  });
  stats.best_genome_id = best_it->id;

  // Create next generation
  std::vector<NPUGenome> next_generation;
  next_generation.reserve(population.size());

  // Elitism - keep top performers
  int elite_count = static_cast<int>(population.size() * config.elitism_rate);
  std::vector<size_t> sorted_indices(population.size());
  std::iota(sorted_indices.begin(), sorted_indices.end(), 0);
  std::sort(sorted_indices.begin(), sorted_indices.end(),
            [&fitness_scores](size_t a, size_t b) {
              return fitness_scores[a] > fitness_scores[b];
            });

  for (int i = 0; i < elite_count; ++i) {
    next_generation.push_back(population[sorted_indices[i]]);
  }

  // Fill rest with crossover offspring
  while (next_generation.size() < population.size()) {
    // Select parents
    auto parents = select_parents(population, fitness_scores, 2);

    std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    if (dist(rng) < config.crossover_rate) {
      // Crossover
      next_generation.push_back(
          crossover(population[parents[0]], population[parents[1]]));
    } else {
      // Clone with mutation
      next_generation.push_back(self_generate(population[parents[0]]));
    }
  }

  population = std::move(next_generation);

  return stats;
}

NPUGenome OntogenesisEngine::get_best_genome(
    const std::vector<NPUGenome> &population) const {

  return *std::max_element(population.begin(), population.end(),
                           [](const NPUGenome &a, const NPUGenome &b) {
                             return a.fitness < b.fitness;
                           });
}

// ================================================================
// Fitness Functions
// ================================================================

FitnessFunction
OntogenesisEngine::entelechy_fitness(UnifiedNanoBrainKernel *kernel) {

  return [kernel](const NPUGenome &genome) -> float {
    if (!kernel || !kernel->is_active()) {
      return 0.0f;
    }

    // Create bridge with genome's configuration
    NanoBrainNPUBridge bridge(genome.expressed_config);
    bridge.attach_nanobrain(kernel);

    // Get entelechy fitness
    return bridge.calculate_entelechy_fitness();
  };
}

FitnessFunction
OntogenesisEngine::coherence_fitness(UnifiedNanoBrainKernel *kernel) {

  return [kernel](const NPUGenome &genome) -> float {
    if (!kernel || !kernel->is_active()) {
      return 0.0f;
    }

    auto metrics = kernel->get_metrics();

    // Weight coherence heavily
    return metrics.quantum_coherence * 0.4f +
           metrics.temporal_stability * 0.3f + metrics.prime_alignment * 0.3f;
  };
}

FitnessFunction
OntogenesisEngine::consciousness_fitness(UnifiedNanoBrainKernel *kernel) {

  return [kernel](const NPUGenome &genome) -> float {
    if (!kernel || !kernel->is_active()) {
      return 0.0f;
    }

    auto metrics = kernel->get_metrics();

    // Weight consciousness heavily
    return metrics.consciousness_emergence * 0.5f +
           metrics.self_awareness_level * 0.3f +
           metrics.system_coherence * 0.2f;
  };
}

// ================================================================
// Private Helpers
// ================================================================

std::string OntogenesisEngine::generate_genome_id() {
  std::ostringstream oss;
  oss << "genome_" << ++genome_counter << "_"
      << std::chrono::steady_clock::now().time_since_epoch().count();
  return oss.str();
}

void OntogenesisEngine::initialize_gene_vector(std::vector<float> &genes,
                                               int size) {
  genes.resize(size);
  std::uniform_real_distribution<float> dist(0.0f, 1.0f);
  for (float &gene : genes) {
    gene = dist(rng);
  }
}

float OntogenesisEngine::mutate_gene(float gene) {
  std::normal_distribution<float> dist(0.0f, config.mutation_strength);
  gene += dist(rng);
  return std::min(1.0f, std::max(0.0f, gene));
}

void OntogenesisEngine::crossover_genes(const std::vector<float> &parent1,
                                        const std::vector<float> &parent2,
                                        std::vector<float> &child) {

  size_t size = std::min(parent1.size(), parent2.size());
  child.resize(size);

  std::uniform_int_distribution<size_t> point_dist(0, size);
  size_t crossover_point = point_dist(rng);

  for (size_t i = 0; i < size; ++i) {
    child[i] = (i < crossover_point) ? parent1[i] : parent2[i];
  }
}

float OntogenesisEngine::calculate_diversity(
    const std::vector<NPUGenome> &population) const {

  if (population.size() < 2) {
    return 0.0f;
  }

  // Calculate average gene distance between pairs
  float total_distance = 0.0f;
  int pairs = 0;

  for (size_t i = 0; i < population.size(); ++i) {
    for (size_t j = i + 1; j < population.size(); ++j) {
      // Compare ontological genes as representative sample
      const auto &g1 = population[i].genes.ontological;
      const auto &g2 = population[j].genes.ontological;

      float dist = 0.0f;
      size_t size = std::min(g1.size(), g2.size());
      for (size_t k = 0; k < size; ++k) {
        dist += std::abs(g1[k] - g2[k]);
      }
      dist /= size;

      total_distance += dist;
      pairs++;
    }
  }

  return (pairs > 0) ? total_distance / pairs : 0.0f;
}

std::vector<size_t>
OntogenesisEngine::select_parents(const std::vector<NPUGenome> &population,
                                  const std::vector<float> &fitness_scores,
                                  int count) {

  // Tournament selection
  std::vector<size_t> selected;
  selected.reserve(count);

  std::uniform_int_distribution<size_t> dist(0, population.size() - 1);

  for (int i = 0; i < count; ++i) {
    // Tournament of 3
    size_t best = dist(rng);
    for (int t = 0; t < 2; ++t) {
      size_t candidate = dist(rng);
      if (fitness_scores[candidate] > fitness_scores[best]) {
        best = candidate;
      }
    }
    selected.push_back(best);
  }

  return selected;
}

// ================================================================
// PhenotypeExpressor Implementation
// ================================================================

NPUEnhancementConfig PhenotypeExpressor::express(const NPUGenome &genome) {
  NPUEnhancementConfig cfg;

  // Express ontological genes -> feature toggles
  if (!genome.genes.ontological.empty()) {
    cfg.enable_ppm_coherence = genome.genes.ontological[0] > 0.5f;
    if (genome.genes.ontological.size() > 1) {
      cfg.enable_consciousness = genome.genes.ontological[1] > 0.5f;
    }
    if (genome.genes.ontological.size() > 2) {
      cfg.enable_entelechy = genome.genes.ontological[2] > 0.5f;
    }
    if (genome.genes.ontological.size() > 3) {
      cfg.enable_attention_boost = genome.genes.ontological[3] > 0.5f;
    }
  }

  // Express teleological genes -> weights and thresholds
  if (!genome.genes.teleological.empty()) {
    cfg.coherence_weight = express_coherence_weight(genome.genes.teleological);
    if (genome.genes.teleological.size() > 1) {
      cfg.consciousness_weight =
          express_consciousness_weight(genome.genes.teleological);
    }
    if (genome.genes.teleological.size() > 2) {
      cfg.attention_weight =
          express_attention_weight(genome.genes.teleological);
    }
    if (genome.genes.teleological.size() > 3) {
      cfg.consciousness_threshold =
          express_threshold(genome.genes.teleological);
    }
    if (genome.genes.teleological.size() > 4) {
      cfg.coherence_threshold = genome.genes.teleological[4] * 0.5f + 0.2f;
    }
  }

  return cfg;
}

float PhenotypeExpressor::express_coherence_weight(
    const std::vector<float> &genes) {
  if (genes.empty())
    return 0.3f;
  // Scale gene to reasonable weight range [0.1, 0.5]
  return genes[0] * 0.4f + 0.1f;
}

float PhenotypeExpressor::express_consciousness_weight(
    const std::vector<float> &genes) {
  if (genes.size() < 2)
    return 0.2f;
  return genes[1] * 0.4f + 0.1f;
}

float PhenotypeExpressor::express_attention_weight(
    const std::vector<float> &genes) {
  if (genes.size() < 3)
    return 0.2f;
  return genes[2] * 0.4f + 0.1f;
}

float PhenotypeExpressor::express_threshold(const std::vector<float> &genes) {
  if (genes.size() < 4)
    return 0.5f;
  // Scale to threshold range [0.3, 0.8]
  return genes[3] * 0.5f + 0.3f;
}
