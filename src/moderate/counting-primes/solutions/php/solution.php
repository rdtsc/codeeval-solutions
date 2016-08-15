#!/usr/bin/env php
<?php

$inputFile = new SplFileObject($argv[1]);

$inputFile->setFlags(SplFileObject::READ_CSV   |
                     SplFileObject::READ_AHEAD |
                     SplFileObject::SKIP_EMPTY |
                     SplFileObject::DROP_NEW_LINE);

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

foreach($inputFile as $line)
{
  list($n, $m) = $line;

  $tally = 0;

  for($i = $n; $i <= $m; ++$i)
  {
    $tally += isPrime($i);
  }

  echo $tally, PHP_EOL;
}
