{$asmmode intel}

unit math;

interface
  function RepeatedSqrt(const x: Double; const n: LongInt): Double;

implementation

  function RepeatedSqrt(const x: Double; const n: LongInt): Double;
  assembler;
  label
    Loop, Done;
  asm
    test   edi, edi
    jz     Done
  Loop:
    sqrtsd xmm1, xmm0
    movsd  xmm0, xmm1
    dec    edi
    jnz    Loop
  Done:
  end;

end.
