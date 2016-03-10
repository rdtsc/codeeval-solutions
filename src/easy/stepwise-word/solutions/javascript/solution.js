#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

lineReader.on('line', (line) =>
{
  line = line.split(' ').reduce((a, b) => a.length >= b.length ? a : b);

  line = line.split('').map((value, index) =>
  {
    const padding = (new Array(index + 1)).join('*');

    return padding + value;
  });

  console.log(line.join(' '));
});
