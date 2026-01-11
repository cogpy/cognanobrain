#include "nanobrain_encoder.h"
#include <cmath>
#include <iostream>


AtomSpaceTensorEncoder::AtomSpaceTensorEncoder(NanoBrainKernel *kernel)
    : kernel(kernel) {
  // Basic vocabulary initialization
  get_vocabulary_id("ConceptNode");
  get_vocabulary_id("PredicateNode");
  get_vocabulary_id("ListLink");
  // ...
}

AtomSpaceTensorEncoder::~AtomSpaceTensorEncoder() {
  // Tensors are managed by the kernel, but we might want to clean up the struct
  // pointers
  for (auto const &[name, tensor] : node_embeddings) {
    delete tensor;
  }
}

int AtomSpaceTensorEncoder::get_vocabulary_id(const std::string &symbol) {
  if (vocabulary_map.find(symbol) == vocabulary_map.end()) {
    int id = vocabulary_map.size();
    vocabulary_map[symbol] = id;
  }
  return vocabulary_map[symbol];
}

NanoBrainTensor *AtomSpaceTensorEncoder::encode_truth_value(const float *tv) {
  NanoBrainTensor *tensor = kernel->create_tensor({3});
  std::vector<float> data = {tv[0], tv[1], std::log(tv[2] + 1.0f) / 10.0f};
  kernel->set_data(tensor, data);
  return tensor;
}

NanoBrainTensor *
AtomSpaceTensorEncoder::encode_attention_value(const float *av) {
  NanoBrainTensor *tensor = kernel->create_tensor({3});
  std::vector<float> data = {av[0] / 100.0f, av[1] / 100.0f,
                             av[2] > 0.5f ? 1.0f : 0.0f};
  kernel->set_data(tensor, data);
  return tensor;
}

NodeTensor *AtomSpaceTensorEncoder::encode_atom(const Atom &atom) {
  if (node_embeddings.find(atom.id) != node_embeddings.end()) {
    return node_embeddings[atom.id];
  }

  NodeTensor *nodeTensor = new NodeTensor();
  nodeTensor->id = "node_tensor_" + atom.id;
  nodeTensor->atom_id = atom.id;

  // Embedding: Size 128 (default)
  // We'll mimic the TS logic: [TypeID, NameHash, ...]
  nodeTensor->embedding = kernel->create_tensor({128}, GGML_TYPE_F32, true);

  // We need to set at least the first few values
  // Since we don't have partial updates, we might need to read back, direct
  // write, or set full buffer. For efficiency in C++, we should probably
  // interact with memory directly if possible, or build the vector.
  std::vector<float> embed_data(128, 0.0f); // Zero init

  // Type encoding
  int typeId = get_vocabulary_id(atom.type);
  embed_data[0] = typeId / 100.0f;

  // Name hash
  std::hash<std::string> hasher;
  size_t hash = hasher(atom.name);
  embed_data[1] = (hash % 1000) / 1000.0f;

  kernel->set_data(nodeTensor->embedding, embed_data);

  nodeTensor->truth_value_tensor = encode_truth_value(atom.truth_value);
  nodeTensor->attention_weights = encode_attention_value(atom.attention_value);

  // Features (Simplified)
  nodeTensor->symbolic_features = kernel->create_tensor({128});

  node_embeddings[atom.id] = nodeTensor;
  return nodeTensor;
}
