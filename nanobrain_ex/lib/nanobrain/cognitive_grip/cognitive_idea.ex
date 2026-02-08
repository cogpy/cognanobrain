defmodule Nanobrain.CognitiveIdea do
  @moduledoc """
  Represents an abstract cognitive idea that can be transformed
  into operational implementations across different programming languages.
  
  A cognitive idea captures the essence of a computational pattern,
  algorithm, or system in a language-agnostic way.
  """
  
  defstruct [:name, :description, :components, :parameters, :constraints, :metadata]
  
  @type component :: %{type: atom(), subtype: atom(), size: pos_integer() | nil}
  @type t :: %__MODULE__{
    name: String.t(),
    description: String.t(),
    components: list(component()),
    parameters: map(),
    constraints: list(term()),
    metadata: map()
  }
  
  @doc """
  Creates a new cognitive idea.
  """
  @spec new(String.t(), String.t(), keyword()) :: t()
  def new(name, description, opts \\ []) do
    %__MODULE__{
      name: name,
      description: description,
      components: Keyword.get(opts, :components, []),
      parameters: Keyword.get(opts, :parameters, %{}),
      constraints: Keyword.get(opts, :constraints, []),
      metadata: Keyword.get(opts, :metadata, %{})
    }
  end
  
  @doc """
  Example: Neural network cognitive idea.
  """
  @spec neural_network_idea() :: t()
  def neural_network_idea do
    new("NeuralNetwork", "Multi-layer perceptron for pattern recognition",
      components: [
        %{type: :layer, subtype: :linear, size: 128},
        %{type: :activation, subtype: :relu, size: nil},
        %{type: :layer, subtype: :linear, size: 64},
        %{type: :activation, subtype: :relu, size: nil},
        %{type: :layer, subtype: :linear, size: 10},
        %{type: :activation, subtype: :softmax, size: nil}
      ],
      parameters: %{
        learning_rate: 0.01,
        batch_size: 32,
        epochs: 100
      }
    )
  end
  
  @doc """
  Example: Time crystal processing idea.
  """
  @spec time_crystal_idea() :: t()
  def time_crystal_idea do
    new("TimeCrystal", "11-dimensional quantum manifold processing",
      components: [
        %{type: :manifold, subtype: :quantum, size: 11},
        %{type: :evolution, subtype: :temporal, size: nil},
        %{type: :measurement, subtype: :prime, size: 15}
      ],
      parameters: %{
        dimensions: 11,
        primes: [2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47]
      }
    )
  end
end
