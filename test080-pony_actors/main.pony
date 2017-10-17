actor Main
  new create(env: Env) =>
    Driver(env).run()

actor Driver
  let _env: Env

  new create(env: Env) =>
    _env = env

  be run() =>
    Fibonacci.compute(this, 50)
    Fibonacci.compute(this, 40)
    Fibonacci.compute(this, 30)
    Fibonacci.compute(this, 20)
    Fibonacci.compute(this, 10)

  be done(n: U64, fib: U64) =>
    _env.out.print(n.string() + " -> " + fib.string())

interface tag Receiver
  be done(n: U64, fib: U64)

actor Fibonacci
  be compute(recv: Receiver, n: U64) =>
    recv.done(n, fibonacci(n))

  fun fibonacci(n: U64): U64 =>
    match n
    | 0 => 1
    | 1 => 1
    else
      fibonacci(n - 1) + fibonacci(n - 2)
    end
