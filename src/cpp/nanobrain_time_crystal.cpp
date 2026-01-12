#include "nanobrain_time_crystal.h"
#include <algorithm>
#include <chrono>
#include <cmath>
#include <iostream>
#include <numeric>
#include <random>
#include <sstream>

// ================================================================
// Utility Functions Implementation
// ================================================================

std::string gml_shape_to_string(GMLShape shape) {
  switch (shape) {
  case GMLShape::Sphere:
    return "Sphere";
  case GMLShape::Torus:
    return "Torus";
  case GMLShape::Helix:
    return "Helix";
  case GMLShape::Fractal:
    return "Fractal";
  case GMLShape::Hypercube:
    return "Hypercube";
  case GMLShape::Simplex:
    return "Simplex";
  case GMLShape::Point:
    return "Point";
  case GMLShape::Line:
    return "Line";
  case GMLShape::Triangle:
    return "Triangle";
  case GMLShape::Square:
    return "Square";
  case GMLShape::Pentagon:
    return "Pentagon";
  case GMLShape::Hexagon:
    return "Hexagon";
  case GMLShape::Circle:
    return "Circle";
  case GMLShape::Tetrahedron:
    return "Tetrahedron";
  case GMLShape::Cube:
    return "Cube";
  case GMLShape::Octahedron:
    return "Octahedron";
  case GMLShape::Dodecahedron:
    return "Dodecahedron";
  case GMLShape::Icosahedron:
    return "Icosahedron";
  case GMLShape::Mobius:
    return "Mobius";
  default:
    return "Unknown";
  }
}

std::string musical_note_to_string(MusicalNote note) {
  switch (note) {
  case MusicalNote::C:
    return "C";
  case MusicalNote::CSharp:
    return "C#";
  case MusicalNote::D:
    return "D";
  case MusicalNote::DSharp:
    return "D#";
  case MusicalNote::E:
    return "E";
  case MusicalNote::F:
    return "F";
  case MusicalNote::FSharp:
    return "F#";
  case MusicalNote::G:
    return "G";
  case MusicalNote::GSharp:
    return "G#";
  case MusicalNote::A:
    return "A";
  case MusicalNote::ASharp:
    return "A#";
  case MusicalNote::B:
    return "B";
  default:
    return "?";
  }
}

int musical_note_to_index(MusicalNote note) { return static_cast<int>(note); }

MusicalNote index_to_musical_note(int index) {
  return static_cast<MusicalNote>(index % 12);
}

GMLShape index_to_gml_shape(int index) {
  static const GMLShape shapes[] = {
      GMLShape::Sphere,     GMLShape::Torus,        GMLShape::Helix,
      GMLShape::Fractal,    GMLShape::Hypercube,    GMLShape::Simplex,
      GMLShape::Point,      GMLShape::Line,         GMLShape::Triangle,
      GMLShape::Square,     GMLShape::Pentagon,     GMLShape::Hexagon,
      GMLShape::Circle,     GMLShape::Tetrahedron,  GMLShape::Cube,
      GMLShape::Octahedron, GMLShape::Dodecahedron, GMLShape::Icosahedron,
      GMLShape::Mobius};
  return shapes[index % 19];
}

// ================================================================
// TimeCrystalKernel Implementation
// ================================================================

TimeCrystalKernel::TimeCrystalKernel(const TimeCrystalConfig &cfg)
    : config(cfg), active(false), cycle_count(0), start_time(0),
      atom_counter(0) {
  // Create underlying tensor kernel
  NanoBrainConfig kernel_config;
  kernel_config.memory_size = config.memory_size;
  kernel_config.use_gpu = false;
  kernel = std::make_unique<NanoBrainKernel>(kernel_config);
}

TimeCrystalKernel::~TimeCrystalKernel() { shutdown(); }

int64_t TimeCrystalKernel::current_time_millis() const {
  return std::chrono::duration_cast<std::chrono::milliseconds>(
             std::chrono::system_clock::now().time_since_epoch())
      .count();
}

std::string TimeCrystalKernel::generate_atom_id() {
  std::stringstream ss;
  ss << "atom_" << atom_counter++;
  return ss.str();
}

void TimeCrystalKernel::initialize() {
  if (active)
    return;

  start_time = current_time_millis();

  // Initialize fundamental atoms from NanoBrain theory
  initialize_fundamental_atoms();

  // Initialize GML shape atoms
  initialize_gml_atoms();

  active = true;
  std::cout << "[TimeCrystalKernel] Initialized with " << atom_space.size()
            << " atoms" << std::endl;
}

void TimeCrystalKernel::shutdown() {
  active = false;
  std::cout << "[TimeCrystalKernel] Shutdown after " << cycle_count << " cycles"
            << std::endl;
}

