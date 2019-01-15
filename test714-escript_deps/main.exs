defmodule Foo.Web do
  use Plug.Router

  plug Plug.Logger
  plug :match
  plug :dispatch

  get "/user/:user_id" do
    {uid, ""} = Integer.parse(user_id)
    conn
    |> put_resp_content_type("application/json")
    |> send_resp(200, Poison.encode!(%{"user_id" => uid}))
  end

  match _ do
    send_resp(conn, 404, "")
  end
end

Plug.Cowboy.http Foo.Web, [], port: 4000
