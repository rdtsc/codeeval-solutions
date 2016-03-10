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

function isPalindrome(n)
{
  let reversed = 0;

  for(let i = n; i; i = Math.floor(i / 10))
  {
    reversed = (reversed * 10) + (i % 10);
  }

  return (reversed == n);
}

for(let i = 997; i > 0; i -= 2)
{
  if(isPalindrome(i) && isPrime(i))
  {
    console.log(i);
    break;
  }
}
