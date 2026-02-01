#include "nanobrain_encoder_full.h"
#include <algorithm>
#include <chrono>
#include <cmath>
#include <iostream>
#include <sstream>

// ================================================================
// Constructor / Destructor
// ================================================================

AtomSpaceTensorEncoderFull::AtomSpaceTensorEncoderFull(
    NanoBrainKernel *kernel, const AtomSpaceTensorConfig &config)
    : kernel(kernel), config(config), next_vocab_id(0) {
  initialize_vocabulary();
}

AtomSpaceTensorEncoderFull::~AtomSpaceTensorEncoderFull() { clear_cache(); }

void AtomSpaceTensorEncoderFull::initialize_vocabulary() {
  // Common atom types
  std::vector<std::string> atom_types = {
      "ConceptNode",     "PredicateNode",   "NumberNode",
      "VariableNode",    "SchemaNode",      "GroundedSchemaNode",
      "TypeNode",        "AnchorNode",      "ListLink",
      "SetLink",         "AndLink",         "OrLink",
      "NotLink",         "InheritanceLink", "SimilarityLink",
      "ImplicationLink", "EvaluationLink",  "ExecutionLink",
      "BindLink",        "MemberLink",      "ContextLink",
      "DefineLink",      "LambdaLink",      "PutLink"};

  for (const auto &type : atom_types) {
    add_to_vocabulary(type);
  }
}

// ================================================================
// Vocabulary Management
// ================================================================

int AtomSpaceTensorEncoderFull::add_to_vocabulary(const std::string &symbol) {
  auto it = vocabulary_map.find(symbol);
  if (it != vocabulary_map.end()) {
    return it->second;
  }

  int id = next_vocab_id++;
  vocabulary_map[symbol] = id;
  reverse_vocabulary_map[id] = symbol;
  return id;
}

int AtomSpaceTensorEncoderFull::get_vocabulary_id(
    const std::string &symbol) const {
  auto it = vocabulary_map.find(symbol);
  if (it != vocabulary_map.end()) {
    return it->second;
  }
  return -1; // Not found
}

std::string AtomSpaceTensorEncoderFull::get_symbol(int id) const {
  auto it = reverse_vocabulary_map.find(id);
  if (it != reverse_vocabulary_map.end()) {
    return it->second;
  }
  return "";
}

VocabularyStats AtomSpaceTensorEncoderFull::get_vocabulary_stats() const {
  VocabularyStats stats;
  stats.size = vocabulary_map.size();
  for (const auto &[symbol, _] : vocabulary_map) {
    stats.symbols.push_back(symbol);
  }
  return stats;
}

// ================================================================
// Utility Functions
// ================================================================

uint32_t AtomSpaceTensorEncoderFull::hash_string(const std::string &str) const {
  uint32_t hash = 0;
  for (char c : str) {
    hash = ((hash << 5) - hash) + static_cast<uint32_t>(c);
  }
  return hash;
}

std::string AtomSpaceTensorEncoderFull::generate_node_tensor_id(
    const std::string &atom_id) {
  return "node_tensor_" + atom_id;
}

std::string AtomSpaceTensorEncoderFull::generate_link_tensor_id(
    const std::string &link_id) {
  return "link_tensor_" + link_id;
}

// ================================================================
// Node Encoding/Decoding
// ================================================================

