defmodule Nanobrain.AdvancedExamples do
  @moduledoc """
  Advanced usage examples for NanoBrain cognitive architecture.
  
  These examples demonstrate more complex patterns and use cases.
  """

  alias Nanobrain.{Kernel, AtomSpace, TimeCrystal, PPM, TruthValue}

  @doc """
  Example 1: Building a knowledge graph with inheritance relationships.
  """
  def knowledge_graph_example do
    IO.puts("\n=== Knowledge Graph Example ===\n")

    # Start AtomSpace directly
    {:ok, as} = AtomSpace.start_link()

    # Create concept hierarchy
    {:ok, animal_id} = AtomSpace.create_atom(as, "ConceptNode", "Animal", 0.95, 0.95)
    {:ok, mammal_id} = AtomSpace.create_atom(as, "ConceptNode", "Mammal", 0.9, 0.9)
    {:ok, cat_id} = AtomSpace.create_atom(as, "ConceptNode", "Cat", 0.85, 0.85)
    {:ok, dog_id} = AtomSpace.create_atom(as, "ConceptNode", "Dog", 0.85, 0.85)

    # Create inheritance links
    truth = TruthValue.new(0.99, 0.95)
    {:ok, _} = AtomSpace.create_link(as, "InheritanceLink", [mammal_id, animal_id], truth)
    {:ok, _} = AtomSpace.create_link(as, "InheritanceLink", [cat_id, mammal_id], truth)
    {:ok, _} = AtomSpace.create_link(as, "InheritanceLink", [dog_id, mammal_id], truth)

    # Query the graph
    concepts = AtomSpace.get_atoms_by_type(as, "ConceptNode")
    IO.puts("Created #{length(concepts)} concept nodes")

    metrics = AtomSpace.get_metrics(as)
    IO.puts("Graph has #{metrics.total_atoms} atoms and #{metrics.total_links} links")
    IO.puts("Consciousness emergence: #{Float.round(metrics.consciousness_emergence, 4)}")

    GenServer.stop(as)
  end

  @doc """
  Example 2: Concurrent processing with multiple AtomSpaces.
  """
  def concurrent_processing_example do
    IO.puts("\n=== Concurrent Processing Example ===\n")

    # Create multiple AtomSpaces concurrently
    tasks =
      for i <- 1..3 do
        Task.async(fn ->
          {:ok, as} = AtomSpace.start_link()

          # Each creates its own atoms
          for j <- 1..5 do
            AtomSpace.create_atom(as, "ConceptNode", "Concept_#{i}_#{j}", 0.8, 0.7)
          end

          # Run processing
          AtomSpace.run_cycles(as, 10)
          metrics = AtomSpace.get_metrics(as)

          GenServer.stop(as)
          {i, metrics}
        end)
      end

    # Wait for all to complete
    results = Task.await_many(tasks, 5000)

    for {space_id, metrics} <- results do
      IO.puts(
        "Space #{space_id}: #{metrics.total_atoms} atoms, consciousness: #{Float.round(metrics.consciousness_emergence, 4)}"
      )
    end
  end

  @doc """
  Example 3: Time crystal synchronization network.
  """
  def time_crystal_network_example do
    IO.puts("\n=== Time Crystal Network Example ===\n")

    # Create a network of interconnected time crystals
    crystals = for _ <- 1..5, do: TimeCrystal.new(11)

    IO.puts("Created #{length(crystals)} time crystals")
    IO.puts("Initial average coherence: #{avg_coherence(crystals)}")

    # Evolve independently
    evolved = Enum.map(crystals, &TimeCrystal.evolve(&1, 0.1))
    IO.puts("After evolution: #{avg_coherence(evolved)}")

    # Cross-entangle pairs
    [c1, c2, c3, c4, c5] = evolved
    {c1, c2} = TimeCrystal.entangle(c1, c2, 0.2)
    {c2, c3} = TimeCrystal.entangle(c2, c3, 0.2)
    {c3, c4} = TimeCrystal.entangle(c3, c4, 0.2)
    {c4, c5} = TimeCrystal.entangle(c4, c5, 0.2)

    network = [c1, c2, c3, c4, c5]
    IO.puts("After entanglement network: #{avg_coherence(network)}")

    # Calculate all pairwise overlaps
    IO.puts("\nPairwise overlaps:")

    for i <- 0..3, j <- (i + 1)..4 do
      overlap = TimeCrystal.overlap(Enum.at(network, i), Enum.at(network, j))
      IO.puts("  Crystal #{i} <-> #{j}: #{Float.round(overlap, 4)}")
    end
  end

  @doc """
  Example 4: Prime-based signal compression and analysis.
  """
  def signal_compression_example do
    IO.puts("\n=== Signal Compression Example ===\n")

    # Generate a complex signal
    signal =
      for i <- 0..127 do
        # Multiple frequency components
        0.5 * :math.sin(i * 0.1) +
          0.3 * :math.cos(i * 0.05) +
          0.2 * :math.sin(i * 0.2)
      end

    IO.puts("Original signal length: #{length(signal)}")

    # Analyze with PPM
    analysis = PPM.analyze_signal(signal)

    # Extract dominant features
    prime_histogram =
      analysis
      |> Enum.flat_map(fn {_, _, primes, _} -> primes end)
      |> Enum.frequencies()
      |> Enum.sort_by(fn {_, count} -> -count end)
      |> Enum.take(10)

    IO.puts("\nTop 10 prime components:")

    for {prime, count} <- prime_histogram do
      percentage = count / length(signal) * 100
      IO.puts("  Prime #{prime}: #{count} occurrences (#{Float.round(percentage, 1)}%)")
    end

    # Coherence distribution
    coherences = Enum.map(analysis, fn {_, _, _, coh} -> coh end)
    avg_coherence = Enum.sum(coherences) / length(coherences)
    max_coherence = Enum.max(coherences)
    min_coherence = Enum.min(coherences)

    IO.puts("\nCoherence statistics:")
    IO.puts("  Average: #{Float.round(avg_coherence, 4)}")
    IO.puts("  Maximum: #{Float.round(max_coherence, 4)}")
    IO.puts("  Minimum: #{Float.round(min_coherence, 4)}")
  end

  @doc """
  Example 5: Consciousness emergence simulation.
  """
  def consciousness_emergence_example do
    IO.puts("\n=== Consciousness Emergence Simulation ===\n")

    kernel = Kernel.new() |> Kernel.initialize()

    # Gradually build up complexity
    IO.puts("Building cognitive architecture...")

    stages = [
      {"Basic concepts", ["Self", "Other", "Object"]},
      {"Spatial relations", ["Above", "Below", "Inside", "Outside"]},
      {"Temporal concepts", ["Before", "After", "During"]},
      {"Abstract concepts", ["Truth", "Beauty", "Meaning"]},
      {"Meta concepts", ["Thinking", "Knowing", "Understanding"]}
    ]

    kernel =
      Enum.reduce(stages, kernel, fn {stage_name, concepts}, acc_kernel ->
        IO.puts("\n#{stage_name}:")

        stage_kernel =
          Enum.reduce(concepts, acc_kernel, fn concept, k ->
            {:ok, _, new_k} = Kernel.create_atom(k, "ConceptNode", concept, 0.9, 0.8)
            new_k
          end)

        # Process and measure
        processed = Kernel.run_cycles(stage_kernel, 5)
        metrics = Kernel.get_metrics(processed)

        IO.puts("  Atoms: #{metrics.total_atoms}")
        IO.puts("  Consciousness: #{Float.round(metrics.consciousness_emergence, 4)}")
        IO.puts("  Coherence: #{Float.round(metrics.time_crystal_coherence, 4)}")

        processed
      end)

    final_metrics = Kernel.get_metrics(kernel)

    IO.puts("\n=== Final System State ===")
    IO.puts("Total atoms: #{final_metrics.total_atoms}")
    IO.puts("Consciousness emergence: #{Float.round(final_metrics.consciousness_emergence, 4)}")
    IO.puts("Quantum coherence: #{Float.round(final_metrics.quantum_coherence, 4)}")
    IO.puts("Time crystal coherence: #{Float.round(final_metrics.time_crystal_coherence, 4)}")

    Kernel.shutdown(kernel)
  end

  @doc """
  Run all advanced examples.
  """
  def run_all do
    IO.puts("\n" <> String.duplicate("=", 60))
    IO.puts("NanoBrain Advanced Examples")
    IO.puts(String.duplicate("=", 60))

    knowledge_graph_example()
    concurrent_processing_example()
    time_crystal_network_example()
    signal_compression_example()
    consciousness_emergence_example()

    IO.puts("\n" <> String.duplicate("=", 60))
    IO.puts("All advanced examples complete!")
    IO.puts(String.duplicate("=", 60) <> "\n")
  end

  # Helper functions

  defp avg_coherence(crystals) do
    avg =
      crystals
      |> Enum.map(& &1.coherence)
      |> Enum.sum()
      |> (fn sum -> sum / length(crystals) end).()

    Float.round(avg, 4)
  end
end
