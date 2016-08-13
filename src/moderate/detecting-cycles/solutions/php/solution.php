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
  $history = $duplicates = [];

  foreach($values as $value)
  {
    if(!array_key_exists($value, $history))
    {
      $history[$value] = true;
    }

    else if(!in_array($value, $duplicates))
    {
      array_push($duplicates, $value);
    }
  }

  echo join(' ', $duplicates), PHP_EOL;
}
