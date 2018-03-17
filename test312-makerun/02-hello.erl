#!

main(Args) ->
    io:format("Hello, erlang~n"),
    showArgs(Args).

showArgs([]) ->
    ok;

showArgs([Arg | Rest]) ->
    io:format("Arg: ~s~n", [Arg]),
    showArgs(Rest).
