defmodule Nanobrain.Atom do
  @moduledoc """
  Represents an atom in the AtomSpace - fundamental unit of knowledge.
  """

  alias Nanobrain.{TruthValue, AttentionValue}

  defstruct id: nil,
            type: "ConceptNode",
            name: "",
            truth: %TruthValue{},
            attention: %AttentionValue{}

  @type t :: %__MODULE__{
          id: String.t() | nil,
          type: String.t(),
          name: String.t(),
          truth: TruthValue.t(),
          attention: AttentionValue.t()
        }

  @doc """
  Create a new Atom.
  """
  def new(type, name, truth \\ nil, attention \\ nil)

  def new(type, name, truth, attention) when is_struct(truth, TruthValue) do
    %__MODULE__{
      id: generate_id(),
      type: type,
      name: name,
      truth: truth,
      attention: attention || %AttentionValue{}
    }
  end

  @doc """
  Create a new Atom with explicit truth values.
  """
  def new(type, name, strength, confidence) when is_number(strength) and is_number(confidence) do
    new(type, name, TruthValue.new(strength, confidence), %AttentionValue{})
  end

  defp generate_id do
    :crypto.strong_rand_bytes(16)
    |> Base.encode16(case: :lower)
  end
end
