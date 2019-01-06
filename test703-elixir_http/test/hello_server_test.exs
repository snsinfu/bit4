defmodule HelloServerTest do
  use ExUnit.Case
  doctest HelloServer

  test "greets the world" do
    assert HelloServer.hello() == :world
  end
end
