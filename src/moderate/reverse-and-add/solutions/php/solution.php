#!/usr/bin/env php
<?php

$inputFile = new SplFileObject($argv[1]);

$inputFile->setFlags(SplFileObject::READ_AHEAD |
                     SplFileObject::SKIP_EMPTY |
                     SplFileObject::DROP_NEW_LINE);

foreach($inputFile as $n)
{
  $stepCount = 1;
  $m = $n + strrev($n);

  while($m != strrev($m))
  {
    ++$stepCount;
    $m += strrev($m);
  }

  echo "$stepCount $m", PHP_EOL;
}
