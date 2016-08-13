#!/usr/bin/env php
<?php

function isPrime($n)
{
  if($n <= 1)        return false;
  else if($n < 4)    return true;
  else if(!($n & 1)) return false;
  else if($n < 9)    return true;
  else if(!($n % 3)) return false;

  $limit = intval(sqrt($n));

  for($i = 5; $i <= $limit; $i += 6)
  {
    if(!($n % $i) || !($n % ($i + 2))) return false;
  }

  return true;
}

function isPalindrome($n)
{
  $reversed = 0;

  for($i = $n; $i; $i = floor($i / 10))
  {
    $reversed = ($reversed * 10) + ($i % 10);
  }

  return ($reversed === $n);
}

for($i = 997; $i > 0; $i -= 2)
{
  if(isPalindrome($i) && isPrime($i))
  {
    echo $i, PHP_EOL;
    break;
  }
}
