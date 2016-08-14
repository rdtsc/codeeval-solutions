#!/usr/bin/env php
<?php

$inputFile = new SplFileObject($argv[1]);

$inputFile->setFlags(SplFileObject::READ_CSV   |
                     SplFileObject::READ_AHEAD |
                     SplFileObject::SKIP_EMPTY |
                     SplFileObject::DROP_NEW_LINE);

foreach($inputFile as $values)
{
  $maxSum = $localMax = $values[0];

  for($i = 1, $j = count($values); $i < $j; ++$i)
  {
    $localMax = max($values[$i], ($localMax + $values[$i]));
    $maxSum = max($maxSum, $localMax);
  }

  echo $maxSum, PHP_EOL;
}
