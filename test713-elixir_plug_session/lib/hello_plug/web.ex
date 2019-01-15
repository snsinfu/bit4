defmodule HelloPlug.Web do
  use Plug.Builder

  plug Plug.Logger
  plug HelloPlug.Router
end
