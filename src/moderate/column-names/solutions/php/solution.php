#!/usr/bin/env php
<?php

$inputFile = new SplFileObject($argv[1]);

$inputFile->setFlags(SplFileObject::READ_AHEAD |
                     SplFileObject::SKIP_EMPTY |
                     SplFileObject::DROP_NEW_LINE);

foreach($inputFile as $n)
{
  $result = [];

  for($i = $n; $i > 0; $i = (int)($i / 26))
  {
    array_unshift($result, chr(ord('A') + (--$i % 26)));
  }

  echo join('', $result), PHP_EOL;
}
