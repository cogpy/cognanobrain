#ifndef NANOBRAIN_ATOMESE_H
#define NANOBRAIN_ATOMESE_H

#include "nanobrain_encoder_full.h"
#include "nanobrain_kernel.h"
#include <functional>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

/**
 * Atomese expression type
 */
enum class AtomeseType {
  // Node types
  ConceptNode,
  PredicateNode,
  NumberNode,
  VariableNode,
  SchemaNode,
  GroundedSchemaNode,
  TypeNode,
  AnchorNode,

  // Link types
  ListLink,
  SetLink,
  AndLink,
  OrLink,
  NotLink,
  InheritanceLink,
  SimilarityLink,
  ImplicationLink,
  EvaluationLink,
  ExecutionLink,
  BindLink,
  MemberLink,
  ContextLink,
  DefineLink,
  LambdaLink,
  PutLink,
  GetLink,
  EquivalenceLink,
  SatisfactionLink,
  StateLink,
  AtTimeLink,

  // Unknown
  Unknown
};

/**
 * Truth value for Atomese expressions
 */
struct AtomeseTruthValue {
  float strength = 1.0f;
  float confidence = 1.0f;
};

/**
 * Atomese expression tree node
 */
struct AtomeseExpression {
  AtomeseType type;
  std::string name; // For nodes
  std::vector<std::shared_ptr<AtomeseExpression>> children;

  // Optional values
  float attention_value = 0.0f;
  AtomeseTruthValue truth_value;

  // Convenience accessors
  bool is_node() const;
  bool is_link() const;
  std::string type_name() const;
};

/**
 * Parse result for Atomese expressions
 */
struct AtomeseParseResult {
  bool success;
  std::string error_message;
  size_t error_position;
  std::shared_ptr<AtomeseExpression> expression;
};

/**
 * Atomese Knowledge Base
 */
struct AtomeseKnowledgeBase {
  std::vector<std::shared_ptr<AtomeseExpression>> expressions;
  std::map<std::string, std::shared_ptr<AtomeseExpression>> named_atoms;
  std::string description;
};

/**
 * Atomese Parser and Generator
 *
 * Parses Scheme/Atomese syntax and generates knowledge bases for
 * NanoBrain cognitive processing.
 */
class AtomeseParser {
public:
  AtomeseParser();
  ~AtomeseParser() = default;

  // ================================================================
  // Parsing
  // ================================================================

  // Parse a single Atomese expression from string
  AtomeseParseResult parse(const std::string &input);

  // Parse multiple expressions from string
  std::vector<AtomeseParseResult> parse_all(const std::string &input);

  // Parse from file
  AtomeseParseResult parse_file(const std::string &filename);

  // ================================================================
  // Generation
  // ================================================================

  // Convert expression to Scheme string
  std::string to_scheme_string(const AtomeseExpression &expr,
                               int indent = 0) const;

  // Convert expression to s-expression string
  std::string to_sexpr_string(const AtomeseExpression &expr) const;

  // Generate complete Scheme output for knowledge base
  std::string generate_scheme_output(const AtomeseKnowledgeBase &kb) const;

  // ================================================================
  // Fundamental Knowledge Bases
  // ================================================================

  // Generate philosophical transformation Atomese
  AtomeseKnowledgeBase generate_philosophical_transformation();

  // Generate fractal tape Atomese (replaces Turing tapes)
  AtomeseKnowledgeBase generate_fractal_tape();

  // Generate Phase Prime Metric Atomese
  AtomeseKnowledgeBase generate_phase_prime_metric();

  // Generate attention allocation Atomese
  AtomeseKnowledgeBase generate_attention_allocation();

  // Generate neural-symbolic bridge Atomese
  AtomeseKnowledgeBase generate_neural_symbolic_bridge();

  // Generate complete knowledge base with all fundamentals
  AtomeseKnowledgeBase generate_complete_knowledge_base();

  // ================================================================
  // Expression Creation Helpers
  // ================================================================

  // Create a ConceptNode
  std::shared_ptr<AtomeseExpression> concept_node(const std::string &name,
                                                  float strength = 1.0f,
                                                  float confidence = 1.0f);

  // Create a PredicateNode
  std::shared_ptr<AtomeseExpression> predicate_node(const std::string &name,
                                                    float strength = 1.0f,
                                                    float confidence = 1.0f);

