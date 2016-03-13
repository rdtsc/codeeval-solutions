#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

function getFrequencies(string)
{
  let results = {};

  for(let i = 0; i < string.length; ++i)
  {
    const c = string[i];

    results[c] = results[c] || 0;

    ++results[c];
  }

  return results;
}

function contains(string, fragments)
{
  const letters = getFrequencies(string);

  for(var c in fragments)
  {
    if(!(c in letters) || fragments[c] > letters[c])
    {
      return false;
    }
  }

  return true;
}

lineReader.on('line', (line) =>
{
  line = line.split(' | ');

  const fragments = getFrequencies(line[1]);

  const names = line[0].split(' ').filter(name => contains(name, fragments));

  console.log(names.join(' ') || 'False');
});
