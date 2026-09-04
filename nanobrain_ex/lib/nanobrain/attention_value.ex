defmodule Nanobrain.AttentionValue do
  @moduledoc """
  Attention Value (STI, LTI, VLTI).
  
  Represents cognitive attention allocation:
  - sti: Short-term importance
  - lti: Long-term importance  
  - vlti: Very-long-term importance
  """

  defstruct sti: 0.0, lti: 0.0, vlti: 0.0

  @type t :: %__MODULE__{
          sti: float(),
          lti: float(),
          vlti: float()
        }

  @doc """
  Create a new AttentionValue.
  """
  def new(sti \\ 0.0, lti \\ 0.0, vlti \\ 0.0) do
    %__MODULE__{
      sti: sti,
      lti: lti,
      vlti: vlti
    }
  end

  @doc """
  Update attention values based on spreading activation.
  """
  def spread(%__MODULE__{} = av, decay_factor \\ 0.9) do
    %__MODULE__{
      sti: av.sti * decay_factor,
      lti: av.lti + av.sti * 0.1,
      vlti: av.vlti + av.lti * 0.01
    }
  end
end
