use "collections"
use "format"

actor Main
  """
  Print some Halton numbers.
  """
  new create(env: Env) =>
    try
      printHaltonNumbers(env, 2)?
    end

  fun printHaltonNumbers(env: Env, radix: U64) ? =>
    let halton = Halton(2)?
    for i in Range[U64](0, 10) do
      let num = halton(i)
      let str: String = Format.int[U64](i)
                      + ": "
                      + Format.float[F64](num)
      env.out.print(str)
    end

class Halton
  let radix: U64

  new create(radix': U64) ? =>
    """
    Creates a Halton sequence with given radix. Raises an error if radix' is zero or one.
    """
    if radix' <= 1 then
      error
    end
    radix = radix'

  fun apply(index: U64): F64 =>
    """
    Computes the index-th number of the Halton sequence. Returns zero if index is zero.
    """
    var result: F64 = 0
    var fraction: F64 = 1
    var i = index
    while i != 0 do
      fraction = fraction / radix.f64()
      result = result + (fraction * (i % radix).f64())
      i = i / radix
    end
    result
