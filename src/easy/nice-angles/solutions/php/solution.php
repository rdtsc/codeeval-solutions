#!/usr/bin/env php
<?php

$inputFile = new SplFileObject($argv[1]);

$inputFile->setFlags(SplFileObject::READ_AHEAD |
                     SplFileObject::SKIP_EMPTY |
                     SplFileObject::DROP_NEW_LINE);

foreach($inputFile as $angle)
{
  $deg = (int)$angle;
  $min = (int)(($angle - $deg) * 60);
  $sec = (int)((($angle - $deg) * 60 - $min) * 60);

  $min = str_pad($min, 2, '0', STR_PAD_LEFT);
  $sec = str_pad($sec, 2, '0', STR_PAD_LEFT);

  echo "$deg.$min'$sec\"", PHP_EOL;
}
