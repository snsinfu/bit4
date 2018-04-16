'use strict';

const moment = require('moment');

function main() {
  const EXAMPLE_DATETIME = '2018-04-16T05:45:22.210Z';
  const TIMEZONE = '+01:23';
  const DELTA = moment.duration(12, 'hours');

  let time = moment(EXAMPLE_DATETIME).utcOffset(TIMEZONE);
  let past = moment(time).subtract(DELTA);

  console.log('time =', time.toISOString(true));
  console.log('past =', past.toISOString(true));
  console.log();
  console.log('dateCode(time) =', dateCode(time));
  console.log('dateCode(past) =', dateCode(past));
}

function dateCode(date) {
  return (date.year() * 12 + date.month()) * 31 + date.date();
}

main();
