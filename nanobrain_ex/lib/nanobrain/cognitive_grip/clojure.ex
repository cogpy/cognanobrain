defmodule Nanobrain.CognitiveGrip.Clojure do
  @moduledoc """
  Clojure language bridge for functional JVM programming.
  
  Clojure emphasizes immutability, functional programming on the JVM,
  and includes powerful spec system for data validation.
  """
  
  alias Nanobrain.CognitiveIdea
  
  @doc """
  Transform a cognitive idea into Clojure code.
  """
  @spec transform(CognitiveIdea.t()) :: String.t()
  def transform(%CognitiveIdea{} = idea) do
    name_lower = String.downcase(idea.name)
    
    """
    (ns nanobrain.#{name_lower}
      "#{idea.description}"
      (:require [clojure.spec.alpha :as s]))

    ;;; #{idea.name}
    ;;; Generated: #{DateTime.utc_now() |> DateTime.to_string()}

    #{generate_specs(idea)}

    #{generate_forward_pass(idea)}

    #{generate_layer_implementations(idea)}

    #{generate_training(idea)}
    """
  end
  
  defp generate_specs(_idea) do
    """
    ;; Spec definitions
    (s/def ::tensor (s/coll-of number? :kind vector?))
    (s/def ::matrix (s/coll-of ::tensor :kind vector?))
    (s/def ::network (s/keys :req [::layers ::parameters]))
    (s/def ::layers (s/coll-of map?))
    """
  end
  
  defp generate_forward_pass(_idea) do
    """
    ;; Forward pass with spec validation
    (defn forward-pass
      "Compute forward pass through network"
      [network input]
      {:pre [(s/valid? ::network network)
             (s/valid? ::tensor input)]
       :post [(s/valid? ::tensor %)]}
      (reduce (fn [acc layer]
                (layer-forward layer acc))
              input
              (:layers network)))
    """
  end
  
  defp generate_layer_implementations(idea) do
    layers = Enum.filter(idea.components, & &1.type == :layer)
    
    implementations = for layer <- layers do
      case layer.subtype do
        :linear -> generate_linear_layer(layer)
        _ -> ""
      end
    end
    
    Enum.join(implementations, "\n\n")
  end
  
  defp generate_linear_layer(_layer) do
    """
    ;; Linear layer implementation
    (defn layer-forward
      "Forward pass for a single layer"
      [layer input]
      (let [weights (:weights layer)
            bias (:bias layer)]
        (map + (matrix-multiply input weights) bias)))

    (defn matrix-multiply
      "Multiply matrix by vector"
      [matrix vector]
      (mapv (fn [row]
              (reduce + (map * row vector)))
            matrix))
    """
  end
  
  defp generate_training(idea) do
    params = idea.parameters
    lr = params[:learning_rate] || 0.01
    
    """
    ;; Training functions
    (defn train-network
      "Train network for specified number of epochs"
      [network data epochs]
      (loop [net network
             epoch 0]
        (if (>= epoch epochs)
          net
          (recur (train-epoch net data) (inc epoch)))))

    (defn train-epoch
      "Train for one epoch"
      [network data]
      (reduce (fn [net batch]
                (let [[inputs targets] batch
                      predictions (forward-pass net inputs)
                      loss (compute-loss predictions targets)
                      gradients (backward-pass net loss)]
                  (update-parameters net gradients #{lr})))
              network
              data))
    """
  end
end
