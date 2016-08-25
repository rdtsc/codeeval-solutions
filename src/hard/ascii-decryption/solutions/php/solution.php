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
  $values = array_filter(explode(' ', $line[2]), function($chunk)
  {
    return is_numeric($chunk);
  });

  $encryptionOffset = (min($values) - ord(' '));

  array_walk($values, function(&$value) use($encryptionOffset)
  {
    $value = chr($value - $encryptionOffset);
  });

  echo join('', $values), PHP_EOL;
}
