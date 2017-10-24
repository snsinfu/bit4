use "time"

actor Main
  new create(env: Env) =>
    let timers = Timers
    timers(Timer(ClockworkPrime(PrimePrinter(env), 100000), 0, 1_000_000))
    timers(Timer(ClockworkPrime(PrimePrinter(env), 200000), 0, 1_000_000))
    timers(Timer(ClockworkPrime(PrimePrinter(env), 50000), 0, 1_000_000))

class PrimePrinter
  let _env: Env

  new iso create(env: Env) =>
    _env = env

  fun ref notify(n: U64, value: U64) =>
    _env.out.print(n.string() + " => " + value.string())

class ClockworkPrime is TimerNotify
  let _printer: PrimePrinter
  let _request: U64
  let _unitTaskCount: U32 = 1000
  var _counter: U64 = 0
  var _primeCount: U64 = 0

  new iso create(printer: PrimePrinter iso, n: U64) =>
    _request = n
    _printer = consume printer

  fun ref apply(timer: Timer, count: U64): Bool =>
    var taskCount: U32 = _unitTaskCount
    while taskCount > 0 do
      if not _proceed() then
        return false
      end
    end
    true

  fun ref _proceed(): Bool =>
    if _counter == _request then
      _printer.notify(_request, _primeCount)
      return false
    end

    var isPrime: Bool = true
    var divisor: U64 = 2
    while divisor < _counter do
      if (_counter % divisor) == 0 then
        isPrime = false
        break
      end
      divisor = divisor + 1
    end

    if isPrime then
      _primeCount = _primeCount + 1
    end

    _counter = _counter + 1
    true
