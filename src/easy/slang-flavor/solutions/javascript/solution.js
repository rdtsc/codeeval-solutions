#!/usr/bin/env node

'use strict';

const slurp = require('fs').readFileSync.bind(null, process.argv[2], 'ascii');

const terms =
[
  ', yeah!',
  ', this is crazy, I tell ya.',
  ', can U believe this?',
  ', eh?',
  ', aw yea.',
  ', yo.',
  '? No way!',
  '. Awesome!'
];

let index = 0;
let injectSlang = false;
let lines = slurp().split('\n');

lines.forEach((line) =>
{
  line = line.split('').map((c) =>
  {
    if(c !== '.' && c !== '!' && c !== '?') return c;

    injectSlang = !injectSlang;

    return injectSlang ? c : terms[index++ % terms.length];
  });

  console.log(line.join(''));
});