void TimeCrystalKernel::initialize_fundamental_atoms() {
  // Chapter 1: Philosophical Transformation root atom
  GeometricPattern philo_geom;
  philo_geom.shape = GMLShape::Sphere;
  philo_geom.dimensions = TIME_CRYSTAL_DIMENSIONS;
  philo_geom.symmetry_group = "SO(11)";
  philo_geom.musical_note = MusicalNote::C;
  philo_geom.prime_resonance = {2, 3, 5};
  philo_geom.scale_factor = 1.0f;

  create_atom("ConceptNode", "PhilosophicalTransformation", {1.0f, 1.0f, 1.0f},
              {1000.0f, 1000.0f, 1000.0f}, {2, 3, 5}, philo_geom);

  // Chapter 2: Fractal Information Theory atom
  GeometricPattern fit_geom;
  fit_geom.shape = GMLShape::Fractal;
  fit_geom.dimensions = config.fractal_resolution;
  fit_geom.symmetry_group = "FractalGroup";
  fit_geom.musical_note = MusicalNote::D;
  fit_geom.prime_resonance = {7, 11, 13};
  fit_geom.scale_factor = static_cast<float>(GOLDEN_RATIO);

  create_atom("ConceptNode", "FractalInformationTheory", {0.95f, 0.9f, 1.0f},
              {950.0f, 900.0f, 850.0f}, {7, 11, 13}, fit_geom);

  // Chapter 3: Create atoms for each fundamental prime
  for (size_t i = 0; i < FUNDAMENTAL_PRIMES.size(); i++) {
    int prime = FUNDAMENTAL_PRIMES[i];
    GeometricPattern prime_geom = generate_prime_geometry(prime, i);

    std::string name = "Prime" + std::to_string(prime);
    float sti = 1000.0f - i * 10.0f;
    float lti = 1000.0f - i * 5.0f;
    float vlti = 1000.0f - i * 2.0f;

    create_atom("NumberNode", name, {0.999f, 0.999f, 1.0f}, {sti, lti, vlti},
                {prime}, prime_geom);
  }
}

void TimeCrystalKernel::initialize_gml_atoms() {
  // 15 fundamental GML shapes
  const char *gml_names[] = {
      "Point",        "Line",        "Triangle",    "Square", "Pentagon",
      "Hexagon",      "Circle",      "Tetrahedron", "Cube",   "Octahedron",
      "Dodecahedron", "Icosahedron", "Sphere",      "Torus",  "Mobius"};

  for (int i = 0; i < 15; i++) {
    GeometricPattern geom;
    geom.shape = (i < 7) ? GMLShape::Sphere : GMLShape::Hypercube;
    geom.dimensions = (i < 7) ? 2 : std::min(i - 4, TIME_CRYSTAL_DIMENSIONS);
    geom.symmetry_group = "GML_" + std::string(gml_names[i]);
    geom.musical_note = index_to_musical_note(i % 7); // C, D, E, F, G, A, B
    geom.prime_resonance = {FUNDAMENTAL_PRIMES[i % FUNDAMENTAL_PRIMES_COUNT]};
    geom.scale_factor = (i + 1.0f) / 15.0f;

    std::string name = "GMLShape" + std::string(gml_names[i]);
    float sti = 500.0f - i * 10.0f;

    create_atom("ConceptNode", name, {0.8f, 0.7f, 1.0f}, {sti, 400.0f, 300.0f},
                geom.prime_resonance, geom);
  }
}

std::string
TimeCrystalKernel::create_atom(const std::string &type, const std::string &name,
                               const TruthValue &tv, const AttentionValue &av,
                               const std::vector<int> &prime_encoding,
                               const GeometricPattern &geometry) {
  std::string id = generate_atom_id();

  TimeCrystalAtom atom;
  atom.id = id;
  atom.type = type;
  atom.name = name;
  atom.truth_value = tv;
  atom.attention_value = av;
  atom.prime_encoding = prime_encoding;
  atom.fractal_geometry = geometry;

  // Generate quantum state
  TimeCrystalQuantumState quantum_state;
  quantum_state.dimensions = generate_quantum_coordinates();
  quantum_state.prime_signature = prime_encoding;

  // Random temporal coherence in 0.5-1.0 range
  static std::mt19937 rng(std::random_device{}());
  static std::uniform_real_distribution<float> coherence_dist(0.5f, 1.0f);
  static std::uniform_real_distribution<float> phase_dist(0.0f, 2.0f * PI);

  quantum_state.temporal_coherence = coherence_dist(rng);
  quantum_state.fractal_dimension =
      geometry.dimensions + (coherence_dist(rng) - 0.5f);
  quantum_state.resonance_frequency =
      calculate_resonance_frequency(prime_encoding);
  quantum_state.quantum_phase = phase_dist(rng);

  atom.time_crystal_state = quantum_state;

  atom_space[id] = atom;
  time_crystals[id] = quantum_state;

  return id;
}

const TimeCrystalAtom *
TimeCrystalKernel::get_atom(const std::string &id) const {
  auto it = atom_space.find(id);
  if (it != atom_space.end()) {
    return &it->second;
  }
  return nullptr;
}

TimeCrystalAtom *TimeCrystalKernel::get_mutable_atom(const std::string &id) {
  auto it = atom_space.find(id);
  if (it != atom_space.end()) {
    return &it->second;
  }
  return nullptr;
}

bool TimeCrystalKernel::remove_atom(const std::string &id) {
  size_t erased = atom_space.erase(id);
  time_crystals.erase(id);
  return erased > 0;
}

std::vector<std::string> TimeCrystalKernel::get_all_atom_ids() const {
  std::vector<std::string> ids;
  ids.reserve(atom_space.size());
  for (const auto &[id, _] : atom_space) {
    ids.push_back(id);
  }
  return ids;
}

// ================================================================
// Phase Prime Metric (PPM) Functions
// ================================================================

float TimeCrystalKernel::compute_ppm_coherence(const std::vector<int> &primes) {
  if (primes.empty())
    return 0.5f;

  // Calculate product of primes
  double product = 1.0;
  for (int p : primes) {
    product *= static_cast<double>(p);
  }

  // Calculate sum of primes
  double sum = 0.0;
  for (int p : primes) {
    sum += static_cast<double>(p);
  }

  // PPM coherence formula: 0.5 + 0.5 * sin(sqrt(prod) * PI / sum)
  double sqrt_prod = std::sqrt(product);
  double arg = sqrt_prod * PI / sum;
  return static_cast<float>(0.5 + 0.5 * std::sin(arg));
}

