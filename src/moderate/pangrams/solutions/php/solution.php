#!/usr/bin/env php
<?php

$inputFile = new SplFileObject($argv[1]);

$inputFile->setFlags(SplFileObject::READ_AHEAD |
                     SplFileObject::SKIP_EMPTY |
                     SplFileObject::DROP_NEW_LINE);

$filter = range('a', 'z');

foreach($inputFile as $line)
{
  $line = strtolower($line);

  $matches = array_intersect($filter, str_split(count_chars($line, 4)));

  echo (empty($matches) ? 'NULL' : join('', $matches)), PHP_EOL;
}
