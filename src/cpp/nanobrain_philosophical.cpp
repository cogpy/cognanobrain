#include "nanobrain_philosophical.h"
#include <cmath>
#include <fstream>
#include <sstream>

// ================================================================
// PhilosophicalTransformationEngine Constructor
// ================================================================

PhilosophicalTransformationEngine::PhilosophicalTransformationEngine() {
  init_field_weights();
}

PhilosophicalTransformationEngine::PhilosophicalTransformationEngine(
    const ConsciousnessConfig &config)
    : config_(config) {
  init_field_weights();
}

void PhilosophicalTransformationEngine::init_field_weights() {
  // Initialize default weights for each research field
  // Weights reflect contribution to NanoBrain architecture
  field_weights_[ResearchField::Neuroscience] = 0.15f;
  field_weights_[ResearchField::QuantumPhysics] = 0.12f;
  field_weights_[ResearchField::ComputerScience] = 0.10f;
  field_weights_[ResearchField::Mathematics] = 0.18f;
  field_weights_[ResearchField::Philosophy] = 0.10f;
  field_weights_[ResearchField::Linguistics] = 0.08f;
  field_weights_[ResearchField::Psychology] = 0.07f;
  field_weights_[ResearchField::Biology] = 0.06f;
  field_weights_[ResearchField::Physics] = 0.08f;
  field_weights_[ResearchField::InformationTheory] = 0.06f;
}

// ================================================================
// Configuration
// ================================================================

void PhilosophicalTransformationEngine::set_config(
    const ConsciousnessConfig &config) {
  if (config.validate()) {
    config_ = config;
  }
}

bool PhilosophicalTransformationEngine::load_config_from_json(
    const std::string &filename) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    return false;
  }

  // Simple JSON parsing for configuration
  std::stringstream buffer;
  buffer << file.rdbuf();
  std::string content = buffer.str();

  // Parse key numeric values (simple implementation)
  auto find_value = [&content](const std::string &key) -> float {
    size_t pos = content.find("\"" + key + "\"");
    if (pos == std::string::npos)
      return -1.0f;
    pos = content.find(":", pos);
    if (pos == std::string::npos)
      return -1.0f;
    pos++;
    while (pos < content.size() &&
           (content[pos] == ' ' || content[pos] == '\t'))
      pos++;
    std::string num;
    while (pos < content.size() &&
           (std::isdigit(content[pos]) || content[pos] == '.' ||
            content[pos] == '-')) {
      num += content[pos++];
    }
    return num.empty() ? -1.0f : std::stof(num);
  };

  float val;
  if ((val = find_value("consciousness_threshold")) >= 0)
    config_.consciousness_threshold = val;
  if ((val = find_value("self_awareness_threshold")) >= 0)
    config_.self_awareness_threshold = val;
  if ((val = find_value("meta_cognition_threshold")) >= 0)
    config_.meta_cognition_threshold = val;
  if ((val = find_value("prime_symmetry_depth")) >= 0)
    config_.prime_symmetry_depth = static_cast<int>(val);
  if ((val = find_value("temporal_coherence_min")) >= 0)
    config_.temporal_coherence_min = val;
  if ((val = find_value("charge_storage_capacity")) >= 0)
    config_.charge_storage_capacity = val;
  if ((val = find_value("crystal_formation_rate")) >= 0)
    config_.crystal_formation_rate = val;
  if ((val = find_value("arrangement_creativity")) >= 0)
    config_.arrangement_creativity = val;

  return config_.validate();
}

bool PhilosophicalTransformationEngine::save_config_to_json(
    const std::string &filename) const {
  std::ofstream file(filename);
  if (!file.is_open()) {
    return false;
  }

  file << "{\n";
  file << "  \"consciousness_threshold\": " << config_.consciousness_threshold
       << ",\n";
  file << "  \"self_awareness_threshold\": " << config_.self_awareness_threshold
       << ",\n";
  file << "  \"meta_cognition_threshold\": " << config_.meta_cognition_threshold
       << ",\n";
  file << "  \"prime_symmetry_depth\": " << config_.prime_symmetry_depth
       << ",\n";
  file << "  \"temporal_coherence_min\": " << config_.temporal_coherence_min
       << ",\n";
  file << "  \"charge_storage_capacity\": " << config_.charge_storage_capacity
       << ",\n";
  file << "  \"crystal_formation_rate\": " << config_.crystal_formation_rate
       << ",\n";
  file << "  \"max_prime_combinations\": " << config_.max_prime_combinations
       << ",\n";
  file << "  \"arrangement_creativity\": " << config_.arrangement_creativity
       << ",\n";
  file << "  \"active_model\": " << static_cast<int>(config_.active_model)
       << "\n";
  file << "}\n";

  return true;
}