NodeTensorFull *
AtomSpaceTensorEncoderFull::encode_atom(const AtomSpaceAtom &atom) {
  // Check cache first
  auto it = node_embeddings.find(atom.id);
  if (it != node_embeddings.end()) {
    return it->second;
  }

  NodeTensorFull *node_tensor = new NodeTensorFull();
  node_tensor->id = generate_node_tensor_id(atom.id);
  node_tensor->atom_id = atom.id;

  // Create base embedding tensor
  node_tensor->embedding =
      kernel->create_tensor({static_cast<int64_t>(config.node_embedding_dim)});

  // Initialize embedding data
  std::vector<float> embed_data(config.node_embedding_dim, 0.0f);

  // Encode atom type (feature 0)
  int type_id = add_to_vocabulary(atom.type);
  embed_data[0] = static_cast<float>(type_id) / 100.0f;

  // Encode atom name hash (feature 1)
  if (!atom.name.empty()) {
    uint32_t name_hash = hash_string(atom.name);
    embed_data[1] = static_cast<float>(name_hash % 1000) / 1000.0f;
  }

  // Encode truth value components (features 2-4)
  embed_data[2] = atom.truth_strength;
  embed_data[3] = atom.truth_confidence;
  embed_data[4] = std::log(atom.truth_count + 1.0f) / 10.0f;

  // Encode attention values (features 5-7)
  embed_data[5] = atom.sti / 100.0f;
  embed_data[6] = atom.lti / 100.0f;
  embed_data[7] = atom.vlti ? 1.0f : 0.0f;

  // Encode importance (feature 8)
  embed_data[8] = atom.importance;

  // Encode timestamp recency (feature 9)
  int64_t now = std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::system_clock::now().time_since_epoch())
                    .count();
  float recency = std::min(1.0f, static_cast<float>(now - atom.timestamp) /
                                     (1000.0f * 60.0f * 60.0f * 24.0f));
  embed_data[9] = recency;

  kernel->set_data(node_tensor->embedding, embed_data);

  // Encode truth value tensor
  node_tensor->truth_value_tensor = encode_truth_value(
      atom.truth_strength, atom.truth_confidence, atom.truth_count);

  // Encode attention value tensor
  node_tensor->attention_weights =
      encode_attention_value(atom.sti, atom.lti, atom.vlti);

  // Create symbolic features
  node_tensor->symbolic_features = create_symbolic_features(atom);

  // Set metadata
  node_tensor->metadata.atom_type = atom.type;
  node_tensor->metadata.symbolic_depth = calculate_symbolic_depth(atom);
  node_tensor->metadata.degrees_of_freedom = calculate_degrees_of_freedom(atom);

  // Cache the tensor
  node_embeddings[atom.id] = node_tensor;

  return node_tensor;
}

AtomSpaceAtom
AtomSpaceTensorEncoderFull::decode_atom(const NodeTensorFull *node_tensor) {
  AtomSpaceAtom atom;

  if (!node_tensor || !node_tensor->embedding) {
    return atom;
  }

  atom.id = node_tensor->atom_id;

  // Decode type from vocabulary
  kernel->compute(node_tensor->embedding);
  int type_id = static_cast<int>(
      std::round(kernel->get_value(node_tensor->embedding, 0) * 100.0f));
  atom.type = get_symbol(type_id);
  if (atom.type.empty()) {
    atom.type = "ConceptNode";
  }

  // Decode name (we can only recover hash, so use a placeholder)
  atom.name = "decoded_" + node_tensor->atom_id;

  // Decode truth value
  decode_truth_value(node_tensor->truth_value_tensor, atom.truth_strength,
                     atom.truth_confidence, atom.truth_count);

  // Decode attention value
  decode_attention_value(node_tensor->attention_weights, atom.sti, atom.lti,
                         atom.vlti);

  // Calculate importance
  atom.importance = atom.truth_strength * atom.truth_confidence;

  // Set timestamp to now
  atom.timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
                       std::chrono::system_clock::now().time_since_epoch())
                       .count();

  return atom;
}

NanoBrainTensor *
AtomSpaceTensorEncoderFull::encode_truth_value(float strength, float confidence,
                                               float count) {
  auto *tensor = kernel->create_tensor({3});
  std::vector<float> data = {strength, confidence,
                             std::log(count + 1.0f) / 10.0f};
  kernel->set_data(tensor, data);
  return tensor;
}

