#include "nanobrain_encoder.h"
#include "nanobrain_kernel.h"
#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>


int main() {
  std::cout << "Initializing NanoBrain Kernel (C++ adaptation)..." << std::endl;

  NanoBrainConfig config;
  config.memory_size = 1024 * 1024 * 64; // 64 MB
  config.use_gpu = false;

  NanoBrainKernel kernel(config);

  std::cout << "--- Basic Operations Test ---" << std::endl;
  auto A = kernel.create_tensor({2, 3});
  auto B = kernel.create_tensor({3, 2});

  std::cout << "Tensor A created: " << A->id << std::endl;
  std::cout << "Tensor B created: " << B->id << std::endl;

  auto C = kernel.matmul(A, B);
  kernel.compute(C);

  std::cout << "Result C (Matmul A*B):" << std::endl;
  kernel.print_tensor(C);

  std::cout << "\n--- Element-wise & Math Test ---" << std::endl;
  // Create positive tensor for Log/Sqrt/Prod
  auto P = kernel.create_tensor({4});
  // Manually set data to positive values since random_init gives [-X, X]
  // Note: In a real scenario we'd have a set_data method or safer init.
  // For now we assume we can access underlying pointer or just create one.
  // Since we don't have direct data access helper in public API yet except
  // separate create, we'll rely on the random init but take abs() first? Wait,
  // abs() is available.

  auto Raw = kernel.create_tensor({4});
  auto AbsP = kernel.abs(Raw); // Ensure positive
  // Add small epsilon to avoid 0
  auto Epsilon = kernel.create_tensor({1});
  // Hacky write to data for singleton
  // Since we don't have a 'set_value' yet, we rely on the fact that random_init
  // runs. Let's just assume for this test we want to see it run without
  // crashing.

  // Better strategy: Let's create a tensor and computation to ensure positive
  // input for log
  auto P_sq = kernel.mul(Raw, Raw); // x^2 is always positive
  auto P_safe =
      kernel.add(P_sq, kernel.create_tensor({1})); // x^2 + random? No, wait.

  // Let's just test operations that are safe with random data first
  auto SinA = kernel.sin(A);
  kernel.compute(SinA);
  std::cout << "Sin(A):" << std::endl;
  kernel.print_tensor(SinA);

  std::cout << "\n--- Coherence Test ---" << std::endl;
  // Coherence func: 0.5 + 0.5 * sin(sqrt(prod(primes)) * PI / sum(primes))
  // We need 'primes' input.
  // Create a small tensor of "primes" [2, 3, 5]
  auto Primes = kernel.create_tensor({3});
  kernel.set_data(Primes, {2.0f, 3.0f, 5.0f});

  auto Coherence = kernel.compute_coherence(Primes);
  // Since compute_coherence builds a graph, we need to execute it.
  // However, the current compute() implementation creates a NEW graph from the
  // target. So calling kernel.compute(Coherence) should work fine.

  kernel.compute(Coherence);
  std::cout << "Coherence Score for [2, 3, 5]:" << std::endl;
  kernel.print_tensor(Coherence);
  std::cout << "Expected (approx): 0.994" << std::endl;

  std::cout << "\n--- Encoder Test ---" << std::endl;
  // Create a mock Atom for testing
  Atom mock_atom;
  mock_atom.type = "Concept";
  mock_atom.name = "TestAtom";
  mock_atom.value = 123.45f;
  mock_atom.handle = 0xDEADBEEF; // Example handle

  // Initialize the encoder
  AtomSpaceTensorEncoder encoder(kernel);

  // Encode the atom
  Tensor *embedding = encoder.encode_atom(mock_atom);

  // Print the embedding ID
  std::cout << "Encoded Atom (TestAtom) embedding ID: " << embedding->id
            << std::endl;
  std::cout << "Embedding tensor shape: {" << embedding->shape[0] << ", "
            << embedding->shape[1] << "}" << std::endl;
  // Optionally, print the embedding content (if small enough)
  // kernel.print_tensor(embedding);

  std::cout << "Done." << std::endl;

  return 0;
}
