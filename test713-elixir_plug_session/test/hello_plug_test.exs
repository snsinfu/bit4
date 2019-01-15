defmodule HelloPlugTest do
  use ExUnit.Case
  doctest HelloPlug

  test "greets the world" do
    assert HelloPlug.hello() == :world
  end
end
