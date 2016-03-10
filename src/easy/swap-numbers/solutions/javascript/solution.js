#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

lineReader.on('line', (line) =>
{
  line = line.split(' ').map((chunk) =>
  {
    const head = chunk[0],
          body = chunk.substr(1).slice(0, -1),
          tail = chunk[chunk.length - 1];

    return `${tail}${body}${head}`;
  });

  console.log(line.join(' '));
});
