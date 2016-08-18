#!/usr/bin/env php
<?php

$inputFile = new SplFileObject($argv[1]);

$inputFile->setFlags(SplFileObject::READ_AHEAD |
                     SplFileObject::SKIP_EMPTY |
                     SplFileObject::DROP_NEW_LINE);

const MAPPING = 'yhesocvxduiglbkrztnwjpfmaq';

foreach($inputFile as $line)
{
  $line = array_map(function($c)
  {
    return ($c === ' ' ? $c : MAPPING[ord($c) - ord('a')]);
  }, str_split($line));

  echo join('', $line), PHP_EOL;
}
