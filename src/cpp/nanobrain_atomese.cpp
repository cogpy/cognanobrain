#include "nanobrain_atomese.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <stdexcept>

// ================================================================
// AtomeseExpression Methods
// ================================================================

bool AtomeseExpression::is_node() const {
  return AtomeseParser::is_node_type(type);
}

bool AtomeseExpression::is_link() const {
  return AtomeseParser::is_link_type(type);
}

std::string AtomeseExpression::type_name() const {
  return AtomeseParser::type_to_string(type);
}

// ================================================================
// AtomeseParser Constructor
// ================================================================

AtomeseParser::AtomeseParser() : current_pos(0) {}

// ================================================================
// Type Conversion
// ================================================================

AtomeseType AtomeseParser::string_to_type(const std::string &type_name) {
  static std::map<std::string, AtomeseType> type_map = {
      {"ConceptNode", AtomeseType::ConceptNode},
      {"PredicateNode", AtomeseType::PredicateNode},
      {"NumberNode", AtomeseType::NumberNode},
      {"VariableNode", AtomeseType::VariableNode},
      {"SchemaNode", AtomeseType::SchemaNode},
      {"GroundedSchemaNode", AtomeseType::GroundedSchemaNode},
      {"TypeNode", AtomeseType::TypeNode},
      {"AnchorNode", AtomeseType::AnchorNode},
      {"ListLink", AtomeseType::ListLink},
      {"SetLink", AtomeseType::SetLink},
      {"AndLink", AtomeseType::AndLink},
      {"OrLink", AtomeseType::OrLink},
      {"NotLink", AtomeseType::NotLink},
      {"InheritanceLink", AtomeseType::InheritanceLink},
      {"SimilarityLink", AtomeseType::SimilarityLink},
      {"ImplicationLink", AtomeseType::ImplicationLink},
      {"EvaluationLink", AtomeseType::EvaluationLink},
      {"ExecutionLink", AtomeseType::ExecutionLink},
      {"BindLink", AtomeseType::BindLink},
      {"MemberLink", AtomeseType::MemberLink},
      {"ContextLink", AtomeseType::ContextLink},
      {"DefineLink", AtomeseType::DefineLink},
      {"LambdaLink", AtomeseType::LambdaLink},
      {"PutLink", AtomeseType::PutLink},
      {"GetLink", AtomeseType::GetLink},
      {"EquivalenceLink", AtomeseType::EquivalenceLink},
      {"SatisfactionLink", AtomeseType::SatisfactionLink},
      {"StateLink", AtomeseType::StateLink},
      {"AtTimeLink", AtomeseType::AtTimeLink}};

  auto it = type_map.find(type_name);
  if (it != type_map.end()) {
    return it->second;
  }
  return AtomeseType::Unknown;
}

std::string AtomeseParser::type_to_string(AtomeseType type) {
  static std::map<AtomeseType, std::string> type_map = {
      {AtomeseType::ConceptNode, "ConceptNode"},
      {AtomeseType::PredicateNode, "PredicateNode"},
      {AtomeseType::NumberNode, "NumberNode"},
      {AtomeseType::VariableNode, "VariableNode"},
      {AtomeseType::SchemaNode, "SchemaNode"},
      {AtomeseType::GroundedSchemaNode, "GroundedSchemaNode"},
      {AtomeseType::TypeNode, "TypeNode"},
      {AtomeseType::AnchorNode, "AnchorNode"},
      {AtomeseType::ListLink, "ListLink"},
      {AtomeseType::SetLink, "SetLink"},
      {AtomeseType::AndLink, "AndLink"},
      {AtomeseType::OrLink, "OrLink"},
      {AtomeseType::NotLink, "NotLink"},
      {AtomeseType::InheritanceLink, "InheritanceLink"},
      {AtomeseType::SimilarityLink, "SimilarityLink"},
      {AtomeseType::ImplicationLink, "ImplicationLink"},
      {AtomeseType::EvaluationLink, "EvaluationLink"},
      {AtomeseType::ExecutionLink, "ExecutionLink"},
      {AtomeseType::BindLink, "BindLink"},
      {AtomeseType::MemberLink, "MemberLink"},
      {AtomeseType::ContextLink, "ContextLink"},
      {AtomeseType::DefineLink, "DefineLink"},
      {AtomeseType::LambdaLink, "LambdaLink"},
      {AtomeseType::PutLink, "PutLink"},
      {AtomeseType::GetLink, "GetLink"},
      {AtomeseType::EquivalenceLink, "EquivalenceLink"},
      {AtomeseType::SatisfactionLink, "SatisfactionLink"},
      {AtomeseType::StateLink, "StateLink"},
      {AtomeseType::AtTimeLink, "AtTimeLink"},
      {AtomeseType::Unknown, "Unknown"}};

  auto it = type_map.find(type);
  if (it != type_map.end()) {
    return it->second;
  }
  return "Unknown";
}

