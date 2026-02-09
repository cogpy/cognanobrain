defmodule Nanobrain.CognitiveGrip.Transformer do
  @moduledoc """
  Multi-language code generation and transformation system.
  
  Enables transformation of cognitive ideas into operational implementations
  across multiple programming paradigms including Racket, Clojure, Scheme,
  Perl, and Raku.
  
  ## Example
  
      idea = Nanobrain.CognitiveIdea.neural_network_idea()
      implementations = Nanobrain.CognitiveGrip.Transformer.transform_idea(idea, [:racket, :clojure])
      
      implementations[:racket] # Generated Racket code
      implementations[:clojure] # Generated Clojure code
  """
  
  alias Nanobrain.CognitiveGrip.{Racket, Clojure, Scheme, Perl, Raku}
  
  @supported_languages [:racket, :clojure, :scheme, :perl, :raku]
  
  @doc """
  Transform a cognitive idea into multiple language implementations.
  """
  @spec transform_idea(Nanobrain.CognitiveIdea.t(), :all | list(atom())) :: map()
  def transform_idea(cognitive_idea, target_languages \\ :all) do
    languages = if target_languages == :all do
      @supported_languages
    else
      target_languages
    end
    
    Enum.map(languages, fn lang ->
      bridge = get_bridge_module(lang)
      {lang, bridge.transform(cognitive_idea)}
    end)
    |> Map.new()
  end
  
  @doc """
  Get list of supported languages.
  """
  @spec supported_languages() :: list(atom())
  def supported_languages, do: @supported_languages
  
  defp get_bridge_module(:racket), do: Racket
  defp get_bridge_module(:clojure), do: Clojure
  defp get_bridge_module(:scheme), do: Scheme
  defp get_bridge_module(:perl), do: Perl
  defp get_bridge_module(:raku), do: Raku
end
