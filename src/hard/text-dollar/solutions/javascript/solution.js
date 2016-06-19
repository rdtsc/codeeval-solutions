#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

function getDigitGroups(value)
{
  const result = [];

  for(let n = value; n; n = ~~(n / 1000))
  {
    result.push(n % 1000);
  }

  return result.reverse();
}

function groupToEnglish(value)
{
  const alphabet =
  [
    'Zero', 'One', 'Two', 'Three', 'Four',
    'Five', 'Six', 'Seven', 'Eight', 'Nine',

    'Ten', 'Eleven', 'Twelve', 'Thirteen', 'Fourteen',
    'Fifteen', 'Sixteen', 'Seventeen', 'Eighteen', 'Nineteen',

    'Twenty', 'Thirty', 'Forty', 'Fifty',
    'Sixty', 'Seventy', 'Eighty', 'Ninety'
  ];

  const result = [];

  const a = ~~(value / 100), bc = (value % 100);

  if(a) result.push(alphabet[a] + 'Hundred');

  if(bc && (bc <= 19)) result.push(alphabet[bc]);

  else if(bc >= 20)
  {
    result.push(alphabet[18 + ~~(bc / 10)]);

    const c = (bc % 10);

    if(c) result.push(alphabet[c]);
  }

  return result.join('');
}

function toEnglish(value)
{
  const result = [],
        bases  = ['', 'Thousand', 'Million', 'Billion'];

  let baseIndex = ~~(Math.log10(value) / 3);

  getDigitGroups(value).forEach((group) =>
  {
    result.push(groupToEnglish(group));

    if(group) result.push(bases[baseIndex]);

    --baseIndex;
  });

  return result.join('') + 'Dollars';
}

lineReader.on('line', value => console.log(toEnglish(+value)));
