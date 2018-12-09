defmodule Math do
  def zero?(0) do
    true
  end

  def zero?(_) do
    false
  end
end

IO.puts Math.zero?(0)
IO.puts Math.zero?(-1)
IO.puts Math.zero?(1)
