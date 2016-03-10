#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

lineReader.on('line', (line) =>
{
  line = line.toLowerCase().replace(/[^a-z]/ig, '').split('');

  let frequencies = Array.apply(null, Array(26)).map(() => 0);

  line.forEach(c => ++frequencies[c.charCodeAt(0) - 'a'.charCodeAt(0)]);

  frequencies = frequencies.sort((a, b) => b - a);

  let multiplier = frequencies.length;

  frequencies = frequencies.map(n => multiplier-- * n);

  console.log(frequencies.reduce((a, b) => a + b));
});
