#!/usr/bin/env php
<?php

$inputFile = new SplFileObject($argv[1]);

$inputFile->setFlags(SplFileObject::READ_AHEAD |
                     SplFileObject::SKIP_EMPTY |
                     SplFileObject::DROP_NEW_LINE);

foreach($inputFile as $line)
{
  $values = explode(' ', preg_replace('/[^\d ]/', '', $line));

  sort($values);

  $distances = [$values[0]];

  for($i = 1, $limit = count($values); $i < $limit; ++$i)
  {
    $distances[] = ($values[$i] - $values[$i - 1]);
  }

  echo join(',', $distances), PHP_EOL;
}
