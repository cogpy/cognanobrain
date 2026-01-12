#include "nanobrain_encoder.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <numeric>

// ================================================================
// Constructor / Destructor
// ================================================================

AtomSpaceTensorEncoder::AtomSpaceTensorEncoder(NanoBrainKernel *kernel)
    : kernel(kernel) {
  // Initialize basic vocabulary for OpenCog node types
  get_vocabulary_id("ConceptNode");
  get_vocabulary_id("PredicateNode");
  get_vocabulary_id("NumberNode");
  get_vocabulary_id("VariableNode");
  get_vocabulary_id("SchemaNode");
  get_vocabulary_id("GroundedNode");
  get_vocabulary_id("AnchorNode");
  get_vocabulary_id("TypeNode");

  // Initialize basic vocabulary for link types
  get_vocabulary_id("ListLink");
  get_vocabulary_id("InheritanceLink");
  get_vocabulary_id("SimilarityLink");
  get_vocabulary_id("ImplicationLink");
  get_vocabulary_id("EquivalenceLink");
  get_vocabulary_id("EvaluationLink");
  get_vocabulary_id("ExecutionLink");
  get_vocabulary_id("MemberLink");
  get_vocabulary_id("SetLink");
  get_vocabulary_id("AndLink");
  get_vocabulary_id("OrLink");
  get_vocabulary_id("NotLink");

  std::cout << "[AtomSpaceTensorEncoder] Initialized with "
            << vocabulary_map.size() << " vocabulary entries" << std::endl;
}

AtomSpaceTensorEncoder::~AtomSpaceTensorEncoder() { clear_cache(); }

// ================================================================
// Vocabulary Management
// ================================================================

int AtomSpaceTensorEncoder::get_vocabulary_id(const std::string &symbol) {
  auto it = vocabulary_map.find(symbol);
  if (it != vocabulary_map.end()) {
    return it->second;
  }

  int id = static_cast<int>(vocabulary_map.size());
  vocabulary_map[symbol] = id;
  reverse_vocabulary_map[id] = symbol;
  return id;
}

std::string AtomSpaceTensorEncoder::get_vocabulary_symbol(int id) const {
  auto it = reverse_vocabulary_map.find(id);
  if (it != reverse_vocabulary_map.end()) {
    return it->second;
  }
  return "";
}

size_t AtomSpaceTensorEncoder::hash_string(const std::string &str) const {
  std::hash<std::string> hasher;
  return hasher(str);
}

// ================================================================
// Truth Value / Attention Value Encoding
// ================================================================

NanoBrainTensor *AtomSpaceTensorEncoder::encode_truth_value(const float *tv) {
  NanoBrainTensor *tensor = kernel->create_tensor({3});
  std::vector<float> data = {
      tv[0],                         // strength
      tv[1],                         // confidence
      std::log(tv[2] + 1.0f) / 10.0f // count (log-scaled)
  };
  kernel->set_data(tensor, data);
  return tensor;
}

NanoBrainTensor *
AtomSpaceTensorEncoder::encode_attention_value(const float *av) {
  NanoBrainTensor *tensor = kernel->create_tensor({3});
  std::vector<float> data = {
      av[0] / 1000.0f,           // STI (normalized)
      av[1] / 1000.0f,           // LTI (normalized)
      av[2] > 0.5f ? 1.0f : 0.0f // VLTI (binary)
  };
  kernel->set_data(tensor, data);
  return tensor;
}

// ================================================================
// Atom Encoding (Node)
// ================================================================

