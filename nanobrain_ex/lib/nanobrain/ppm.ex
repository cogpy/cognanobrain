defmodule Nanobrain.PPM do
  @moduledoc """
  Phase Prime Metrics - mathematical framework using prime number patterns.
  
  Implements the core PPM coherence calculations and prime encoding
  that forms the foundation of NanoBrain's consciousness modeling.
  """

  alias Nanobrain.Constants

  @doc """
  Compute Phase Prime Metric coherence from a list of primes.
  
  Formula: coherence = 0.5 + 0.5 * sin(sqrt(product) * π / sum)
  """
  def compute_coherence([]), do: 0.5

  def compute_coherence(primes) when is_list(primes) do
    product = Enum.reduce(primes, 1, &*/2)
    total = Enum.sum(primes)

    if total == 0 do
      0.5
    else
      sqrt_product = :math.sqrt(product)
      0.5 + 0.5 * :math.sin(sqrt_product * Constants.pi() / total)
    end
  end

  @doc """
  Encode a floating point value as a prime signature.
  
  Uses golden ratio-based pseudo-random selection from fundamental primes.
  """
  def prime_encode(value, num_primes \\ 5) do
    abs_val = abs(value)
    primes = Constants.fundamental_primes()

    1..num_primes
    |> Enum.reduce({abs_val, []}, fn _, {val, acc} ->
      idx = trunc(val * length(primes)) |> rem(length(primes))
      prime = Enum.at(primes, idx)
      new_val = (val * Constants.golden_ratio()) |> :math.fmod(1.0)
      {new_val, [prime | acc]}
    end)
    |> elem(1)
    |> Enum.uniq()
    |> Enum.sort()
  end

  @doc """
  Generate golden spiral coordinates for n points.
  
  Used for visualization of consciousness patterns.
  """
  def golden_spiral(n) do
    for i <- 0..(n - 1) do
      theta = i * 2 * Constants.pi() / Constants.golden_ratio()
      r = :math.sqrt(i + 1)
      x = r * :math.cos(theta)
      y = r * :math.sin(theta)
      {x, y}
    end
  end

  @doc """
  Calculate prime spectrum analysis of a signal.
  
  Returns the dominant prime components and their amplitudes.
  """
  def analyze_signal(signal) when is_list(signal) do
    signal
    |> Enum.with_index()
    |> Enum.map(fn {value, idx} ->
      primes = prime_encode(value)
      coherence = compute_coherence(primes)
      {idx, value, primes, coherence}
    end)
  end
end
