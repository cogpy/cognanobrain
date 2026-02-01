#ifndef NANOBRAIN_TYPES_H
#define NANOBRAIN_TYPES_H

#include "nanobrain_kernel.h"
#include <map>
#include <string>
#include <vector>

// Equivalent to AtomSpaceAtom
struct Atom {
  std::string id;
  std::string type;
  std::string name;
  float truth_value[3];     // strength, confidence, count
  float attention_value[3]; // sti, lti, vlti
                            // ... other fields
};

struct Link {
  std::string id;
  std::string type;
  std::vector<std::string> outgoing;
  float truth_value[3];
  float attention_value[3];
}; // ... other fields

// Structures for Tensor Representations

struct NodeTensor {
  std::string id;
  std::string atom_id;
  NanoBrainTensor *embedding;
  NanoBrainTensor *attention_weights;
  NanoBrainTensor *truth_value_tensor;
  NanoBrainTensor *symbolic_features;
  std::vector<int64_t> shape; // Shape of embedding tensor
  // metadata...
};

struct LinkTensor {
  std::string id;
  std::string atom_id;
  std::vector<std::string> source_nodes;
  std::vector<std::string> target_nodes;
  NanoBrainTensor *relation_tensor;
  NanoBrainTensor *attention_weights;
  NanoBrainTensor *truth_value_tensor;
  // metadata...
};

#endif // NANOBRAIN_TYPES_H
