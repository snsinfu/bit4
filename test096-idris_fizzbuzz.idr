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

makeFizzBuzzStr : Nat -> String
makeFizzBuzzStr n = cast (makeFizzBuzz n)

range : Nat -> Nat -> List Nat
range x y = if x == y then []
                      else x :: range (S x) y

printStrings : List String -> IO ()
printStrings Nil = pure ()
printStrings (x::xs) = do putStrLn x
                          printStrings xs

main : IO ()
main = printStrings (map makeFizzBuzzStr (range 1 100))
