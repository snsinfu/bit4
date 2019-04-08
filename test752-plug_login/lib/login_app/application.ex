defmodule LoginApp.Application do
  # See https://hexdocs.pm/elixir/Application.html
  # for more information on OTP Applications
  @moduledoc false

  use Application

  def start(_type, _args) do
    case get_port() do
      :error -> {:error, "invalid PORT environment variable"}
      {:ok, port} -> do_start(port)
    end
  end

  defp do_start(port) do
    # List all child processes to be supervised
    children = [
      Plug.Cowboy.child_spec(
        scheme: :http,
        plug: LoginApp.Web,
        options: [port: port]
      )
    ]

    # See https://hexdocs.pm/elixir/Supervisor.html
    # for other strategies and supported options
    opts = [strategy: :one_for_one, name: LoginApp.Supervisor]
    Supervisor.start_link(children, opts)
  end

  defp get_port() do
    case System.get_env("PORT") do
      nil ->
        {:ok, Application.get_env(:login_app, :default_port)}

      str ->
        case Integer.parse(str) do
          {n, ""} -> {:ok, n}
          _ -> :error
        end
    end
  end
end