float TimeCrystalKernel::calculate_prime_importance(
    const std::vector<int> &primes) {
  if (primes.empty())
    return 1.0f;

  // Smaller primes are more fundamental, hence more important
  float total_score = 0.0f;
  for (int p : primes) {
    int fundamental_idx = get_fundamental_index(p);
    if (fundamental_idx >= 0) {
      total_score += (15.0f - fundamental_idx) / 15.0f;
    } else {
      total_score += 0.1f; // Non-fundamental primes get minimal score
    }
  }

  return 0.5f + (total_score / primes.size()) * 0.5f;
}

float TimeCrystalKernel::calculate_resonance_frequency(
    const std::vector<int> &primes) {
  if (primes.empty())
    return 440.0f; // Default A note

  // Use PPM to calculate harmonic frequency
  int64_t product = 1;
  int sum = 0;
  for (int p : primes) {
    product *= p;
    sum += p;
  }

  // Map to audible frequency range (20 Hz - 20 kHz)
  float base_freq = 440.0f * std::pow(2.0f, (sum % 12) / 12.0f);
  return base_freq * (1.0f + (product % 100) / 1000.0f);
}

bool TimeCrystalKernel::is_fundamental_prime(int prime) const {
  return get_fundamental_index(prime) >= 0;
}

int TimeCrystalKernel::get_fundamental_index(int prime) const {
  for (size_t i = 0; i < FUNDAMENTAL_PRIMES.size(); i++) {
    if (FUNDAMENTAL_PRIMES[i] == prime) {
      return static_cast<int>(i);
    }
  }
  return -1;
}

// ================================================================
// Time Crystal Quantum State Functions
// ================================================================

std::array<float, TIME_CRYSTAL_DIMENSIONS>
TimeCrystalKernel::generate_quantum_coordinates() {
  std::array<float, TIME_CRYSTAL_DIMENSIONS> coords;

  for (int i = 0; i < TIME_CRYSTAL_DIMENSIONS; i++) {
    int prime = FUNDAMENTAL_PRIMES[i % FUNDAMENTAL_PRIMES_COUNT];
    coords[i] = static_cast<float>(std::sin(prime * PI / 180.0) *
                                   std::cos(i * PI / 6.0));
  }

  return coords;
}

void TimeCrystalKernel::update_time_crystal_states() {
  float freq_factor = 2.0f * PI / config.temporal_processing_frequency;

  for (auto &[atom_id, crystal] : time_crystals) {
    auto *atom = get_mutable_atom(atom_id);
    if (!atom)
      continue;

    // Phase evolution based on prime resonance
    crystal.quantum_phase += crystal.resonance_frequency * freq_factor;
    while (crystal.quantum_phase >= 2.0f * PI) {
      crystal.quantum_phase -= 2.0f * PI;
    }

    // Temporal coherence decay and regeneration
    crystal.temporal_coherence *= 0.999f;
    if (crystal.temporal_coherence < config.quantum_coherence_threshold) {
      // Regenerate coherence through prime alignment
      crystal.temporal_coherence =
          compute_ppm_coherence(crystal.prime_signature);
    }

    // Update fractal dimension evolution
    crystal.fractal_dimension += std::sin(crystal.quantum_phase) * 0.01f;
    crystal.fractal_dimension = std::max(
        1.0f,
        std::min(static_cast<float>(atom->fractal_geometry.dimensions + 1),
                 crystal.fractal_dimension));

    // Sync back to atom
    atom->time_crystal_state = crystal;
  }
}

const TimeCrystalQuantumState *
TimeCrystalKernel::get_time_crystal_state(const std::string &atom_id) const {
  auto it = time_crystals.find(atom_id);
  if (it != time_crystals.end()) {
    return &it->second;
  }
  return nullptr;
}

float TimeCrystalKernel::calculate_temporal_flow(
    const TimeCrystalQuantumState &state1,
    const TimeCrystalQuantumState &state2) {
  float phase_diff = state2.quantum_phase - state1.quantum_phase;
  return std::sin(phase_diff); // -1 to 1 range
}

// ================================================================
// Geometric Musical Language (GML) Functions
// ================================================================

float TimeCrystalKernel::calculate_geometric_resonance(
    const GeometricPattern &pattern1, const GeometricPattern &pattern2) {
  // Shape compatibility
  float shape_resonance = (pattern1.shape == pattern2.shape) ? 1.0f : 0.3f;

  // Dimensional harmony
  float dim_resonance =
      1.0f - std::abs(pattern1.dimensions - pattern2.dimensions) / 11.0f;

  // Musical harmony
  float musical_resonance =
      calculate_musical_harmony(pattern1.musical_note, pattern2.musical_note);

  // Prime resonance overlap
  std::vector<int> intersection;
  for (int p : pattern1.prime_resonance) {
    if (std::find(pattern2.prime_resonance.begin(),
                  pattern2.prime_resonance.end(),
                  p) != pattern2.prime_resonance.end()) {
      intersection.push_back(p);
    }
  }
  size_t max_size = std::max(pattern1.prime_resonance.size(),
                             pattern2.prime_resonance.size());
  float prime_overlap =
      max_size > 0 ? static_cast<float>(intersection.size()) / max_size : 0.0f;

  return (shape_resonance + dim_resonance + musical_resonance + prime_overlap) /
         4.0f;
}

float TimeCrystalKernel::calculate_musical_harmony(MusicalNote note1,
                                                   MusicalNote note2) {
  int index1 = musical_note_to_index(note1);
  int index2 = musical_note_to_index(note2);

  int interval = std::abs(index1 - index2) % 12;

  // Harmonic intervals (major scale intervals)
  static const int harmonic_intervals[] = {0, 3, 4, 5, 7, 8, 9};
  for (int h : harmonic_intervals) {
    if (interval == h)
      return 1.0f;
  }
  return 0.3f;
}

