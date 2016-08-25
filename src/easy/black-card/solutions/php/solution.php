#!/usr/bin/env php
<?php

$inputFile = new SplFileObject($argv[1]);

$inputFile->setFlags(SplFileObject::READ_CSV   |
                     SplFileObject::READ_AHEAD |
                     SplFileObject::SKIP_EMPTY |
                     SplFileObject::DROP_NEW_LINE);

$inputFile->setCsvControl('|');

foreach($inputFile as $line)
{
  list($names, $target) = $line;

  $names = array_filter(explode(' ', $names), 'strlen');

  while(count($names) > 1)
  {
    array_splice($names, ($target % count($names) - 1), 1, []);
  }

  echo $names[0], PHP_EOL;
}
