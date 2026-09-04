# NanoBrain - Pure Elixir Implementation

Pure Elixir implementation of the NanoBrain cognitive architecture - a revolutionary platform for consciousness modeling and artificial intelligence.

## 🧠 Overview

NanoBrain is a cognitive architecture that combines cutting-edge theoretical frameworks:

- **Phase Prime Metrics (PPM)**: Mathematical framework using prime number patterns for consciousness modeling
- **11-Dimensional Time Crystals**: Quantum manifold processing for temporal quantum structures
- **OpenCog-Inspired AtomSpace**: Hypergraph knowledge representation with uncertain truth values
- **Attention Allocation (ECAN)**: Economic attention network with spreading activation
- **Meta-Cognitive Processing**: Multi-layered consciousness emergence modeling

This pure Elixir implementation provides all core functionality without requiring C++ dependencies.

## 🚀 Quick Start

### Installation

Add to your `mix.exs`:

```elixir
def deps do
  [
    {:nanobrain, path: "../nanobrain_ex"}
  ]
end
```

Or install locally:

```bash
cd nanobrain_ex
mix deps.get
mix compile
```

### Basic Usage

```elixir
# Initialize the kernel
kernel = Nanobrain.Kernel.new(%{
  time_crystal_dimensions: 11,
  enable_meta_cognition: true,
  num_time_crystals: 3
})

kernel = Nanobrain.Kernel.initialize(kernel)

# Create knowledge atoms
{:ok, cat_id, kernel} = Nanobrain.Kernel.create_atom(
  kernel, 
  "ConceptNode", 
  "Cat", 
  0.9,  # strength
  0.8   # confidence
)

{:ok, animal_id, kernel} = Nanobrain.Kernel.create_atom(
  kernel,
  "ConceptNode",
  "Animal",
  0.95,
  0.9
)

# Get consciousness metrics
metrics = Nanobrain.Kernel.get_metrics(kernel)
IO.inspect(metrics)
# %{
#   total_atoms: 2,
#   total_links: 0,
#   quantum_coherence: 1.0,
#   consciousness_emergence: 0.0398,
#   time_crystal_coherence: 0.5019
# }

# Run cognitive processing cycles
kernel = Nanobrain.Kernel.run_cycles(kernel, 10)

# Analyze signals with PPM
signal = for i <- 0..31, do: :math.sin(i * 0.5)
analysis = Nanobrain.Kernel.analyze_signal(kernel, signal)

# Cleanup
Nanobrain.Kernel.shutdown(kernel)
```

## 📚 Core Modules

### Nanobrain.Kernel

Main unified kernel integrating all subsystems.

```elixir
kernel = Nanobrain.Kernel.new()
kernel = Nanobrain.Kernel.initialize(kernel)
{:ok, atom_id, kernel} = Nanobrain.Kernel.create_atom(kernel, type, name, strength, confidence)
metrics = Nanobrain.Kernel.get_metrics(kernel)
kernel = Nanobrain.Kernel.run_cycles(kernel, num_cycles)
Nanobrain.Kernel.shutdown(kernel)
```

### Nanobrain.AtomSpace

Hypergraph knowledge representation system (GenServer).

```elixir
{:ok, as} = Nanobrain.AtomSpace.start_link()
{:ok, atom_id} = Nanobrain.AtomSpace.create_atom(as, "ConceptNode", "Cat", 0.9, 0.8)
atom = Nanobrain.AtomSpace.get_atom(as, atom_id)
atoms = Nanobrain.AtomSpace.get_atoms_by_type(as, "ConceptNode")
metrics = Nanobrain.AtomSpace.get_metrics(as)
```

### Nanobrain.TimeCrystal

11-Dimensional quantum manifold processing.

```elixir
tc = Nanobrain.TimeCrystal.new(11)
tc = Nanobrain.TimeCrystal.evolve(tc)
measurement = Nanobrain.TimeCrystal.measure(tc, prime)
{tc1, tc2} = Nanobrain.TimeCrystal.entangle(tc1, tc2, coupling)
overlap = Nanobrain.TimeCrystal.overlap(tc1, tc2)
```

