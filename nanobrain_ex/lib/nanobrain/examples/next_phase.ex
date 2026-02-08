defmodule Nanobrain.Examples.NextPhase do
  @moduledoc """
  Examples demonstrating the next phase implementation:
  1. Learnability embeddings with neural networks
  2. Cognitive grip fabric with multi-language transformation
  3. Integration of ideas into operational implementations
  """
  
  alias Nanobrain.{Tensor, NN, CognitiveGrip, CognitiveIdea}
  alias Nanobrain.CognitiveGrip.Transformer
  
  def example_neural_network do
    IO.puts("\n=== Example 1: Neural Network Forward/Backward Pass ===\n")
    
    layer1 = NN.Linear.new(4, 3)
    layer2 = NN.Linear.new(3, 2)
    
    {:ok, input} = Tensor.new([[1.0, 2.0, 3.0, 4.0]])
    IO.puts("Input shape: #{inspect(Tensor.shape(input))}")
    
    {hidden, layer1} = NN.Linear.forward(layer1, input)
    IO.puts("Hidden layer shape: #{inspect(Tensor.shape(hidden))}")
    
    {output, layer2} = NN.Linear.forward(layer2, hidden)
    IO.puts("Output shape: #{inspect(Tensor.shape(output))}")
    
    {:ok, grad_output} = Tensor.new([[0.1, -0.1]])
    {grad_hidden, layer2} = NN.Linear.backward(layer2, grad_output)
    {_grad_input, _layer1} = NN.Linear.backward(layer1, grad_hidden)
    
    IO.puts("\n✓ Neural network example completed successfully!")
  end
  
  def example_cognitive_transformation do
    IO.puts("\n=== Example 2: Cognitive Grip Fabric ===\n")
    
    idea = CognitiveIdea.neural_network_idea()
    
    IO.puts("Cognitive Idea: #{idea.name}")
    IO.puts("Description: #{idea.description}")
    
    racket_code = CognitiveGrip.Racket.transform(idea)
    IO.puts("\n--- Racket Code Preview ---")
    IO.puts(String.slice(racket_code, 0, 300) <> "...")
    
    IO.puts("\n✓ Cognitive transformation example completed successfully!")
  end
  
  def run_all do
    IO.puts("╔═══════════════════════════════════════════════════════════════╗")
    IO.puts("║  NanoBrain Next Phase Implementation Examples                ║")
    IO.puts("╚═══════════════════════════════════════════════════════════════╝")
    
    example_neural_network()
    example_cognitive_transformation()
    
    IO.puts("\n✓ All examples completed!\n")
  end
end
