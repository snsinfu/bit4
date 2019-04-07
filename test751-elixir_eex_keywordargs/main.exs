defmodule Foo do
  require EEx

  def foo(a: a, b: b) do
    a + b
  end

  EEx.function_from_string :def, :bar, "<%= a + b %>", [:a, :b]

  def baz(a: a, b: b) do
    unquote(EEx.compile_string("<%= a + b %>"))
  end
end

IO.puts Foo.foo(a: 20, b: 10)
IO.puts Foo.bar(20, 10)
IO.puts Foo.baz(a: 20, b: 10)
