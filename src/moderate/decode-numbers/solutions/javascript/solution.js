#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

const code = string => string.charCodeAt(0);

function getDecodingCount(string, index)
{
  if(string.length - index <= 1) return 1;

  const i = code(string[index + 0]),
        j = code(string[index + 1]);

  if((i === code('2') && j >= code('7')) || i >= code('3'))
  {
    if(i === code('2') && j >= code('7')) ++index;

    return getDecodingCount(string, index + 1)
  }

  const lhs = getDecodingCount(string, index + 1),
        rhs = getDecodingCount(string, index + 2);

  return (lhs + rhs);
}

lineReader.on('line', line => console.log(getDecodingCount(line, 0)));
