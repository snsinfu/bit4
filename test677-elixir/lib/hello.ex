defmodule Hello do
  def main(args) do
    {options, values} = args |> parse_args
    print_options(options)
    print_values(values)
  end

  defp print_options(options) do
    IO.puts("Options:")

    Enum.each(options, fn {key, val} ->
      IO.puts("--#{key} = #{val}")
    end)

    IO.puts("")
  end

  defp print_values(values) do
    IO.puts("Remaining args:")

    values
    |> Enum.with_index()
    |> Enum.each(fn {val, idx} ->
      IO.puts("#{idx} = #{val}")
    end)
  end

  def parse_args(args) do
    {options, values, []} =
      OptionParser.parse(args,
        strict: [def: :boolean]
      )

    {options, values}
  end
end
