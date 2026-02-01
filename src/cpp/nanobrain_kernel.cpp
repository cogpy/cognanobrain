#include "nanobrain_kernel.h"
#include <cmath>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <random>

NanoBrainKernel::NanoBrainKernel(NanoBrainConfig config) {
  struct ggml_init_params params = {
      /*.mem_size   =*/config.memory_size,
      /*.mem_buffer =*/NULL,
      /*.no_alloc   =*/false,
  };

  this->ctx = ggml_init(params);
  if (!this->ctx) {
    std::cerr << "Failed to initialize GGML context" << std::endl;
  }

  // Seed random number generator
  std::srand(static_cast<unsigned int>(std::time(nullptr)));
}

NanoBrainKernel::~NanoBrainKernel() {
  if (this->ctx) {
    ggml_free(this->ctx);
  }
  // Clean up NanoBrainTensor wrappers
  for (auto const &[id, tensor] : this->tensors) {
    delete tensor;
  }
}

std::string NanoBrainKernel::generate_id() {
  static int counter = 0;
  return "tensor_" + std::to_string(counter++);
}

void NanoBrainKernel::random_init(NanoBrainTensor *tensor) {
  if (!tensor || !tensor->ggml_tensor)
    return;

  struct ggml_tensor *t = tensor->ggml_tensor;
  int64_t size = ggml_nelements(t);
  float *data = (float *)t->data;

  // Xavier/Glorot initialization
  // limit = sqrt(6 / (fan_in + fan_out))
  // Roughly approximated here using just dimensions sum
  float limit = std::sqrt(
      6.0f / (t->ne[0] + (t->n_dims > 1
                              ? t->ne[1]
                              : 1))); // ne is number of elements per dimension

  for (int64_t i = 0; i < size; i++) {
    data[i] = ((float)std::rand() / RAND_MAX - 0.5f) * 2.0f * limit;
  }
}

void NanoBrainKernel::set_data(NanoBrainTensor *tensor,
                               const std::vector<float> &data) {
  if (!tensor || !tensor->ggml_tensor)
    return;

  // Check size match
  if (data.size() != (size_t)ggml_nelements(tensor->ggml_tensor)) {
    std::cerr << "Data size mismatch for tensor " << tensor->id << std::endl;
    return;
  }

  // Copy data
  memcpy(tensor->ggml_tensor->data, data.data(), data.size() * sizeof(float));
}

NanoBrainTensor *NanoBrainKernel::create_tensor(std::vector<int64_t> shape,
                                                ggml_type dtype,
                                                bool requires_grad) {
  NanoBrainTensor *tensor = new NanoBrainTensor();
  tensor->id = generate_id();
  tensor->requires_grad = requires_grad;
  tensor->gradient = nullptr;

  if (shape.size() == 1) {
    tensor->ggml_tensor = ggml_new_tensor_1d(this->ctx, dtype, shape[0]);
  } else if (shape.size() == 2) {
    tensor->ggml_tensor =
        ggml_new_tensor_2d(this->ctx, dtype, shape[0], shape[1]);
  } else if (shape.size() == 3) {
    tensor->ggml_tensor =
        ggml_new_tensor_3d(this->ctx, dtype, shape[0], shape[1], shape[2]);
  } else if (shape.size() == 4) {
    tensor->ggml_tensor = ggml_new_tensor_4d(this->ctx, dtype, shape[0],
                                             shape[1], shape[2], shape[3]);
  } else {
    std::cerr << "Unsupported tensor dimension: " << shape.size() << std::endl;
    delete tensor;
    return nullptr;
  }

  if (requires_grad) {
    // In a full implementation, we'd allocate gradient tensor here
    // tensor->gradient = ...
  }

  // Initialize data
  random_init(tensor);

  this->tensors[tensor->id] = tensor;
  return tensor;
}

NanoBrainTensor *NanoBrainKernel::matmul(NanoBrainTensor *a,
                                         NanoBrainTensor *b) {
  NanoBrainTensor *result = new NanoBrainTensor();
  result->id = generate_id();
  result->requires_grad = a->requires_grad || b->requires_grad;

  // ggml_mul_mat computes A * B^T typically, or standard matmul depending on
  // newness. Assuming standard matrix multiplication A * B
  result->ggml_tensor = ggml_mul_mat(this->ctx, a->ggml_tensor, b->ggml_tensor);

  this->tensors[result->id] = result;
  return result;
}

