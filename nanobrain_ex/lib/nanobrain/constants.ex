defmodule Nanobrain.Constants do
  @moduledoc """
  Core constants for NanoBrain cognitive architecture.
  """

  @fundamental_primes [2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47]
  @golden_ratio 1.618033988749895
  @pi :math.pi()
  @time_crystal_dimensions 11

  def fundamental_primes, do: @fundamental_primes
  def golden_ratio, do: @golden_ratio
  def pi, do: @pi
  def time_crystal_dimensions, do: @time_crystal_dimensions
end
