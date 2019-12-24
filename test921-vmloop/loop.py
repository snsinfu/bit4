# Handle loop without AST.

from typing import Any, List, Tuple


def main():
    """
    repeat 5
      print "Hello, world!"
    next
    exit
    """
    program = [
        Repeat(IntegerExpression(5)),
        Print(StringExpression("Hello, world!")),
        Next(),
        Exit(),
    ]
    context = Context(program)

    while True:
        try:
            context.current_op.run(context)
            context.next()
        except TerminationSignal:
            break


class Context:
    def __init__(self, program: List["Operation"]):
        self._program = program
        self._counter = 0
        self._loop_stack: List[Tuple[int, int]] = []

    @property
    def current_op(self) -> "Operation":
        return self._program[self._counter]

    def next(self) -> None:
        self._counter += 1

    def jump(self, pc: int) -> None:
        self._counter = pc

    def push_loop(self, n: int, pc: int = None) -> None:
        if pc is None:
            pc = self._counter
        self._loop_stack.append((n, pc))

    def pop_loop(self) -> Tuple[int, int]:
        return self._loop_stack.pop()


class Expression:
    def eval(self, context: Context) -> Any:
        assert False


class IntegerExpression(Expression):
    def __init__(self, value: int):
        self._value = value

    def eval(self, context: Context) -> int:
        return self._value


class StringExpression(Expression):
    def __init__(self, value: str):
        self._value = value

    def eval(self, context: Context) -> str:
        return self._value


class Operation:
    def run(self, context: Context) -> None:
        assert False


class Repeat(Operation):
    def __init__(self, n: Expression):
        self._n = n

    def run(self, context: Context) -> None:
        n = self._n.eval(context)
        context.push_loop(n)


class Print(Operation):
    def __init__(self, msg: Expression):
        self._msg = msg

    def run(self, context: Context) -> None:
        print(self._msg.eval(context))


class Next(Operation):
    def run(self, context: Context) -> None:
        n, pc = context.pop_loop()
        n -= 1
        if n > 0:
            context.push_loop(n, pc)
            context.jump(pc)


class Exit(Operation):
    def run(self, context: Context) -> None:
        raise TerminationSignal("exit")


class TerminationSignal(Exception):
    pass


if __name__ == "__main__":
    main()
