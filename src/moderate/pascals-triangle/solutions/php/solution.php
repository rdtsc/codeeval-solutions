#!/usr/bin/env php
<?php

$inputFile = new SplFileObject($argv[1]);

$inputFile->setFlags(SplFileObject::READ_AHEAD |
                     SplFileObject::SKIP_EMPTY |
                     SplFileObject::DROP_NEW_LINE);

function nCk($n, $k)
{
  $f = function($n) use(&$f)
  {
    return (($n > 0) ? ($n * $f($n - 1)) : (1));
  };

  return (int)($f($n) / ($f($k) * $f($n - $k)));
}

foreach($inputFile as $n)
{
  for($d = 0; $d <  $n; ++$d)
  for($i = 0; $i <= $d; ++$i)
  {
    echo nCk($d, $i), ' ';
  }

  echo PHP_EOL;
}
