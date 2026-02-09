defmodule Nanobrain.Tensor do
  @moduledoc """
  Multi-dimensional tensor operations for NanoBrain neural computing.
  
  Provides efficient tensor storage and operations using Elixir binaries
  for memory efficiency and BEAM VM optimization.
  
  Inspired by PyTorch and Torch7 tensor APIs.
  
  ## Examples
  
      iex> {:ok, t} = Nanobrain.Tensor.new([1, 2, 3, 4])
      iex> Nanobrain.Tensor.shape(t)
      {4}
      
      iex> {:ok, t} = Nanobrain.Tensor.new([[1, 2], [3, 4]])
      iex> Nanobrain.Tensor.shape(t)
      {2, 2}
  """
  
  alias __MODULE__
  
  defstruct [:data, :shape, :dtype]
  
  @type dtype :: :float32 | :float64 | :int32 | :int64
  @type shape :: tuple()
  @type t :: %__MODULE__{
    data: binary(),
    shape: shape(),
    dtype: dtype()
  }
  
  @doc """
  Creates a new tensor from a list or nested list.
  """
  @spec new(list() | number(), keyword()) :: {:ok, t()} | {:error, term()}
  def new(data, opts \\ []) do
    dtype = Keyword.get(opts, :dtype, :float32)
    
    cond do
      is_number(data) ->
        new_scalar(data, dtype)
      
      is_list(data) ->
        new_from_list(data, dtype)
      
      true ->
        {:error, :invalid_input}
    end
  end
  
  @doc """
  Creates a tensor filled with zeros.
  """
  @spec zeros(shape()) :: {:ok, t()}
  def zeros(shape) do
    size = tuple_product(shape)
    data = :binary.copy(<<0.0::float-32-native>>, size)
    {:ok, %Tensor{data: data, shape: shape, dtype: :float32}}
  end
  
  @doc """
  Creates a tensor filled with ones.
  """
  @spec ones(shape()) :: {:ok, t()}
  def ones(shape) do
    size = tuple_product(shape)
    data = :binary.copy(<<1.0::float-32-native>>, size)
    {:ok, %Tensor{data: data, shape: shape, dtype: :float32}}
  end
  
  @doc """
  Creates a tensor with random uniform values between min and max.
  """
  @spec uniform(shape(), float(), float()) :: {:ok, t()}
  def uniform(shape, min \\ 0.0, max \\ 1.0) do
    size = tuple_product(shape)
    range = max - min
    
    data = for _ <- 1..size do
      value = :rand.uniform() * range + min
      <<value::float-32-native>>
    end |> IO.iodata_to_binary()
    
    {:ok, %Tensor{data: data, shape: shape, dtype: :float32}}
  end
  
  @doc """
  Returns the shape of a tensor.
  """
  @spec shape(t()) :: shape()
  def shape(%Tensor{shape: shape}), do: shape
  
  @doc """
  Returns the total number of elements in a tensor.
  """
  @spec numel(t()) :: non_neg_integer()
  def numel(%Tensor{shape: shape}), do: tuple_product(shape)
  
  @doc """
  Element-wise addition of two tensors.
  """
  @spec add(t(), t()) :: {:ok, t()} | {:error, term()}
  def add(%Tensor{shape: s1} = t1, %Tensor{shape: s2} = t2) when s1 == s2 do
    result = binary_op(t1, t2, &+/2)
    {:ok, result}
  end
  def add(_t1, _t2), do: {:error, :shape_mismatch}
  
  @doc """
  Element-wise subtraction of two tensors.
  """
  @spec subtract(t(), t()) :: {:ok, t()} | {:error, term()}
  def subtract(%Tensor{shape: s1} = t1, %Tensor{shape: s2} = t2) when s1 == s2 do
    result = binary_op(t1, t2, &-/2)
    {:ok, result}
  end
  def subtract(_t1, _t2), do: {:error, :shape_mismatch}
  
  @doc """
  Element-wise multiplication of two tensors.
  """
  @spec multiply(t(), t()) :: {:ok, t()} | {:error, term()}
  def multiply(%Tensor{shape: s1} = t1, %Tensor{shape: s2} = t2) when s1 == s2 do
    result = binary_op(t1, t2, &*/2)
    {:ok, result}
  end
  def multiply(_t1, _t2), do: {:error, :shape_mismatch}
  
  @doc """
  Matrix multiplication of two 2D tensors.
  """
  @spec matmul(t(), t()) :: {:ok, t()} | {:error, term()}
  def matmul(%Tensor{shape: {m, k}} = t1, %Tensor{shape: {k2, n}} = t2) when k == k2 do
    result = matmul_impl(t1, t2, m, k, n)
    {:ok, result}
  end
  def matmul(_t1, _t2), do: {:error, :shape_mismatch}
  
  @doc """
  Transposes a 2D tensor.
  """
  @spec transpose(t()) :: {:ok, t()} | {:error, term()}
  def transpose(%Tensor{shape: {m, n}} = tensor) do
    result = transpose_impl(tensor, m, n)
    {:ok, result}
  end
  def transpose(_), do: {:error, :not_2d_tensor}
  
  @doc """
  Converts tensor to a flat list.
  """
  @spec to_list(t()) :: list()
  def to_list(%Tensor{data: data, dtype: :float32}) do
    for <<value::float-32-native <- data>>, do: value
  end
  
  # Private functions
  
  defp new_scalar(value, dtype) when is_number(value) do
    data = encode_value(value, dtype)
    {:ok, %Tensor{data: data, shape: {}, dtype: dtype}}
  end
  
  defp new_from_list(list, dtype) do
    {shape, flat_list} = infer_shape_and_flatten(list)
    
    data = flat_list
    |> Enum.map(&encode_value(&1, dtype))
    |> IO.iodata_to_binary()
    
    {:ok, %Tensor{data: data, shape: shape, dtype: dtype}}
  end
  
  defp infer_shape_and_flatten(list) do
    cond do
      Enum.all?(list, &is_number/1) ->
        {{length(list)}, list}
      
      Enum.all?(list, &is_list/1) ->
        rows = length(list)
        cols = length(hd(list))
        flat = List.flatten(list)
        {{rows, cols}, flat}
      
      true ->
        raise "Unsupported nested structure"
    end
  end
  
  defp encode_value(value, :float32) when is_number(value) do
    <<value::float-32-native>>
  end
  
  defp encode_value(value, :float64) when is_number(value) do
    <<value::float-64-native>>
  end
  
  defp binary_op(%Tensor{data: d1, shape: shape, dtype: dtype}, 
                 %Tensor{data: d2}, 
                 op) do
    result_data = for {v1, v2} <- Enum.zip(decode_data(d1, dtype), decode_data(d2, dtype)) do
      op.(v1, v2) |> encode_value(dtype)
    end |> IO.iodata_to_binary()
    
    %Tensor{data: result_data, shape: shape, dtype: dtype}
  end
  
  defp decode_data(data, :float32) do
    for <<value::float-32-native <- data>>, do: value
  end
  
  defp matmul_impl(%Tensor{data: d1, dtype: dtype1} = t1, 
                   %Tensor{data: d2, dtype: dtype2} = t2, 
                   m, k, n) do
    # Convert to lists for easier manipulation
    mat1 = decode_data(d1, dtype1) |> Enum.chunk_every(k)
    mat2 = decode_data(d2, dtype2) |> Enum.chunk_every(n)
    
    # Transpose mat2 for easier dot product
    mat2_t = transpose_list(mat2, k, n)
    
    # Compute matrix multiplication
    result = for row <- mat1 do
      for col <- mat2_t do
        dot_product(row, col)
      end
    end
    
    # Flatten and encode
    flat_result = List.flatten(result)
    result_data = flat_result
    |> Enum.map(&encode_value(&1, :float32))
    |> IO.iodata_to_binary()
    
    %Tensor{data: result_data, shape: {m, n}, dtype: :float32}
  end
  
  defp transpose_impl(%Tensor{data: data, dtype: dtype}, m, n) do
    mat = decode_data(data, dtype) |> Enum.chunk_every(n)
    transposed = transpose_list(mat, m, n)
    
    flat_result = List.flatten(transposed)
    result_data = flat_result
    |> Enum.map(&encode_value(&1, dtype))
    |> IO.iodata_to_binary()
    
    %Tensor{data: result_data, shape: {n, m}, dtype: dtype}
  end
  
  defp transpose_list(matrix, _rows, cols) do
    for col <- 0..(cols - 1) do
      for row <- matrix do
        Enum.at(row, col)
      end
    end
  end
  
  defp dot_product(v1, v2) do
    Enum.zip(v1, v2)
    |> Enum.map(fn {a, b} -> a * b end)
    |> Enum.sum()
  end
  
  defp tuple_product(shape) when is_tuple(shape) do
    shape
    |> Tuple.to_list()
    |> Enum.reduce(1, &*/2)
  end
end
