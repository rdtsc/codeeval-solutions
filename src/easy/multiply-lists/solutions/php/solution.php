#!/usr/bin/env php
<?php

$inputFile = new SplFileObject($argv[1]);

$inputFile->setFlags(SplFileObject::READ_AHEAD |
                     SplFileObject::SKIP_EMPTY |
                     SplFileObject::DROP_NEW_LINE);

foreach($inputFile as $line)
{
  $line = preg_replace('/\D/', ' ', $line);

  $line = array_filter(explode(' ', $line), function($item)
  {
    return is_numeric($item);
  });

  list($lhs, $rhs) = array_chunk($line, count($line) >> 1);

  $values = array_map(function($lhs, $rhs)
  {
    return ($lhs * $rhs);
  }, $lhs, $rhs);

  echo join(' ', $values), PHP_EOL;
}
