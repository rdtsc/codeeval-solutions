#!/usr/bin/env node

'use strict';

function pad(n, width)
{
  const padding = (new Array(width + 1)).join(' ');

  return (padding + n).slice(-padding.length);
}

for(let y = 1; y <= 12; ++y)
{
  process.stdout.write(y.toString());

  for(let x = 2; x <= 12; ++x)
  {
    process.stdout.write(pad(x * y, 4));
  }

  process.stdout.write('\n');
}
