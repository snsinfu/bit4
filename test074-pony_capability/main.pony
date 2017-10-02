
actor Main
  new create(env: Env) =>
    ConsumeIso(make_string())
    ConsumeBox(make_string())
    IsoTrnRefBoxTag(make_string())
    IsoTrnValBoxTag(make_string())
    FinishingTrn(make_string())
    BorrowAndRecover(make_string())

  fun make_string(): String iso^ =>
    "The quick brown fox".clone()

//------------------------------------------------------------------------------

actor ConsumeIso
  new create(a: String iso) =>
    let b: String val = consume a // consumeu(iso) = iso^ => iso => val
//  let c: String val = consume a // Error: can't use a consumed local

//------------------------------------------------------------------------------

actor ConsumeBox
  new create(a: String iso) =>
    let b: String box = consume a // consume(iso) = iso^ => iso => box
    let c: String box^ = consume b // consume(box) = box^ => box
    let d: String box = c
    let e: String box^ = d

//------------------------------------------------------------------------------

actor IsoTrnRefBoxTag
  new create(a: String iso) =>
    let b: String trn = consume a // consume(iso!) = iso^ => iso => trn
    let c: String ref = consume b // consume(trn!) = trn^ => trn => ref
    let d: String box = c // ref! => ref => box
    let e: String tag = d // box! => box => tag

//------------------------------------------------------------------------------

actor IsoTrnValBoxTag
  new create(a: String iso) =>
    let b: String trn = consume a // consume(iso!) = iso^ => iso => trn
    let c: String val = consume b // consume(trn!) = trn^ => trn => val
    let d: String box = c // val! => val => box
    let e: String tag = d // box! => box => tag

//------------------------------------------------------------------------------

actor FinishingTrn
  new create(a: String iso) =>
    let b: String val = makeTrn(consume a) // trn^ => trn => val

  fun makeTrn(a: String iso): String trn^ =>
    let b: String trn = consume a // consume(iso!) => iso^ => iso => trn
    b

//------------------------------------------------------------------------------

actor BorrowAndRecover
  new create(a: String iso) =>
    let x: String iso = doit(consume a)

  fun doit(a: String iso): String iso^ =>
    recover
      let b: String ref = consume a
      let c: String ref = b
      b.append(" jumps")
      b.append(" over")
      c.append(" the lazy dog")
      c
    end

//------------------------------------------------------------------------------
//
// iso: Unique reader/writeer; no other reference
// trn: Unique writer, multiple reader
// ref: Multiple reader/writeer within actor
// val: Immutable
// box: Read-only; our actor may have writing reference
// tag: No read/write
//
// !: lvalue
// ^: rvalue
//
// consume: a! => a^
// recover: iso/trn/ref => any | val/box => val/box/tag
//
// Simple substitution:
//
//   iso => trn => ref => box => tag
//              => val => box => tag
//
// Alias substitution:
//
//   iso => iso! => tag
//   trn => trn! => box
//   ref -> ref! => ref
//   val -> val! => val
//   box -> box! => box
//   tag -> tag! => tag
//
// Ephemeral substitution:
//
//   iso^ => iso
//   trn^ => trn
//   ref^ == ref
//   val^ == val
//   box^ == box
//   tag^ == tag
//
// https://tutorial.ponylang.org/capabilities/reference-capabilities.html
// https://tutorial.ponylang.org/capabilities/guarantees.html
// https://tutorial.ponylang.org/capabilities/capability-subtyping.html
// https://tutorial.ponylang.org/capabilities/recovering-capabilities.html
