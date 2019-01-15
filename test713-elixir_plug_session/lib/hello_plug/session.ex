defmodule HelloPlug.Session do
  use Plug.Builder

  plug :put_secret_key_base

  plug Plug.Session,
    store: :cookie,
    key: "_session",
    signing_salt: HelloPlug.Env.session_signing_salt

  defp put_secret_key_base(conn, _) do
    put_in(conn.secret_key_base, HelloPlug.Env.secret_key_base)
  end
end
