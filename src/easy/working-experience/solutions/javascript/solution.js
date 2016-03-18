#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

function getEpochOffset(monthYear, epochYear)
{
  let chunk = monthYear.split(' ');

  const month =
  {
    'jan': 1, 'feb':  2, 'mar':  3, 'apr':  4,
    'may': 5, 'jun':  6, 'jul':  7, 'aug':  8,
    'sep': 9, 'oct': 10, 'nov': 11, 'dec': 12
  }[chunk[0].toLowerCase()];

  const year = +chunk[1];

  return (year - epochYear) * 12 + month;
}

lineReader.on('line', (line) =>
{
  const epochYear = 1990;

  let dates = line.split('; ').map((range) =>
  {
    range = range.split('-');

    return [range[0], range[1]];
  });

  let experience = Array((2020 - epochYear) * 12).fill(false);

  dates.forEach((date) =>
  {
    const lhs = getEpochOffset(date[0], epochYear),
          rhs = getEpochOffset(date[1], epochYear);

    for(let i = lhs; i <= rhs; ++i)
    {
      experience[i] = true;
    }
  });

  console.log(Math.trunc(experience.filter(Boolean).length / 12));
});
