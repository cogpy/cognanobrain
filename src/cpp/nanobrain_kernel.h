#ifndef NANOBRAIN_KERNEL_H
#define NANOBRAIN_KERNEL_H

#include "ggml/ggml.h"
#include <vector>
#include <string>
#include <map>
#include <memory>

struct NanoBrainTensor {
    std::string id;
    struct ggml_tensor* gradient; // Optional gradient
    bool requires_grad;
    struct ggml_tensor* ggml_tensor; // Pointer to the underlying ggml tensor
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
    NanoBrainTensor* create_tensor(std::vector<int64_t> shape, ggml_type dtype = GGML_TYPE_F32, bool requires_grad = false);
    
    // Operations
    NanoBrainTensor* matmul(NanoBrainTensor* a, NanoBrainTensor* b);
    NanoBrainTensor* add(NanoBrainTensor* a, NanoBrainTensor* b);
    NanoBrainTensor* softmax(NanoBrainTensor* a);
    NanoBrainTensor* contract(NanoBrainTensor* a, NanoBrainTensor* b); // Dot product for now
    
    // Utilities
    void compute(NanoBrainTensor* target); // Execute the graph ending at target
    void print_tensor(NanoBrainTensor* tensor);
    float get_value(NanoBrainTensor* tensor, int idx);

private:
    struct ggml_context* ctx;
    std::vector<uint8_t> buffer; // Memory buffer for ggml
    std::map<std::string, NanoBrainTensor*> tensors; // Keep track of created tensors

    std::string generate_id();
    void random_init(NanoBrainTensor* tensor); // Xavier initialization
};

#endif // NANOBRAIN_KERNEL_H
