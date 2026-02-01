defmodule Nanobrain.MixProject do
  use Mix.Project

  def project do
    [
      app: :nanobrain,
      version: "1.0.0",
      elixir: "~> 1.14",
      start_permanent: Mix.env() == :prod,
      deps: deps(),
      description: "Pure Elixir implementation of NanoBrain cognitive architecture",
      package: package(),
      name: "Nanobrain",
      source_url: "https://github.com/o9nn/nanocyc"
    ]
  end

  # Run "mix help compile.app" to learn about applications.
  def application do
    [
      extra_applications: [:logger, :crypto]
    ]
  end

  # Run "mix help deps" to learn about dependencies.
  defp deps do
    [
      {:ex_doc, "~> 0.29", only: :dev, runtime: false}
    ]
  end

  defp package do
    [
      licenses: ["MIT"],
      links: %{"GitHub" => "https://github.com/o9nn/nanocyc"}
    ]
  end
end
