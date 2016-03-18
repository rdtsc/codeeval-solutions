#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

lineReader.on('line', (line) =>
{
  line = line.split(' ').map((value, index, values) =>
  {
    return index & 1 ? [] : [values[index], values[index + 1]];
  });

  let result = 0;

  line.filter(pair => pair.length).forEach((pair) =>
  {
    const invert = pair[0].length === 2;

    for(let i = 0; i < pair[1].length; ++i)
    {
      result *= 2;
      result += invert;
    }
  });

  console.log(result);
});
