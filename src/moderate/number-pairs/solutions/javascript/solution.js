#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

lineReader.on('line', (line) =>
{
  let terms = line.split(/\D/).map(Number);

  const targetSum = terms.pop(),
        termCount = terms.length;

  if(termCount >= 2 && terms[termCount - 1] + terms[termCount - 2] < targetSum)
  {
    console.log('NULL');
    return;
  }

  let pairs = [];

  for(let i =   0  ; i < termCount; ++i)
  for(let j = i + 1; j < termCount; ++j)
  {
    const pairSum = terms[i] + terms[j];

    if(pairSum > targetSum) break;

    else if(pairSum == targetSum)
    {
      pairs.push([terms[i], terms[j]]);
      break;
    }
  }

  pairs = pairs.map(pair => pair.join());

  console.log(pairs.length ? pairs.join(';') : 'NULL');
});