void AtomSpaceTensorEncoderFull::decode_truth_value(
    const NanoBrainTensor *tensor, float &strength, float &confidence,
    float &count) {
  if (!tensor) {
    strength = 0.5f;
    confidence = 0.5f;
    count = 1.0f;
    return;
  }

  kernel->compute(const_cast<NanoBrainTensor *>(tensor));
  strength = kernel->get_value(const_cast<NanoBrainTensor *>(tensor), 0);
  confidence = kernel->get_value(const_cast<NanoBrainTensor *>(tensor), 1);
  float log_count = kernel->get_value(const_cast<NanoBrainTensor *>(tensor), 2);
  count = std::exp(log_count * 10.0f) - 1.0f;
}

NanoBrainTensor *AtomSpaceTensorEncoderFull::encode_attention_value(float sti,
                                                                    float lti,
                                                                    bool vlti) {
  auto *tensor = kernel->create_tensor({3});
  std::vector<float> data = {sti / 100.0f, lti / 100.0f, vlti ? 1.0f : 0.0f};
  kernel->set_data(tensor, data);
  return tensor;
}

void AtomSpaceTensorEncoderFull::decode_attention_value(
    const NanoBrainTensor *tensor, float &sti, float &lti, bool &vlti) {
  if (!tensor) {
    sti = 0.0f;
    lti = 0.0f;
    vlti = false;
    return;
  }

  kernel->compute(const_cast<NanoBrainTensor *>(tensor));
  sti = kernel->get_value(const_cast<NanoBrainTensor *>(tensor), 0) * 100.0f;
  lti = kernel->get_value(const_cast<NanoBrainTensor *>(tensor), 1) * 100.0f;
  vlti = kernel->get_value(const_cast<NanoBrainTensor *>(tensor), 2) > 0.5f;
}

// ================================================================
// Link Encoding/Decoding
// ================================================================

LinkTensorFull *
AtomSpaceTensorEncoderFull::encode_link(const AtomSpaceLink &link) {
  // Check cache first
  auto it = link_embeddings.find(link.id);
  if (it != link_embeddings.end()) {
    return it->second;
  }

  LinkTensorFull *link_tensor = new LinkTensorFull();
  link_tensor->id = generate_link_tensor_id(link.id);
  link_tensor->atom_id = link.id;

  // Create relation tensor
  link_tensor->relation_tensor =
      kernel->create_tensor({static_cast<int64_t>(config.link_embedding_dim)});

  // Initialize embedding data
  std::vector<float> embed_data(config.link_embedding_dim, 0.0f);

  // Encode link type (feature 0)
  int type_id = add_to_vocabulary(link.type);
  embed_data[0] = static_cast<float>(type_id) / 100.0f;

  // Encode arity (feature 1)
  embed_data[1] = static_cast<float>(link.outgoing.size()) / 10.0f;

  // Encode outgoing atom relationships (features 2-6)
  for (size_t i = 0; i < std::min(link.outgoing.size(), size_t(5)); i++) {
    uint32_t atom_hash = hash_string(link.outgoing[i]);
    embed_data[2 + i] = static_cast<float>(atom_hash % 1000) / 1000.0f;
  }

  // Encode link strength (feature 7)
  embed_data[7] = link.strength;

  // Encode truth values (features 8-10)
  embed_data[8] = link.truth_strength;
  embed_data[9] = link.truth_confidence;
  embed_data[10] = std::log(link.truth_count + 1.0f) / 10.0f;

  kernel->set_data(link_tensor->relation_tensor, embed_data);

  // Parse source and target nodes
  // Convention: all but last outgoing are sources, last is target
  if (link.outgoing.size() >= 2) {
    for (size_t i = 0; i < link.outgoing.size() - 1; i++) {
      link_tensor->source_nodes.push_back(link.outgoing[i]);
    }
    link_tensor->target_nodes.push_back(link.outgoing.back());
  } else if (link.outgoing.size() == 1) {
    link_tensor->target_nodes.push_back(link.outgoing[0]);
  }

  // Encode truth value tensor
  link_tensor->truth_value_tensor = encode_truth_value(
      link.truth_strength, link.truth_confidence, link.truth_count);

  // Encode attention value tensor
  link_tensor->attention_weights =
      encode_attention_value(link.sti, link.lti, link.vlti);

  // Set metadata
  link_tensor->metadata.link_type = link.type;
  link_tensor->metadata.arity = static_cast<int>(link.outgoing.size());
  link_tensor->metadata.symbolic_depth = calculate_link_symbolic_depth(link);

  // Cache the tensor
  link_embeddings[link.id] = link_tensor;

  return link_tensor;
}

