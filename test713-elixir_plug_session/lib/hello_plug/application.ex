defmodule HelloPlug.Application do
  use Application

  @default_port 4000

  def start(_type, _args) do
    case get_port() do
      :error ->
        {:error, "invalid PORT environment variable"}

      {:ok, port} ->
        do_start(port)
    end
  end

  def do_start(port) do
    children = [
      Plug.Cowboy.child_spec(
        scheme: :http,
        plug: HelloPlug.Web,
        options: [port: port]
      )
    ]

    # See https://hexdocs.pm/elixir/Supervisor.html
    # for other strategies and supported options
    opts = [strategy: :one_for_one, name: HelloPlug.Supervisor]
    Supervisor.start_link(children, opts)
  end

  def get_port() do
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
