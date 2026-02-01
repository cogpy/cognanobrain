# Phase 3: Phase Prime Metric (PPM) Implementation

Implementing the 10 PPM metric classes and related functionality as defined in Chapter 3 of the DEVELOPMENT_FRAMEWORK.md.

## User Review Required

> [!IMPORTANT]
> **Large Implementation Scope**: This phase adds ~1,500 lines of C++ code across 2 new files and 2 modified files. Please confirm the approach before implementation.

## Proposed Changes

### Core PPM Module

#### [NEW] [nanobrain_ppm.h](file:///e:/antg/nanob/cognanobrain/src/cpp/nanobrain_ppm.h)

PPM metric class definitions (~300 lines):

```cpp
// Base PPM metric interface
class PPMMetric {
public:
    virtual ~PPMMetric() = default;
    virtual float compute(const std::vector<int>& primes) = 0;
    virtual std::string name() const = 0;
};

// 10 PPM Metric Classes
class PPMMetric1_GeometricShape;   // Integer → Shape mapping
class PPMMetric2_OrderedFactor;    // Product decomposition
class PPMMetric3_PhasePath;        // CW/CCW 360° phase
class PPMMetric4_DomainLimit;      // Prime domain bounds
class PPMMetric5_HighOrderedFactor;// OF >> integer  
class PPMMetric6_HoleFinder;       // Empty space detection
class PPMMetric7_PrimeStatistics;  // Silent/active primes
class PPMMetric8_PeriodicRipples;  // Normalized periodicity
class PPMMetric9_PrimeLattice;     // Twin/cousin/co-primes
class PPMMetric10_ImaginaryLayers; // Multi-layer operations

// Operator chain for composing metrics
class PPMOperatorChain;

// Precomputed coherence lookup
class PPMCoherenceTable;
```

Key structures:

- `PPMResult` - Output from metric computation
- `OrderedFactor` - Prime factorization representation
- `PhasePath` - 360° phase path structure
- `PrimeLatticeNode` - Graph node for prime relationships

---

#### [NEW] [nanobrain_ppm.cpp](file:///e:/antg/nanob/cognanobrain/src/cpp/nanobrain_ppm.cpp)

PPM metric implementations (~1,000 lines):

| Class | Description | Key Operations |
|-------|-------------|----------------|
| `PPMMetric1` | Integer→Shape | Map integers to GML shapes via prime factors |
| `PPMMetric2` | Ordered Factor | Compute prime decomposition with ordering |
| `PPMMetric3` | Phase Path | Calculate 360° phase using golden ratio |
| `PPMMetric4` | Domain Limit | Determine prime domain boundaries |
| `PPMMetric5` | High OF | Compute OF where OF >> original integer |
| `PPMMetric6` | Hole Finder | Detect gaps in prime coverage |
| `PPMMetric7` | Statistics | Track silent/active prime ratios |
| `PPMMetric8` | Ripples | Compute normalized periodic patterns |
| `PPMMetric9` | Lattice | Build twin/cousin/co-prime graph |
| `PPMMetric10` | Imaginary | Multi-layer complex operations |

Additional implementations:

- `PPMOperatorChain::compose()` - Chain metrics together
- `PPMCoherenceTable::lookup()` - O(1) coherence for 15 primes
- `evolve_with_ppm()` - PPM-driven time crystal evolution

---

### Integration with Time Crystal

#### [MODIFY] [nanobrain_time_crystal.h](file:///e:/antg/nanob/cognanobrain/src/cpp/nanobrain_time_crystal.h)

Add new methods to `TimeCrystalKernel`:

```cpp
// PPM evolution methods
void evolve_with_ppm(const std::string& atom_id, const PPMOperatorChain& chain);
PPMResult compute_ppm_metrics(const std::vector<int>& primes);

// Phase plot export
bool export_phase_plot(const std::string& filename, 
                       const std::vector<int>& primes);
```

---

#### [MODIFY] [CMakeLists.txt](file:///e:/antg/nanob/cognanobrain/src/cpp/CMakeLists.txt)

Add new source files:

```cmake
add_library(nanobrain_kernel STATIC
    # ... existing files ...
    nanobrain_ppm.cpp
)

set(NANOBRAIN_HEADERS
    # ... existing headers ...
    nanobrain_ppm.h
)
```

---

### Demo Program

#### [NEW] [ppm_demo.cpp](file:///e:/antg/nanob/cognanobrain/src/cpp/ppm_demo.cpp)

Demonstration of PPM functionality (~150 lines):

- Test all 10 metric classes
- Show operator chain composition
- Demonstrate coherence table lookup
- Export sample phase plot

---

## Verification Plan

### Automated Tests

**Command to run existing tests:**

```bash
cd e:\antg\nanob\cognanobrain\src\cpp\build
./nanobrain_test
./time_crystal_demo
```

**New test targets to add:**

```bash
./ppm_demo    # New PPM-specific demo
```

### Test Cases for PPM Metrics

| Metric | Test Input | Expected Behavior |
|--------|------------|-------------------|
| Metric1 | `{2,3,5}` | Returns Triangle shape |
| Metric2 | `60` | Returns `{2,2,3,5}` factors |
| Metric3 | `{2,3,5,7}` | 360° phase < 2π |
| Metric4 | `47` | Domain bound ≈ 15th prime |
| Metric5 | Large composite | OF >> input |
| Metric6 | Sparse primes | Detects gaps |
| Metric7 | Full 15 primes | ~10% silent ratio |
| Metric8 | `{2,4,8,16}` | Periodic = true |
| Metric9 | `{11,13}` | Twin prime detected |
| Metric10 | Complex input | Multi-layer output |

### Coherence Table Verification

Verify precomputed coherence for known prime combinations:

```cpp
// Expected coherence values
{2, 3} → 0.981
{2, 3, 5} → 0.994
{2, 3, 5, 7} → 0.997
{2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47} → 0.9999
```

### Manual Verification

1. **Build verification**: Build project and confirm no new warnings
2. **Run ppm_demo**: Execute `./ppm_demo` and verify output matches expected
3. **Phase plot export**: Verify `phase_plot.csv` is created and contains valid data

## Implementation Order

1. Create `nanobrain_ppm.h` with class definitions
2. Create `nanobrain_ppm.cpp` with implementations
3. Update `CMakeLists.txt` to include new files
4. Create `ppm_demo.cpp` for testing
5. Modify `nanobrain_time_crystal.h/.cpp` to add `evolve_with_ppm()`
6. Run verification suite
