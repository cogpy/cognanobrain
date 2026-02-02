defmodule Nanobrain.Application do
  @moduledoc """
  OTP Application for Nanobrain cognitive architecture.
  """

  use Application

  @impl true
  def start(_type, _args) do
    children = [
      # AtomSpace supervisor
      {Nanobrain.AtomSpace, name: Nanobrain.AtomSpace}
    ]

    opts = [strategy: :one_for_one, name: Nanobrain.Supervisor]
    Supervisor.start_link(children, opts)
  end
end
