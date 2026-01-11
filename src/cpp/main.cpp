#include "nanobrain_kernel.h"
#include <iostream>
#include <vector>

int main() {
  std::cout << "Initializing NanoBrain Kernel (C++ adaptation)..." << std::endl;

  NanoBrainConfig config;
  config.memory_size = 1024 * 1024 * 64; // 64 MB
  config.use_gpu = false;

  NanoBrainKernel kernel(config);

  std::cout << "Creating tensors..." << std::endl;
  // Create two matrices A (2x3) and B (3x2)
  // Note: ggml uses column-major order implicitly in some contexts, but let's
  // assume standard shape usage A: 2 rows, 3 cols
  auto A = kernel.create_tensor({2, 3});
  // B: 3 rows, 2 cols
  auto B = kernel.create_tensor({3, 2});

  std::cout << "Tensor A created: " << A->id << std::endl;
  kernel.print_tensor(A);
  std::cout << "Tensor B created: " << B->id << std::endl;
  kernel.print_tensor(B);

  std::cout << "Performing Matrix Multiplication C = A * B..." << std::endl;
  auto C = kernel.matmul(A, B);

  std::cout << "Computing graph..." << std::endl;
  kernel.compute(C);

  std::cout << "Result C:" << std::endl;
  kernel.print_tensor(C);

  std::cout << "Performing Softmax on C..." << std::endl;
  auto S = kernel.softmax(C);
  kernel.compute(S);

  std::cout << "Result Softmax(C):" << std::endl;
  kernel.print_tensor(S);

  std::cout << "Done." << std::endl;

  return 0;
}
