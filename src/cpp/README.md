# NanoBrain C++ / llama.cpp Adaptation

A high-performance C++ implementation of the NanoBrain Time Crystal Architecture,
designed for integration with [llama.cpp](https://github.com/ggerganov/llama.cpp)
and the [ggml](https://github.com/ggerganov/ggml) tensor library.

## Overview

This module implements the complete OpenCog NanoBrain ecosystem including:

- **Phase Prime Metric (PPM)** - Universal pattern recognition using prime number harmonics
- **Time Crystal Processing** - 11-dimensional quantum manifold state management
- **Geometric Musical Language (GML)** - Shape and musical note resonance calculations
- **ECAN Attention Allocation** - Economic attention with rent/wage mechanics
- **PLN Reasoning** - Probabilistic Logic Network inference with time crystal enhancement
- **Recursive Reasoning Engine** - Multi-chain differentiable inference paths
- **Tensor Operations** - All operations via ggml for llama.cpp compatibility

## Files

| File | Description |
|------|-------------|
| `nanobrain_kernel.h/cpp` | Core tensor operations wrapper around ggml |
| `nanobrain_types.h` | Shared data structures (Atom, NodeTensor, etc.) |
| `nanobrain_encoder.h/cpp` | AtomSpace to tensor encoding |
| `nanobrain_time_crystal.h/cpp` | Unified Time Crystal Kernel |
| `nanobrain_reasoning.h/cpp` | PLN recursive reasoning engine |
| `nanobrain_attention.h/cpp` | Attention allocation subsystem |
| `time_crystal_demo.cpp` | Comprehensive feature demonstration |
| `main.cpp` | Basic tests |
| `CMakeLists.txt` | Build configuration |

## Building

### Prerequisites

- CMake 3.10+
- C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2019+)
- ggml or llama.cpp source code

### Build Steps

```bash
# Clone llama.cpp (includes ggml)
git clone https://github.com/ggerganov/llama.cpp
cd llama.cpp && mkdir build && cd build
cmake .. && make ggml

# Build NanoBrain
cd /path/to/nanobrain/src/cpp
mkdir build && cd build
cmake .. -DGGML_PATH=/path/to/llama.cpp -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
```

### Windows (Visual Studio)

```powershell
cmake -B build -DGGML_PATH=C:\path\to\llama.cpp
cmake --build build --config Release
```

## Quick Start

```cpp
#include "nanobrain_time_crystal.h"

int main() {
    // Initialize kernel
    TimeCrystalConfig config;
    TimeCrystalKernel kernel(config);
    kernel.initialize();

    // Compute PPM coherence for primes [2, 3, 5]
    float coherence = kernel.compute_ppm_coherence({2, 3, 5});
    // Expected: ~0.994 (high coherence)

    // Run processing cycles
    for (int i = 0; i < 100; i++) {
        kernel.process_cycle();
    }

    // Get metrics
    auto metrics = kernel.get_metrics();
    printf("Consciousness emergence: %f\n", metrics.consciousness_emergence);

    kernel.shutdown();
    return 0;
}
```

## Key Concepts

### Phase Prime Metric (PPM)

The PPM coherence formula:

```
coherence = 0.5 + 0.5 * sin(sqrt(Π primes) * π / Σ primes)
```

This creates a harmonic relationship between prime numbers that serves as a
universal pattern recognition mechanism.

### 11D Time Crystal States

Each atom maintains an 11-dimensional quantum state with:

- Temporal coherence (stability measure)
- Fractal dimension (complexity measure)
- Quantum phase (oscillation state)
- Resonance frequency (derived from prime encoding)

### Geometric Musical Language (GML)

Shapes and musical notes are mapped to prime numbers, creating a unified
representation for geometric and acoustic patterns.

## Performance

- Header-only dependencies minimize compile times
- ggml backend enables future GPU acceleration
- Memory-efficient tensor pooling via ggml context
- Single-threaded by default (multi-threading planned)

## License

Same as parent project.

## References

- [OpenCog AtomSpace](https://wiki.opencog.org/w/AtomSpace)
- [PLN (Probabilistic Logic Networks)](https://wiki.opencog.org/w/PLN)
- [ECAN (Economic Attention Allocation)](https://wiki.opencog.org/w/ECAN)
- [ggml](https://github.com/ggerganov/ggml)
- [llama.cpp](https://github.com/ggerganov/llama.cpp)