### Nanobrain.PPM

Phase Prime Metrics for consciousness modeling.

```elixir
primes = Nanobrain.PPM.prime_encode(0.75)
coherence = Nanobrain.PPM.compute_coherence(primes)
spiral = Nanobrain.PPM.golden_spiral(100)
analysis = Nanobrain.PPM.analyze_signal(signal)
```

### Nanobrain.TruthValue & AttentionValue

Uncertain knowledge representation.

```elixir
truth = Nanobrain.TruthValue.new(0.9, 0.8)
merged = Nanobrain.TruthValue.merge(tv1, tv2)

attention = Nanobrain.AttentionValue.new(100.0, 50.0, 10.0)
spread = Nanobrain.AttentionValue.spread(attention)
```

### Nanobrain.Atom & Link

Knowledge graph elements.

```elixir
atom = Nanobrain.Atom.new("ConceptNode", "Cat", 0.9, 0.8)
link = Nanobrain.Link.new("InheritanceLink", [cat_id, animal_id])
```

## 🧪 Running Examples

```bash
cd nanobrain_ex
mix run -e "Nanobrain.Examples.run_all()"
```

This will run demonstrations of:
- Basic kernel operations
- Phase Prime Metrics
- Time Crystal evolution and entanglement
- Signal analysis

## 🧪 Testing

Run the comprehensive test suite:

```bash
cd nanobrain_ex
mix test
```

All 23 tests cover:
- TruthValue operations
- AttentionValue spreading
- Atom and Link creation
- PPM coherence and encoding
- Time Crystal evolution
- AtomSpace knowledge management
- Kernel integration

## 🔬 Architecture

### Cognitive Processing Pipeline

1. **Perception**: Create atoms representing knowledge
2. **Attention Allocation**: ECAN spreads activation through the graph
3. **Pattern Matching**: Find related concepts via hypergraph traversal
4. **Reasoning**: Apply inference rules using truth value revision
5. **Time Crystal Processing**: Quantum-inspired temporal evolution
6. **Consciousness Emergence**: Multi-scale integration metrics

### Key Features

- **Pure Elixir**: No C++ or NIFs required
- **OTP Compliant**: Uses GenServer for AtomSpace
- **Concurrent**: Leverages BEAM VM for parallel processing
- **Functional**: Immutable data structures throughout
- **Type Specs**: Full type specifications for dialyzer
- **Tested**: Comprehensive test coverage

## 📖 Constants

```elixir
Nanobrain.fundamental_primes()      # [2, 3, 5, 7, 11, ...]
Nanobrain.golden_ratio()            # 1.618033988749895
Nanobrain.time_crystal_dimensions() # 11
```

## 🎯 Use Cases

- **Cognitive Architecture Research**: Prototype consciousness models
- **AI Development**: Build knowledge-based reasoning systems
- **Pattern Recognition**: Analyze signals with prime-based transforms
- **Consciousness Studies**: Model emergence and integration
- **Educational Platform**: Learn about cognitive architectures

## 🔗 Comparison with Python Implementation

| Feature | Python (C++ bindings) | Elixir (Pure) |
|---------|----------------------|---------------|
| AtomSpace | ✅ Via pybind11 | ✅ GenServer |
| Time Crystals | ✅ C++ GGML | ✅ Pure Elixir |
| PPM | ✅ C++ | ✅ Pure Elixir |
| Concurrency | Threading | OTP/BEAM |
| Dependencies | C++17, GGML, pybind11 | None |
| Type Safety | Python hints | Elixir specs |

## 📄 License

MIT License - see LICENSE file for details.

## 🙏 Acknowledgments

- OpenCog Foundation for cognitive architecture inspiration
- NanoBrain theoretical framework contributors
- Elixir and OTP communities

---

**"Pure functional cognitive architecture for consciousness emergence"**

© 2026 NanoBrain Cognitive Architecture

