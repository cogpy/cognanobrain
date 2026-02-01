defmodule Nanobrain.Kernel do
  @moduledoc """
  Unified Kernel - Main entry point for NanoBrain cognitive architecture.
  
  Integrates AtomSpace, Time Crystals, and Phase Prime Metrics
  into a unified cognitive processing system.
  """

  alias Nanobrain.{AtomSpace, TimeCrystal, PPM}

  defstruct atom_space: nil,
            time_crystals: [],
            config: %{},
            active: false

  @type t :: %__MODULE__{
          atom_space: pid() | nil,
          time_crystals: [TimeCrystal.t()],
          config: map(),
          active: boolean()
        }

  @doc """
  Initialize a new unified kernel with configuration.
  """
  def new(config \\ %{}) do
    default_config = %{
      time_crystal_dimensions: 11,
      num_time_crystals: 3,
      enable_meta_cognition: true,
      debug_output: false
    }

    merged_config = Map.merge(default_config, config)

    %__MODULE__{
      config: merged_config
    }
  end

  @doc """
  Initialize the kernel and start all subsystems.
  """
  def initialize(%__MODULE__{} = kernel) do
    # Start AtomSpace
    {:ok, atom_space_pid} = AtomSpace.start_link(name: nil)

    # Initialize Time Crystals
    time_crystals =
      for _ <- 1..kernel.config.num_time_crystals do
        TimeCrystal.new(kernel.config.time_crystal_dimensions)
      end

    %{kernel | atom_space: atom_space_pid, time_crystals: time_crystals, active: true}
  end

  @doc """
  Check if kernel is active.
  """
  def active?(%__MODULE__{active: active}), do: active

  @doc """
  Create an atom in the AtomSpace.
  """
  def create_atom(%__MODULE__{atom_space: as} = kernel, type, name, strength \\ 0.9, confidence \\ 0.8) do
    case AtomSpace.create_atom(as, type, name, strength, confidence) do
      {:ok, atom_id} -> {:ok, atom_id, kernel}
      error -> error
    end
  end

  @doc """
  Get metrics from the kernel.
  """
  def get_metrics(%__MODULE__{atom_space: as, time_crystals: tcs} = _kernel) do
    atom_space_metrics = AtomSpace.get_metrics(as)
    
    avg_tc_coherence =
      if length(tcs) > 0 do
        tcs
        |> Enum.map(& &1.coherence)
        |> Enum.sum()
        |> Kernel./(length(tcs))
      else
        0.5
      end

    Map.merge(atom_space_metrics, %{
      time_crystal_coherence: avg_tc_coherence,
      num_time_crystals: length(tcs)
    })
  end

  @doc """
  Run processing cycles.
  """
  def run_cycles(%__MODULE__{atom_space: as, time_crystals: tcs} = kernel, num_cycles \\ 1) do
    # Run attention allocation cycles
    AtomSpace.run_cycles(as, num_cycles)

    # Evolve time crystals
    new_tcs =
      Enum.map(tcs, fn tc ->
        Enum.reduce(1..num_cycles, tc, fn _, acc ->
          TimeCrystal.evolve(acc)
        end)
      end)

    %{kernel | time_crystals: new_tcs}
  end

  @doc """
  Analyze a signal using PPM and Time Crystal transform.
  """
  def analyze_signal(%__MODULE__{} = _kernel, signal) when is_list(signal) do
    # PPM analysis
    ppm_analysis = PPM.analyze_signal(signal)

    # Extract dominant primes
    dominant_primes =
      ppm_analysis
      |> Enum.flat_map(fn {_, _, primes, _} -> primes end)
      |> Enum.frequencies()
      |> Enum.sort_by(fn {_, count} -> -count end)
      |> Enum.take(5)
      |> Enum.map(fn {prime, _} -> prime end)

    overall_coherence = PPM.compute_coherence(dominant_primes)

    %{
      ppm_analysis: ppm_analysis,
      dominant_primes: dominant_primes,
      overall_coherence: overall_coherence,
      signal_length: length(signal)
    }
  end

  @doc """
  Shutdown the kernel and cleanup resources.
  """
  def shutdown(%__MODULE__{atom_space: as} = kernel) do
    if as && Process.alive?(as) do
      GenServer.stop(as)
    end

    %{kernel | active: false, atom_space: nil}
  end
end
