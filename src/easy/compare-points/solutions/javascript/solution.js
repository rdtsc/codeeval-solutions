#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

lineReader.on('line', (line) =>
{
  line = line.split(' ').map(Number);

  const x0 = line[0], y0 = line[1],
        x1 = line[2], y1 = line[3];

  let bearing = [];

  if(y0 != y1) bearing.push(y0 > y1 ? 'S' : 'N');
  if(x0 != x1) bearing.push(x0 > x1 ? 'W' : 'E');

  console.log(bearing.join('') || 'here');
});
