#ifndef NANOBRAIN_ENCODER_H
#define NANOBRAIN_ENCODER_H

#include "nanobrain_kernel.h"
#include "nanobrain_types.h"
#include <map>
#include <string>

class AtomSpaceTensorEncoder {
public:
  AtomSpaceTensorEncoder(NanoBrainKernel *kernel);
  ~AtomSpaceTensorEncoder();

  // Encode atom into NodeTensor
  NodeTensor *encode_atom(const Atom &atom);

  // Encode link into LinkTensor
  LinkTensor *encode_link(const Link &link);

  // Decode tensors back to Atoms/Links
  Atom decode_node_tensor(const NodeTensor *tensor);
  Link decode_link_tensor(const LinkTensor *tensor);

  // Get all cached embeddings
  std::vector<NodeTensor *> get_all_node_tensors() const;
  std::vector<LinkTensor *> get_all_link_tensors() const;

  // Clear cached embeddings
  void clear_cache();

  // Get vocabulary statistics
  size_t get_vocabulary_size() const { return vocabulary_map.size(); }

private:
  NanoBrainKernel *kernel;
  std::map<std::string, NodeTensor *> node_embeddings;
  std::map<std::string, LinkTensor *> link_embeddings;
  std::map<std::string, int> vocabulary_map;
  std::map<int, std::string> reverse_vocabulary_map;

  int get_vocabulary_id(const std::string &symbol);
  std::string get_vocabulary_symbol(int id) const;
  NanoBrainTensor *encode_truth_value(const float *tv);
  NanoBrainTensor *encode_attention_value(const float *av);
  size_t hash_string(const std::string &str) const;
};

#endif // NANOBRAIN_ENCODER_H
