# Chapter 2: Fractal Tape & GML Implementation Plan

Implementing tasks 2.1-2.5 from the NanoBrain Development Framework for fractal information
theory and geometric musical language features.

## Background

Chapter 2 replaces the Turing tape with fractal information theory. The existing codebase
already has:

- 19 GML shapes in `GMLShape` enum (exceeds 15 requirement)
- `GeometricPattern` struct with prime resonance
- `TimeCrystalQuantumState` with 11D dimensions
- Musical note encoding via `MusicalNote` enum

## Proposed Changes

### Component 1: GML Shape Operations

#### [MODIFY] [nanobrain_time_crystal.h](file:///e:/antg/nanob/cognanobrain/src/cpp/nanobrain_time_crystal.h)

- Add `ShapeToTensorConverter` class declaration
- Add composition operators: `compose_shapes()`, `transform_shape()`
- Add prime-shape resonance calculation methods

#### [MODIFY] [nanobrain_time_crystal.cpp](file:///e:/antg/nanob/cognanobrain/src/cpp/nanobrain_time_crystal.cpp)

- Implement `shape_to_tensor()` conversion
- Implement `compose_shapes()` for combining geometric patterns
- Add `calculate_shape_prime_resonance()`

---

### Component 2: FractalTape System

#### [NEW] [nanobrain_fractal_tape.h](file:///e:/antg/nanob/cognanobrain/src/cpp/nanobrain_fractal_tape.h)

- `FractalTapeConfig` struct
- `FractalTapeCell` struct for nested data
- `FractalTape` class with:
  - Multi-dimensional tape navigation
  - Sphere surgery for 2D→3D transformations
  - Self-assembly operators
  - Integration point with `TimeCrystalKernel`

#### [NEW] [nanobrain_fractal_tape.cpp](file:///e:/antg/nanob/cognanobrain/src/cpp/nanobrain_fractal_tape.cpp)

- Complete implementation of `FractalTape` class
- ~400-500 lines estimated

---

### Component 3: Multidimensional Sensors

#### [MODIFY] [nanobrain_fractal_tape.h](file:///e:/antg/nanob/cognanobrain/src/cpp/nanobrain_fractal_tape.h)

- `MultidimensionalSensor` interface class
- `NerveBundleSimulator` for sensory processing
- `OperationalChart` state machine

---

### Component 4: Wilczek Time Crystal Comparison

#### [NEW] [nanobrain_wilczek.h](file:///e:/antg/nanob/cognanobrain/src/cpp/nanobrain_wilczek.h)

- `WilczekTimeCrystal` class implementing discrete time translation symmetry breaking
- Comparison methods with existing NanoBrain time crystal approach

#### [NEW] [nanobrain_wilczek.cpp](file:///e:/antg/nanob/cognanobrain/src/cpp/nanobrain_wilczek.cpp)

- Implementation of Wilczek time crystal dynamics
- Benchmark comparison functions

---

### Component 5: Fractal Information Metrics

#### [MODIFY] [nanobrain_fractal_tape.h](file:///e:/antg/nanob/cognanobrain/src/cpp/nanobrain_fractal_tape.h)

- `FractalMetrics` struct
- `FractalDimensionCalculator` class
- Self-similarity measurement functions

#### [MODIFY] [nanobrain_fractal_tape.cpp](file:///e:/antg/nanob/cognanobrain/src/cpp/nanobrain_fractal_tape.cpp)

- Box-counting fractal dimension algorithm
- Hausdorff dimension approximation
- Visualization export as CSV/JSON

---

### Build System Updates

#### [MODIFY] [CMakeLists.txt](file:///e:/antg/nanob/cognanobrain/src/cpp/CMakeLists.txt)

- Add `nanobrain_fractal_tape.cpp` to sources
- Add `nanobrain_wilczek.cpp` to sources
- Add new headers to install list

---

## Verification Plan

### Automated Tests

1. **Demo Program Updates**

   ```bash
   cd build
   cmake .. -DGGML_PATH=/path/to/llama.cpp
   cmake --build .
   ./time_crystal_demo    # Should show new GML operations
   ./unified_demo         # Should include fractal tape features
   ```

2. **Shape-to-Tensor Test** (in main.cpp)
   - Convert each of 19 shapes to tensor
   - Verify tensor dimensions match shape complexity
   - Test shape composition produces valid tensors

3. **FractalTape Unit Test**
   - Test nested cell creation
   - Verify 2D→3D sphere surgery transformation
   - Test self-assembly operators

4. **Wilczek Comparison Benchmark**
   - Run both time crystal variants
   - Compare coherence metrics
   - Output comparison report

### Manual Verification

1. **Build on Windows**

   ```powershell
   cd e:\antg\nanob\cognanobrain\src\cpp
   cmake -B build -DGGML_PATH=path\to\llama.cpp
   cmake --build build --config Release
   ```

2. **Run Tests**

   ```powershell
   .\build\Release\nanobrain_test.exe
   .\build\Release\time_crystal_demo.exe
   ```

3. **Verify Output**
   - Check console output for GML shape conversions
   - Verify fractal dimension calculations
   - Confirm no build errors or warnings

> [!NOTE]
> The existing GMLShape enum already has 19 shapes, exceeding the 15-shape requirement.
> Task 2.1 focuses on adding tensor conversion and composition operations.

---

## Estimated Effort

| Task | New Files | Modified Files | Lines Added |
|------|-----------|----------------|-------------|
| 2.1 GML Library | 0 | 2 | ~150 |
| 2.2 FractalTape | 2 | 1 | ~500 |
| 2.3 11D Sensors | 0 | 2 | ~200 |
| 2.4 Wilczek TC | 2 | 1 | ~300 |
| 2.5 Fractal Metrics | 0 | 2 | ~200 |
| **Total** | **4** | **8** | **~1350** |