// ================================================================
// Brain Model Operations
// ================================================================

float PhilosophicalTransformationEngine::compare_models(
    BrainModel model1, BrainModel model2) const {
  BrainModelInfo info1 = get_brain_model_info(model1);
  BrainModelInfo info2 = get_brain_model_info(model2);

  // Compute weighted comparison score
  // Positive = model1 better, Negative = model2 better
  float score = 0.0f;

  // Higher consciousness score is better
  score += (info1.consciousness_score - info2.consciousness_score) * 0.4f;

  // Higher temporal resolution is better
  score += (info1.temporal_resolution - info2.temporal_resolution) * 0.3f;

  // Lower Turing alignment is better for NanoBrain philosophy
  score += (info2.turing_alignment - info1.turing_alignment) * 0.2f;

  // More primes in signature is better
  float prime_ratio =
      static_cast<float>(info1.prime_signature.size()) /
      std::max(1.0f, static_cast<float>(info2.prime_signature.size()));
  score += (prime_ratio - 1.0f) * 0.1f;

  return score;
}

std::vector<BrainModelInfo>
PhilosophicalTransformationEngine::get_all_model_info() const {
  std::vector<BrainModelInfo> results;
  results.push_back(get_brain_model_info(BrainModel::HodgkinHuxley));
  results.push_back(get_brain_model_info(BrainModel::IntegrateAndFire));
  results.push_back(get_brain_model_info(BrainModel::Connectome));
  results.push_back(get_brain_model_info(BrainModel::GlobalWorkspace));
  results.push_back(get_brain_model_info(BrainModel::IntegratedInformation));
  results.push_back(get_brain_model_info(BrainModel::PredictiveCoding));
  results.push_back(get_brain_model_info(BrainModel::FreeEnergy));
  results.push_back(get_brain_model_info(BrainModel::Oscillatory));
  results.push_back(get_brain_model_info(BrainModel::Quantum));
  results.push_back(get_brain_model_info(BrainModel::TimeCrystal));
  return results;
}

// ================================================================
// Consciousness Metrics
// ================================================================

float PhilosophicalTransformationEngine::calculate_consciousness_emergence(
    float coherence, float awareness, float integration) const {
  // Clamp inputs
  coherence = std::max(0.0f, std::min(1.0f, coherence));
  awareness = std::max(0.0f, std::min(1.0f, awareness));
  integration = std::max(0.0f, std::min(1.0f, integration));

  // Get active model info for weights
  BrainModelInfo model_info = get_brain_model_info(config_.active_model);

  // Base emergence from weighted combination
  float base_emergence =
      0.35f * coherence + 0.35f * awareness + 0.30f * integration;

  // Model-specific adjustment
  float model_factor = model_info.consciousness_score;

  // Prime coherence bonus (more primes = higher potential)
  float prime_bonus = 0.0f;
  auto symmetry_primes = config_.get_symmetry_primes();
  if (!symmetry_primes.empty()) {
    for (int p : symmetry_primes) {
      prime_bonus += 1.0f / static_cast<float>(p);
    }
    prime_bonus = std::min(0.2f, prime_bonus / 10.0f);
  }

  // Temporal coherence threshold gate
  float temporal_gate =
      coherence >= config_.temporal_coherence_min ? 1.0f : 0.5f;

  // Final emergence calculation
  float emergence = base_emergence * model_factor * temporal_gate + prime_bonus;

  return std::max(0.0f, std::min(1.0f, emergence));
}

// ================================================================
// Research Field Integration
// ================================================================

float PhilosophicalTransformationEngine::get_field_weight(
    ResearchField field) const {
  auto it = field_weights_.find(field);
  return it != field_weights_.end() ? it->second : 0.0f;
}

