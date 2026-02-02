defmodule Nanobrain do
  @moduledoc """
  Pure Elixir implementation of NanoBrain cognitive architecture.
  
  NanoBrain is a revolutionary cognitive architecture that combines:
  - Phase Prime Metrics (PPM) for consciousness modeling
  - 11-Dimensional Time Crystals for quantum manifold processing
  - OpenCog-inspired AtomSpace for knowledge representation
  - Attention allocation and spreading activation
  
  ## Quick Start
  
      # Initialize kernel
      kernel = Nanobrain.Kernel.new(%{
        time_crystal_dimensions: 11,
        enable_meta_cognition: true
      })
      
      kernel = Nanobrain.Kernel.initialize(kernel)
      
      # Create atoms
      {:ok, cat_id, kernel} = Nanobrain.Kernel.create_atom(kernel, "ConceptNode", "Cat", 0.9, 0.8)
      {:ok, animal_id, kernel} = Nanobrain.Kernel.create_atom(kernel, "ConceptNode", "Animal", 0.95, 0.9)
      
      # Get metrics
      metrics = Nanobrain.Kernel.get_metrics(kernel)
      IO.inspect(metrics)
      
      # Run processing cycles
      kernel = Nanobrain.Kernel.run_cycles(kernel, 5)
      
      # Cleanup
      Nanobrain.Kernel.shutdown(kernel)
  
  ## Modules
  
  - `Nanobrain.Kernel` - Main unified kernel
  - `Nanobrain.AtomSpace` - Hypergraph knowledge representation
  - `Nanobrain.TimeCrystal` - 11D quantum manifold processing
  - `Nanobrain.PPM` - Phase Prime Metrics
  - `Nanobrain.Atom` - Knowledge atoms
  - `Nanobrain.Link` - Knowledge links
  - `Nanobrain.TruthValue` - Uncertain truth representation
  - `Nanobrain.AttentionValue` - Cognitive attention allocation
  """

  alias Nanobrain.{PPM, Constants}

  @doc """
  Get NanoBrain version.
  """
  def version, do: "1.0.0"

  @doc """
  Get fundamental primes used in PPM.
  """
  def fundamental_primes, do: Constants.fundamental_primes()

  @doc """
  Get golden ratio constant.
  """
  def golden_ratio, do: Constants.golden_ratio()

  @doc """
  Get time crystal dimensions.
  """
  def time_crystal_dimensions, do: Constants.time_crystal_dimensions()

  @doc """
  Compute PPM coherence from primes.
  """
  defdelegate compute_coherence(primes), to: PPM, as: :compute_coherence

  @doc """
  Encode value as prime signature.
  """
  defdelegate prime_encode(value, num_primes \\ 5), to: PPM, as: :prime_encode
end
