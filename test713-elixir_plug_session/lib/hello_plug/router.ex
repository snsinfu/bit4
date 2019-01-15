defmodule HelloPlug.Router do
  use Plug.Router

  plug HelloPlug.Session
  plug :match
  plug :dispatch

  get "/" do
    toppage = """
    <!DOCTYPE html>
    <html lang="en">
      <head>
        <meta charset="utf-8">
        <meta name="viewport" content="width=device-width, initial-width=1">
        <title><%= title %></title>
      </head>
      <body>
        <h1><%= title %></h1>
      </body>
    </html>
    """

    conn
    |> fetch_session
    |> configure_session(renew: true) # FIXME
    |> put_resp_content_type("text/html")
    |> send_resp(200, EEx.eval_string(toppage, title: "Home"))
  end

  match _ do
    send_resp(conn, 404, "")
  end
end
