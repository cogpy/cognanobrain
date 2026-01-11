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

  NodeTensor *encode_atom(const Atom &atom);
  // LinkTensor* encode_link(const AtomSpaceLink& link); // TODO: Define
  // AtomSpaceLink

private:
  NanoBrainKernel *kernel;
  std::map<std::string, NodeTensor *> node_embeddings;
  std::map<std::string, int> vocabulary_map;

  int get_vocabulary_id(const std::string &symbol);
  NanoBrainTensor *encode_truth_value(const float *tv);
  NanoBrainTensor *encode_attention_value(const float *av);
};

#endif // NANOBRAIN_ENCODER_H