GeometricPattern TimeCrystalKernel::generate_prime_geometry(int prime,
                                                            int index) {
  GeometricPattern geom;

  static const GMLShape shapes[] = {GMLShape::Sphere,    GMLShape::Torus,
                                    GMLShape::Helix,     GMLShape::Fractal,
                                    GMLShape::Hypercube, GMLShape::Simplex};

  geom.shape = shapes[index % 6];
  geom.dimensions = std::min((prime % 8) + 3, TIME_CRYSTAL_DIMENSIONS);
  geom.symmetry_group = "C" + std::to_string(prime);
  geom.musical_note = index_to_musical_note(prime % 12);
  geom.prime_resonance = {prime};
  geom.scale_factor = std::sqrt(static_cast<float>(prime)) / 7.0f;

  return geom;
}

GeometricPattern
TimeCrystalKernel::combine_patterns(const GeometricPattern &p1,
                                    const GeometricPattern &p2) {
  GeometricPattern combined;
  combined.shape = (p1.dimensions > p2.dimensions) ? p1.shape : p2.shape;
  combined.dimensions = std::max(p1.dimensions, p2.dimensions);
  combined.symmetry_group =
      "Combined(" + p1.symmetry_group + "," + p2.symmetry_group + ")";
  combined.musical_note = p1.musical_note; // Use first pattern's note
  combined.scale_factor = (p1.scale_factor + p2.scale_factor) / 2.0f;

  // Combine prime resonances
  combined.prime_resonance = p1.prime_resonance;
  for (int p : p2.prime_resonance) {
    if (std::find(combined.prime_resonance.begin(),
                  combined.prime_resonance.end(),
                  p) == combined.prime_resonance.end()) {
      combined.prime_resonance.push_back(p);
    }
  }

  return combined;
}

void TimeCrystalKernel::update_gml_resonances() {
  for (auto &[atom_id, atom] : atom_space) {
    if (atom.name.find("GML") != std::string::npos) {
      auto it = time_crystals.find(atom_id);
      if (it != time_crystals.end()) {
        auto &crystal = it->second;
        // Update resonance based on current phase and prime alignment
        crystal.resonance_frequency =
            calculate_resonance_frequency(atom.prime_encoding) *
            (1.0f + std::sin(crystal.quantum_phase) * 0.1f);
      }
    }
  }
}

// ================================================================
// ECAN Attention Allocation
// ================================================================

void TimeCrystalKernel::perform_attention_allocation() {
  // Calculate PPM-weighted importance for each atom
  struct ImportanceScore {
    std::string id;
    float importance;
  };

  std::vector<ImportanceScore> scores;
  scores.reserve(atom_space.size());

  for (const auto &[id, atom] : atom_space) {
    float base_importance =
        atom.attention_value.sti + atom.attention_value.lti * 0.1f;
    float prime_weight = calculate_prime_importance(atom.prime_encoding);
    float coherence_bonus = atom.time_crystal_state.temporal_coherence * 100.0f;

    scores.push_back({id, base_importance * prime_weight + coherence_bonus});
  }

  // Sort by importance (descending)
  std::sort(scores.begin(), scores.end(), [](const auto &a, const auto &b) {
    return a.importance > b.importance;
  });

  // Allocate budget
  float remaining_budget = config.resource_budget;
  for (const auto &score : scores) {
    if (remaining_budget <= 0)
      break;

    auto *atom = get_mutable_atom(score.id);
    if (!atom)
      continue;

    // Allocate attention proportional to importance
    float allocation = std::min(remaining_budget * 0.1f, // Max 10% per atom
                                score.importance * 0.01f);

    atom->attention_value.sti += allocation;
    atom->attention_value.sti *= (1.0f - config.attention_decay_rate);

    remaining_budget -= allocation;
  }
}

void TimeCrystalKernel::apply_attention_decay() {
  for (auto &[id, atom] : atom_space) {
    atom.attention_value.sti *= (1.0f - config.attention_decay_rate);
    atom.attention_value.lti *= (1.0f - config.attention_decay_rate * 0.1f);
  }
}

void TimeCrystalKernel::spread_attention(
    const std::vector<std::string> &link_ids) {
  for (const auto &link_id : link_ids) {
    auto it = link_space.find(link_id);
    if (it == link_space.end())
      continue;

    const auto &inference = it->second;

    // Get conclusion atom
    auto *conclusion = get_mutable_atom(inference.conclusion_id);
    if (!conclusion)
      continue;

    // Spread attention from premises to conclusion
    float total_attention = 0.0f;
    for (const auto &premise_id : inference.premise_ids) {
      const auto *premise = get_atom(premise_id);
      if (premise) {
        total_attention += premise->attention_value.sti;
      }
    }

    float spread_amount = total_attention * config.diffusion_strength *
                          inference.quantum_coherence;
    conclusion->attention_value.sti += spread_amount;
  }
}

std::vector<std::string>
TimeCrystalKernel::get_top_attention_atoms(size_t k) const {
  std::vector<std::pair<std::string, float>> scored_atoms;
  scored_atoms.reserve(atom_space.size());

  for (const auto &[id, atom] : atom_space) {
    scored_atoms.push_back({id, atom.attention_value.sti});
  }

  std::sort(scored_atoms.begin(), scored_atoms.end(),
            [](const auto &a, const auto &b) { return a.second > b.second; });

  std::vector<std::string> result;
  result.reserve(std::min(k, scored_atoms.size()));
  for (size_t i = 0; i < std::min(k, scored_atoms.size()); i++) {
    result.push_back(scored_atoms[i].first);
  }

  return result;
}

