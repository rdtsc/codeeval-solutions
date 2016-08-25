#!/usr/bin/env php
<?php

$inputFile = new SplFileObject($argv[1]);

$inputFile->setFlags(SplFileObject::READ_AHEAD |
                     SplFileObject::SKIP_EMPTY |
                     SplFileObject::DROP_NEW_LINE);

foreach($inputFile as $line)
{
  if(strlen($line) <= 55)
  {
    echo $line, PHP_EOL;
    continue;
  }

  $line = substr($line, 0, 40);
  $lastSpace = strrpos($line, ' ');

  if($lastSpace !== false) $line = substr($line, 0, $lastSpace);

  echo "$line... <Read More>", PHP_EOL;
}
