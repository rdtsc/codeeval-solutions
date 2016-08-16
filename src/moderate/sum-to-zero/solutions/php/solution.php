#!/usr/bin/env php
<?php

$inputFile = new SplFileObject($argv[1]);

$inputFile->setFlags(SplFileObject::READ_CSV   |
                     SplFileObject::READ_AHEAD |
                     SplFileObject::SKIP_EMPTY |
                     SplFileObject::DROP_NEW_LINE);

foreach($inputFile as $terms)
{
  $termCount = count($terms);

  $result = 0;

  for($a = (   0  ); $a < ($termCount - 3); ++$a)
  for($b = ($a + 1); $b < ($termCount - 2); ++$b)
  for($c = ($b + 1); $c < ($termCount - 1); ++$c)
  for($d = ($c + 1); $d < ($termCount - 0); ++$d)
  {
    $result += !($terms[$a] + $terms[$b] + $terms[$c] + $terms[$d]);
  }

  echo $result, PHP_EOL;
}
