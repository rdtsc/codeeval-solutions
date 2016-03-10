#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

lineReader.on('line', (line) =>
{
  line = line.replace(/[^\d\s.-]/g, '')
             .split(' ')
             .filter(chunk => chunk.length)
             .map(Number);

  const cx = line[0],
        cy = line[1],
        r  = line[2],
        px = line[3],
        py = line[4];

  const distance = Math.hypot(px - cx, py - cy);

  console.log(distance < r);
});
