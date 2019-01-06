defmodule Config do
  @default_port 4000

  def get_port do
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


case Config.get_port() do
  {:ok, port} ->
    IO.puts("port: #{port}")

  :error ->
    IO.puts("error")
end