AtomSpaceLink
AtomSpaceTensorEncoderFull::decode_link(const LinkTensorFull *link_tensor) {
  AtomSpaceLink link;

  if (!link_tensor || !link_tensor->relation_tensor) {
    return link;
  }

  link.id = link_tensor->atom_id;

  // Decode type from vocabulary
  kernel->compute(link_tensor->relation_tensor);
  int type_id = static_cast<int>(
      std::round(kernel->get_value(link_tensor->relation_tensor, 0) * 100.0f));
  link.type = get_symbol(type_id);
  if (link.type.empty()) {
    link.type = "InheritanceLink";
  }

  // Reconstruct outgoing from source/target
  link.outgoing = link_tensor->source_nodes;
  for (const auto &target : link_tensor->target_nodes) {
    link.outgoing.push_back(target);
  }

  // Decode truth value
  decode_truth_value(link_tensor->truth_value_tensor, link.truth_strength,
                     link.truth_confidence, link.truth_count);

  // Decode attention value
  decode_attention_value(link_tensor->attention_weights, link.sti, link.lti,
                         link.vlti);

  // Get strength from relation tensor
  link.strength = kernel->get_value(link_tensor->relation_tensor, 7);

  return link;
}

// ================================================================
// Symbolic Features
// ================================================================

NanoBrainTensor *AtomSpaceTensorEncoderFull::create_symbolic_features(
    const AtomSpaceAtom &atom) {
  auto *features = kernel->create_tensor(
      {static_cast<int64_t>(config.symbolic_feature_dim)});

  std::vector<float> feat_data(config.symbolic_feature_dim, 0.0f);

  // Feature 0: Type complexity (length of type name)
  feat_data[0] = static_cast<float>(atom.type.length()) / 20.0f;

  // Feature 1: Name complexity
  if (!atom.name.empty()) {
    feat_data[1] = static_cast<float>(atom.name.length()) / 50.0f;
  }

  // Feature 2: Importance
  feat_data[2] = atom.importance;

  // Feature 3: Is a link type?
  feat_data[3] = (atom.type.find("Link") != std::string::npos) ? 1.0f : 0.0f;

  // Feature 4: Is a node type?
  feat_data[4] = (atom.type.find("Node") != std::string::npos) ? 1.0f : 0.0f;

  // Feature 5: Truth value combined score
  feat_data[5] = atom.truth_strength * atom.truth_confidence;

  // Feature 6: Attention combined score
  feat_data[6] = (atom.sti + atom.lti) / 200.0f;

  // Feature 7: VLTI flag
  feat_data[7] = atom.vlti ? 1.0f : 0.0f;

  kernel->set_data(features, feat_data);
  return features;
}

NanoBrainTensor *AtomSpaceTensorEncoderFull::create_link_symbolic_features(
    const AtomSpaceLink &link) {
  auto *features = kernel->create_tensor(
      {static_cast<int64_t>(config.symbolic_feature_dim)});

  std::vector<float> feat_data(config.symbolic_feature_dim, 0.0f);

  // Feature 0: Type complexity
  feat_data[0] = static_cast<float>(link.type.length()) / 20.0f;

  // Feature 1: Arity
  feat_data[1] = static_cast<float>(link.outgoing.size()) / 10.0f;

  // Feature 2: Link strength
  feat_data[2] = link.strength;

  // Feature 3: Is inheritance?
  feat_data[3] = (link.type == "InheritanceLink") ? 1.0f : 0.0f;

  // Feature 4: Is similarity?
  feat_data[4] = (link.type == "SimilarityLink") ? 1.0f : 0.0f;

  // Feature 5: Is implication?
  feat_data[5] = (link.type == "ImplicationLink") ? 1.0f : 0.0f;

  // Feature 6: Is evaluation?
  feat_data[6] = (link.type == "EvaluationLink") ? 1.0f : 0.0f;

  // Feature 7: Truth combined score
  feat_data[7] = link.truth_strength * link.truth_confidence;

  kernel->set_data(features, feat_data);
  return features;
}

