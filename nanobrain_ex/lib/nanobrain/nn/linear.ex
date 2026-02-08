defmodule Nanobrain.NN.Linear do
  @moduledoc """
  Linear (fully connected) layer implementation.
  Computes: output = input * weight^T + bias
  Inspired by Torch7's nn.Linear.
  """
  
  @behaviour Nanobrain.NN.Module
  
  alias Nanobrain.Tensor
  
  defstruct [:weight, :bias, :input_size, :output_size, :grad_weight, :grad_bias, :last_input]
  
  @spec new(pos_integer(), pos_integer()) :: %__MODULE__{}
  def new(input_size, output_size) do
    limit = :math.sqrt(6.0 / (input_size + output_size))
    
    {:ok, weight} = Tensor.uniform({output_size, input_size}, -limit, limit)
    {:ok, bias} = Tensor.zeros({output_size})
    {:ok, grad_weight} = Tensor.zeros({output_size, input_size})
    {:ok, grad_bias} = Tensor.zeros({output_size})
    
    %__MODULE__{
      weight: weight,
      bias: bias,
      input_size: input_size,
      output_size: output_size,
      grad_weight: grad_weight,
      grad_bias: grad_bias
    }
  end
  
  @impl Nanobrain.NN.Module
  def forward(%__MODULE__{} = layer, input) do
    {:ok, weight_t} = Tensor.transpose(layer.weight)
    {:ok, output} = Tensor.matmul(input, weight_t)
    output = add_bias(output, layer.bias)
    {output, %{layer | last_input: input}}
  end
  
  @impl Nanobrain.NN.Module
  def backward(%__MODULE__{last_input: nil}, _grad_output) do
    raise "Must call forward before backward"
  end
  
  def backward(%__MODULE__{} = layer, grad_output) do
    {:ok, grad_output_t} = Tensor.transpose(grad_output)
    {:ok, grad_weight} = Tensor.matmul(grad_output_t, layer.last_input)
    grad_bias = sum_rows(grad_output)
    {:ok, grad_input} = Tensor.matmul(grad_output, layer.weight)
    
    layer = %{layer | grad_weight: grad_weight, grad_bias: grad_bias}
    {grad_input, layer}
  end
  
  @impl Nanobrain.NN.Module
  def parameters(%__MODULE__{} = layer), do: [layer.weight, layer.bias]
  
  @impl Nanobrain.NN.Module
  def gradients(%__MODULE__{} = layer), do: [layer.grad_weight, layer.grad_bias]
  
  @impl Nanobrain.NN.Module
  def zero_grad(%__MODULE__{} = layer) do
    {:ok, grad_weight} = Tensor.zeros(Tensor.shape(layer.weight))
    {:ok, grad_bias} = Tensor.zeros(Tensor.shape(layer.bias))
    %{layer | grad_weight: grad_weight, grad_bias: grad_bias}
  end
  
  defp add_bias(tensor, bias) do
    bias_list = Tensor.to_list(bias)
    tensor_list = Tensor.to_list(tensor)
    {_rows, cols} = Tensor.shape(tensor)
    
    result = for row <- Enum.chunk_every(tensor_list, cols) do
      Enum.zip(row, bias_list) |> Enum.map(fn {x, b} -> x + b end)
    end |> List.flatten()
    
    {:ok, tensor} = Tensor.new(Enum.chunk_every(result, cols))
    tensor
  end
  
  defp sum_rows(tensor) do
    {_rows, cols} = Tensor.shape(tensor)
    tensor_list = Tensor.to_list(tensor)
    
    result = tensor_list
    |> Enum.chunk_every(cols)
    |> Enum.reduce(List.duplicate(0.0, cols), fn row, acc ->
      Enum.zip(row, acc) |> Enum.map(fn {x, sum} -> x + sum end)
    end)
    
    {:ok, bias} = Tensor.new(result)
    bias
  end
end
