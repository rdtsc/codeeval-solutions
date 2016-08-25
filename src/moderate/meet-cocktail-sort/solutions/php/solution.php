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
  list($values, $iterations) = $line;

  $values = array_filter(explode(' ', $values), 'strlen');

  $valueCount = count($values);

  for($i = 0; $i < $iterations; ++$i)
  {
    for($j = 0; $j < ($valueCount - 1); ++$j)
    {
      if($values[$j] > $values[$j + 1])
      {
        list($values[$j], $values[$j + 1]) = [$values[$j + 1], $values[$j]];
      }
    }

    for($j = ($valueCount - 1); $j > 0; --$j)
    {
      if($values[$j] < $values[$j - 1])
      {
        list($values[$j], $values[$j - 1]) = [$values[$j - 1], $values[$j]];
      }
    }
  }

  echo join(' ', $values), PHP_EOL;
}
