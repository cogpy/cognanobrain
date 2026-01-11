# NanoBrain C++ Kernel Adaptation

This directory contains the C++ adaptation of the NanoBrain cognitive kernel functions, utilizing the `ggml` tensor library (the core of `llama.cpp`).

## Prerequisites

- **ggml**: You need the `ggml` library. You can get it from the [llama.cpp repository](https://github.com/ggerganov/llama.cpp) or the standalone [ggml repository](https://github.com/ggerganov/ggml).

## Structure

- `nanobrain_kernel.h`: Header file defining the `NanoBrainKernel` class and tensor structures.
- `nanobrain_kernel.cpp`: Implementation of the kernel using `ggml` functions.
- `main.cpp`: A simple test driver demonstrating tensor creation, matrix multiplication, and softmax operations.
- `CMakeLists.txt`: CMake build configuration.

## building

1. Ensure you have `cmake` and a C++ compiler installed.
2. Clone `llama.cpp` or `ggml` to a directory (e.g., `./ggml`).
3. Update `GGML_PATH` in `CMakeLists.txt` if necessary.
4. Run:
   ```bash
   mkdir build
   cd build
   cmake ..
   make
   ```
5. Run the test:
   ```bash
   ./nanobrain_test
   ```

## Implementation Details

The adaptation maps high-level NanoBrain operations to `ggml` primitives:
- `createTensor` -> `ggml_new_tensor` (with Xavier initialization)
- `matmul` -> `ggml_mul_mat`
- `softmax` -> `ggml_soft_max`
- `add` -> `ggml_add`

The `NanoBrainKernel` class manages the `ggml_context` and handles memory management for tensors.
