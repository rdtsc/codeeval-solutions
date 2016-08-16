#!/usr/bin/env php
<?php

$inputFile = new SplFileObject($argv[1]);

$inputFile->setFlags(SplFileObject::READ_AHEAD |
                     SplFileObject::SKIP_EMPTY |
                     SplFileObject::DROP_NEW_LINE);

foreach($inputFile as $value)
{
  $n = str_split($value);
  $digitCount = count($n);

  $n = array_map(function($digit) use($digitCount)
  {
    return ($digit ** $digitCount);
  }, $n);

  echo (array_sum($n) == $value ? 'True' : 'False'), PHP_EOL;
}