NanoBrainTensor *NanoBrainKernel::add(NanoBrainTensor *a, NanoBrainTensor *b) {
  NanoBrainTensor *result = new NanoBrainTensor();
  result->id = generate_id();
  result->requires_grad = a->requires_grad || b->requires_grad;

  result->ggml_tensor = ggml_add(this->ctx, a->ggml_tensor, b->ggml_tensor);

  this->tensors[result->id] = result;
  return result;
}

NanoBrainTensor *NanoBrainKernel::softmax(NanoBrainTensor *a) {
  NanoBrainTensor *result = new NanoBrainTensor();
  result->id = generate_id();
  result->requires_grad = a->requires_grad;

  result->ggml_tensor = ggml_soft_max(this->ctx, a->ggml_tensor);

  this->tensors[result->id] = result;
  return result;
}

NanoBrainTensor *NanoBrainKernel::contract(NanoBrainTensor *a,
                                           NanoBrainTensor *b) {
  // For now, implementing as dot product via mul_mat if 1D, or matmul
  // If 1D: dot product
  if (a->ggml_tensor->n_dims == 1 && b->ggml_tensor->n_dims == 1) {
    // 1D dot product can be done via mul_mat by treating them as 1xN vectors?
    // Or ggml_dot? ggml doesn't have a direct 'dot' for 1D float arrays exposed
    // simply without reshaping. Actually ggml_mul_mat is broadly used.
    return matmul(a, b);
  }
  return matmul(a, b);
}

NanoBrainTensor *NanoBrainKernel::sub(NanoBrainTensor *a, NanoBrainTensor *b) {
  NanoBrainTensor *result = new NanoBrainTensor();
  result->id = generate_id();
  result->requires_grad = a->requires_grad || b->requires_grad;
  result->ggml_tensor = ggml_sub(this->ctx, a->ggml_tensor, b->ggml_tensor);
  this->tensors[result->id] = result;
  return result;
}

NanoBrainTensor *NanoBrainKernel::mul(NanoBrainTensor *a, NanoBrainTensor *b) {
  NanoBrainTensor *result = new NanoBrainTensor();
  result->id = generate_id();
  result->requires_grad = a->requires_grad || b->requires_grad;
  result->ggml_tensor = ggml_mul(this->ctx, a->ggml_tensor, b->ggml_tensor);
  this->tensors[result->id] = result;
  return result;
}

NanoBrainTensor *NanoBrainKernel::div(NanoBrainTensor *a, NanoBrainTensor *b) {
  NanoBrainTensor *result = new NanoBrainTensor();
  result->id = generate_id();
  result->requires_grad = a->requires_grad || b->requires_grad;
  result->ggml_tensor = ggml_div(this->ctx, a->ggml_tensor, b->ggml_tensor);
  this->tensors[result->id] = result;
  return result;
}

NanoBrainTensor *NanoBrainKernel::sin(NanoBrainTensor *a) {
  NanoBrainTensor *result = new NanoBrainTensor();
  result->id = generate_id();
  result->requires_grad = a->requires_grad;
  result->ggml_tensor = ggml_sin(this->ctx, a->ggml_tensor);
  this->tensors[result->id] = result;
  return result;
}

NanoBrainTensor *NanoBrainKernel::cos(NanoBrainTensor *a) {
  NanoBrainTensor *result = new NanoBrainTensor();
  result->id = generate_id();
  result->requires_grad = a->requires_grad;
  result->ggml_tensor = ggml_cos(this->ctx, a->ggml_tensor);
  this->tensors[result->id] = result;
  return result;
}

NanoBrainTensor *NanoBrainKernel::sqrt(NanoBrainTensor *a) {
  NanoBrainTensor *result = new NanoBrainTensor();
  result->id = generate_id();
  result->requires_grad = a->requires_grad;
  result->ggml_tensor = ggml_sqrt(this->ctx, a->ggml_tensor);
  this->tensors[result->id] = result;
  return result;
}

NanoBrainTensor *NanoBrainKernel::abs(NanoBrainTensor *a) {
  NanoBrainTensor *result = new NanoBrainTensor();
  result->id = generate_id();
  result->requires_grad = a->requires_grad;
  result->ggml_tensor = ggml_abs(this->ctx, a->ggml_tensor);
  this->tensors[result->id] = result;
  return result;
}

NanoBrainTensor *NanoBrainKernel::log(NanoBrainTensor *a) {
  NanoBrainTensor *result = new NanoBrainTensor();
  result->id = generate_id();
  result->requires_grad = a->requires_grad;
  result->ggml_tensor = ggml_log(this->ctx, a->ggml_tensor);
  this->tensors[result->id] = result;
  return result;
}