bool AtomeseParser::is_node_type(AtomeseType type) {
  return type == AtomeseType::ConceptNode ||
         type == AtomeseType::PredicateNode ||
         type == AtomeseType::NumberNode || type == AtomeseType::VariableNode ||
         type == AtomeseType::SchemaNode ||
         type == AtomeseType::GroundedSchemaNode ||
         type == AtomeseType::TypeNode || type == AtomeseType::AnchorNode;
}

bool AtomeseParser::is_link_type(AtomeseType type) {
  return !is_node_type(type) && type != AtomeseType::Unknown;
}

// ================================================================
// Parsing Helpers
// ================================================================

void AtomeseParser::skip_whitespace() {
  while (current_pos < current_input.size()) {
    char c = current_input[current_pos];
    if (c == ' ' || c == '\t' || c == '\n' || c == '\r') {
      current_pos++;
    } else if (c == ';') {
      // Skip comment until end of line
      while (current_pos < current_input.size() &&
             current_input[current_pos] != '\n') {
        current_pos++;
      }
    } else {
      break;
    }
  }
}

bool AtomeseParser::match(char c) {
  skip_whitespace();
  if (current_pos < current_input.size() && current_input[current_pos] == c) {
    current_pos++;
    return true;
  }
  return false;
}

bool AtomeseParser::match(const std::string &s) {
  skip_whitespace();
  if (current_pos + s.size() <= current_input.size() &&
      current_input.substr(current_pos, s.size()) == s) {
    current_pos += s.size();
    return true;
  }
  return false;
}

bool AtomeseParser::peek(char c) const {
  size_t pos = current_pos;
  while (pos < current_input.size()) {
    char ch = current_input[pos];
    if (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r') {
      pos++;
    } else {
      return ch == c;
    }
  }
  return false;
}

std::string AtomeseParser::read_symbol() {
  skip_whitespace();
  std::string symbol;
  while (current_pos < current_input.size()) {
    char c = current_input[current_pos];
    if (std::isalnum(c) || c == '_' || c == '-' || c == ':' || c == '$') {
      symbol += c;
      current_pos++;
    } else {
      break;
    }
  }
  return symbol;
}

std::string AtomeseParser::read_string() {
  skip_whitespace();
  std::string result;

  if (match('"')) {
    while (current_pos < current_input.size() &&
           current_input[current_pos] != '"') {
      if (current_input[current_pos] == '\\' &&
          current_pos + 1 < current_input.size()) {
        current_pos++;
        result += current_input[current_pos];
      } else {
        result += current_input[current_pos];
      }
      current_pos++;
    }
    match('"');
  } else {
    result = read_symbol();
  }

  return result;
}

float AtomeseParser::read_number() {
  skip_whitespace();
  std::string num_str;
  bool has_dot = false;
  bool has_minus = false;

  if (current_pos < current_input.size() && current_input[current_pos] == '-') {
    num_str += '-';
    current_pos++;
    has_minus = true;
  }

  while (current_pos < current_input.size()) {
    char c = current_input[current_pos];
    if (std::isdigit(c)) {
      num_str += c;
      current_pos++;
    } else if (c == '.' && !has_dot) {
      num_str += c;
      current_pos++;
      has_dot = true;
    } else {
      break;
    }
  }

  return num_str.empty() ? 0.0f : std::stof(num_str);
}

