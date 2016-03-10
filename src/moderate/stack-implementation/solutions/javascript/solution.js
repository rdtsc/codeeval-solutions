#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

lineReader.on('line', (line) =>
{
  for(let stack = line.split(' '); stack.length; stack.pop())
  {
    process.stdout.write(stack.pop() + ' ');
  }

  process.stdout.write('\n');
});
