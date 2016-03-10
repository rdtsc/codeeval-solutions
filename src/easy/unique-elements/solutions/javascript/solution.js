#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

lineReader.on('line', (line) =>
{
  line = line.split(',');

  line = line.filter((value, index) =>
  {
    return line.indexOf(value) === index;
  });

  console.log(line.join(','));
});
