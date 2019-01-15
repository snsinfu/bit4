defmodule Foo.MixProject do
  use Mix.Project

  def project do
    [
      app: :foo,
      version: "1.0.0",
      elixir: "~> 1.8",
      deps: [
        {:plug_cowboy, "~> 2.0"},
        {:poison, "~> 4.0"}
      ]
    ]
  end
end