// ================================================================
// PLN Reasoning
// ================================================================

std::string TimeCrystalKernel::create_inference(const std::string &atom1_id,
                                                const std::string &atom2_id,
                                                InferenceRuleType rule) {
  const auto *atom1 = get_atom(atom1_id);
  const auto *atom2 = get_atom(atom2_id);

  if (!atom1 || !atom2) {
    return "";
  }

  // Calculate geometric resonance
  float resonance = calculate_geometric_resonance(atom1->fractal_geometry,
                                                  atom2->fractal_geometry);

  // Create conclusion atom
  std::string rule_name;
  switch (rule) {
  case InferenceRuleType::Similarity:
    rule_name = "Similar";
    break;
  case InferenceRuleType::Inheritance:
    rule_name = "InheritsFrom";
    break;
  case InferenceRuleType::Implication:
    rule_name = "Implies";
    break;
  case InferenceRuleType::Deduction:
    rule_name = "Deduces";
    break;
  case InferenceRuleType::Induction:
    rule_name = "Induces";
    break;
  case InferenceRuleType::Abduction:
    rule_name = "Abduces";
    break;
  }

  std::string conclusion_name = atom1->name + rule_name + atom2->name;

  // Combine prime encodings
  std::vector<int> combined_primes = atom1->prime_encoding;
  for (int p : atom2->prime_encoding) {
    if (std::find(combined_primes.begin(), combined_primes.end(), p) ==
        combined_primes.end()) {
      combined_primes.push_back(p);
    }
  }

  GeometricPattern combined_geom =
      combine_patterns(atom1->fractal_geometry, atom2->fractal_geometry);

  TruthValue tv;
  tv.strength = resonance * 0.8f;
  tv.confidence =
      (atom1->truth_value.confidence + atom2->truth_value.confidence) / 2.0f;
  tv.count = 1.0f;

  AttentionValue av;
  av.sti = 50.0f;
  av.lti = 25.0f;
  av.vlti = 10.0f;

  std::string conclusion_id = create_atom("ConceptNode", conclusion_name, tv,
                                          av, combined_primes, combined_geom);

  // Create inference link
  TimeCrystalInference inference;
  inference.rule = rule;
  inference.premise_ids = {atom1_id, atom2_id};
  inference.conclusion_id = conclusion_id;
  inference.temporal_flow = calculate_temporal_flow(atom1->time_crystal_state,
                                                    atom2->time_crystal_state);
  inference.prime_consistency =
      calculate_prime_consistency(atom1->prime_encoding, atom2->prime_encoding);
  inference.fractal_convergence = resonance;
  inference.quantum_coherence = (atom1->time_crystal_state.temporal_coherence +
                                 atom2->time_crystal_state.temporal_coherence) /
                                2.0f;

  std::string link_id =
      atom1_id + "-" + std::to_string(static_cast<int>(rule)) + "-" + atom2_id;
  link_space[link_id] = inference;

  return link_id;
}

void TimeCrystalKernel::perform_pln_reasoning() {
  // Get high-attention atoms for reasoning
  auto active_atoms = get_top_attention_atoms(10);

  // Generate inferences using time crystal enhanced PLN
  for (size_t i = 0; i < active_atoms.size(); i++) {
    for (size_t j = i + 1; j < active_atoms.size(); j++) {
      const auto *atom1 = get_atom(active_atoms[i]);
      const auto *atom2 = get_atom(active_atoms[j]);

      if (!atom1 || !atom2)
        continue;

      // Check for geometric resonance
      float resonance = calculate_geometric_resonance(atom1->fractal_geometry,
                                                      atom2->fractal_geometry);

      if (resonance > 0.5f) {
        create_inference(active_atoms[i], active_atoms[j],
                         InferenceRuleType::Similarity);
      }
    }
  }
}

std::vector<std::string> TimeCrystalKernel::get_all_inference_ids() const {
  std::vector<std::string> ids;
  ids.reserve(link_space.size());
  for (const auto &[id, _] : link_space) {
    ids.push_back(id);
  }
  return ids;
}

const TimeCrystalInference *
TimeCrystalKernel::get_inference(const std::string &id) const {
  auto it = link_space.find(id);
  if (it != link_space.end()) {
    return &it->second;
  }
  return nullptr;
}

float TimeCrystalKernel::calculate_prime_consistency(
    const std::vector<int> &primes1, const std::vector<int> &primes2) {
  if (primes1.empty() || primes2.empty())
    return 0.0f;

  // Calculate intersection and union
  std::vector<int> union_primes;
  std::vector<int> intersection;

  for (int p : primes1) {
    union_primes.push_back(p);
    if (std::find(primes2.begin(), primes2.end(), p) != primes2.end()) {
      intersection.push_back(p);
    }
  }
  for (int p : primes2) {
    if (std::find(union_primes.begin(), union_primes.end(), p) ==
        union_primes.end()) {
      union_primes.push_back(p);
    }
  }

  return static_cast<float>(intersection.size()) /
         static_cast<float>(union_primes.size());
}

// ================================================================
// Tensor Operations
// ================================================================

