'use strict';

module.exports = function join(xs, ys, compare) {
  if (compare === undefined) {
    compare = (x, y) => (x > y) - (x < y);
  }

  let joiner = {
    xs: xs,
    ys: ys,
    compare: compare
  };

  joiner.forEach = function(callback) {
    let xs = this.xs;
    let ys = this.ys;
    let compare = this.compare;

    while (xs.length && ys.length) {
      let x = xs[0];
      let y = ys[0];
      let order = compare(x, y);

      if (order < 0) {
        callback(x, undefined);
        xs = xs.slice(1);
      } else if (order > 0) {
        callback(undefined, y);
        ys = ys.slice(1);
      } else {
        callback(x, y);
        xs = xs.slice(1);
        ys = ys.slice(1);
      }
    }

    xs.forEach(x => callback(x, undefined));
    ys.forEach(y => callback(undefined, y));
  };

  return joiner;
};
