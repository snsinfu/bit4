'use strict';

const join = require('./join.js');

let xs = [1, 2, 3];
let ys = [2, 3, 4];

join(xs, ys).forEach((x, y) => {
  console.log(x, y);
});