NodeTensor *AtomSpaceTensorEncoder::encode_atom(const Atom &atom) {
  // Check cache first
  auto it = node_embeddings.find(atom.id);
  if (it != node_embeddings.end()) {
    return it->second;
  }

  NodeTensor *nodeTensor = new NodeTensor();
  nodeTensor->id = "node_tensor_" + atom.id;
  nodeTensor->atom_id = atom.id;

  // Create 128-dimensional embedding tensor
  nodeTensor->embedding = kernel->create_tensor({128}, GGML_TYPE_F32, true);
  nodeTensor->shape = {128};

  std::vector<float> embed_data(128, 0.0f);

  // Position 0: Type encoding (normalized vocabulary ID)
  int typeId = get_vocabulary_id(atom.type);
  embed_data[0] = static_cast<float>(typeId) / 100.0f;

  // Position 1-2: Name hash (spread across two positions for more bits)
  size_t name_hash = hash_string(atom.name);
  embed_data[1] = static_cast<float>(name_hash % 1000) / 1000.0f;
  embed_data[2] = static_cast<float>((name_hash / 1000) % 1000) / 1000.0f;

  // Position 3-5: Truth value encoding
  embed_data[3] = atom.truth_value[0];                          // strength
  embed_data[4] = atom.truth_value[1];                          // confidence
  embed_data[5] = std::log(atom.truth_value[2] + 1.0f) / 10.0f; // count

  // Position 6-8: Attention value encoding
  embed_data[6] = atom.attention_value[0] / 1000.0f;            // STI
  embed_data[7] = atom.attention_value[1] / 1000.0f;            // LTI
  embed_data[8] = atom.attention_value[2] > 0.5f ? 1.0f : 0.0f; // VLTI

  // Position 9-24: Character-level name encoding (first 16 chars)
  for (size_t i = 0; i < std::min(atom.name.size(), size_t(16)); i++) {
    embed_data[9 + i] = static_cast<float>(atom.name[i]) / 256.0f;
  }

  // Position 25-40: Positional encoding (sinusoidal)
  for (int i = 0; i < 16; i++) {
    float pos = static_cast<float>(i);
    embed_data[25 + i] = std::sin(pos / std::pow(10000.0f, (2.0f * i) / 16.0f));
  }

  // Position 41-56: Type-specific learned features (random init for now)
  for (int i = 0; i < 16; i++) {
    embed_data[41 + i] = std::sin(typeId * (i + 1) * 0.1f) * 0.5f;
  }

  // Remaining positions are zero-initialized for future extensions

  kernel->set_data(nodeTensor->embedding, embed_data);

  // Create separate tensors for truth value and attention
  nodeTensor->truth_value_tensor = encode_truth_value(atom.truth_value);
  nodeTensor->attention_weights = encode_attention_value(atom.attention_value);

  // Create symbolic features tensor
  nodeTensor->symbolic_features = kernel->create_tensor({128});
  std::vector<float> symbolic_data(128, 0.0f);
  // Symbolic features based on type
  symbolic_data[typeId % 128] = 1.0f; // One-hot for type
  kernel->set_data(nodeTensor->symbolic_features, symbolic_data);

  // Cache and return
  node_embeddings[atom.id] = nodeTensor;
  return nodeTensor;
}

// ================================================================
// Link Encoding
// ================================================================

