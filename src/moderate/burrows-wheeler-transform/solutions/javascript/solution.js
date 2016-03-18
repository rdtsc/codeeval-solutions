#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

lineReader.on('line', (line) =>
{
  let lhs = line.split('').slice(0, -1),
      rhs = lhs.slice();

  let mapping = [];
  let mappingIndex = 0;

  lhs.sort().forEach((c) =>
  {
    const row = rhs.indexOf(c);

    if(c === '$') mappingIndex = row;

    mapping.push(row);

    rhs[row] = '';
  });

  rhs = [];

  mapping.forEach((i) =>
  {
    mappingIndex = mapping[mappingIndex];

    rhs.push(line[mappingIndex]);
  });

  console.log(rhs.join(''));
});
