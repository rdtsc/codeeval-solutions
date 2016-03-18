#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

lineReader.on('line', (line) =>
{
  let actions = line.split(' ');

  let lo = 0,
      hi = +actions.shift();

  let guess = 0;

  actions.forEach((action) =>
  {
    const delta = hi - lo;

    guess = lo + (delta & 1) + (delta >> 1);

    switch(action[0])
    {
      case 'H': lo = guess + 1; break;
      case 'L': hi = guess - 1; break;
    }
  });

  console.log(guess);
});