NanoBrainTensor *
TimeCrystalKernel::encode_atom_to_tensor(const std::string &atom_id) {
  const auto *atom = get_atom(atom_id);
  if (!atom)
    return nullptr;

  // Create 128-dimensional embedding tensor
  auto *tensor = kernel->create_tensor({128});

  std::vector<float> data(128, 0.0f);

  // Type encoding (first element)
  std::hash<std::string> hasher;
  size_t type_hash = hasher(atom->type);
  data[0] = (type_hash % 100) / 100.0f;

  // Name hash (second element)
  size_t name_hash = hasher(atom->name);
  data[1] = (name_hash % 1000) / 1000.0f;

  // Truth value (elements 2-4)
  data[2] = atom->truth_value.strength;
  data[3] = atom->truth_value.confidence;
  data[4] = std::log(atom->truth_value.count + 1.0f) / 10.0f;

  // Attention value (elements 5-7)
  data[5] = atom->attention_value.sti / 100.0f;
  data[6] = atom->attention_value.lti / 100.0f;
  data[7] = atom->attention_value.vlti > 0.5f ? 1.0f : 0.0f;

  // Prime encoding (elements 8-22)
  for (size_t i = 0; i < std::min(atom->prime_encoding.size(), size_t(15));
       i++) {
    data[8 + i] = atom->prime_encoding[i] / 100.0f;
  }

  // Time crystal state (elements 23-33)
  for (int i = 0; i < TIME_CRYSTAL_DIMENSIONS; i++) {
    data[23 + i] = atom->time_crystal_state.dimensions[i];
  }

  // Additional quantum properties (elements 34-37)
  data[34] = atom->time_crystal_state.temporal_coherence;
  data[35] = atom->time_crystal_state.fractal_dimension / 11.0f;
  data[36] = atom->time_crystal_state.resonance_frequency / 20000.0f;
  data[37] = atom->time_crystal_state.quantum_phase / (2.0f * PI);

  kernel->set_data(tensor, data);
  return tensor;
}

NanoBrainTensor *
TimeCrystalKernel::compute_tensor_coherence(NanoBrainTensor *primes_tensor) {
  return kernel->compute_coherence(primes_tensor);
}

// ================================================================
// Metrics
// ================================================================

NanoBrainMetrics TimeCrystalKernel::get_metrics() const {
  NanoBrainMetrics metrics;

  metrics.total_atoms = atom_space.size();
  metrics.total_links = link_space.size();

  if (atom_space.empty()) {
    metrics.average_attention = 0.0f;
    metrics.quantum_coherence = 0.0f;
  } else {
    float total_attention = 0.0f;
    float total_coherence = 0.0f;

    for (const auto &[_, atom] : atom_space) {
      total_attention += atom.attention_value.sti;
      total_coherence += atom.time_crystal_state.temporal_coherence;
    }

    metrics.average_attention = total_attention / atom_space.size();
    metrics.quantum_coherence = total_coherence / atom_space.size();
  }

  metrics.temporal_stability = calculate_temporal_stability();
  metrics.prime_alignment = calculate_overall_prime_alignment();
  metrics.fractal_complexity = calculate_fractal_complexity();

  int64_t elapsed_ms = current_time_millis() - start_time;
  metrics.inference_rate =
      elapsed_ms > 0 ? (link_space.size() * 1000.0f / elapsed_ms) : 0.0f;

  metrics.consciousness_emergence = calculate_consciousness_emergence();

  return metrics;
}

float TimeCrystalKernel::calculate_overall_prime_alignment() const {
  if (atom_space.empty())
    return 0.0f;

  float total_alignment = 0.0f;
  for (const auto &[_, atom] : atom_space) {
    float atom_alignment = 0.0f;
    for (int prime : atom.prime_encoding) {
      if (is_fundamental_prime(prime)) {
        atom_alignment += 1.0f;
      }
    }
    if (!atom.prime_encoding.empty()) {
      total_alignment += atom_alignment / atom.prime_encoding.size();
    }
  }

  return total_alignment / atom_space.size();
}

float TimeCrystalKernel::calculate_fractal_complexity() const {
  if (time_crystals.empty())
    return 0.0f;

  float total_fractal_dim = 0.0f;
  for (const auto &[_, crystal] : time_crystals) {
    total_fractal_dim += crystal.fractal_dimension;
  }

  float avg = total_fractal_dim / time_crystals.size();
  return std::min(avg / config.time_crystal_dimensions, 1.0f);
}

float TimeCrystalKernel::calculate_temporal_stability() const {
  if (time_crystals.empty())
    return 0.0f;

  // Measure coherence variance as stability indicator
  std::vector<float> coherences;
  coherences.reserve(time_crystals.size());
  for (const auto &[_, crystal] : time_crystals) {
    coherences.push_back(crystal.temporal_coherence);
  }

  float avg = std::accumulate(coherences.begin(), coherences.end(), 0.0f) /
              coherences.size();
  float variance = 0.0f;
  for (float c : coherences) {
    variance += (c - avg) * (c - avg);
  }
  variance /= coherences.size();

  // Lower variance = higher stability
  return 1.0f - std::min(std::sqrt(variance) * 2.0f, 1.0f);
}

float TimeCrystalKernel::calculate_consciousness_emergence() const {
  // Consciousness emergence is a product of multiple factors
  NanoBrainMetrics m;
  m.quantum_coherence = 0.0f;
  m.prime_alignment = 0.0f;
  m.fractal_complexity = 0.0f;
  m.temporal_stability = 0.0f;

  if (!atom_space.empty()) {
    float total_coherence = 0.0f;
    for (const auto &[_, atom] : atom_space) {
      total_coherence += atom.time_crystal_state.temporal_coherence;
    }
    m.quantum_coherence = total_coherence / atom_space.size();
  }

  m.prime_alignment = calculate_overall_prime_alignment();
  m.fractal_complexity = calculate_fractal_complexity();
  m.temporal_stability = calculate_temporal_stability();

  // Consciousness emergence formula (geometric mean of factors)
  return std::pow(m.quantum_coherence * m.prime_alignment *
                      m.fractal_complexity * m.temporal_stability,
                  0.25f);
}

