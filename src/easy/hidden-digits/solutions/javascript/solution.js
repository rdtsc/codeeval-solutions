#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

lineReader.on('line', (line) =>
{
  line = line.split('').filter((c) =>
  {
    return c.match(/[\da-j]/);
  });

  line = line.map((c) =>
  {
    if(!isNaN(c)) return c;

    return c.charCodeAt(0) - 'a'.charCodeAt(0);
  });

  console.log(line.join('') || 'NONE');
});
