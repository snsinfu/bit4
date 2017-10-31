module Main

data FizzBuzz : Nat -> Type where
  One : FizzBuzz 1
  S : (FizzBuzz n) -> (FizzBuzz (S n))

cast : (FizzBuzz n) -> String
cast {n = n} _ with (n `mod` 3, n `mod` 5)
  | (Z, Z) = "FizzBuzz"
  | (Z, _) = "Fizz"
  | (_, Z) = "Buzz"
  | (_, _) = cast (toIntNat n)

makeFizzBuzz : (n:Nat) -> FizzBuzz n
makeFizzBuzz (S Z) = One
makeFizzBuzz (S k) = S (makeFizzBuzz k)

main : IO ()
main = putStrLn (cast (makeFizzBuzz 15))
