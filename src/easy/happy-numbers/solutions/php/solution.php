#!/usr/bin/env php
<?php

$inputFile = new SplFileObject($argv[1]);

$inputFile->setFlags(SplFileObject::READ_AHEAD |
                     SplFileObject::SKIP_EMPTY |
                     SplFileObject::DROP_NEW_LINE);

function reduce($n)
{
  $result = 0;

  while($n)
  {
    $result += ($n % 10) ** 2;
    $n = (int)($n / 10);
  }

  return $result;
}

function isHappy($n)
{
  $chain = [];

  for($n = reduce($n); $n !== 1; $n = reduce($n))
  {
    if(array_key_exists($n, $chain)) return false;

    $chain[$n] = true;
  }

  return true;
}

foreach($inputFile as $n)
{
  echo (isHappy($n) ? 1 : 0), PHP_EOL;
}
