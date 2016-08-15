#!/usr/bin/env php
<?php

$inputFile = new SplFileObject($argv[1]);

$inputFile->setFlags(SplFileObject::READ_AHEAD |
                     SplFileObject::SKIP_EMPTY |
                     SplFileObject::DROP_NEW_LINE);

foreach($inputFile as $n)
{
  $result = 0;

  foreach([5, 3] as $denomination)
  {
    $result += (int)($n / $denomination);
    $n -= (int)($n / $denomination) * $denomination;
  }

  echo ($result + $n), PHP_EOL;
}
