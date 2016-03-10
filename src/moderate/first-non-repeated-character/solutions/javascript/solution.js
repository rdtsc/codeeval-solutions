#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

lineReader.on('line', (line) =>
{
  line = line.split('');

  let hitCount = {};

  line.forEach((c) =>
  {
    hitCount[c] = hitCount[c] || 0;

    ++hitCount[c];
  });

  for(let i = 0; i < line.length; ++i)
  {
    if(hitCount[line[i]] === 1)
    {
      console.log(line[i]);
      break;
    }
  }
});
