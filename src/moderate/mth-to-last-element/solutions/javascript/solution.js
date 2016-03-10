#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

lineReader.on('line', (line) =>
{
  line = line.split(' ');

  const index = line.length - line.pop() - 1;

  if(index >= 0) console.log(line[index]);
});
