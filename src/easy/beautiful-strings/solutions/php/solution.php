#!/usr/bin/env php
<?php

$inputFile = new SplFileObject($argv[1]);

$inputFile->setFlags(SplFileObject::READ_AHEAD |
                     SplFileObject::SKIP_EMPTY |
                     SplFileObject::DROP_NEW_LINE);

foreach($inputFile as $line)
{
  $line = strtolower(preg_replace('/[^a-z]/i', '', $line));

  $frequencies = count_chars($line, 1);

  arsort($frequencies);

  $multiplier = count(range('a', 'z'));

  array_walk($frequencies, function(&$value) use(&$multiplier)
  {
    $value = ($multiplier-- * $value);
  });

  echo array_sum($frequencies), PHP_EOL;
}
