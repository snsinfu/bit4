use "assert"

actor Main
  new create(env: Env) =>
    let a1 = A(1)
    let a2 = A(2)
    a1.swap(a2)
    try
      Fact(a1.value() == 2)?
      Fact(a2.value() == 1)?
      env.out.print("OK")
    end

class A
  var _x: I64

  new create(x: I64) =>
    _x = x

  fun value(): I64 =>
    _x

  fun ref swap(other: A) =>
    _x = (other._x = _x)
