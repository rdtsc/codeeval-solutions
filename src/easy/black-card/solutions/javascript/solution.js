#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

lineReader.on('line', (line) =>
{
  line = line.split(' | ');

  let names = line[0].split(' ');
  const target = +line[1];

  while(names.length > 1)
  {
    names.splice(target % names.length - 1, 1);
  }

  console.log(names[0]);
});
