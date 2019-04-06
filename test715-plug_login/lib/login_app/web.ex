defmodule LoginApp.Web do
  use Plug.Router

  @server_signature Application.get_env(:login_app, :server_signature)

  plug(Plug.Logger)
  plug(:match)
  plug(:dispatch)

  get "/" do
    homepage = """
    <!DOCTYPE html>
    <html lang="en">
      <head>
        <meta charset="utf-8">
        <meta name="viewport" content="width=device-width, initial-scale=1">
        <title>Home</title>
      </head>
      <body>
        <h1>Home</h1>
      </body>
    </html>
    """
    conn
    |> put_resp_content_type("text/html")
    |> update_resp_header("server", @server_signature, &(&1))
    |> send_resp(200, homepage)
  end

  match _ do
    send_resp(conn, 404, "")
  end
end