// ================================================================
// Parsing
// ================================================================

AtomeseParseResult AtomeseParser::parse(const std::string &input) {
  current_input = input;
  current_pos = 0;

  AtomeseParseResult result;
  result.success = false;
  result.error_position = 0;

  try {
    skip_whitespace();
    result.expression = parse_expression();
    result.success = (result.expression != nullptr);
  } catch (const std::exception &e) {
    result.error_message = e.what();
    result.error_position = current_pos;
  }

  return result;
}

std::vector<AtomeseParseResult>
AtomeseParser::parse_all(const std::string &input) {
  std::vector<AtomeseParseResult> results;
  current_input = input;
  current_pos = 0;

  while (current_pos < current_input.size()) {
    skip_whitespace();
    if (current_pos >= current_input.size())
      break;

    AtomeseParseResult result;
    result.error_position = current_pos;

    try {
      result.expression = parse_expression();
      result.success = (result.expression != nullptr);
    } catch (const std::exception &e) {
      result.error_message = e.what();
      result.success = false;
    }

    if (!result.success)
      break;
    results.push_back(result);
  }

  return results;
}

AtomeseParseResult AtomeseParser::parse_file(const std::string &filename) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    AtomeseParseResult result;
    result.success = false;
    result.error_message = "Could not open file: " + filename;
    return result;
  }

  std::stringstream buffer;
  buffer << file.rdbuf();
  return parse(buffer.str());
}

std::shared_ptr<AtomeseExpression> AtomeseParser::parse_expression() {
  skip_whitespace();

  if (!match('(')) {
    return nullptr;
  }

  std::string type_name = read_symbol();
  AtomeseType type = string_to_type(type_name);

  auto expr = std::make_shared<AtomeseExpression>();
  expr->type = type;

  if (is_node_type(type)) {
    // Parse node name
    expr->name = read_string();

    // Check for optional truth value (stv strength confidence)
    skip_whitespace();
    if (peek('(')) {
      match('(');
      std::string tv_type = read_symbol();
      if (tv_type == "stv" || tv_type == "SimpleTruthValue") {
        float strength = read_number();
        float confidence = read_number();
        expr->truth_value.strength = strength;
        expr->truth_value.confidence = confidence;
      }
      match(')');
    }
  } else if (is_link_type(type)) {
    // Parse child expressions
    skip_whitespace();
    while (!peek(')') && current_pos < current_input.size()) {
      // Check for truth value
      if (peek('(')) {
        size_t saved_pos = current_pos;
        match('(');
        std::string possible_tv = read_symbol();
        if (possible_tv == "stv" || possible_tv == "SimpleTruthValue") {
          float strength = read_number();
          float confidence = read_number();
          expr->truth_value.strength = strength;
          expr->truth_value.confidence = confidence;
          match(')');
          continue;
        } else {
          // Restore position and parse as child expression
          current_pos = saved_pos;
        }
      }

      auto child = parse_expression();
      if (child) {
        expr->children.push_back(child);
      }
      skip_whitespace();
    }
  }

  if (!match(')')) {
    throw std::runtime_error("Expected ')' at position " +
                             std::to_string(current_pos));
  }

  return expr;
}

// ================================================================
// Generation
// ================================================================

std::string AtomeseParser::to_scheme_string(const AtomeseExpression &expr,
                                            int indent) const {
  std::stringstream ss;
  std::string indent_str(indent * 2, ' ');

  ss << indent_str << "(" << type_to_string(expr.type);

  if (expr.is_node()) {
    ss << " \"" << expr.name << "\"";
    if (expr.truth_value.strength < 1.0f ||
        expr.truth_value.confidence < 1.0f) {
      ss << " (stv " << expr.truth_value.strength << " "
         << expr.truth_value.confidence << ")";
    }
    ss << ")";
  } else {
    if (expr.truth_value.strength < 1.0f ||
        expr.truth_value.confidence < 1.0f) {
      ss << " (stv " << expr.truth_value.strength << " "
         << expr.truth_value.confidence << ")";
    }
    ss << "\n";
    for (const auto &child : expr.children) {
      ss << to_scheme_string(*child, indent + 1) << "\n";
    }
    ss << indent_str << ")";
  }

  return ss.str();
}