// ================================================================
// Processing Cycle
// ================================================================

void TimeCrystalKernel::process_cycle() {
  if (!active)
    return;

  // 1. Update time crystal quantum states
  update_time_crystal_states();

  // 2. Perform ECAN attention allocation with PPM weighting
  perform_attention_allocation();

  // 3. Execute PLN reasoning with fractal enhancement
  perform_pln_reasoning();

  // 4. Update geometric musical language resonances
  update_gml_resonances();

  cycle_count++;
}

// ================================================================
// GML Shape Tensor Operations (Task 2.1)
// ================================================================

int get_shape_complexity(GMLShape shape) {
  // Complexity based on geometric properties
  switch (shape) {
  case GMLShape::Point:
    return 1;
  case GMLShape::Line:
    return 2;
  case GMLShape::Triangle:
    return 3;
  case GMLShape::Square:
    return 4;
  case GMLShape::Pentagon:
    return 5;
  case GMLShape::Hexagon:
    return 6;
  case GMLShape::Circle:
    return 7;
  case GMLShape::Tetrahedron:
    return 8;
  case GMLShape::Cube:
    return 9;
  case GMLShape::Octahedron:
    return 10;
  case GMLShape::Sphere:
    return 11;
  case GMLShape::Torus:
    return 12;
  case GMLShape::Helix:
    return 13;
  case GMLShape::Dodecahedron:
    return 14;
  case GMLShape::Icosahedron:
    return 15;
  case GMLShape::Mobius:
    return 16;
  case GMLShape::Simplex:
    return 17;
  case GMLShape::Hypercube:
    return 18;
  case GMLShape::Fractal:
    return 19;
  default:
    return 1;
  }
}

int get_shape_prime(GMLShape shape) {
  // Map each shape to a prime number based on complexity
  static const int primes[] = {2,  3,  5,  7,  11, 13, 17, 19, 23, 29,
                               31, 37, 41, 43, 47, 53, 59, 61, 67};
  int complexity = get_shape_complexity(shape);
  return primes[std::min(complexity - 1, 18)];
}

std::vector<float> shape_to_tensor(GMLShape shape,
                                   const ShapeTensorParams &params) {
  int size = params.tensor_size;
  std::vector<float> tensor(size, 0.0f);

  int complexity = get_shape_complexity(shape);
  int shape_prime = get_shape_prime(shape);

  // Base encoding: shape type and complexity
  tensor[0] = static_cast<float>(static_cast<int>(shape)) / 19.0f;
  tensor[1] = static_cast<float>(complexity) / 19.0f;
  tensor[2] = static_cast<float>(shape_prime) / 67.0f;

  // Vertex encoding (if enabled)
  if (params.include_vertices && size >= 32) {
    // Generate vertex positions based on shape geometry
    int num_vertices = 0;
    switch (shape) {
    case GMLShape::Point:
      num_vertices = 1;
      break;
    case GMLShape::Line:
      num_vertices = 2;
      break;
    case GMLShape::Triangle:
      num_vertices = 3;
      break;
    case GMLShape::Square:
      num_vertices = 4;
      break;
    case GMLShape::Pentagon:
      num_vertices = 5;
      break;
    case GMLShape::Hexagon:
      num_vertices = 6;
      break;
    case GMLShape::Circle:
      num_vertices = 12;
      break;
    case GMLShape::Tetrahedron:
      num_vertices = 4;
      break;
    case GMLShape::Cube:
      num_vertices = 8;
      break;
    case GMLShape::Octahedron:
      num_vertices = 6;
      break;
    case GMLShape::Dodecahedron:
      num_vertices = 20;
      break;
    case GMLShape::Icosahedron:
      num_vertices = 12;
      break;
    default:
      num_vertices = complexity;
    }

    tensor[3] = static_cast<float>(num_vertices) / 20.0f;

    // Encode vertex positions using trigonometric patterns
    for (int i = 0; i < std::min(num_vertices, 12); i++) {
      float angle = 2.0f * M_PI * i / num_vertices;
      tensor[4 + i * 2] = std::cos(angle) * params.scale;
      tensor[5 + i * 2] = std::sin(angle) * params.scale;
    }
  }

  // Symmetry group encoding (if enabled)
  if (params.include_symmetry && size >= 48) {
    // Encode symmetry group based on shape
    int symmetry_order = 1;
    switch (shape) {
    case GMLShape::Circle:
    case GMLShape::Sphere:
      symmetry_order = 360;
      break;
    case GMLShape::Triangle:
    case GMLShape::Tetrahedron:
      symmetry_order = 3;
      break;
    case GMLShape::Square:
    case GMLShape::Cube:
      symmetry_order = 4;
      break;
    case GMLShape::Pentagon:
      symmetry_order = 5;
      break;
    case GMLShape::Hexagon:
      symmetry_order = 6;
      break;
    case GMLShape::Octahedron:
      symmetry_order = 8;
      break;
    case GMLShape::Dodecahedron:
      symmetry_order = 12;
      break;
    case GMLShape::Icosahedron:
      symmetry_order = 20;
      break;
    default:
      symmetry_order = complexity;
    }

    tensor[32] = static_cast<float>(symmetry_order) / 360.0f;
    tensor[33] =
        std::log2(symmetry_order + 1) / 10.0f; // Log symmetry complexity
  }

  // Prime harmonic encoding
  if (size >= 64) {
    for (int i = 0; i < 15; i++) {
      float harmonic =
          std::sin(FUNDAMENTAL_PRIMES[i] * complexity * M_PI / 100.0f);
      tensor[48 + i] = harmonic;
    }
  }

  return tensor;
}

