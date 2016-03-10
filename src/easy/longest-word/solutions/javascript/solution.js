#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

lineReader.on('line', (line) =>
{
  line = line.split(' ').reduce((a, b) =>
  {
    return (a.length >= b.length ? a : b);
  });

  console.log(line);
});