LinkTensor *AtomSpaceTensorEncoder::encode_link(const Link &link) {
  // Check cache first
  auto it = link_embeddings.find(link.id);
  if (it != link_embeddings.end()) {
    return it->second;
  }

  LinkTensor *linkTensor = new LinkTensor();
  linkTensor->id = "link_tensor_" + link.id;
  linkTensor->atom_id = link.id;

  // Store outgoing atoms as source/target
  // For simplicity: first half are sources, second half are targets
  size_t mid = link.outgoing.size() / 2;
  if (mid == 0)
    mid = 1;

  for (size_t i = 0; i < link.outgoing.size(); i++) {
    if (i < mid) {
      linkTensor->source_nodes.push_back(link.outgoing[i]);
    } else {
      linkTensor->target_nodes.push_back(link.outgoing[i]);
    }
  }

  // If only one outgoing, it's both source and target (self-link)
  if (link.outgoing.size() == 1) {
    linkTensor->target_nodes.push_back(link.outgoing[0]);
  }

  // Create relation tensor (64-dimensional edge embedding)
  linkTensor->relation_tensor = kernel->create_tensor({64});
  std::vector<float> relation_data(64, 0.0f);

  // Position 0: Link type encoding
  int typeId = get_vocabulary_id(link.type);
  relation_data[0] = static_cast<float>(typeId) / 100.0f;

  // Position 1: Number of outgoing atoms
  relation_data[1] = static_cast<float>(link.outgoing.size()) / 10.0f;

  // Position 2-4: Truth value
  relation_data[2] = link.truth_value[0];
  relation_data[3] = link.truth_value[1];
  relation_data[4] = std::log(link.truth_value[2] + 1.0f) / 10.0f;

  // Position 5-7: Attention value
  relation_data[5] = link.attention_value[0] / 1000.0f;
  relation_data[6] = link.attention_value[1] / 1000.0f;
  relation_data[7] = link.attention_value[2] > 0.5f ? 1.0f : 0.0f;

  // Position 8-23: Hash of outgoing atom IDs
  for (size_t i = 0; i < std::min(link.outgoing.size(), size_t(16)); i++) {
    size_t hash = hash_string(link.outgoing[i]);
    relation_data[8 + i] = static_cast<float>(hash % 1000) / 1000.0f;
  }

  // Position 24-39: Type-specific encoding
  for (int i = 0; i < 16; i++) {
    relation_data[24 + i] = std::cos(typeId * (i + 1) * 0.15f) * 0.5f;
  }

  // Position 40-55: Directionality encoding
  // Asymmetric links get directional encoding
  bool is_symmetric = (link.type.find("Similarity") != std::string::npos) ||
                      (link.type.find("Equiv") != std::string::npos);
  for (int i = 0; i < 16; i++) {
    if (is_symmetric) {
      relation_data[40 + i] = 0.0f; // Symmetric
    } else {
      relation_data[40 + i] = std::sin(i * 0.2f) * 0.5f; // Directional
    }
  }

  kernel->set_data(linkTensor->relation_tensor, relation_data);

  // Create attention weights tensor
  linkTensor->attention_weights = kernel->create_tensor({1});
  std::vector<float> att_data = {link.attention_value[0] / 1000.0f};
  kernel->set_data(linkTensor->attention_weights, att_data);

  // Create truth value tensor
  linkTensor->truth_value_tensor = encode_truth_value(link.truth_value);

  // Cache and return
  link_embeddings[link.id] = linkTensor;
  return linkTensor;
}

// ================================================================
// Tensor Decoding
// ================================================================

Atom AtomSpaceTensorEncoder::decode_node_tensor(const NodeTensor *tensor) {
  Atom atom;
  atom.id = tensor->atom_id;

  // We can only approximately reconstruct the atom
  // Get embedding data
  if (tensor->embedding && tensor->embedding->ggml_tensor) {
    kernel->compute(tensor->embedding);
    float *data = (float *)tensor->embedding->ggml_tensor->data;

    // Type ID from position 0
    int typeId = static_cast<int>(data[0] * 100.0f + 0.5f);
    atom.type = get_vocabulary_symbol(typeId);
    if (atom.type.empty()) {
      atom.type = "ConceptNode"; // Default
    }

    // Name cannot be fully recovered from hash
    atom.name = "decoded_" + tensor->atom_id;
  }

  // Decode truth value
  if (tensor->truth_value_tensor && tensor->truth_value_tensor->ggml_tensor) {
    kernel->compute(tensor->truth_value_tensor);
    float *tv_data = (float *)tensor->truth_value_tensor->ggml_tensor->data;
    atom.truth_value[0] = tv_data[0];
    atom.truth_value[1] = tv_data[1];
    atom.truth_value[2] = std::exp(tv_data[2] * 10.0f) - 1.0f;
  } else {
    atom.truth_value[0] = 1.0f;
    atom.truth_value[1] = 0.0f;
    atom.truth_value[2] = 1.0f;
  }

  // Decode attention value
  if (tensor->attention_weights && tensor->attention_weights->ggml_tensor) {
    kernel->compute(tensor->attention_weights);
    float *av_data = (float *)tensor->attention_weights->ggml_tensor->data;
    atom.attention_value[0] = av_data[0] * 1000.0f;
    atom.attention_value[1] = av_data[1] * 1000.0f;
    atom.attention_value[2] = av_data[2] > 0.5f ? 1.0f : 0.0f;
  } else {
    atom.attention_value[0] = 0.0f;
    atom.attention_value[1] = 0.0f;
    atom.attention_value[2] = 0.0f;
  }

  return atom;
}

