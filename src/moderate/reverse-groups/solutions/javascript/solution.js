#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

lineReader.on('line', (line) =>
{
  line = line.split(/\D/);

  const width = +line.pop();

  let groups = line.map((value, index) =>
  {
    if(index % width) return false;

    let result = line.slice(index, index + width);

    if(result.length === width) result.reverse();

    return result.join(',');
  }).filter(Boolean);

  console.log(groups.join(','));
});
