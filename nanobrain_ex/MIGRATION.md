# Migration Guide: Python to Elixir

Guide for migrating from the Python/C++ implementation to the pure Elixir implementation of NanoBrain.

## Core Differences

### Python (C++ Bindings)
```python
import nanobrain as nb

config = nb.UnifiedConfig()
config.time_crystal_dimensions = 11

kernel = nb.UnifiedKernel(config)
kernel.initialize()

atom = kernel.create_atom("ConceptNode", "Cat", 0.9, 0.8)
metrics = kernel.get_metrics()
kernel.shutdown()
```

### Elixir (Pure)
```elixir
alias Nanobrain.Kernel

kernel = Kernel.new(%{time_crystal_dimensions: 11})
kernel = Kernel.initialize(kernel)

{:ok, atom_id, kernel} = Kernel.create_atom(kernel, "ConceptNode", "Cat", 0.9, 0.8)
metrics = Kernel.get_metrics(kernel)
Kernel.shutdown(kernel)
```

## Key Architectural Changes

### 1. Immutability

**Python**: Mutable objects
```python
kernel = nb.UnifiedKernel(config)
kernel.initialize()  # Modifies kernel in place
atom_id = kernel.create_atom(...)  # Returns ID, modifies kernel
```

**Elixir**: Immutable data structures
```elixir
kernel = Kernel.new(config)
kernel = Kernel.initialize(kernel)  # Returns new kernel
{:ok, atom_id, kernel} = Kernel.create_atom(kernel, ...)  # Returns new kernel
```

### 2. Error Handling

**Python**: Exceptions
```python
try:
    atom = kernel.create_atom("ConceptNode", "Cat", 0.9, 0.8)
except Exception as e:
    print(f"Error: {e}")
```

**Elixir**: Tagged tuples
```elixir
case Kernel.create_atom(kernel, "ConceptNode", "Cat", 0.9, 0.8) do
  {:ok, atom_id, kernel} -> # success
  {:error, reason} -> # handle error
end
```

### 3. Concurrency Model

**Python**: Threading/multiprocessing
```python
import threading

def process_atoms():
    # ... work with atoms
    
thread = threading.Thread(target=process_atoms)
thread.start()
```

**Elixir**: Actor model with OTP
```elixir
# AtomSpace is a GenServer
{:ok, as} = AtomSpace.start_link()
Task.async(fn ->
  # Safe concurrent access via message passing
  AtomSpace.create_atom(as, "ConceptNode", "Cat", 0.9, 0.8)
end)
```

## API Mapping

### Configuration

| Python | Elixir |
|--------|--------|
| `config = nb.UnifiedConfig()` | `config = %{}` (plain map) |
| `config.time_crystal_dimensions = 11` | `config = %{time_crystal_dimensions: 11}` |
| `config.enable_meta_cognition = True` | `config = %{enable_meta_cognition: true}` |

### Kernel Operations

| Python | Elixir |
|--------|--------|
| `kernel.initialize()` | `kernel = Kernel.initialize(kernel)` |
| `kernel.is_active()` | `Kernel.active?(kernel)` |
| `id = kernel.create_atom(...)` | `{:ok, id, kernel} = Kernel.create_atom(kernel, ...)` |
| `metrics = kernel.get_metrics()` | `metrics = Kernel.get_metrics(kernel)` |
| `kernel.run_cycles(5)` | `kernel = Kernel.run_cycles(kernel, 5)` |
| `kernel.shutdown()` | `Kernel.shutdown(kernel)` |

### Truth Values

| Python | Elixir |
|--------|--------|
| `tv = nb.TruthValue(0.9, 0.8)` | `tv = TruthValue.new(0.9, 0.8)` |
| `tv.strength` | `tv.strength` |
| `tv.merge(other)` | `TruthValue.merge(tv, other)` |

### Time Crystals

