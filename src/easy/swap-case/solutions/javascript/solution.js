#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

lineReader.on('line', (line) =>
{
  line = line.split('').map((c) =>
  {
    if(c === c.toLowerCase()) return c.toUpperCase();

    return c.toLowerCase();
  });

  console.log(line.join(''));
});