std::string
AtomeseParser::to_sexpr_string(const AtomeseExpression &expr) const {
  std::stringstream ss;
  ss << "(" << type_to_string(expr.type);

  if (expr.is_node()) {
    ss << " \"" << expr.name << "\")";
  } else {
    for (const auto &child : expr.children) {
      ss << " " << to_sexpr_string(*child);
    }
    ss << ")";
  }

  return ss.str();
}

std::string
AtomeseParser::generate_scheme_output(const AtomeseKnowledgeBase &kb) const {
  std::stringstream ss;

  ss << ";; " << kb.description << "\n";
  ss << ";; Generated by NanoBrain Atomese Parser\n\n";

  for (const auto &expr : kb.expressions) {
    ss << to_scheme_string(*expr) << "\n\n";
  }

  return ss.str();
}

// ================================================================
// Expression Creation Helpers
// ================================================================

std::shared_ptr<AtomeseExpression>
AtomeseParser::concept_node(const std::string &name, float strength,
                            float confidence) {
  auto expr = std::make_shared<AtomeseExpression>();
  expr->type = AtomeseType::ConceptNode;
  expr->name = name;
  expr->truth_value.strength = strength;
  expr->truth_value.confidence = confidence;
  return expr;
}

std::shared_ptr<AtomeseExpression>
AtomeseParser::predicate_node(const std::string &name, float strength,
                              float confidence) {
  auto expr = std::make_shared<AtomeseExpression>();
  expr->type = AtomeseType::PredicateNode;
  expr->name = name;
  expr->truth_value.strength = strength;
  expr->truth_value.confidence = confidence;
  return expr;
}

std::shared_ptr<AtomeseExpression> AtomeseParser::number_node(float value) {
  auto expr = std::make_shared<AtomeseExpression>();
  expr->type = AtomeseType::NumberNode;
  expr->name = std::to_string(value);
  return expr;
}

std::shared_ptr<AtomeseExpression>
AtomeseParser::variable_node(const std::string &name) {
  auto expr = std::make_shared<AtomeseExpression>();
  expr->type = AtomeseType::VariableNode;
  expr->name = name;
  return expr;
}

std::shared_ptr<AtomeseExpression>
AtomeseParser::inheritance_link(std::shared_ptr<AtomeseExpression> child,
                                std::shared_ptr<AtomeseExpression> parent,
                                float strength, float confidence) {
  auto expr = std::make_shared<AtomeseExpression>();
  expr->type = AtomeseType::InheritanceLink;
  expr->children.push_back(child);
  expr->children.push_back(parent);
  expr->truth_value.strength = strength;
  expr->truth_value.confidence = confidence;
  return expr;
}

std::shared_ptr<AtomeseExpression>
AtomeseParser::similarity_link(std::shared_ptr<AtomeseExpression> a,
                               std::shared_ptr<AtomeseExpression> b,
                               float strength, float confidence) {
  auto expr = std::make_shared<AtomeseExpression>();
  expr->type = AtomeseType::SimilarityLink;
  expr->children.push_back(a);
  expr->children.push_back(b);
  expr->truth_value.strength = strength;
  expr->truth_value.confidence = confidence;
  return expr;
}

std::shared_ptr<AtomeseExpression>
AtomeseParser::implication_link(std::shared_ptr<AtomeseExpression> antecedent,
                                std::shared_ptr<AtomeseExpression> consequent,
                                float strength, float confidence) {
  auto expr = std::make_shared<AtomeseExpression>();
  expr->type = AtomeseType::ImplicationLink;
  expr->children.push_back(antecedent);
  expr->children.push_back(consequent);
  expr->truth_value.strength = strength;
  expr->truth_value.confidence = confidence;
  return expr;
}

