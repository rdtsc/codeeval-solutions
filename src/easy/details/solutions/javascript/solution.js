#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

lineReader.on('line', (line) =>
{
  if(line.indexOf('XY') !== -1) return console.log(0);

  else
  {
    let minGap = Number.MAX_VALUE;

    line.split(',').forEach((row) =>
    {
      const gapWidth = row.split('.').length - 1;

      if(gapWidth < minGap) minGap = gapWidth;
    });

    console.log(minGap);
  }
});
