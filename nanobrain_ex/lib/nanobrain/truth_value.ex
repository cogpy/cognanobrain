defmodule Nanobrain.TruthValue do
  @moduledoc """
  Simple Truth Value (strength, confidence, count).
  
  Represents uncertain truth with:
  - strength: how true (0.0 to 1.0)
  - confidence: how certain (0.0 to 1.0)
  - count: evidence count
  """

  defstruct strength: 1.0, confidence: 0.0, count: 0.0

  @type t :: %__MODULE__{
          strength: float(),
          confidence: float(),
          count: float()
        }

  @doc """
  Create a new TruthValue.
  """
  def new(strength \\ 1.0, confidence \\ 0.0, count \\ 0.0) do
    %__MODULE__{
      strength: strength,
      confidence: confidence,
      count: count
    }
  end

  @doc """
  Merge two truth values using revision formula.
  """
  def merge(%__MODULE__{} = tv1, %__MODULE__{} = tv2) do
    s1 = tv1.strength
    c1 = tv1.confidence
    s2 = tv2.strength
    c2 = tv2.confidence

    count = c1 + c2 - c1 * c2

    strength =
      if count > 0 do
        (s1 * c1 + s2 * c2 - s1 * c1 * c2 * s2) / count
      else
        0.5
      end

    new(strength, count, tv1.count + tv2.count)
  end
end
