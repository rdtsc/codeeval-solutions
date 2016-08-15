#!/usr/bin/env php
<?php

$inputFile = new SplFileObject($argv[1]);

$inputFile->setFlags(SplFileObject::READ_AHEAD |
                     SplFileObject::SKIP_EMPTY |
                     SplFileObject::DROP_NEW_LINE);

foreach($inputFile as $line)
{
  $line = explode(',', str_replace(';', ',', $line));

  $width = array_pop($line);

  $line = array_chunk($line, $width);

  array_walk($line, function(&$entry) use($width)
  {
    if(count($entry) == $width)
    {
      $entry = array_reverse($entry);
    }
  });

  $line = call_user_func_array('array_merge', $line);

  echo join(',', $line), PHP_EOL;
}