std::shared_ptr<AtomeseExpression>
AtomeseParser::evaluation_link(std::shared_ptr<AtomeseExpression> predicate,
                               std::shared_ptr<AtomeseExpression> arguments,
                               float strength, float confidence) {
  auto expr = std::make_shared<AtomeseExpression>();
  expr->type = AtomeseType::EvaluationLink;
  expr->children.push_back(predicate);
  expr->children.push_back(arguments);
  expr->truth_value.strength = strength;
  expr->truth_value.confidence = confidence;
  return expr;
}

std::shared_ptr<AtomeseExpression> AtomeseParser::list_link(
    const std::vector<std::shared_ptr<AtomeseExpression>> &elements) {
  auto expr = std::make_shared<AtomeseExpression>();
  expr->type = AtomeseType::ListLink;
  expr->children = elements;
  return expr;
}

std::shared_ptr<AtomeseExpression> AtomeseParser::and_link(
    const std::vector<std::shared_ptr<AtomeseExpression>> &elements) {
  auto expr = std::make_shared<AtomeseExpression>();
  expr->type = AtomeseType::AndLink;
  expr->children = elements;
  return expr;
}

std::shared_ptr<AtomeseExpression> AtomeseParser::or_link(
    const std::vector<std::shared_ptr<AtomeseExpression>> &elements) {
  auto expr = std::make_shared<AtomeseExpression>();
  expr->type = AtomeseType::OrLink;
  expr->children = elements;
  return expr;
}

std::shared_ptr<AtomeseExpression>
AtomeseParser::not_link(std::shared_ptr<AtomeseExpression> expr) {
  auto result = std::make_shared<AtomeseExpression>();
  result->type = AtomeseType::NotLink;
  result->children.push_back(expr);
  return result;
}

std::shared_ptr<AtomeseExpression>
AtomeseParser::context_link(std::shared_ptr<AtomeseExpression> context,
                            std::shared_ptr<AtomeseExpression> content) {
  auto expr = std::make_shared<AtomeseExpression>();
  expr->type = AtomeseType::ContextLink;
  expr->children.push_back(context);
  expr->children.push_back(content);
  return expr;
}

// ================================================================
// Fundamental Knowledge Bases
// ================================================================

AtomeseKnowledgeBase AtomeseParser::generate_philosophical_transformation() {
  AtomeseKnowledgeBase kb;
  kb.description = "Philosophical Transformation - Cognitive Substrates";

  // Root axiom: Cognitive Unity
  auto cognitive_unity = concept_node("CognitiveUnity", 1.0f, 1.0f);
  auto consciousness = concept_node("Consciousness", 0.9f, 0.95f);
  auto cognition = concept_node("Cognition", 0.95f, 0.90f);
  auto reasoning = concept_node("Reasoning", 0.85f, 0.85f);
  auto perception = concept_node("Perception", 0.80f, 0.80f);
  auto memory = concept_node("Memory", 0.85f, 0.90f);
  auto learning = concept_node("Learning", 0.80f, 0.85f);

  // Core inheritance hierarchy
  kb.expressions.push_back(
      inheritance_link(consciousness, cognitive_unity, 0.95f, 0.95f));
  kb.expressions.push_back(
      inheritance_link(cognition, cognitive_unity, 0.90f, 0.95f));
  kb.expressions.push_back(
      inheritance_link(reasoning, cognition, 0.85f, 0.90f));
  kb.expressions.push_back(
      inheritance_link(perception, cognition, 0.80f, 0.85f));
  kb.expressions.push_back(inheritance_link(memory, cognition, 0.85f, 0.90f));
  kb.expressions.push_back(inheritance_link(learning, cognition, 0.80f, 0.85f));

  // Recursive reasoning pathway
  auto recursive_reasoning = concept_node("RecursiveReasoning", 0.9f, 0.9f);
  auto meta_reasoning = concept_node("MetaReasoning", 0.85f, 0.85f);
  auto self_reflection = concept_node("SelfReflection", 0.8f, 0.8f);

  kb.expressions.push_back(
      inheritance_link(recursive_reasoning, reasoning, 0.9f, 0.9f));
  kb.expressions.push_back(
      inheritance_link(meta_reasoning, recursive_reasoning, 0.85f, 0.85f));
  kb.expressions.push_back(
      inheritance_link(self_reflection, meta_reasoning, 0.8f, 0.8f));

  // Implication: Self-reflection enables consciousness
  kb.expressions.push_back(
      implication_link(self_reflection, consciousness, 0.7f, 0.75f));

  return kb;
}

