#!/usr/bin/env php
<?php

$inputFile = new SplFileObject($argv[1]);

$inputFile->setFlags(SplFileObject::READ_CSV   |
                     SplFileObject::READ_AHEAD |
                     SplFileObject::SKIP_EMPTY |
                     SplFileObject::DROP_NEW_LINE);

$inputFile->setCsvControl(' ');

foreach($inputFile as $names)
{
  $pattern = array_shift($names);

  $pattern = str_replace('.', '\.',  $pattern);
  $pattern = str_replace('?', '.',   $pattern);
  $pattern = str_replace('*', '.*?', $pattern);

  $names = array_filter($names, function($name) use($pattern)
  {
    return preg_match("/^$pattern$/", $name) === 1;
  });

  $names = join(' ', $names);

  echo ($names ? $names : '-'), PHP_EOL;
}
