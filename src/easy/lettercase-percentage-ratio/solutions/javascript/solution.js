#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

lineReader.on('line', (line) =>
{
  line = line.split('');

  let lowerCase = 0;

  line.forEach(c => lowerCase += c.match(/[a-z]/) ? 1 : 0);

  let upperCase = line.length - lowerCase;

  lowerCase = (lowerCase / (line.length * 0.01)).toFixed(2);
  upperCase = (upperCase / (line.length * 0.01)).toFixed(2);

  console.log(`lowercase: ${lowerCase} uppercase: ${upperCase}`);
});