AtomeseKnowledgeBase AtomeseParser::generate_fractal_tape() {
  AtomeseKnowledgeBase kb;
  kb.description = "Fractal Information Theory & Geometric Musical Language";

  // Fractal tape replaces Turing tape
  auto fractal_tape = concept_node("FractalTape", 1.0f, 1.0f);
  auto fractal_cell = concept_node("FractalCell", 0.95f, 0.95f);
  auto self_similar = concept_node("SelfSimilar", 0.9f, 0.9f);
  auto scale_invariant = concept_node("ScaleInvariant", 0.9f, 0.9f);

  kb.expressions.push_back(
      inheritance_link(fractal_cell, fractal_tape, 0.95f, 0.95f));
  kb.expressions.push_back(
      inheritance_link(self_similar, fractal_tape, 0.9f, 0.9f));
  kb.expressions.push_back(
      inheritance_link(scale_invariant, fractal_tape, 0.9f, 0.9f));

  // Geometric Musical Language
  auto gml = concept_node("GeometricMusicalLanguage", 1.0f, 1.0f);
  auto geometric_shape = concept_node("GeometricShape", 0.95f, 0.95f);
  auto musical_note = concept_node("MusicalNote", 0.95f, 0.95f);
  auto resonance = concept_node("Resonance", 0.9f, 0.9f);
  auto harmony = concept_node("Harmony", 0.85f, 0.9f);

  kb.expressions.push_back(
      inheritance_link(geometric_shape, gml, 0.95f, 0.95f));
  kb.expressions.push_back(inheritance_link(musical_note, gml, 0.95f, 0.95f));
  kb.expressions.push_back(inheritance_link(resonance, gml, 0.9f, 0.9f));
  kb.expressions.push_back(inheritance_link(harmony, resonance, 0.85f, 0.9f));

  // GML-Fractal connection
  kb.expressions.push_back(similarity_link(fractal_tape, gml, 0.8f, 0.85f));

  return kb;
}

AtomeseKnowledgeBase AtomeseParser::generate_phase_prime_metric() {
  AtomeseKnowledgeBase kb;
  kb.description = "Phase Prime Metric - Prime-Based Symmetry & Geometry";

  // Phase Prime Metric
  auto ppm = concept_node("PhasePrimeMetric", 1.0f, 1.0f);
  auto prime_encoding = concept_node("PrimeEncoding", 0.95f, 0.95f);
  auto symmetry_group = concept_node("SymmetryGroup", 0.9f, 0.9f);
  auto coherence = concept_node("Coherence", 0.9f, 0.9f);

  kb.expressions.push_back(inheritance_link(prime_encoding, ppm, 0.95f, 0.95f));
  kb.expressions.push_back(inheritance_link(symmetry_group, ppm, 0.9f, 0.9f));
  kb.expressions.push_back(inheritance_link(coherence, ppm, 0.9f, 0.9f));

  // Fundamental primes
  auto prime_2 = concept_node("Prime-2", 1.0f, 1.0f);
  auto prime_3 = concept_node("Prime-3", 1.0f, 1.0f);
  auto prime_5 = concept_node("Prime-5", 1.0f, 1.0f);
  auto prime_7 = concept_node("Prime-7", 1.0f, 1.0f);
  auto prime_11 = concept_node("Prime-11", 1.0f, 1.0f);

  kb.expressions.push_back(
      inheritance_link(prime_2, prime_encoding, 1.0f, 1.0f));
  kb.expressions.push_back(
      inheritance_link(prime_3, prime_encoding, 1.0f, 1.0f));
  kb.expressions.push_back(
      inheritance_link(prime_5, prime_encoding, 1.0f, 1.0f));
  kb.expressions.push_back(
      inheritance_link(prime_7, prime_encoding, 1.0f, 1.0f));
  kb.expressions.push_back(
      inheritance_link(prime_11, prime_encoding, 1.0f, 1.0f));

  // 11-dimensional manifold
  auto manifold_11d = concept_node("Manifold-11D", 0.95f, 0.95f);
  kb.expressions.push_back(
      inheritance_link(manifold_11d, symmetry_group, 0.95f, 0.95f));

  return kb;
}

