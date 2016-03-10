#!/usr/bin/env node

'use strict';

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

let primeSum = 5;

for(let i = 5, primeCount = 2; primeCount < 1000; ++i)
{
  if(isPrime(i))
  {
    ++primeCount;
    primeSum += i;
  }
}

console.log(primeSum);
