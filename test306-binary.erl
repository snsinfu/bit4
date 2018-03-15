#!/usr/bin/env escript

main(_) ->
    Code = 4#1230123,
    <<X:4, Y:4, Z:10, _:6>> = <<Code:24>>,
    ok = io:format("~p,~p,~p~n", [X, Y, Z]).
