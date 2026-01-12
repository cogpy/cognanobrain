# NanoBrain Python Bindings

Python interface to the NanoBrain C++ cognitive architecture.

## Installation

### Prerequisites

- Python 3.8+
- C++17 compiler
- pybind11
- ggml library (from llama.cpp or standalone)

### Install from source

```bash
# Clone the repository
git clone https://github.com/nanobrain/nanobrain.git
cd nanobrain/python

# Set GGML path (adjust to your installation)
export GGML_PATH=/path/to/llama.cpp

# Install in development mode
pip install -e .
```

## Quick Start

```python
import nanobrain as nb

# Create configuration
config = nb.UnifiedConfig()
config.time_crystal_dimensions = 11
config.enable_meta_cognition = True

# Initialize kernel
kernel = nb.UnifiedKernel(config)
kernel.initialize()

# Create atoms
cat = kernel.create_atom("ConceptNode", "Cat", 0.9, 0.8)
animal = kernel.create_atom("ConceptNode", "Animal", 0.95, 0.9)

# Check metrics
metrics = kernel.get_metrics()
print(f"Atoms: {metrics.total_atoms}")
print(f"Consciousness: {metrics.consciousness_emergence}")

# Cleanup
kernel.shutdown()
```

## Core Classes

### UnifiedKernel

Main entry point for NanoBrain functionality.

```python
kernel = nb.UnifiedKernel(config)
kernel.initialize()
atom = kernel.create_atom("ConceptNode", "Cat", 0.9, 0.8)
metrics = kernel.get_metrics()
```

### TimeCrystalKernel

11D quantum manifold processing.

```python
tc_config = nb.TimeCrystalConfig()
tc = nb.TimeCrystalKernel(tc_config)
tc.initialize()
coherence = tc.compute_ppm_coherence([2, 3, 5, 7])
```

### TCTransformEngine

Prime-based signal analysis (alternative to FFT).

```python
transform = nb.TCTransformEngine(kernel, nb.TCTransformConfig())
result = transform.transform([0.1, 0.2, 0.3, 0.4, 0.5])
print(f"Dominant primes: {result.dominant_primes}")
```

### GardenOfGardens

Nested time crystal hierarchy.

```python
gog = nb.GardenOfGardens(kernel, nb.GOGConfig())
gog.initialize()
garden_id = gog.create_garden("main_garden")
```

## Constants

```python
print(nb.FUNDAMENTAL_PRIMES)  # [2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47]
print(nb.TIME_CRYSTAL_DIMENSIONS)  # 11
print(nb.GOLDEN_RATIO)  # 1.618...
```

## License

MIT License
