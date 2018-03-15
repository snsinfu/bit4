#!/usr/bin/env escript

main(_) ->
    L = [{1, 2}, {3, 4}, {5, 6}],
    K = [a, b, c],
    [X, Y, Z, W | Rest] = [{X + Y, Z} || {X, Y} <- L, Z <- K],
    io:format("XYZW: ~p,~p,~p,~p~n", [X, Y, Z, W]),
    io:format("Rest: ~p~n", [Rest]).
