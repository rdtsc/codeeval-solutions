#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

lineReader.on('line', (line) =>
{
  line = line.split(' | ').map((n) =>
  {
    n = parseInt(n, 2);

    n ^= (n >> 1);
    n ^= (n >> 2);
    n ^= (n >> 4);

    return n;
  });

  console.log(line.join(' | '));
});
