#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

lineReader.on('line', (line) =>
{
  line = line.replace(/[^-\d\s]/g, '').split(' ').map(Number);

  const x0 = line[0], y0 = line[1],
        x1 = line[2], y1 = line[3];

  console.log(Math.hypot(x1 - x0, y1 - y0));
});
