module Main

data FizzBuzz : Nat -> Type where
  One : FizzBuzz 1
  S : (FizzBuzz n) -> (FizzBuzz (S n))

cast : (FizzBuzz n) -> String
cast {n = n} _ with (n `mod` 3, n `mod` 5)
  | (Z, _) = "Fizz"
  | (_, Z) = "Buzz"
  | (Z, Z) = "FizzBuzz"
  | (_, _) = cast (toIntNat n)

main : IO ()
main = putStrLn (cast (S (S One)))
