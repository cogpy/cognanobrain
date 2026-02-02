defmodule Nanobrain.Link do
  @moduledoc """
  Represents a link between atoms in the AtomSpace - relationships in knowledge graph.
  """

  alias Nanobrain.{TruthValue, AttentionValue}

  defstruct id: nil,
            type: "InheritanceLink",
            outgoing: [],
            truth: %TruthValue{},
            attention: %AttentionValue{}

  @type t :: %__MODULE__{
          id: String.t() | nil,
          type: String.t(),
          outgoing: [String.t()],
          truth: TruthValue.t(),
          attention: AttentionValue.t()
        }

  @doc """
  Create a new Link connecting atoms.
  """
  def new(type, outgoing_ids, truth \\ %TruthValue{}, attention \\ %AttentionValue{}) do
    %__MODULE__{
      id: generate_id(),
      type: type,
      outgoing: outgoing_ids,
      truth: truth,
      attention: attention
    }
  end

  defp generate_id do
    :crypto.strong_rand_bytes(16)
    |> Base.encode16(case: :lower)
  end
end
