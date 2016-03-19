#!/usr/bin/env node

'use strict';

const slurp = require('fs').readFileSync.bind(null, process.argv[2], 'ascii');

let track = slurp().split('\n').map(slice => slice.split(''));

let lastPos = track[0].indexOf('_');

track[0][lastPos] = '|';

for(let y = 1; y < track.length; ++y)
{
  let x = track[y].indexOf('C');

  if(x < 0) x = track[y].indexOf('_');

  if(lastPos === x) track[y][x] = '|';

  else track[y][x] = lastPos > x ? '/' : '\\';

  lastPos = x;
}

console.log(track.map(slice => slice.join('')).join('\n'));
