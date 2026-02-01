"""
NanoBrain Python Usage Examples

Run after installing the package:
    pip install -e .
    python examples/quickstart.py
"""

# Note: This example requires the C++ bindings to be built
# See README.md for build instructions

try:
    import nanobrain as nb
except ImportError:
    print("Error: NanoBrain not installed. Run 'pip install -e .' from python/ directory")
    exit(1)


def main():
    print("=" * 60)
    print("NanoBrain Python Example")
    print("=" * 60)
    
    # Show version and constants
    print(f"\nVersion: {nb.__version__}")
    print(f"Fundamental Primes: {nb.FUNDAMENTAL_PRIMES}")
    print(f"Time Crystal Dimensions: {nb.TIME_CRYSTAL_DIMENSIONS}")
    print(f"Golden Ratio: {nb.GOLDEN_RATIO:.6f}")
    
    # Create and configure unified kernel
    print("\n--- Unified Kernel Demo ---")
    config = nb.UnifiedConfig()
    config.time_crystal_dimensions = 11
    config.enable_meta_cognition = True
    config.debug_output = False
    
    kernel = nb.UnifiedKernel(config)
    kernel.initialize()
    print(f"Kernel active: {kernel.is_active()}")
    
    # Create some atoms
    print("\nCreating atoms...")
    atoms = [
        ("ConceptNode", "Cat", 0.9, 0.8),
        ("ConceptNode", "Animal", 0.95, 0.9),
        ("ConceptNode", "Pet", 0.85, 0.7),
        ("PredicateNode", "is-a", 0.99, 0.95),
    ]
    
    atom_ids = []
    for type_, name, strength, confidence in atoms:
        atom_id = kernel.create_atom(type_, name, strength, confidence)
        atom_ids.append(atom_id)
        print(f"  Created: {type_}:{name} -> {atom_id[:8]}...")
    
    # Get metrics
    metrics = kernel.get_metrics()
    print(f"\nMetrics:")
    print(f"  Total atoms: {metrics.total_atoms}")
    print(f"  Total links: {metrics.total_links}")
    print(f"  Quantum coherence: {metrics.quantum_coherence:.4f}")
    print(f"  Consciousness emergence: {metrics.consciousness_emergence:.4f}")
    
    # Process some cycles
    print("\nRunning processing cycles...")
    kernel.run_cycles(5)
    
    new_metrics = kernel.get_metrics()
    print(f"  Updated consciousness: {new_metrics.consciousness_emergence:.4f}")
    
    # TC Transform example
    print("\n--- TC Transform Demo ---")
    tc_config = nb.TCTransformConfig()
    tc_config.num_prime_components = 10
    
    transform = nb.TCTransformEngine(kernel, tc_config)
    
    # Analyze a simple signal
    import math
    signal = [math.sin(i * 0.5) + 0.5 * math.cos(i * 0.3) for i in range(64)]
    
    result = transform.transform(signal)
    print(f"Signal analysis:")
    print(f"  Overall coherence: {result.overall_coherence:.4f}")
    print(f"  Dominant primes: {result.dominant_primes}")
    print(f"  Spectrum (first 5): {result.prime_spectrum[:5]}")
    
    # Garden of Gardens example
    print("\n--- Garden of Gardens Demo ---")
    gog_config = nb.GOGConfig()
    gog_config.max_depth = 3
    
    gog = nb.GardenOfGardens(kernel, gog_config)
    gog.initialize()
    
    garden_id = gog.create_garden("consciousness_garden")
    print(f"Created garden: {garden_id[:20]}...")
    
    gog_metrics = gog.get_metrics()
    print(f"GOG Metrics:")
    print(f"  Total gardens: {gog_metrics.total_gardens}")
    print(f"  Total petals: {gog_metrics.total_petals}")
    print(f"  Average coherence: {gog_metrics.average_coherence:.4f}")
    
    # Cleanup
    gog.shutdown()
    kernel.shutdown()
    
    print("\n" + "=" * 60)
    print("Demo complete!")
    print("=" * 60)


if __name__ == "__main__":
    main()