| Python | Elixir |
|--------|--------|
| `tc = nb.TimeCrystalKernel(config)` | `tc = TimeCrystal.new(11)` |
| `tc.initialize()` | N/A (initialized on creation) |
| `coherence = tc.compute_ppm_coherence([2,3,5])` | `coherence = PPM.compute_coherence([2,3,5])` |

### PPM Functions

| Python | Elixir |
|--------|--------|
| `nb.compute_ppm_coherence(primes)` | `PPM.compute_coherence(primes)` |
| `nb.prime_encode(0.5)` | `PPM.prime_encode(0.5)` |
| `nb.golden_spiral(10)` | `PPM.golden_spiral(10)` |

## Common Patterns

### Pattern 1: Initialize and Use

**Python**:
```python
import nanobrain as nb

config = nb.UnifiedConfig()
kernel = nb.UnifiedKernel(config)
kernel.initialize()

cat = kernel.create_atom("ConceptNode", "Cat", 0.9, 0.8)
metrics = kernel.get_metrics()

kernel.shutdown()
```

**Elixir**:
```elixir
alias Nanobrain.Kernel

kernel = 
  Kernel.new()
  |> Kernel.initialize()

{:ok, cat_id, kernel} = Kernel.create_atom(kernel, "ConceptNode", "Cat", 0.9, 0.8)
metrics = Kernel.get_metrics(kernel)

Kernel.shutdown(kernel)
```

### Pattern 2: Multiple Operations

**Python**:
```python
atoms = []
for name in ["Cat", "Dog", "Bird"]:
    atom_id = kernel.create_atom("ConceptNode", name, 0.9, 0.8)
    atoms.append(atom_id)
```

**Elixir**:
```elixir
{atom_ids, kernel} =
  Enum.map_reduce(["Cat", "Dog", "Bird"], kernel, fn name, acc_kernel ->
    {:ok, atom_id, new_kernel} = Kernel.create_atom(acc_kernel, "ConceptNode", name, 0.9, 0.8)
    {atom_id, new_kernel}
  end)
```

### Pattern 3: Signal Analysis

**Python**:
```python
import math

signal = [math.sin(i * 0.5) for i in range(64)]
transform = nb.TCTransformEngine(kernel, nb.TCTransformConfig())
result = transform.transform(signal)
```

**Elixir**:
```elixir
signal = for i <- 0..63, do: :math.sin(i * 0.5)
analysis = Kernel.analyze_signal(kernel, signal)
```

## Performance Considerations

### Python/C++ Strengths
- Native C++ performance for heavy computations
- Direct GGML integration for tensor operations
- Optimized memory layout

### Elixir Strengths
- Superior concurrency via BEAM VM
- No GIL (Global Interpreter Lock)
- Fault tolerance via supervision trees
- Hot code reloading
- Better for distributed systems

## When to Use Which

### Use Python/C++ When:
- You need maximum single-threaded performance
- You're integrating with existing C++ codebases
- You need direct GGML/tensor operations
- You're doing heavy numerical computations

### Use Elixir When:
- You need high concurrency
- You're building distributed systems
- You want fault tolerance
- You prefer functional programming
- You don't want C++ dependencies
- You're prototyping or exploring

## Migration Checklist

- [ ] Replace `import nanobrain` with `alias Nanobrain.{Kernel, ...}`
- [ ] Change object instantiation to function calls
- [ ] Thread state through function calls (immutability)
- [ ] Replace `try/except` with pattern matching
- [ ] Use tagged tuples `{:ok, result}` instead of return values
- [ ] Replace Python lists with Elixir lists/tuples
- [ ] Replace Python dicts with Elixir maps
- [ ] Update method calls: `obj.method()` → `Module.function(obj)`
- [ ] Handle GenServer communication for AtomSpace
- [ ] Use pipe operator `|>` for function chaining

## Examples Repository

See `lib/nanobrain/examples.ex` for comprehensive examples of all features.

Run with:
```bash
mix run -e "Nanobrain.Examples.run_all()"
```

## Need Help?

- Check the README.md for full API documentation
- Run `mix test` to see usage examples in tests
- Explore the examples in `lib/nanobrain/examples.ex`
