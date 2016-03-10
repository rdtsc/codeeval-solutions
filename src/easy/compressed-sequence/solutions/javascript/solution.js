#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

lineReader.on('line', (line) =>
{
  line = line.split(' ');

  let itemCount = 1;

  for(let i = 1; i < line.length; ++i)
  {
    if(line[i] === line[i - 1]) ++itemCount;

    else
    {
      process.stdout.write(`${itemCount} ${line[i - 1]} `);
      itemCount = 1;
    }
  }

  console.log(`${itemCount} ${line[line.length - 1]}`);
});
