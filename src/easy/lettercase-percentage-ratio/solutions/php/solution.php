#!/usr/bin/env php
<?php

$inputFile = new SplFileObject($argv[1]);

$inputFile->setFlags(SplFileObject::READ_AHEAD |
                     SplFileObject::SKIP_EMPTY |
                     SplFileObject::DROP_NEW_LINE);

foreach($inputFile as $line)
{
  $line = array_filter(str_split($line), 'ctype_alpha');

  $stats = [0, 0];

  foreach($line as $c) ++$stats[ctype_upper($c)];

  $stats = array_map(function($value) use($line)
  {
    return number_format($value / (count($line) * 0.01), 2);
  }, $stats);

  echo "lowercase: $stats[0] uppercase: $stats[1]", PHP_EOL;
}
