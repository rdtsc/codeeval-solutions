#!/usr/bin/env php
<?php

$inputFile = new SplFileObject($argv[1]);

$inputFile->setFlags(SplFileObject::READ_AHEAD |
                     SplFileObject::SKIP_EMPTY |
                     SplFileObject::DROP_NEW_LINE);

foreach($inputFile as $line)
{
  $hitCount = count_chars($line, 1);

  foreach(str_split($line) as $c)
  {
    if($hitCount[ord($c)] === 1)
    {
      echo $c;
      break;
    }
  }

  echo PHP_EOL;
}