void PhilosophicalTransformationEngine::set_field_weight(ResearchField field,
                                                         float weight) {
  field_weights_[field] = std::max(0.0f, std::min(1.0f, weight));
}

AtomeseKnowledgeBase
PhilosophicalTransformationEngine::generate_field_integration_kb() const {
  AtomeseParser parser;
  AtomeseKnowledgeBase kb;
  kb.description =
      "Research Field Integration - 10 Fields of Consciousness Science";

  // Root concept
  auto unified_science =
      parser.concept_node("UnifiedConsciousnessScience", 1.0f, 1.0f);

  // Create nodes for each research field
  std::vector<std::shared_ptr<AtomeseExpression>> field_nodes;
  for (int i = 0; i < 10; ++i) {
    ResearchField field = static_cast<ResearchField>(i);
    std::string name = research_field_name(field);
    float weight = get_field_weight(field);
    auto node = parser.concept_node(name, weight, 0.9f);
    field_nodes.push_back(node);

    // Link to unified science
    kb.expressions.push_back(
        parser.inheritance_link(node, unified_science, weight, 0.9f));
  }

  // Create cross-field similarity links for related fields
  // Mathematics <-> Physics
  kb.expressions.push_back(
      parser.similarity_link(field_nodes[3], field_nodes[8], 0.85f, 0.9f));
  // Neuroscience <-> Biology
  kb.expressions.push_back(
      parser.similarity_link(field_nodes[0], field_nodes[7], 0.9f, 0.95f));
  // Philosophy <-> Psychology
  kb.expressions.push_back(
      parser.similarity_link(field_nodes[4], field_nodes[6], 0.75f, 0.85f));
  // Quantum Physics <-> Mathematics
  kb.expressions.push_back(
      parser.similarity_link(field_nodes[1], field_nodes[3], 0.8f, 0.9f));
  // Computer Science <-> Information Theory
  kb.expressions.push_back(
      parser.similarity_link(field_nodes[2], field_nodes[9], 0.85f, 0.9f));
  // Linguistics <-> Philosophy
  kb.expressions.push_back(
      parser.similarity_link(field_nodes[5], field_nodes[4], 0.7f, 0.85f));

  return kb;
}

// ================================================================
// Linguistic Wheel Operations
// ================================================================

LinguisticWheel PhilosophicalTransformationEngine::create_wheel_from_concept(
    const std::string &concept) const {
  LinguisticWheel wheel;

  // Hash the concept name to generate deterministic wheel position
  size_t hash = std::hash<std::string>{}(concept);

  // Use hash bits for spatial position
  wheel.space_x = static_cast<float>((hash & 0xFF)) / 255.0f * 2.0f - 1.0f;
  wheel.space_y = static_cast<float>((hash >> 8) & 0xFF) / 255.0f * 2.0f - 1.0f;
  wheel.space_z =
      static_cast<float>((hash >> 16) & 0xFF) / 255.0f * 2.0f - 1.0f;

  // Temporal components from higher hash bits
  wheel.time_phase =
      static_cast<float>((hash >> 24) & 0xFF) / 255.0f * 6.28318f;
  wheel.time_frequency =
      1.0f + static_cast<float>((hash >> 32) & 0xFF) / 255.0f * 2.0f;

  // Imaginary components
  wheel.imaginary_1 =
      static_cast<float>((hash >> 40) & 0xFF) / 255.0f * 2.0f - 1.0f;
  wheel.imaginary_2 =
      static_cast<float>((hash >> 48) & 0xFF) / 255.0f * 2.0f - 1.0f;
  wheel.imaginary_3 =
      static_cast<float>((hash >> 56) & 0x0F) / 15.0f * 2.0f - 1.0f;
  wheel.imaginary_4 =
      static_cast<float>((hash >> 60) & 0x0F) / 15.0f * 2.0f - 1.0f;

  // Prime encoding based on concept length
  size_t len = concept.size();
  static const std::vector<int> primes = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29};
  wheel.prime_encoding.clear();
  for (size_t i = 0; i < std::min(len, primes.size()); ++i) {
    wheel.prime_encoding.push_back(primes[i]);
  }
  if (wheel.prime_encoding.empty()) {
    wheel.prime_encoding = {2, 3, 5};
  }

  return wheel;
}

