#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

function isJolly(terms)
{
  const termCount = terms.length;

  if(termCount == 1) return true;

  let used = Array(termCount).fill(false);

  for(let i = 0; i < termCount - 1; ++i)
  {
    let delta = Math.abs(terms[i] - terms[i + 1]);

    if(!delta || delta >= termCount || used[delta - 1])
    {
      return false;
    }

    used[delta - 1] = true;
  }

  return true;
}

lineReader.on('line', (line) =>
{
  line = line.split(' ').map(Number).slice(1);

  console.log(isJolly(line) ? 'Jolly' : 'Not jolly');
});
