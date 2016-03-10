#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

lineReader.on('line', (line) =>
{
  line = line.split(' ');

  let lhs = line[0].split('');
  const rhs = line[1];

  lhs = lhs.map((value, index) =>
  {
    return rhs[index] === '1' ? value.toUpperCase() : value;
  });

  console.log(lhs.join(''));
});