LinguisticWheel PhilosophicalTransformationEngine::translate_wheel(
    const LinguisticWheel &source, const LinguisticWheel &target,
    float interpolation) const {
  float t = std::max(0.0f, std::min(1.0f, interpolation));
  float s = 1.0f - t;

  LinguisticWheel result;
  result.space_x = source.space_x * s + target.space_x * t;
  result.space_y = source.space_y * s + target.space_y * t;
  result.space_z = source.space_z * s + target.space_z * t;

  // Interpolate phase with circular awareness
  float phase_diff = target.time_phase - source.time_phase;
  if (phase_diff > 3.14159f)
    phase_diff -= 6.28318f;
  if (phase_diff < -3.14159f)
    phase_diff += 6.28318f;
  result.time_phase = source.time_phase + phase_diff * t;

  result.time_frequency = source.time_frequency * s + target.time_frequency * t;

  result.imaginary_1 = source.imaginary_1 * s + target.imaginary_1 * t;
  result.imaginary_2 = source.imaginary_2 * s + target.imaginary_2 * t;
  result.imaginary_3 = source.imaginary_3 * s + target.imaginary_3 * t;
  result.imaginary_4 = source.imaginary_4 * s + target.imaginary_4 * t;

  // Use source primes for first half, target for second half
  result.prime_encoding =
      t < 0.5f ? source.prime_encoding : target.prime_encoding;

  return result;
}

LinguisticWheel PhilosophicalTransformationEngine::compose_wheels(
    const std::vector<LinguisticWheel> &wheels) const {
  if (wheels.empty()) {
    return LinguisticWheel();
  }
  if (wheels.size() == 1) {
    return wheels[0];
  }

  LinguisticWheel result;
  float n = static_cast<float>(wheels.size());

  for (const auto &w : wheels) {
    result.space_x += w.space_x / n;
    result.space_y += w.space_y / n;
    result.space_z += w.space_z / n;
    result.time_frequency += w.time_frequency / n;
    result.imaginary_1 += w.imaginary_1 / n;
    result.imaginary_2 += w.imaginary_2 / n;
    result.imaginary_3 += w.imaginary_3 / n;
    result.imaginary_4 += w.imaginary_4 / n;
  }

  // Average phase using circular mean
  float sin_sum = 0.0f, cos_sum = 0.0f;
  for (const auto &w : wheels) {
    sin_sum += std::sin(w.time_phase);
    cos_sum += std::cos(w.time_phase);
  }
  result.time_phase = std::atan2(sin_sum, cos_sum);

  // Merge prime encodings (union of all, limited to 15)
  std::vector<int> merged_primes;
  for (const auto &w : wheels) {
    for (int p : w.prime_encoding) {
      bool found = false;
      for (int mp : merged_primes) {
        if (mp == p) {
          found = true;
          break;
        }
      }
      if (!found && merged_primes.size() < 15) {
        merged_primes.push_back(p);
      }
    }
  }
  std::sort(merged_primes.begin(), merged_primes.end());
  result.prime_encoding = merged_primes;

  return result;
}

// ================================================================
// Knowledge Base Generation
// ================================================================

