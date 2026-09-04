defmodule Nanobrain.Examples do
  @moduledoc """
  Example usage of NanoBrain pure Elixir implementation.
  """

  alias Nanobrain.{Kernel, TimeCrystal, PPM}

  def run_all do
    IO.puts("\n" <> String.duplicate("=", 60))
    IO.puts("NanoBrain Pure Elixir Examples")
    IO.puts(String.duplicate("=", 60) <> "\n")

    basic_kernel_demo()
    ppm_demo()
    time_crystal_demo()
    signal_analysis_demo()

    IO.puts("\n" <> String.duplicate("=", 60))
    IO.puts("Examples complete!")
    IO.puts(String.duplicate("=", 60) <> "\n")
  end

  def basic_kernel_demo do
    IO.puts("--- Basic Kernel Demo ---\n")

    # Show version and constants
    IO.puts("Version: #{Nanobrain.version()}")
    IO.puts("Fundamental Primes: #{inspect(Nanobrain.fundamental_primes())}")
    IO.puts("Time Crystal Dimensions: #{Nanobrain.time_crystal_dimensions()}")
    IO.puts("Golden Ratio: #{Nanobrain.golden_ratio()}\n")

    # Create and configure kernel
    kernel =
      Kernel.new(%{
        time_crystal_dimensions: 11,
        enable_meta_cognition: true,
        num_time_crystals: 3
      })

    kernel = Kernel.initialize(kernel)
    IO.puts("Kernel active: #{Kernel.active?(kernel)}")

    # Create atoms
    IO.puts("\nCreating atoms...")

    atoms = [
      {"ConceptNode", "Cat", 0.9, 0.8},
      {"ConceptNode", "Animal", 0.95, 0.9},
      {"ConceptNode", "Pet", 0.85, 0.7},
      {"PredicateNode", "is-a", 0.99, 0.95}
    ]

    kernel =
      Enum.reduce(atoms, kernel, fn {type, name, strength, confidence}, acc_kernel ->
        {:ok, atom_id, new_kernel} = Kernel.create_atom(acc_kernel, type, name, strength, confidence)
        IO.puts("  Created: #{type}:#{name} -> #{String.slice(atom_id, 0, 8)}...")
        new_kernel
      end)

    # Get metrics
    metrics = Kernel.get_metrics(kernel)
    IO.puts("\nMetrics:")
    IO.puts("  Total atoms: #{metrics.total_atoms}")
    IO.puts("  Total links: #{metrics.total_links}")
    IO.puts("  Quantum coherence: #{Float.round(metrics.quantum_coherence, 4)}")
    IO.puts("  Consciousness emergence: #{Float.round(metrics.consciousness_emergence, 4)}")
    IO.puts("  Time crystal coherence: #{Float.round(metrics.time_crystal_coherence, 4)}")

    # Run cycles
    IO.puts("\nRunning processing cycles...")
    kernel = Kernel.run_cycles(kernel, 5)

    new_metrics = Kernel.get_metrics(kernel)
    IO.puts("  Updated consciousness: #{Float.round(new_metrics.consciousness_emergence, 4)}")

    # Cleanup
    Kernel.shutdown(kernel)
    IO.puts("\nKernel shut down.\n")
  end

  def ppm_demo do
    IO.puts("--- Phase Prime Metrics Demo ---\n")

    # Test prime encoding
    values = [0.5, 0.75, 0.3, 0.9]
    IO.puts("Prime encoding:")

    for value <- values do
      primes = PPM.prime_encode(value)
      coherence = PPM.compute_coherence(primes)

      IO.puts(
        "  #{value} -> #{inspect(primes)} (coherence: #{Float.round(coherence, 4)})"
      )
    end

    # Golden spiral
    IO.puts("\nGolden spiral (first 5 points):")
    spiral = PPM.golden_spiral(5)

    for {x, y} <- spiral do
      IO.puts("  (#{Float.round(x, 4)}, #{Float.round(y, 4)})")
    end

    IO.puts("")
  end

  def time_crystal_demo do
    IO.puts("--- Time Crystal Demo ---\n")

    # Create time crystals
    tc1 = TimeCrystal.new(11)
    tc2 = TimeCrystal.new(11)

    IO.puts("Created two 11D time crystals")
    IO.puts("TC1 coherence: #{Float.round(tc1.coherence, 4)}")
    IO.puts("TC2 coherence: #{Float.round(tc2.coherence, 4)}")

    # Evolve
    IO.puts("\nEvolving time crystals...")
    tc1 = TimeCrystal.evolve(tc1)
    tc2 = TimeCrystal.evolve(tc2)

    IO.puts("After evolution:")
    IO.puts("TC1 coherence: #{Float.round(tc1.coherence, 4)}")
    IO.puts("TC2 coherence: #{Float.round(tc2.coherence, 4)}")

    # Measure overlap
    overlap = TimeCrystal.overlap(tc1, tc2)
    IO.puts("Overlap: #{Float.round(overlap, 4)}")

    # Entangle
    IO.puts("\nEntangling time crystals...")
    {tc1_entangled, tc2_entangled} = TimeCrystal.entangle(tc1, tc2, 0.1)

    new_overlap = TimeCrystal.overlap(tc1_entangled, tc2_entangled)
    IO.puts("After entanglement overlap: #{Float.round(new_overlap, 4)}")
    IO.puts("")
  end

  def signal_analysis_demo do
    IO.puts("--- Signal Analysis Demo ---\n")

    # Create a simple signal
    signal =
      for i <- 0..31 do
        :math.sin(i * 0.5) + 0.5 * :math.cos(i * 0.3)
      end

    IO.puts("Analyzing signal of length #{length(signal)}...")

    # Initialize kernel
    kernel = Kernel.new() |> Kernel.initialize()

    # Analyze signal
    analysis = Kernel.analyze_signal(kernel, signal)

    IO.puts("\nSignal Analysis Results:")
    IO.puts("  Overall coherence: #{Float.round(analysis.overall_coherence, 4)}")
    IO.puts("  Dominant primes: #{inspect(analysis.dominant_primes)}")
    IO.puts("  Signal length: #{analysis.signal_length}")

    # Cleanup
    Kernel.shutdown(kernel)
    IO.puts("")
  end
end
