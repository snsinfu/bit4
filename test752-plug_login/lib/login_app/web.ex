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

  plug(Plug.Logger)
  plug(:put_secret_key_base)
  plug(Plug.Session, @session_opts)
  plug(:fetch_session)
  plug(Plug.Parsers, parsers: [:urlencoded])
  plug(Plug.CSRFProtection)
  plug(:match)
  plug(:dispatch)

  def init(opts) do
    # The table needs to be public because the handlers are invoked from Cowboy's process.
    # Otherwise the VM raises ArgumentError on calling :ets.insert_new/2 from a handler.
    :ets.new(:users, [:set, :public, :named_table])
    opts
  end

  defp put_secret_key_base(conn, _) do
    conn
    |> Map.put(:secret_key_base, @session_secret)
  end

  get "/" do
    html = EEx.eval_file("lib/login_app/templates/home.html.eex", session: get_session(conn))

    conn
    |> put_resp_content_type("text/html")
    |> send_resp(200, html)
  end

  get "/signup" do
    html =
      EEx.eval_file(
        "lib/login_app/templates/signup.html.eex",
        session: get_session(conn),
        csrf_token: Plug.CSRFProtection.get_csrf_token()
      )

    conn
    |> put_resp_content_type("text/html")
    |> send_resp(200, html)
  end

  post "/signup" do
    %{"username" => username, "password" => password} = conn.body_params

    # XXX: Password should be hashed with a random-salted PBKDF. Also, the key should
    # be a generated uid, not raw username.
    conn =
      if :ets.insert_new(:users, {username, %{password: password}}) do
        conn = clear_session(conn)
        conn = put_session(conn, "username", username)
        conn
      else
        conn
      end

    conn
    |> put_resp_header("location", "/")
    |> send_resp(302, "")
  end

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

  post "/login" do
    %{"username" => username, "password" => password} = conn.body_params

    # XXX: Again, uid should be used instead of the raw username and password should
    # really be hashed. Hash the password! Hash the password!
    conn =
      case :ets.lookup(:users, username) do
        [{^username, %{password: ^password}}] ->
          conn = clear_session(conn)
          conn = put_session(conn, "username", username)
          conn

        [{^username, %{}}] ->
          # Incorrect password
          conn

        [] ->
          # Nonexistent user
          conn
      end

    conn
    |> put_resp_header("location", "/")
    |> send_resp(302, "")
  end

  get "/logout" do
    conn = clear_session(conn)

    conn
    |> put_resp_header("location", "/")
    |> send_resp(302, "")
  end

  match _ do
    send_resp(conn, 404, "")
  end

  def handle_errors(conn, %{kind: _kind, reason: _reason, stack: _stack}) do
    send_resp(conn, conn.status, "")
  end
end
