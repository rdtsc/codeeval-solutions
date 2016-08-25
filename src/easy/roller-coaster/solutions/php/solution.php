#!/usr/bin/env php
<?php

$inputFile = new SplFileObject($argv[1]);

$inputFile->setFlags(SplFileObject::READ_AHEAD |
                     SplFileObject::SKIP_EMPTY |
                     SplFileObject::DROP_NEW_LINE);

foreach($inputFile as $line)
{
  for($i = 0, $j = 0, $length = strlen($line); $i < $length; ++$i)
  {
    if(ctype_alpha($line[$i]))
    {
      $line[$i] = (++$j & 1) ? strtoupper($line[$i]) :
                               strtolower($line[$i]);
    }
  }

  echo $line, PHP_EOL;
}