AtomeseKnowledgeBase AtomeseParser::generate_attention_allocation() {
  AtomeseKnowledgeBase kb;
  kb.description = "Adaptive Attention Allocation Mechanism";

  // Attention system
  auto attention = concept_node("AttentionSystem", 1.0f, 1.0f);
  auto ecan = concept_node("ECAN", 0.95f, 0.95f);
  auto softmax_attention = concept_node("SoftmaxAttention", 0.9f, 0.9f);
  auto sti = concept_node("STI", 0.9f, 0.9f);
  auto lti = concept_node("LTI", 0.85f, 0.85f);

  kb.expressions.push_back(inheritance_link(ecan, attention, 0.95f, 0.95f));
  kb.expressions.push_back(
      inheritance_link(softmax_attention, attention, 0.9f, 0.9f));
  kb.expressions.push_back(inheritance_link(sti, ecan, 0.9f, 0.9f));
  kb.expressions.push_back(inheritance_link(lti, ecan, 0.85f, 0.85f));

  // Attention economics
  auto rent = concept_node("AttentionRent", 0.8f, 0.85f);
  auto wage = concept_node("AttentionWage", 0.8f, 0.85f);
  auto diffusion = concept_node("AttentionDiffusion", 0.85f, 0.9f);

  kb.expressions.push_back(inheritance_link(rent, ecan, 0.8f, 0.85f));
  kb.expressions.push_back(inheritance_link(wage, ecan, 0.8f, 0.85f));
  kb.expressions.push_back(inheritance_link(diffusion, ecan, 0.85f, 0.9f));

  return kb;
}

AtomeseKnowledgeBase AtomeseParser::generate_neural_symbolic_bridge() {
  AtomeseKnowledgeBase kb;
  kb.description = "Neural-Symbolic Integration Bridge";

  // Bridge components
  auto neural_symbolic = concept_node("NeuralSymbolicBridge", 1.0f, 1.0f);
  auto tensor_encoding = concept_node("TensorEncoding", 0.95f, 0.95f);
  auto symbolic_grounding = concept_node("SymbolicGrounding", 0.9f, 0.9f);
  auto differentiable_logic = concept_node("DifferentiableLogic", 0.9f, 0.9f);

  kb.expressions.push_back(
      inheritance_link(tensor_encoding, neural_symbolic, 0.95f, 0.95f));
  kb.expressions.push_back(
      inheritance_link(symbolic_grounding, neural_symbolic, 0.9f, 0.9f));
  kb.expressions.push_back(
      inheritance_link(differentiable_logic, neural_symbolic, 0.9f, 0.9f));

  // Tensor operations
  auto ggml_ops = concept_node("GGMLOperations", 0.95f, 0.95f);
  auto matmul = concept_node("MatMul", 1.0f, 1.0f);
  auto softmax = concept_node("Softmax", 1.0f, 1.0f);
  auto activation = concept_node("Activation", 0.95f, 0.95f);

  kb.expressions.push_back(inheritance_link(matmul, ggml_ops, 1.0f, 1.0f));
  kb.expressions.push_back(inheritance_link(softmax, ggml_ops, 1.0f, 1.0f));
  kb.expressions.push_back(
      inheritance_link(activation, ggml_ops, 0.95f, 0.95f));

  // Connection to tensor encoding
  kb.expressions.push_back(
      inheritance_link(ggml_ops, tensor_encoding, 0.95f, 0.95f));

  return kb;
}

