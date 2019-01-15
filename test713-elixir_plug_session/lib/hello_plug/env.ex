defmodule HelloPlug.Env do
  def session_signing_salt do
    System.get_env("HELLOPLUG_SESSION_SIGNING_SALT")
  end

  def secret_key_base do
    System.get_env("HELLOPLUG_SECRET_KEY_BASE")
  end
end
