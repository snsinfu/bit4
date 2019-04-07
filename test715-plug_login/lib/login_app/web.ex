defmodule LoginApp.Web do
  use Plug.Router
  use Plug.ErrorHandler

  require EEx

  @session_opts [
    store: :cookie,
    key: "_session",
    encryption_salt: Application.get_env(:login_app, :session_encryption_salt),
    signing_salt: Application.get_env(:login_app, :session_signing_salt)
  ]
  @session_secret Application.get_env(:login_app, :session_secret)
  @server_signature Application.get_env(:login_app, :server_signature)

  plug(Plug.Logger)
  plug(:put_secret_key_base)
  plug(Plug.Session, @session_opts)
  plug(:fetch_session)
  plug(Plug.Parsers, parsers: [:urlencoded])
  plug(Plug.CSRFProtection)
  plug(:match)
  plug(:dispatch)

  @doc """
  Puts `secret_key_base` in the connection.
  """
  defp put_secret_key_base(conn, _) do
    conn
    |> Map.put(:secret_key_base, @session_secret)
  end

  @doc """
  Displays home page. User information is displayed if it is available in the session
  (that is, the user is logged in). Otherwise a link to the login page is shown.
  """
  get "/" do
    html = EEx.eval_file("lib/login_app/templates/home.html.eex", session: get_session(conn))

    conn
    |> put_resp_content_type("text/html")
    |> send_resp(200, html)
  end

  @doc """
  Displays login page.
  """
  get "/login" do
    html =
      EEx.eval_file(
        "lib/login_app/templates/login.html.eex",
        session: get_session(conn),
        csrf_token: Plug.CSRFProtection.get_csrf_token()
      )

    conn
    |> put_resp_content_type("text/html")
    |> send_resp(200, html)
  end

  @doc """
  Lets client log in as the requested user. Session is refreshed with the user
  information. The client is redirected to the home page.
  """
  post "/login" do
    conn = clear_session(conn)
    conn = put_session(conn, "username", Map.get(conn.body_params, "username"))

    conn
    |> put_resp_header("location", "/")
    |> send_resp(302, "")
  end

  @doc """
  Clears session. The client is redirected to the home page.
  """
  get "/logout" do
    conn = clear_session(conn)

    conn
    |> put_resp_header("location", "/")
    |> send_resp(302, "")
  end

  @doc """
  Fallback handler. It responds with 404 Not Found.
  """
  match _ do
    send_resp(conn, 404, "")
  end

  @doc """
  Handles exception raised in request handler.
  """
  def handle_errors(conn, %{kind: _kind, reason: _reason, stack: _stack}) do
    send_resp(conn, conn.status, "")
  end
end