AtomeseKnowledgeBase AtomeseParser::generate_complete_knowledge_base() {
  AtomeseKnowledgeBase kb;
  kb.description = "Complete NanoBrain Fundamental Knowledge Base";

  // Combine all fundamental KBs
  auto philosophical = generate_philosophical_transformation();
  auto fractal = generate_fractal_tape();
  auto ppm = generate_phase_prime_metric();
  auto attention = generate_attention_allocation();
  auto bridge = generate_neural_symbolic_bridge();

  kb.expressions.insert(kb.expressions.end(), philosophical.expressions.begin(),
                        philosophical.expressions.end());
  kb.expressions.insert(kb.expressions.end(), fractal.expressions.begin(),
                        fractal.expressions.end());
  kb.expressions.insert(kb.expressions.end(), ppm.expressions.begin(),
                        ppm.expressions.end());
  kb.expressions.insert(kb.expressions.end(), attention.expressions.begin(),
                        attention.expressions.end());
  kb.expressions.insert(kb.expressions.end(), bridge.expressions.begin(),
                        bridge.expressions.end());

  return kb;
}

// ================================================================
// AtomeseTensorBridge
// ================================================================

AtomeseTensorBridge::AtomeseTensorBridge(NanoBrainKernel *kernel,
                                         AtomSpaceTensorEncoderFull *encoder)
    : kernel(kernel), encoder(encoder), next_id(0) {}

std::string AtomeseTensorBridge::generate_id() {
  return "atomese_" + std::to_string(next_id++);
}

AtomSpaceAtom
AtomeseTensorBridge::expression_to_atom(const AtomeseExpression &expr,
                                        const std::string &id) {
  AtomSpaceAtom atom;
  atom.id = id;
  atom.type = AtomeseParser::type_to_string(expr.type);
  atom.name = expr.name;
  atom.truth_strength = expr.truth_value.strength;
  atom.truth_confidence = expr.truth_value.confidence;
  atom.truth_count = 1.0f;
  atom.sti = expr.attention_value;
  atom.lti = expr.attention_value * 0.8f;
  atom.vlti = expr.attention_value > 0.9f;
  atom.importance = expr.truth_value.strength * expr.truth_value.confidence;
  atom.timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
                       std::chrono::system_clock::now().time_since_epoch())
                       .count();
  return atom;
}

AtomSpaceLink
AtomeseTensorBridge::expression_to_link(const AtomeseExpression &expr,
                                        const std::string &id) {
  AtomSpaceLink link;
  link.id = id;
  link.type = AtomeseParser::type_to_string(expr.type);

  // Generate IDs for children
  for (const auto &child : expr.children) {
    std::string child_id = generate_id();
    link.outgoing.push_back(child_id);
  }

  link.truth_strength = expr.truth_value.strength;
  link.truth_confidence = expr.truth_value.confidence;
  link.truth_count = 1.0f;
  link.sti = expr.attention_value;
  link.lti = expr.attention_value * 0.8f;
  link.vlti = expr.attention_value > 0.9f;
  link.strength = expr.truth_value.strength;

  return link;
}

void AtomeseTensorBridge::encode_knowledge_base(
    const AtomeseKnowledgeBase &kb) {
  for (const auto &expr : kb.expressions) {
    std::string id = generate_id();

    if (expr->is_node()) {
      AtomSpaceAtom atom = expression_to_atom(*expr, id);
      encoder->encode_atom(atom);
    } else {
      AtomSpaceLink link = expression_to_link(*expr, id);
      encoder->encode_link(link);

      // Recursively encode children
      for (size_t i = 0; i < expr->children.size(); i++) {
        const auto &child = expr->children[i];
        if (child->is_node()) {
          AtomSpaceAtom child_atom =
              expression_to_atom(*child, link.outgoing[i]);
          encoder->encode_atom(child_atom);
        }
      }
    }
  }
}

std::vector<NodeTensorFull *> AtomeseTensorBridge::get_node_tensors() const {
  return encoder->get_node_tensors();
}

std::vector<LinkTensorFull *> AtomeseTensorBridge::get_link_tensors() const {
  return encoder->get_link_tensors();
}
