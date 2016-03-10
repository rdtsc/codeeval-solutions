#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

lineReader.on('line', (line) =>
{
  line = JSON.parse(line);

  let result = 0;

  line.menu.items.forEach((item) =>
  {
    if(item && item['label'])
    {
      result += item.id;
    }
  });

  console.log(result);
});
