defmodule Nanobrain.NN.Module do
  @moduledoc """
  Behaviour for all neural network modules.
  
  Inspired by Torch7's nn.Module interface, this provides the foundation
  for building neural networks in a modular, composable way.
  
  All neural network components (layers, activations, loss functions) implement
  this behaviour to provide consistent forward and backward pass interfaces.
  """
  
  @callback forward(module_state :: map(), input :: term()) :: 
    {output :: term(), updated_module_state :: map()}
  
  @callback backward(module_state :: map(), grad_output :: term()) :: 
    {grad_input :: term(), updated_module_state :: map()}
  
  @callback parameters(module_state :: map()) :: list(Nanobrain.Tensor.t())
  
  @callback gradients(module_state :: map()) :: list(Nanobrain.Tensor.t())
  
  @callback zero_grad(module_state :: map()) :: map()
  
  @optional_callbacks [parameters: 1, gradients: 1, zero_grad: 1]
end