  // Create a NumberNode
  std::shared_ptr<AtomeseExpression> number_node(float value);

  // Create a VariableNode
  std::shared_ptr<AtomeseExpression> variable_node(const std::string &name);

  // Create an InheritanceLink
  std::shared_ptr<AtomeseExpression>
  inheritance_link(std::shared_ptr<AtomeseExpression> child,
                   std::shared_ptr<AtomeseExpression> parent,
                   float strength = 1.0f, float confidence = 1.0f);

  // Create a SimilarityLink
  std::shared_ptr<AtomeseExpression>
  similarity_link(std::shared_ptr<AtomeseExpression> a,
                  std::shared_ptr<AtomeseExpression> b, float strength = 1.0f,
                  float confidence = 1.0f);

  // Create an ImplicationLink
  std::shared_ptr<AtomeseExpression>
  implication_link(std::shared_ptr<AtomeseExpression> antecedent,
                   std::shared_ptr<AtomeseExpression> consequent,
                   float strength = 1.0f, float confidence = 1.0f);

  // Create an EvaluationLink
  std::shared_ptr<AtomeseExpression>
  evaluation_link(std::shared_ptr<AtomeseExpression> predicate,
                  std::shared_ptr<AtomeseExpression> arguments,
                  float strength = 1.0f, float confidence = 1.0f);

  // Create a ListLink
  std::shared_ptr<AtomeseExpression>
  list_link(const std::vector<std::shared_ptr<AtomeseExpression>> &elements);

  // Create an AndLink
  std::shared_ptr<AtomeseExpression>
  and_link(const std::vector<std::shared_ptr<AtomeseExpression>> &elements);

  // Create an OrLink
  std::shared_ptr<AtomeseExpression>
  or_link(const std::vector<std::shared_ptr<AtomeseExpression>> &elements);

  // Create a NotLink
  std::shared_ptr<AtomeseExpression>
  not_link(std::shared_ptr<AtomeseExpression> expr);

  // Create a ContextLink
  std::shared_ptr<AtomeseExpression>
  context_link(std::shared_ptr<AtomeseExpression> context,
               std::shared_ptr<AtomeseExpression> content);

  // ================================================================
  // Type Conversion
  // ================================================================

  // Convert string to AtomeseType
  static AtomeseType string_to_type(const std::string &type_name);

  // Convert AtomeseType to string
  static std::string type_to_string(AtomeseType type);

  // Check if type is a node type
  static bool is_node_type(AtomeseType type);

  // Check if type is a link type
  static bool is_link_type(AtomeseType type);

private:
  // Parsing state
  size_t current_pos;
  std::string current_input;

  // Parsing helpers
  void skip_whitespace();
  bool match(char c);
  bool match(const std::string &s);
  bool peek(char c) const;
  std::string read_symbol();
  std::string read_string();
  float read_number();

  // Recursive parsing
  std::shared_ptr<AtomeseExpression> parse_expression();
  std::shared_ptr<AtomeseExpression> parse_node();
  std::shared_ptr<AtomeseExpression> parse_link();
  AtomeseTruthValue parse_truth_value();
};

/**
 * Convert Atomese to tensor representation
 */
class AtomeseTensorBridge {
public:
  AtomeseTensorBridge(NanoBrainKernel *kernel,
                      AtomSpaceTensorEncoderFull *encoder);

  // Convert Atomese expression to AtomSpaceAtom
  AtomSpaceAtom expression_to_atom(const AtomeseExpression &expr,
                                   const std::string &id);

  // Convert Atomese link expression to AtomSpaceLink
  AtomSpaceLink expression_to_link(const AtomeseExpression &expr,
                                   const std::string &id);

  // Encode entire knowledge base
  void encode_knowledge_base(const AtomeseKnowledgeBase &kb);

  // Get all encoded node tensors
  std::vector<NodeTensorFull *> get_node_tensors() const;

  // Get all encoded link tensors
  std::vector<LinkTensorFull *> get_link_tensors() const;

private:
  NanoBrainKernel *kernel;
  AtomSpaceTensorEncoderFull *encoder;
  int next_id = 0;

  std::string generate_id();
};

#endif // NANOBRAIN_ATOMESE_H
