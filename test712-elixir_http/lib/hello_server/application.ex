defmodule HelloServer.Application do
  use Application

  @default_port 4000

  def start(_type, _args) do
    case get_port() do
      :error -> {:error, "invalid PORT environment variable"}
      {:ok, port} -> do_start(port)
    end
  end

  defp do_start(port) do
    # List all child processes to be supervise
    children = [
      Plug.Cowboy.child_spec(
        scheme: :http,
        plug: HelloServer.Router,
        options: [port: port]
      )
    ]

    # See https://hexdocs.pm/elixir/Supervisor.html
    # for other strategies and supported options
    opts = [strategy: :one_for_one, name: HelloServer.Supervisor]
    Supervisor.start_link(children, opts)
  end

  defp get_port() do
    case System.get_env("PORT") do
      nil ->
        {:ok, @default_port}

      s ->
        case Integer.parse(s) do
          {n, ""} -> {:ok, n}
          _ -> :error
        end
    end
  end
end
