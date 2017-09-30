use "format"

use @sin[F64](x: F64)
use @sinf[F32](x: F32)

actor Main
  new create(env: Env) =>
    try
      let x: F64 = 1.23
      let y: F64 = Math.sin(x) as F64
      env.out.print(Format.float[F64](y))
    end

primitive Math
  fun sin(x: F64): F64 => @sin(x)
  fun sin(x: F32): F32 => @sinf(x)
