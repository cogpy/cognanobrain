#ifndef NANOBRAIN_ENCODER_FULL_H
#define NANOBRAIN_ENCODER_FULL_H

#include "nanobrain_kernel.h"
#include "nanobrain_types.h"
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

// Forward declarations
struct AtomSpaceAtom;
struct AtomSpaceLink;

/**
 * Encoder configuration
 */
struct AtomSpaceTensorConfig {
  int node_embedding_dim = 128;
  int link_embedding_dim = 64;
  int symbolic_feature_dim = 32;
  int max_vocabulary_size = 10000;
  bool normalize_embeddings = true;
  float embedding_scale = 1.0f;
};

/**
 * AtomSpace Atom structure (full version)
 */
struct AtomSpaceAtom {
  std::string id;
  std::string type; // ConceptNode, PredicateNode, NumberNode, VariableNode
  std::string name;
  float truth_strength;
  float truth_confidence;
  float truth_count;
  float sti; // Short-term importance
  float lti; // Long-term importance
  bool vlti; // Very long-term importance flag
  float importance;
  int64_t timestamp;
};

/**
 * AtomSpace Link structure
 */
struct AtomSpaceLink {
  std::string id;
  std::string type; // InheritanceLink, SimilarityLink, ImplicationLink, etc.
  std::vector<std::string> outgoing; // IDs of connected atoms
  float truth_strength;
  float truth_confidence;
  float truth_count;
  float sti;
  float lti;
  bool vlti;
  float strength; // Link strength
};

/**
 * Node tensor metadata
 */
struct NodeTensorMetadata {
  std::string atom_type;
  int symbolic_depth;
  int degrees_of_freedom;
};

/**
 * Link tensor metadata
 */
struct LinkTensorMetadata {
  std::string link_type;
  int arity;
  int symbolic_depth;
};

/**
 * Extended NodeTensor with metadata
 */
struct NodeTensorFull {
  std::string id;
  std::string atom_id;
  NanoBrainTensor *embedding;
  NanoBrainTensor *attention_weights;
  NanoBrainTensor *truth_value_tensor;
  NanoBrainTensor *symbolic_features;
  NodeTensorMetadata metadata;
};

/**
 * Extended LinkTensor with metadata
 */
struct LinkTensorFull {
  std::string id;
  std::string atom_id;
  std::vector<std::string> source_nodes;
  std::vector<std::string> target_nodes;
  NanoBrainTensor *relation_tensor;
  NanoBrainTensor *attention_weights;
  NanoBrainTensor *truth_value_tensor;
  LinkTensorMetadata metadata;
};

/**
 * Vocabulary statistics
 */
struct VocabularyStats {
  size_t size;
  std::vector<std::string> symbols;
};

/**
 * Full AtomSpace Tensor Encoder
 *
 * Implements routines to encode hypergraph nodes and links as ggml-compatible
 * tensors, enabling neural-symbolic integration within the cognitive kernel.
 */
class AtomSpaceTensorEncoderFull {
public:
  AtomSpaceTensorEncoderFull(NanoBrainKernel *kernel,
                             const AtomSpaceTensorConfig &config);
  ~AtomSpaceTensorEncoderFull();

  // ================================================================
  // Vocabulary Management
  // ================================================================

  // Add symbol to vocabulary
  int add_to_vocabulary(const std::string &symbol);

  // Get vocabulary ID for symbol
  int get_vocabulary_id(const std::string &symbol) const;

  // Get symbol for vocabulary ID
  std::string get_symbol(int id) const;

  // Get vocabulary statistics
  VocabularyStats get_vocabulary_stats() const;

  // ================================================================
  // Node Encoding/Decoding
  // ================================================================

  // Encode AtomSpace atom as tensor
  NodeTensorFull *encode_atom(const AtomSpaceAtom &atom);

  // Decode tensor back to AtomSpace atom
  AtomSpaceAtom decode_atom(const NodeTensorFull *node_tensor);

  // Encode truth value as tensor
  NanoBrainTensor *encode_truth_value(float strength, float confidence,
                                      float count);

  // Decode truth value from tensor
  void decode_truth_value(const NanoBrainTensor *tensor, float &strength,
                          float &confidence, float &count);

  // Encode attention value as tensor
  NanoBrainTensor *encode_attention_value(float sti, float lti, bool vlti);

  // Decode attention value from tensor
  void decode_attention_value(const NanoBrainTensor *tensor, float &sti,
                              float &lti, bool &vlti);

  // ================================================================
  // Link Encoding/Decoding
  // ================================================================

  // Encode AtomSpace link as tensor
  LinkTensorFull *encode_link(const AtomSpaceLink &link);

  // Decode tensor back to AtomSpace link
  AtomSpaceLink decode_link(const LinkTensorFull *link_tensor);

  // ================================================================
  // Symbolic Features
  // ================================================================

  // Create symbolic features tensor for atom
  NanoBrainTensor *create_symbolic_features(const AtomSpaceAtom &atom);

  // Create symbolic features tensor for link
  NanoBrainTensor *create_link_symbolic_features(const AtomSpaceLink &link);

  // Calculate symbolic depth for atom
  int calculate_symbolic_depth(const AtomSpaceAtom &atom);

  // Calculate symbolic depth for link
  int calculate_link_symbolic_depth(const AtomSpaceLink &link);

  // Calculate degrees of freedom for atom
  int calculate_degrees_of_freedom(const AtomSpaceAtom &atom);

  // ================================================================
  // Batch Operations
  // ================================================================

  // Encode multiple atoms
  std::vector<NodeTensorFull *>
  encode_atoms(const std::vector<AtomSpaceAtom> &atoms);

  // Encode multiple links
  std::vector<LinkTensorFull *>
  encode_links(const std::vector<AtomSpaceLink> &links);

  // ================================================================
  // Cache Management
  // ================================================================

  // Get all encoded node tensors
  std::vector<NodeTensorFull *> get_node_tensors() const;

  // Get all encoded link tensors
  std::vector<LinkTensorFull *> get_link_tensors() const;

  // Get node tensor by atom ID
  NodeTensorFull *get_node_tensor(const std::string &atom_id);

  // Get link tensor by link ID
  LinkTensorFull *get_link_tensor(const std::string &link_id);

  // Clear all cached tensors
  void clear_cache();

  // ================================================================
  // Utility
  // ================================================================

  // Get tensor kernel for direct operations
  NanoBrainKernel *get_kernel() { return kernel; }

  // Get configuration
  const AtomSpaceTensorConfig &get_config() const { return config; }

  // Hash a string for embedding
  uint32_t hash_string(const std::string &str) const;

private:
  NanoBrainKernel *kernel;
  AtomSpaceTensorConfig config;

  // Vocabulary maps
  std::map<std::string, int> vocabulary_map;
  std::map<int, std::string> reverse_vocabulary_map;
  int next_vocab_id;

  // Tensor caches
  std::map<std::string, NodeTensorFull *> node_embeddings;
  std::map<std::string, LinkTensorFull *> link_embeddings;

  // Initialize vocabulary with common types
  void initialize_vocabulary();

  // Generate tensor ID
  std::string generate_node_tensor_id(const std::string &atom_id);
  std::string generate_link_tensor_id(const std::string &link_id);
};

#endif // NANOBRAIN_ENCODER_FULL_H
