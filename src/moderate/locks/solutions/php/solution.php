#!/usr/bin/env php
<?php

$inputFile = new SplFileObject($argv[1]);

$inputFile->setFlags(SplFileObject::READ_CSV   |
                     SplFileObject::READ_AHEAD |
                     SplFileObject::SKIP_EMPTY |
                     SplFileObject::DROP_NEW_LINE);

$inputFile->setCsvControl(' ');

foreach($inputFile as $line)
{
  list($n, $m) = $line;

  $locks = array_fill(0, $n, false);

  for($i = 1; $i < $m; ++$i)
  {
    for($j = 1; $j < $n; $j += 2) $locks[$j] = true;
    for($j = 2; $j < $n; $j += 3) $locks[$j] = !$locks[$j];
  }

  $locks[$n - 1] = !$locks[$n - 1];

  echo count(array_filter($locks, function($lock)
  {
    return !$lock;
  })), PHP_EOL;
}
