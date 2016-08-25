#!/usr/bin/env php
<?php

date_default_timezone_set('UTC');

$inputFile = new SplFileObject($argv[1]);

$inputFile->setFlags(SplFileObject::READ_CSV   |
                     SplFileObject::READ_AHEAD |
                     SplFileObject::SKIP_EMPTY |
                     SplFileObject::DROP_NEW_LINE);

$inputFile->setCsvControl(' ');

foreach($inputFile as $times)
{
  usort($times, function($lhs, $rhs)
  {
    return (strtotime($rhs) - strtotime($lhs));
  });

  echo join(' ', $times), PHP_EOL;
}
