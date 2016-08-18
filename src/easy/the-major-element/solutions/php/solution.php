#!/usr/bin/env php
<?php

$inputFile = new SplFileObject($argv[1]);

$inputFile->setFlags(SplFileObject::READ_CSV   |
                     SplFileObject::READ_AHEAD |
                     SplFileObject::SKIP_EMPTY |
                     SplFileObject::DROP_NEW_LINE);

foreach($inputFile as $values)
{
  $frequencies = array_count_values($values);

  $threshold = (count($values) >> 1);

  $frequencies = array_filter($frequencies, function($value) use($threshold)
  {
    return ($value > $threshold);
  });

  echo (empty($frequencies) ? 'None' : array_keys($frequencies)[0]), PHP_EOL;
}
