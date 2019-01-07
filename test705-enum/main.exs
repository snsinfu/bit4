{:ok, content} = File.read("/usr/share/dict/words")

words =
  content
  |> String.split("\n", trim: true)
  |> Enum.map(&String.downcase/1)
  |> Enum.sort()
  |> Enum.uniq()

palindromes =
  Stream.zip(words, words |> Stream.map(&String.reverse/1))
  |> Stream.filter(fn {w1, w2} -> w1 == w2 end)
  |> Stream.map(fn {w1, _} -> w1 end)
  |> Stream.filter(fn w -> String.length(w) > 1 end)
  |> Enum.to_list()

IO.puts(palindromes |> Enum.join("\n"))