AtomeseKnowledgeBase
PhilosophicalTransformationEngine::generate_extended_philosophical_kb() const {
  AtomeseParser parser;
  AtomeseKnowledgeBase kb;
  kb.description = "Extended Philosophical Transformation - Chapter 1 Complete";

  // ----------------------------------------------------------------
  // Section 1.1-1.3: Worldview Transformation
  // ----------------------------------------------------------------

  auto worldview = parser.concept_node("NanoBrainWorldview", 1.0f, 1.0f);
  auto turing_paradigm = parser.concept_node("TuringParadigm", 0.5f, 0.9f);
  auto time_crystal_paradigm =
      parser.concept_node("TimeCrystalParadigm", 0.95f, 0.95f);

  // NanoBrain differs from Turing paradigm
  kb.expressions.push_back(
      parser.inheritance_link(time_crystal_paradigm, worldview, 0.95f, 0.95f));

  // Turing paradigm is limited
  auto turing_limitation = parser.concept_node("TuringLimitation", 0.8f, 0.9f);
  kb.expressions.push_back(
      parser.implication_link(turing_paradigm, turing_limitation, 0.85f, 0.9f));

  // Universe within, not side by side
  auto universe_within = parser.concept_node("UniverseWithin", 0.9f, 0.9f);
  kb.expressions.push_back(parser.inheritance_link(
      universe_within, time_crystal_paradigm, 0.9f, 0.9f));

  // ----------------------------------------------------------------
  // Section 1.4: 10 Brain Models
  // ----------------------------------------------------------------

  auto brain_models = parser.concept_node("BrainModels", 1.0f, 1.0f);
  auto models = get_all_model_info();

  for (const auto &model : models) {
    auto model_node = parser.concept_node(model.name, model.consciousness_score,
                                          1.0f - model.turing_alignment);
    kb.expressions.push_back(
        parser.inheritance_link(model_node, brain_models, 0.9f, 0.95f));

    // Best model is time crystal
    if (model.model == BrainModel::TimeCrystal) {
      kb.expressions.push_back(parser.inheritance_link(
          model_node, time_crystal_paradigm, 0.95f, 0.95f));
    }
  }

  // ----------------------------------------------------------------
  // Section 1.5: Different Tapes (Turing vs Fractal)
  // ----------------------------------------------------------------

  auto turing_tape = parser.concept_node("TuringTape", 0.6f, 0.9f);
  auto fractal_tape = parser.concept_node("FractalTape", 0.95f, 0.95f);

  kb.expressions.push_back(
      parser.inheritance_link(turing_tape, turing_paradigm, 0.9f, 0.95f));
  kb.expressions.push_back(parser.inheritance_link(
      fractal_tape, time_crystal_paradigm, 0.95f, 0.95f));

  // Fractal tape supersedes Turing tape
  kb.expressions.push_back(parser.implication_link(
      fractal_tape, parser.not_link(turing_limitation), 0.8f, 0.85f));

  // ----------------------------------------------------------------
  // Section 1.10: Three Concepts Defining Artificial Brain
  // ----------------------------------------------------------------

  auto artificial_brain = parser.concept_node("ArtificialBrain", 1.0f, 1.0f);

  // Concept 1: Time crystal language written by prime symmetry
  auto time_crystal_language =
      parser.concept_node("TimeCrystalLanguage", 0.95f, 0.95f);
  auto prime_symmetry = parser.concept_node("PrimeSymmetry", 0.95f, 0.95f);
  kb.expressions.push_back(parser.inheritance_link(
      time_crystal_language, artificial_brain, 0.95f, 0.95f));
  kb.expressions.push_back(parser.implication_link(
      prime_symmetry, time_crystal_language, 0.9f, 0.9f));

  // Concept 2: Magnetic light device storing charge
  auto magnetic_light = parser.concept_node("MagneticLight", 0.9f, 0.9f);
  auto charge_storage = parser.concept_node("ChargeStorage", 0.85f, 0.9f);
  kb.expressions.push_back(
      parser.inheritance_link(magnetic_light, artificial_brain, 0.9f, 0.9f));
  kb.expressions.push_back(
      parser.implication_link(charge_storage, magnetic_light, 0.85f, 0.9f));

  // Concept 3: Pattern of prime arrangements
  auto prime_pattern =
      parser.concept_node("PrimeArrangementPattern", 0.95f, 0.95f);
  kb.expressions.push_back(
      parser.inheritance_link(prime_pattern, artificial_brain, 0.95f, 0.95f));

  // ----------------------------------------------------------------
  // Consciousness Thresholds
  // ----------------------------------------------------------------

  auto consciousness = parser.concept_node("Consciousness", 0.9f, 0.95f);
  auto threshold_concept = parser.concept_node(
      "ConsciousnessThreshold", config_.consciousness_threshold, 0.95f);
  kb.expressions.push_back(parser.implication_link(
      threshold_concept, consciousness, config_.consciousness_threshold, 0.9f));

  // ----------------------------------------------------------------
  // Research Field Integration
  // ----------------------------------------------------------------

  auto field_kb = generate_field_integration_kb();
  kb.expressions.insert(kb.expressions.end(), field_kb.expressions.begin(),
                        field_kb.expressions.end());

  return kb;
}
