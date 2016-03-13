#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

const tender =
[
  [1, "PENNY"], [5, "NICKEL"], [10, "DIME"], [25, "QUARTER"],
  [50, "HALF DOLLAR"], [100, "ONE"], [200, "TWO"], [500, "FIVE"],
  [1000, "TEN"], [2000, "TWENTY"], [5000, "FIFTY"], [10000, "ONE HUNDRED"]
];

function toCents(string)
{
  let result = 0;

  for(let i = 0; i < string.length; ++i) if(!isNaN(string[i]))
  {
    result = (result * 10) + (string.charCodeAt(i) - '0'.charCodeAt(0));
  }

  if(string.indexOf('.') === -1) result *= 100;

  return result;
}

lineReader.on('line', (line) =>
{
  line = line.split(';').map(toCents);

  const price = line[0],
        cash  = line[1];

  let change = cash - price;

  if(price > cash) console.log('ERROR');

  else if(price == cash) console.log('ZERO');

  else
  {
    for(let i = tender.length - 1; change > 0;)
    {
      if(i > 0 && tender[i][0] > change) --i;

      else
      {
        change -= tender[i][0];

        process.stdout.write(tender[i][1] + (change > 0 ? ',' : '\n'));
      }
    }
  }
});
