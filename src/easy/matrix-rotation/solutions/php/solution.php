#!/usr/bin/env php
<?php

$inputFile = new SplFileObject($argv[1]);

$inputFile->setFlags(SplFileObject::READ_CSV   |
                     SplFileObject::READ_AHEAD |
                     SplFileObject::SKIP_EMPTY |
                     SplFileObject::DROP_NEW_LINE);

$inputFile->setCsvControl(' ');

foreach($inputFile as $values)
{
  $count = count($values);
  $width = sqrt($count);

  $rotated = [];

  for($i = 0; $i < ($width + 0); ++$i)
  for($j = 1; $j < ($width + 1); ++$j)
  {
    array_push($rotated, $values[$count + $i - $width * $j]);
  }

  echo join(' ', $rotated), PHP_EOL;
}
