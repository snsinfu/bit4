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

  it('works with empty input', function() {
    let count = 0;
    join([], []).forEach(_ => count++);
    assert.equal(count, 0);
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
