#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

lineReader.on('line', (line) =>
{
  line = line.split(' ').map((word) =>
  {
    return word[0].toUpperCase() + word.slice(1);
  });

  console.log(line.join(' '));
});
