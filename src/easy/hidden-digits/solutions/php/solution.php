#!/usr/bin/env php
<?php

$inputFile = new SplFileObject($argv[1]);

$inputFile->setFlags(SplFileObject::READ_AHEAD |
                     SplFileObject::SKIP_EMPTY |
                     SplFileObject::DROP_NEW_LINE);

foreach($inputFile as $line)
{
  $line = array_filter(str_split($line), function($c)
  {
    return preg_match('/[\da-j]/', $c);
  });

  $line = array_map(function($c)
  {
    return (is_numeric($c) ? $c : (ord($c) - ord('a')));
  }, $line);

  echo (empty($line) ? 'NONE' : join('', $line)), PHP_EOL;
}