ComposedShape compose_shapes(GMLShape primary, GMLShape secondary,
                             float blend_factor) {
  ComposedShape result;
  result.primary = primary;
  result.secondary = secondary;
  result.composition_strength = blend_factor;

  // Get tensors for both shapes
  ShapeTensorParams params;
  params.tensor_size = 64;
  std::vector<float> tensor_a = shape_to_tensor(primary, params);
  std::vector<float> tensor_b = shape_to_tensor(secondary, params);

  // Blend tensors
  result.composition_tensor.resize(params.tensor_size);
  for (int i = 0; i < params.tensor_size; i++) {
    result.composition_tensor[i] =
        tensor_a[i] * (1.0f - blend_factor) + tensor_b[i] * blend_factor;
  }

  // Combine primes
  int prime_a = get_shape_prime(primary);
  int prime_b = get_shape_prime(secondary);
  result.combined_primes = {prime_a, prime_b};

  // Add GCD and LCM primes
  int gcd = prime_a;
  int temp = prime_b;
  while (temp != 0) {
    int remainder = gcd % temp;
    gcd = temp;
    temp = remainder;
  }
  result.combined_primes.push_back(gcd);
  result.combined_primes.push_back((prime_a * prime_b) / gcd);

  return result;
}

std::vector<float> transform_shape(GMLShape shape, float rotation_angle,
                                   float scale_factor) {
  ShapeTensorParams params;
  params.scale = scale_factor;
  std::vector<float> tensor = shape_to_tensor(shape, params);

  // Apply rotation to vertex positions
  float cos_r = std::cos(rotation_angle);
  float sin_r = std::sin(rotation_angle);

  for (int i = 4; i < 28; i += 2) {
    float x = tensor[i];
    float y = tensor[i + 1];
    tensor[i] = x * cos_r - y * sin_r;
    tensor[i + 1] = x * sin_r + y * cos_r;
  }

  // Add rotation encoding
  if (tensor.size() >= 64) {
    tensor[34] = rotation_angle / (2.0f * M_PI);
    tensor[35] = scale_factor;
  }

  return tensor;
}

ShapePrimeResonance
calculate_shape_prime_resonance(GMLShape shape,
                                const std::vector<int> &prime_set) {
  ShapePrimeResonance result;
  result.shape = shape;
  result.resonance_strength = 0.0f;

  int shape_prime = get_shape_prime(shape);
  int complexity = get_shape_complexity(shape);

  // Calculate resonance with each prime in the set
  for (int prime : prime_set) {
    // Resonance based on harmonic relationship
    float harmonic_ratio =
        static_cast<float>(shape_prime) / static_cast<float>(prime);

    // Check for octave relationships (power of 2 ratios)
    float log_ratio = std::log2(harmonic_ratio);
    float octave_distance = std::abs(log_ratio - std::round(log_ratio));

    // Resonance is stronger when ratio is close to integer
    float resonance = 1.0f - octave_distance;

    if (resonance > 0.5f) {
      result.resonant_primes.push_back(prime);
      result.resonance_strength += resonance;
    }
  }

  // Normalize resonance
  if (!prime_set.empty()) {
    result.resonance_strength /= prime_set.size();
  }

  // Harmonic frequency based on shape complexity and prime
  result.harmonic_frequency = shape_prime * complexity * 0.1f;

  return result;
}

std::vector<std::pair<GMLShape, float>> get_shapes_for_prime(int prime) {
  std::vector<std::pair<GMLShape, float>> result;

  static const GMLShape all_shapes[] = {
      GMLShape::Point,      GMLShape::Line,         GMLShape::Triangle,
      GMLShape::Square,     GMLShape::Pentagon,     GMLShape::Hexagon,
      GMLShape::Circle,     GMLShape::Tetrahedron,  GMLShape::Cube,
      GMLShape::Octahedron, GMLShape::Sphere,       GMLShape::Torus,
      GMLShape::Helix,      GMLShape::Dodecahedron, GMLShape::Icosahedron,
      GMLShape::Mobius,     GMLShape::Simplex,      GMLShape::Hypercube,
      GMLShape::Fractal};

  for (GMLShape shape : all_shapes) {
    int shape_prime = get_shape_prime(shape);

    // Calculate resonance strength with the given prime
    float harmonic_ratio =
        static_cast<float>(shape_prime) / static_cast<float>(prime);
    float log_ratio = std::log2(harmonic_ratio);
    float octave_distance = std::abs(log_ratio - std::round(log_ratio));
    float resonance = 1.0f - octave_distance;

    if (resonance > 0.3f) {
      result.push_back({shape, resonance});
    }
  }

  // Sort by resonance strength
  std::sort(result.begin(), result.end(),
            [](const auto &a, const auto &b) { return a.second > b.second; });

  return result;
}

float shape_harmonic_distance(GMLShape a, GMLShape b) {
  if (a == b)
    return 0.0f;

  int prime_a = get_shape_prime(a);
  int prime_b = get_shape_prime(b);
  int complexity_a = get_shape_complexity(a);
  int complexity_b = get_shape_complexity(b);

  // Distance based on prime relationship
  float prime_distance = std::abs(
      std::log2(static_cast<float>(prime_a) / static_cast<float>(prime_b)));

  // Distance based on complexity difference
  float complexity_distance =
      std::abs(static_cast<float>(complexity_a - complexity_b)) / 19.0f;

  // Combined distance (weighted average)
  return (prime_distance / 6.0f + complexity_distance) / 2.0f;
}
