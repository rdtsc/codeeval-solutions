#!/usr/bin/env node

'use strict';

const lineReader = require('readline').createInterface(
{
  input: require('fs').createReadStream(process.argv[2])
});

function isPalindrome(n)
{
  let reversed = 0;

  for(let i = n; i; i = Math.floor(i / 10))
  {
    reversed = (reversed * 10) + (i % 10);
  }

  return (reversed == n);
}

const isInteresting = () =>
{
  const memoThreshold = 1000;

  let memo  = Array(memoThreshold).fill(false),
      cache = Array(memoThreshold).fill(0);

  return (lowerBound, upperBound) =>
  {
    let palindromeCount = 0;

    for(let i = lowerBound; i <= upperBound; ++i)
    {
      if(i < memoThreshold)
      {
        if(!memo[i])
        {
          memo[i] = true;
          cache[i] = isPalindrome(i);
        }

        palindromeCount += cache[i];
      }

      else palindromeCount += isPalindrome(i);
    }

    return !(palindromeCount & 1);
  };
}();

lineReader.on('line', (line) =>
{
  line = line.split(' ').map(Number);

  const lhs = line[0],
        rhs = line[1];

  let interestingRangeCount = 0;

  for(let i = lhs; i <= rhs; ++i)
  for(let j =  i ; j <= rhs; ++j)
  {
    interestingRangeCount += isInteresting(i, j);
  }

  console.log(interestingRangeCount);
});
