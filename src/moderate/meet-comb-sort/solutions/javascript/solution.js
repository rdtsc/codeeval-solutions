#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

lineReader.on('line', (line) =>
{
  let values = line.split(' ').map(Number),
      result = 0;

  for(let margin = values.length;;)
  {
    let traded = false;

    margin = Math.max(Math.trunc(margin / 1.25), 1);

    for(let i = 0; i < values.length - margin; ++i)
    {
      let j = i + margin;

      if(values[i] > values[j])
      {
        const scratch = values[i];

        values[i] = values[j];
        values[j] = scratch;

        traded = true;
      }
    }

    if(!traded && margin === 1) break;

    ++result;
  }

  console.log(result);
});
