#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

function isPrime(n)
{
  if(n <= 1)        return false;
  else if(n < 4)    return true;
  else if(!(n & 1)) return false;
  else if(n < 9)    return true;
  else if(!(n % 3)) return false;

  const limit = Math.trunc(Math.sqrt(n));

  for(let i = 5; i <= limit; i += 6)
  {
    if(!(n % i) || !(n % (i + 2))) return false;
  }

  return true;
}

function getShortestRepetitionLength(string)
{
  const length = string.length;

  if(Array(string.length).fill(string[0]).join('') === string) return 1;

  else if(isPrime(length)) return length;

  for(let i = 2; i <= (length >> 1); ++i)
  for(let j = i; j < length; j += i)
  {
    if(string.slice(0, i) === string.slice(i, (j << 1)))
    {
      return i;
    }
  }

  return length;
}

lineReader.on('line', line => console.log(getShortestRepetitionLength(line)));
