'use strict';

function partition(arr, key) {
  const parts = [];

  let i = 0;
  while (i < items.length) {
    const rest = items.slice(i);
    const pivot = key(items[i]);

    let len = rest.findIndex(item => key(item) !== pivot);
    if (len === -1) {
      len = rest.length;
    }

    const end = i + len;
    parts.push(items.slice(i, end));
    i = end;
  }

  return parts;
}

const items = [
  {time: new Date(Date.parse('2018-04-03T02:00:00Z'))},
  {time: new Date(Date.parse('2018-04-03T01:00:00Z'))},
  {time: new Date(Date.parse('2018-04-03T00:00:00Z'))},
  {time: new Date(Date.parse('2018-04-02T23:00:00Z'))},
  {time: new Date(Date.parse('2018-04-02T20:00:00Z'))},
  {time: new Date(Date.parse('2018-04-02T11:00:00Z'))},
  {time: new Date(Date.parse('2018-04-02T04:00:00Z'))},
  {time: new Date(Date.parse('2018-04-01T22:00:00Z'))},
  {time: new Date(Date.parse('2018-04-01T09:00:00Z'))},
  {time: new Date(Date.parse('2018-03-31T15:00:00Z'))},
  {time: new Date(Date.parse('2018-03-31T01:00:00Z'))},
  {time: new Date(Date.parse('2018-03-30T12:00:00Z'))},
  {time: new Date(Date.parse('2018-03-29T23:00:00Z'))},
  {time: new Date(Date.parse('2018-03-29T06:00:00Z'))}
];

function dateKey(item) {
  const t = item.time;
  return (t.getFullYear() * 12 + t.getMonth()) * 31 + t.getDay();
}
