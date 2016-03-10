#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

lineReader.on('line', (line) =>
{
  if(line.length <= 55)
  {
    console.log(line);
    return;
  }

  line = line.substr(0, 40);

  const lastSpace = line.lastIndexOf(' ');

  if(lastSpace >= 0) line = line.substr(0, lastSpace);

  console.log('%s... <Read More>', line);
});
