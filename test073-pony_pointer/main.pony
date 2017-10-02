use "format"

actor Main
  new create(env: Env) =>
    let a = recover val [as I32: 1; 2; 3; 4; 5] end
    let p = a.cpointer()
    let q = a.trim(a.size() - 1, a.size()).cpointer()
    env.out.print(Format.int[USize](p.usize(), FormatHexBare))
    env.out.print(Format.int[USize](q.usize(), FormatHexBare))
