#!/usr/bin/env php
<?php

$inputFile = new SplFileObject($argv[1]);

$inputFile->setFlags(SplFileObject::READ_AHEAD |
                     SplFileObject::SKIP_EMPTY |
                     SplFileObject::DROP_NEW_LINE);

foreach($inputFile as $line)
{
  $line = preg_replace('/[^\d\s]/', '', $line);

  $line = array_filter(explode(' ', $line), function($chunk)
  {
    return is_numeric($chunk);
  });

  list($v, $z, $w, $h) = array_values($line);

  $loot   = (($v * 3) + ($z * 4) + ($w * 5)) * $h;
  $shares = ($v + $z + $w);

  echo ((int)($loot / $shares)), PHP_EOL;
}