int AtomSpaceTensorEncoderFull::calculate_symbolic_depth(
    const AtomSpaceAtom &atom) {
  int depth = 1;

  // Name complexity adds depth
  if (!atom.name.empty()) {
    // Count segments separated by common delimiters
    int segments = 1;
    for (char c : atom.name) {
      if (c == '-' || c == '_' || c == ':') {
        segments++;
      }
    }
    depth += std::min(3, segments);
  }

  // Link types are deeper
  if (atom.type.find("Link") != std::string::npos) {
    depth += 1;
  }

  return depth;
}

int AtomSpaceTensorEncoderFull::calculate_link_symbolic_depth(
    const AtomSpaceLink &link) {
  return 1 + std::min(3, static_cast<int>(link.outgoing.size()));
}

int AtomSpaceTensorEncoderFull::calculate_degrees_of_freedom(
    const AtomSpaceAtom &atom) {
  int dof = 1;

  // Truth value adds 2 DOF (strength, confidence)
  dof += 2;

  // Attention value adds 3 DOF (STI, LTI, VLTI)
  dof += 3;

  // Link types have additional DOF
  if (atom.type.find("Link") != std::string::npos) {
    dof += 2;
  }

  return dof;
}

// ================================================================
// Batch Operations
// ================================================================

std::vector<NodeTensorFull *> AtomSpaceTensorEncoderFull::encode_atoms(
    const std::vector<AtomSpaceAtom> &atoms) {
  std::vector<NodeTensorFull *> results;
  results.reserve(atoms.size());

  for (const auto &atom : atoms) {
    results.push_back(encode_atom(atom));
  }

  return results;
}

std::vector<LinkTensorFull *> AtomSpaceTensorEncoderFull::encode_links(
    const std::vector<AtomSpaceLink> &links) {
  std::vector<LinkTensorFull *> results;
  results.reserve(links.size());

  for (const auto &link : links) {
    results.push_back(encode_link(link));
  }

  return results;
}

// ================================================================
// Cache Management
// ================================================================

std::vector<NodeTensorFull *>
AtomSpaceTensorEncoderFull::get_node_tensors() const {
  std::vector<NodeTensorFull *> results;
  results.reserve(node_embeddings.size());

  for (const auto &[_, tensor] : node_embeddings) {
    results.push_back(tensor);
  }

  return results;
}

std::vector<LinkTensorFull *>
AtomSpaceTensorEncoderFull::get_link_tensors() const {
  std::vector<LinkTensorFull *> results;
  results.reserve(link_embeddings.size());

  for (const auto &[_, tensor] : link_embeddings) {
    results.push_back(tensor);
  }

  return results;
}

NodeTensorFull *
AtomSpaceTensorEncoderFull::get_node_tensor(const std::string &atom_id) {
  auto it = node_embeddings.find(atom_id);
  if (it != node_embeddings.end()) {
    return it->second;
  }
  return nullptr;
}

LinkTensorFull *
AtomSpaceTensorEncoderFull::get_link_tensor(const std::string &link_id) {
  auto it = link_embeddings.find(link_id);
  if (it != link_embeddings.end()) {
    return it->second;
  }
  return nullptr;
}

void AtomSpaceTensorEncoderFull::clear_cache() {
  for (auto &[_, tensor] : node_embeddings) {
    delete tensor;
  }
  node_embeddings.clear();

  for (auto &[_, tensor] : link_embeddings) {
    delete tensor;
  }
  link_embeddings.clear();
}
