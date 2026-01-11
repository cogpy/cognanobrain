#ifndef NANOBRAIN_KERNEL_H
#define NANOBRAIN_KERNEL_H

#include "ggml/ggml.h"
#include <map>
#include <memory>
#include <string>
#include <vector>

struct NanoBrainTensor {
  std::string id;
  struct ggml_tensor *gradient; // Optional gradient
  bool requires_grad;
  struct ggml_tensor *ggml_tensor; // Pointer to the underlying ggml tensor
};

struct NanoBrainConfig {
  size_t memory_size; // size in bytes
  bool use_gpu;
};

class NanoBrainKernel {
public:
  NanoBrainKernel(NanoBrainConfig config);
  ~NanoBrainKernel();

  // Tensor Creation
  NanoBrainTensor *create_tensor(std::vector<int64_t> shape,
                                 ggml_type dtype = GGML_TYPE_F32,
                                 bool requires_grad = false);

  // Operations
  NanoBrainTensor *matmul(NanoBrainTensor *a, NanoBrainTensor *b);
  NanoBrainTensor *add(NanoBrainTensor *a, NanoBrainTensor *b);
  NanoBrainTensor *softmax(NanoBrainTensor *a);
  NanoBrainTensor *contract(NanoBrainTensor *a,
                            NanoBrainTensor *b); // Dot product for now

  // Element-wise Operations
  NanoBrainTensor *sub(NanoBrainTensor *a, NanoBrainTensor *b);
  NanoBrainTensor *mul(NanoBrainTensor *a, NanoBrainTensor *b);
  NanoBrainTensor *div(NanoBrainTensor *a, NanoBrainTensor *b);
  NanoBrainTensor *sin(NanoBrainTensor *a);
  NanoBrainTensor *cos(NanoBrainTensor *a);
  NanoBrainTensor *sqrt(NanoBrainTensor *a);
  NanoBrainTensor *abs(NanoBrainTensor *a);
  NanoBrainTensor *log(NanoBrainTensor *a);
  NanoBrainTensor *exp(NanoBrainTensor *a);

  // Activation & Clamping
  NanoBrainTensor *relu(NanoBrainTensor *a);
  NanoBrainTensor *gelu(NanoBrainTensor *a);
  NanoBrainTensor *clamp(NanoBrainTensor *a, float min_val, float max_val);
  NanoBrainTensor *max(NanoBrainTensor *a,
                       NanoBrainTensor *b); // Element-wise max
  NanoBrainTensor *min(NanoBrainTensor *a,
                       NanoBrainTensor *b); // Element-wise min

  // Reductions
  NanoBrainTensor *sum(NanoBrainTensor *a);
  NanoBrainTensor *mean(NanoBrainTensor *a);
  NanoBrainTensor *prod(NanoBrainTensor *a);

  // NanoBrain Core Functions
  // Computes coherence metric: 0.5 + 0.5 * sin(sqrt(product(primes)) * PI /
  // sum(primes)) Input is expected to be a tensor of primes per item (e.g.
  // [Batch, NumPrimes])
  NanoBrainTensor *compute_coherence(NanoBrainTensor *primes);

  // Utilities
  void compute(NanoBrainTensor *target); // Execute the graph ending at target
  void print_tensor(NanoBrainTensor *tensor);
  float get_value(NanoBrainTensor *tensor, int idx);
  void set_data(NanoBrainTensor *tensor, const std::vector<float> &data);

private:
  struct ggml_context *ctx;
  std::vector<uint8_t> buffer; // Memory buffer for ggml
  std::map<std::string, NanoBrainTensor *>
      tensors; // Keep track of created tensors

  std::string generate_id();
  void random_init(NanoBrainTensor *tensor); // Xavier initialization
};

#endif // NANOBRAIN_KERNEL_H
