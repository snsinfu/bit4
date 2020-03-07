"use strict";

export function outerJoin(xs, ys, cmp) {
  if (cmp === undefined) {
    cmp = (x, y) => (x > y) - (x < y);
  }

  const pairs = [];

  while (xs.length && ys.length) {
    let x = xs[0];
    let y = ys[0];
    let order = cmp(x, y);

    if (order < 0) {
      pairs.push([x, undefined]);
      xs = xs.slice(1);
    } else if (order > 0) {
      pairs.push([undefined, y]);
      ys = ys.slice(1);
    } else {
      pairs.push([x, y]);
      xs = xs.slice(1);
      ys = ys.slice(1);
    }
  }

  xs.forEach(x => pairs.push([x, undefined]));
  ys.forEach(y => pairs.push([undefined, y]));

  return pairs;
}
