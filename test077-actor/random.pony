use "random"
use "time"

primitive Utility
  fun makeRandom(): Random =>
    (let seconds, let nanoseconds) = Time.now() // XXX: Bad seed
    let random = XorOshiro128Plus(seconds.u64(), nanoseconds.u64())
    discard(random, 10)
    random

  fun discard(random: Random ref, count: USize) =>
    var i: USize = 0
    while i < count do
      random.next()
      i = i + 1
    end
