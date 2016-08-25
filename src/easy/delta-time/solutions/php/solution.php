#!/usr/bin/env php
<?php

date_default_timezone_set('UTC');

$inputFile = new SplFileObject($argv[1]);

$inputFile->setFlags(SplFileObject::READ_CSV   |
                     SplFileObject::READ_AHEAD |
                     SplFileObject::SKIP_EMPTY |
                     SplFileObject::DROP_NEW_LINE);

$inputFile->setCsvControl(' ');

foreach($inputFile as $time)
{
  $time = array_map(function($timestamp)
  {
    return new DateTime($timestamp);
  }, $time);

  echo $time[0]->diff($time[1])->format('%H:%I:%S'), PHP_EOL;
}
