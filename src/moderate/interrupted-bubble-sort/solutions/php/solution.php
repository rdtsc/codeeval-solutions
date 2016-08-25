#!/usr/bin/env php
<?php

$inputFile = new SplFileObject($argv[1]);

$inputFile->setFlags(SplFileObject::READ_AHEAD |
                     SplFileObject::SKIP_EMPTY |
                     SplFileObject::DROP_NEW_LINE);

foreach($inputFile as $line)
{
  $values = array_filter(preg_split('/\D/', $line), function($chunk)
  {
    return strlen($chunk);
  });

  $iterations = array_pop($values);

  $count = count($values);

  for($i = 0, $limit = min($count, $iterations); $i < $limit; ++$i)
  {
    for($j = 0; $j < ($count - 1); ++$j)
    {
      if($values[$j] > $values[$j + 1])
      {
        list($values[$j], $values[$j + 1]) = [$values[$j + 1], $values[$j]];
      }
    }
  }

  echo join(' ', $values), PHP_EOL;
}
