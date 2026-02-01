defmodule NanobrainTest do
  use ExUnit.Case
  doctest Nanobrain

  alias Nanobrain.{
    Kernel,
    AtomSpace,
    TimeCrystal,
    PPM,
    TruthValue,
    AttentionValue,
    Atom,
    Link
  }

  describe "TruthValue" do
    test "creates new truth value" do
      tv = TruthValue.new(0.9, 0.8, 10.0)
      assert tv.strength == 0.9
      assert tv.confidence == 0.8
      assert tv.count == 10.0
    end

    test "merges truth values" do
      tv1 = TruthValue.new(0.9, 0.8)
      tv2 = TruthValue.new(0.7, 0.6)
      merged = TruthValue.merge(tv1, tv2)

      assert is_float(merged.strength)
      assert merged.strength >= 0.0 and merged.strength <= 1.0
    end
  end

  describe "AttentionValue" do
    test "creates new attention value" do
      av = AttentionValue.new(100.0, 50.0, 10.0)
      assert av.sti == 100.0
      assert av.lti == 50.0
      assert av.vlti == 10.0
    end

    test "spreads attention" do
      av = AttentionValue.new(100.0, 0.0, 0.0)
      spread = AttentionValue.spread(av)

      assert spread.sti < av.sti
      assert spread.lti > av.lti
    end
  end

  describe "Atom" do
    test "creates new atom" do
      atom = Atom.new("ConceptNode", "Cat", 0.9, 0.8)
      assert atom.type == "ConceptNode"
      assert atom.name == "Cat"
      assert atom.id != nil
    end
  end

  describe "Link" do
    test "creates new link" do
      link = Link.new("InheritanceLink", ["id1", "id2"])
      assert link.type == "InheritanceLink"
      assert length(link.outgoing) == 2
      assert link.id != nil
    end
  end

  describe "PPM" do
    test "computes coherence from primes" do
      primes = [2, 3, 5, 7]
      coherence = PPM.compute_coherence(primes)

      assert is_float(coherence)
      assert coherence >= 0.0 and coherence <= 1.0
    end

    test "encodes value as primes" do
      primes = PPM.prime_encode(0.5, 5)
      assert is_list(primes)
      assert length(primes) <= 5
    end

    test "generates golden spiral" do
      points = PPM.golden_spiral(10)
      assert length(points) == 10

      for {x, y} <- points do
        assert is_float(x)
        assert is_float(y)
      end
    end
  end

  describe "TimeCrystal" do
    test "creates new time crystal" do
      tc = TimeCrystal.new(11)
      assert tc.dimensions == 11
      assert length(tc.state_vector) == 11
      assert is_float(tc.coherence)
    end

    test "evolves time crystal" do
      tc = TimeCrystal.new(11)
      evolved = TimeCrystal.evolve(tc)

      assert length(evolved.state_vector) == 11
      assert is_float(evolved.coherence)
    end

    test "measures time crystal" do
      tc = TimeCrystal.new(11)
      measurement = TimeCrystal.measure(tc, 2)
      assert is_float(measurement)
    end

    test "entangles time crystals" do
      tc1 = TimeCrystal.new(11)
      tc2 = TimeCrystal.new(11)

      {tc1_ent, tc2_ent} = TimeCrystal.entangle(tc1, tc2, 0.1)

      assert length(tc1_ent.state_vector) == 11
      assert length(tc2_ent.state_vector) == 11
    end

    test "calculates overlap" do
      tc1 = TimeCrystal.new(11)
      tc2 = TimeCrystal.new(11)

      overlap = TimeCrystal.overlap(tc1, tc2)
      assert is_float(overlap)
      assert overlap >= -1.0 and overlap <= 1.0
    end
  end

  describe "AtomSpace" do
    setup do
      {:ok, atom_space} = AtomSpace.start_link(name: nil)
      %{atom_space: atom_space}
    end

    test "creates atoms", %{atom_space: as} do
      {:ok, atom_id} = AtomSpace.create_atom(as, "ConceptNode", "Cat", 0.9, 0.8)
      assert is_binary(atom_id)

      atom = AtomSpace.get_atom(as, atom_id)
      assert atom.name == "Cat"
    end

    test "gets atoms by type", %{atom_space: as} do
      {:ok, _} = AtomSpace.create_atom(as, "ConceptNode", "Cat", 0.9, 0.8)
      {:ok, _} = AtomSpace.create_atom(as, "ConceptNode", "Dog", 0.9, 0.8)

      atoms = AtomSpace.get_atoms_by_type(as, "ConceptNode")
      assert length(atoms) == 2
    end

    test "gets metrics", %{atom_space: as} do
      metrics = AtomSpace.get_metrics(as)
      assert metrics.total_atoms == 0

      {:ok, _} = AtomSpace.create_atom(as, "ConceptNode", "Cat", 0.9, 0.8)
      
      new_metrics = AtomSpace.get_metrics(as)
      assert new_metrics.total_atoms == 1
    end

    test "runs cycles", %{atom_space: as} do
      {:ok, _} = AtomSpace.create_atom(as, "ConceptNode", "Cat", 0.9, 0.8)
      :ok = AtomSpace.run_cycles(as, 5)

      metrics = AtomSpace.get_metrics(as)
      assert is_float(metrics.consciousness_emergence)
    end
  end

  describe "Kernel" do
    test "creates and initializes kernel" do
      kernel = Kernel.new()
      kernel = Kernel.initialize(kernel)

      assert Kernel.active?(kernel)
      assert kernel.atom_space != nil
      assert length(kernel.time_crystals) > 0

      Kernel.shutdown(kernel)
    end

    test "creates atoms through kernel" do
      kernel = Kernel.new() |> Kernel.initialize()

      {:ok, atom_id, kernel} = Kernel.create_atom(kernel, "ConceptNode", "Cat", 0.9, 0.8)
      assert is_binary(atom_id)

      Kernel.shutdown(kernel)
    end

    test "gets metrics" do
      kernel = Kernel.new() |> Kernel.initialize()

      metrics = Kernel.get_metrics(kernel)
      assert is_map(metrics)
      assert Map.has_key?(metrics, :total_atoms)
      assert Map.has_key?(metrics, :time_crystal_coherence)

      Kernel.shutdown(kernel)
    end

    test "runs cycles" do
      kernel = Kernel.new() |> Kernel.initialize()
      {:ok, _, kernel} = Kernel.create_atom(kernel, "ConceptNode", "Cat", 0.9, 0.8)

      kernel = Kernel.run_cycles(kernel, 5)
      metrics = Kernel.get_metrics(kernel)

      assert metrics.total_atoms == 1

      Kernel.shutdown(kernel)
    end

    test "analyzes signal" do
      kernel = Kernel.new() |> Kernel.initialize()

      signal = for i <- 0..15, do: :math.sin(i * 0.5)
      analysis = Kernel.analyze_signal(kernel, signal)

      assert is_map(analysis)
      assert Map.has_key?(analysis, :dominant_primes)
      assert Map.has_key?(analysis, :overall_coherence)
      assert analysis.signal_length == 16

      Kernel.shutdown(kernel)
    end
  end
end
