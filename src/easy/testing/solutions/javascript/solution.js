#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

function getSeverity(n)
{
  if(n == 0) return 'Done';
  if(n <= 2) return 'Low';
  if(n <= 4) return 'Medium';
  if(n <= 6) return 'High';

  return 'Critical';
}

lineReader.on('line', (line) =>
{
  line = line.split(' | ');

  const lhs = line[0],
        rhs = line[1];

  let errorCount = 0;

  for(let i = 0; i < lhs.length; ++i)
  {
    if(lhs[i] != rhs[i]) ++errorCount;
  }

  console.log(getSeverity(errorCount));
});