Link AtomSpaceTensorEncoder::decode_link_tensor(const LinkTensor *tensor) {
  Link link;
  link.id = tensor->atom_id;

  // Reconstruct outgoing from source/target nodes
  link.outgoing = tensor->source_nodes;
  link.outgoing.insert(link.outgoing.end(), tensor->target_nodes.begin(),
                       tensor->target_nodes.end());

  // Get type from relation tensor
  if (tensor->relation_tensor && tensor->relation_tensor->ggml_tensor) {
    kernel->compute(tensor->relation_tensor);
    float *data = (float *)tensor->relation_tensor->ggml_tensor->data;

    int typeId = static_cast<int>(data[0] * 100.0f + 0.5f);
    link.type = get_vocabulary_symbol(typeId);
    if (link.type.empty()) {
      link.type = "ListLink"; // Default
    }
  } else {
    link.type = "ListLink";
  }

  // Decode truth value
  if (tensor->truth_value_tensor && tensor->truth_value_tensor->ggml_tensor) {
    kernel->compute(tensor->truth_value_tensor);
    float *tv_data = (float *)tensor->truth_value_tensor->ggml_tensor->data;
    link.truth_value[0] = tv_data[0];
    link.truth_value[1] = tv_data[1];
    link.truth_value[2] = std::exp(tv_data[2] * 10.0f) - 1.0f;
  } else {
    link.truth_value[0] = 1.0f;
    link.truth_value[1] = 0.0f;
    link.truth_value[2] = 1.0f;
  }

  // Decode attention value
  if (tensor->attention_weights && tensor->attention_weights->ggml_tensor) {
    kernel->compute(tensor->attention_weights);
    float *av_data = (float *)tensor->attention_weights->ggml_tensor->data;
    link.attention_value[0] = av_data[0] * 1000.0f;
    link.attention_value[1] = 0.0f; // Only STI stored
    link.attention_value[2] = 0.0f;
  } else {
    link.attention_value[0] = 0.0f;
    link.attention_value[1] = 0.0f;
    link.attention_value[2] = 0.0f;
  }

  return link;
}

// ================================================================
// Cache Management
// ================================================================

std::vector<NodeTensor *> AtomSpaceTensorEncoder::get_all_node_tensors() const {
  std::vector<NodeTensor *> result;
  result.reserve(node_embeddings.size());
  for (const auto &[_, tensor] : node_embeddings) {
    result.push_back(tensor);
  }
  return result;
}

std::vector<LinkTensor *> AtomSpaceTensorEncoder::get_all_link_tensors() const {
  std::vector<LinkTensor *> result;
  result.reserve(link_embeddings.size());
  for (const auto &[_, tensor] : link_embeddings) {
    result.push_back(tensor);
  }
  return result;
}

void AtomSpaceTensorEncoder::clear_cache() {
  // Delete NodeTensor structs (ggml tensors managed by kernel)
  for (auto &[_, tensor] : node_embeddings) {
    delete tensor;
  }
  node_embeddings.clear();

  // Delete LinkTensor structs
  for (auto &[_, tensor] : link_embeddings) {
    delete tensor;
  }
  link_embeddings.clear();

  std::cout << "[AtomSpaceTensorEncoder] Cache cleared" << std::endl;
}
