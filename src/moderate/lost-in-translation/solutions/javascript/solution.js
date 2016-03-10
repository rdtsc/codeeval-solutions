#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

const mapping = 'yhesocvxduiglbkrztnwjpfmaq';

lineReader.on('line', (line) =>
{
  line = line.split('').map((c) =>
  {
    return c === ' ' ? c : mapping[c.charCodeAt(0) - 'a'.charCodeAt(0)];
  });

  console.log(line.join(''));
});
