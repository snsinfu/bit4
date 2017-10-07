// http://rerun.me/2014/09/11/introducing-actors-akka-notes-part-1/
// http://rerun.me/2014/09/19/akka-notes-actor-messaging-1/

use "random"

actor Main
  new create(env: Env) =>
    let teacher = Teacher(env.out)
    teacher.quoteRequest()

actor Teacher
  let _quotes: Array[String] = [
    "Moderation is for cowrds"
    "Anything worth doing is worth overdoing"
    "The trouble is you think you have time"
    "You never gonna know if you never even try"
  ]
  let _random: Random
  let _out: OutStream

  new create(out: OutStream) =>
    _random = Utility.makeRandom()
    _out = out

  be quoteRequest() =>
    let index = _random.int[USize](_quotes.size())
    try
      let response = _quotes(index)?
      _out.write(response + "\n")
    end
