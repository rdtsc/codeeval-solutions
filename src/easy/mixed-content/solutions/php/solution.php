#!/usr/bin/env php
<?php

$inputFile = new SplFileObject($argv[1]);

$inputFile->setFlags(SplFileObject::READ_CSV   |
                     SplFileObject::READ_AHEAD |
                     SplFileObject::SKIP_EMPTY |
                     SplFileObject::DROP_NEW_LINE);

foreach($inputFile as $values)
{
  $words = $digits = [];

  foreach($values as $value)
  {
    is_numeric($value) ? $digits[] = $value : $words[] = $value;
  }

  $words  = join(',', $words);
  $digits = join(',', $digits);

  $result = array_filter([$words, $digits], function($chunk)
  {
    return !empty($chunk);
  });

  echo join('|', $result), PHP_EOL;
}
