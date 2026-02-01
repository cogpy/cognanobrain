defmodule Nanobrain.AtomSpace do
  @moduledoc """
  AtomSpace - Hypergraph knowledge representation system.
  
  Manages a collection of atoms and links forming a hypergraph,
  with attention allocation and pattern matching capabilities.
  """

  use GenServer

  alias Nanobrain.{Atom, Link, TruthValue, AttentionValue}

  defstruct atoms: %{},
            links: %{},
            indexes: %{type: %{}, name: %{}},
            metrics: %{
              total_atoms: 0,
              total_links: 0,
              quantum_coherence: 0.5,
              consciousness_emergence: 0.0
            }

  ## Client API

  @doc """
  Start the AtomSpace GenServer.
  """
  def start_link(opts \\ []) do
    GenServer.start_link(__MODULE__, :ok, opts)
  end

  @doc """
  Create a new atom in the AtomSpace.
  """
  def create_atom(server \\ __MODULE__, type, name, strength \\ 0.9, confidence \\ 0.8) do
    GenServer.call(server, {:create_atom, type, name, strength, confidence})
  end

  @doc """
  Create a new link in the AtomSpace.
  """
  def create_link(server \\ __MODULE__, type, outgoing_ids, truth \\ %TruthValue{}) do
    GenServer.call(server, {:create_link, type, outgoing_ids, truth})
  end

  @doc """
  Get an atom by ID.
  """
  def get_atom(server \\ __MODULE__, atom_id) do
    GenServer.call(server, {:get_atom, atom_id})
  end

  @doc """
  Get all atoms of a specific type.
  """
  def get_atoms_by_type(server \\ __MODULE__, type) do
    GenServer.call(server, {:get_atoms_by_type, type})
  end

  @doc """
  Get current metrics.
  """
  def get_metrics(server \\ __MODULE__) do
    GenServer.call(server, :get_metrics)
  end

  @doc """
  Run attention allocation cycles.
  """
  def run_cycles(server \\ __MODULE__, num_cycles \\ 1) do
    GenServer.call(server, {:run_cycles, num_cycles})
  end

  ## Server Callbacks

  @impl true
  def init(:ok) do
    {:ok, %__MODULE__{}}
  end

  @impl true
  def handle_call({:create_atom, type, name, strength, confidence}, _from, state) do
    truth = TruthValue.new(strength, confidence)
    attention = AttentionValue.new(100.0, 0.0, 0.0)
    atom = Atom.new(type, name, truth, attention)

    new_state =
      state
      |> put_in([Access.key(:atoms), atom.id], atom)
      |> update_index(:type, type, atom.id)
      |> update_index(:name, name, atom.id)
      |> update_in([Access.key(:metrics), :total_atoms], &(&1 + 1))
      |> recalculate_metrics()

    {:reply, {:ok, atom.id}, new_state}
  end

  @impl true
  def handle_call({:create_link, type, outgoing_ids, truth}, _from, state) do
    attention = AttentionValue.new(100.0, 0.0, 0.0)
    link = Link.new(type, outgoing_ids, truth, attention)

    new_state =
      state
      |> put_in([Access.key(:links), link.id], link)
      |> update_in([Access.key(:metrics), :total_links], &(&1 + 1))
      |> recalculate_metrics()

    {:reply, {:ok, link.id}, new_state}
  end

  @impl true
  def handle_call({:get_atom, atom_id}, _from, state) do
    atom = Map.get(state.atoms, atom_id)
    {:reply, atom, state}
  end

  @impl true
  def handle_call({:get_atoms_by_type, type}, _from, state) do
    atom_ids = get_in(state.indexes, [:type, type]) || []
    atoms = Enum.map(atom_ids, &Map.get(state.atoms, &1))
    {:reply, atoms, state}
  end

  @impl true
  def handle_call(:get_metrics, _from, state) do
    {:reply, state.metrics, state}
  end

  @impl true
  def handle_call({:run_cycles, num_cycles}, _from, state) do
    new_state =
      Enum.reduce(1..num_cycles, state, fn _, acc ->
        acc
        |> spread_activation()
        |> recalculate_metrics()
      end)

    {:reply, :ok, new_state}
  end

  ## Private Functions

  defp update_index(state, index_type, key, atom_id) do
    update_in(
      state,
      [Access.key(:indexes), index_type, key],
      fn existing -> [atom_id | existing || []] end
    )
  end

  defp spread_activation(state) do
    # Update attention values using spreading activation
    atoms =
      state.atoms
      |> Enum.map(fn {id, atom} ->
        {id, %{atom | attention: AttentionValue.spread(atom.attention)}}
      end)
      |> Map.new()

    %{state | atoms: atoms}
  end

  defp recalculate_metrics(state) do
    # Calculate quantum coherence from attention values
    avg_sti =
      if map_size(state.atoms) > 0 do
        state.atoms
        |> Map.values()
        |> Enum.map(& &1.attention.sti)
        |> Enum.sum()
        |> Kernel./(map_size(state.atoms))
      else
        0.0
      end

    # Consciousness emergence as function of atoms, links, and coherence
    consciousness =
      if state.metrics.total_atoms > 0 do
        :math.tanh(
          (state.metrics.total_atoms + state.metrics.total_links) / 100.0 *
            (1.0 + avg_sti / 100.0)
        )
      else
        0.0
      end

    metrics = %{
      state.metrics
      | quantum_coherence: min(1.0, avg_sti / 100.0),
        consciousness_emergence: consciousness
    }

    %{state | metrics: metrics}
  end
end
