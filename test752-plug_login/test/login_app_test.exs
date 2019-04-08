defmodule LoginAppTest do
  use ExUnit.Case
  doctest LoginApp

  test "greets the world" do
    assert LoginApp.hello() == :world
  end
end
