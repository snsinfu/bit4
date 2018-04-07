const assert = require('assert');
const join = require('../join.js')

describe('join', function() {
  it('aligns equivalent values', function() {
    let xs = [2, 3, 5];
    let ys = [1, 2, 3, 4];
    let track = [];

    join(xs, ys).forEach((x, y) => track.push([x, y]));

    assert.deepEqual(track, [
      [undefined, 1],
      [2, 2],
      [3, 3],
      [undefined, 4],
      [5, undefined]
    ]);
  });

  it('does nothing when both input is empty', function() {
    let count = 0;

    join([], []).forEach(_ => count++);

    assert.equal(count, 0);
  });

  it('outputs the left array if the right one is empty', function() {
    let input = [2, 3, 5];
    let track = [];

    join(input, []).forEach((x, y) => track.push([x, y]));

    assert.deepEqual(track, [
      [2, undefined],
      [3, undefined],
      [5, undefined]
    ]);
  });

  it('outputs the right array if the left one is empty', function() {
    let input = [2, 3, 5];
    let track = [];

    join([], input).forEach((x, y) => track.push([x, y]));

    assert.deepEqual(track, [
      [undefined, 2],
      [undefined, 3],
      [undefined, 5]
    ]);
  });

  it('works with strings', function() {
    let xs = ['bcd', 'cde', 'efg'];
    let ys = ['abc', 'bcd', 'cde', 'def'];
    let track = [];

    join(xs, ys).forEach((x, y) => track.push([x, y]));

    assert.deepEqual(track, [
      [undefined, 'abc'],
      ['bcd', 'bcd'],
      ['cde', 'cde'],
      [undefined, 'def'],
      ['efg', undefined]
    ]);
  });

  it('can emulate left join', function() {
    let xs = [2, 3, 5];
    let ys = [1, 2, 3, 4];
    let left = [];

    join(xs, ys).forEach((x, y) => {
      if (y === undefined) {
        left.push(x);
      }
    });

    assert.deepEqual(left, [5]);
  });

  it('can emulate right join', function() {
    let xs = [2, 3, 5];
    let ys = [1, 2, 3, 4];
    let right = [];

    join(xs, ys).forEach((x, y) => {
      if (x === undefined) {
        right.push(y);
      }
    });

    assert.deepEqual(right, [1, 4]);
  });

  it('can emulate inner join', function() {
    let xs = [2, 3, 5];
    let ys = [1, 2, 3, 4];
    let inner = [];

    join(xs, ys).forEach((x, y) => {
      if (x !== undefined && y !== undefined) {
        inner.push(x);
      }
    });

    assert.deepEqual(inner, [2, 3]);
  });
});
