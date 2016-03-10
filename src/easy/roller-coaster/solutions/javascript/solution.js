#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

lineReader.on('line', (line) =>
{
  let index = 0;

  line = line.split('').map((c) =>
  {
    if(c.match(/[a-z]/i))
    {
      if(++index & 1) return c.toUpperCase();

      return c.toLowerCase();
    }

    return c;
  });

  console.log(line.join(''));
});