NanoBrainTensor *NanoBrainKernel::exp(NanoBrainTensor *a) {
  NanoBrainTensor *result = new NanoBrainTensor();
  result->id = generate_id();
  result->requires_grad = a->requires_grad;
  result->ggml_tensor = ggml_exp(this->ctx, a->ggml_tensor);
  this->tensors[result->id] = result;
  return result;
}

NanoBrainTensor *NanoBrainKernel::sum(NanoBrainTensor *a) {
  NanoBrainTensor *result = new NanoBrainTensor();
  result->id = generate_id();
  result->requires_grad = a->requires_grad;
  result->ggml_tensor = ggml_sum(this->ctx, a->ggml_tensor);
  this->tensors[result->id] = result;
  return result;
}

NanoBrainTensor *NanoBrainKernel::mean(NanoBrainTensor *a) {
  NanoBrainTensor *result = new NanoBrainTensor();
  result->id = generate_id();
  result->requires_grad = a->requires_grad;
  result->ggml_tensor = ggml_mean(this->ctx, a->ggml_tensor);
  this->tensors[result->id] = result;
  return result;
}

NanoBrainTensor *NanoBrainKernel::prod(NanoBrainTensor *a) {
  // prod = exp(sum(log(a)))
  NanoBrainTensor *l = log(a);
  NanoBrainTensor *s = sum(l);
  return exp(s);
}

NanoBrainTensor *NanoBrainKernel::compute_coherence(NanoBrainTensor *primes) {
  // 0.5 + 0.5 * sin(sqrt(prod(primes)) * PI / sum(primes))

  NanoBrainTensor *p_prod = prod(primes);
  NanoBrainTensor *p_sum = sum(primes);
  NanoBrainTensor *p_sqrt_prod = sqrt(p_prod);

  NanoBrainTensor *t_pi = create_tensor({1});
  ((float *)t_pi->ggml_tensor->data)[0] = 3.1415926535f;

  NanoBrainTensor *t_half = create_tensor({1});
  ((float *)t_half->ggml_tensor->data)[0] = 0.5f;

  NanoBrainTensor *num = mul(p_sqrt_prod, t_pi);
  NanoBrainTensor *arg = div(num, p_sum);

  NanoBrainTensor *s = sin(arg);
  NanoBrainTensor *term = mul(t_half, s);
  NanoBrainTensor *result = add(t_half, term);

  return result;
}

void NanoBrainKernel::compute(NanoBrainTensor *target) {
  struct ggml_cgraph *gf = ggml_new_graph(this->ctx);
  ggml_build_forward_expand(gf, target->ggml_tensor);
  ggml_graph_compute_with_ctx(this->ctx, gf, 1); // 1 thread for simplicity
}

void NanoBrainKernel::print_tensor(NanoBrainTensor *tensor) {
  if (!tensor || !tensor->ggml_tensor)
    return;
  struct ggml_tensor *t = tensor->ggml_tensor;

  std::cout << "Tensor " << tensor->id << " Shape: [";
  for (int i = 0; i < t->n_dims; i++) {
    std::cout << t->ne[i] << (i < t->n_dims - 1 ? ", " : "");
  }
  std::cout << "]" << std::endl;

  // Print first few elements
  float *data = (float *)t->data;
  int count = std::min((int)ggml_nelements(t), 10);
  std::cout << "Data: [";
  for (int i = 0; i < count; i++) {
    std::cout << std::fixed << std::setprecision(4) << data[i]
              << (i < count - 1 ? ", " : "");
  }
  if (ggml_nelements(t) > 10)
    std::cout << "...";
  std::cout << "]" << std::endl;
}

float NanoBrainKernel::get_value(NanoBrainTensor *tensor, int idx) {
  if (!tensor || !tensor->ggml_tensor)
    return 0.0f;
  float *data = (float *)tensor->ggml_tensor->data;
  if (idx >= 0 && idx < ggml_nelements(tensor->ggml_tensor)) {
    return data[idx];
  }
  return 0.0f;
}

void NanoBrainKernel::set_data(NanoBrainTensor *tensor,
                               const std::vector<float> &data) {
  if (!tensor || !tensor->ggml_tensor)
    return;

  struct ggml_tensor *t = tensor->ggml_tensor;
  int64_t size = ggml_nelements(t);

  if (data.size() != (size_t)size) {
    std::cerr << "Warning: set_data size mismatch. Expected " << size
              << ", got " << data.size() << std::endl;
    return;
  }

  // memcpy or loop
  float *t_data = (float *)t->data;
  for (size_t i = 0; i < data.size(); i++) {
    t_data[i] = data[i];
  }
}
