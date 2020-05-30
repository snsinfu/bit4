{$mode objfpc}{$H+}{$J-}

program main;

uses math in 'math.pas';

var
  x: Double;
  y: Double;
  n: LongInt;
begin
  x := 16;
  n := 3;
  y := RepeatedSqrt(x, n);
  WriteLn(x, y);
end.
