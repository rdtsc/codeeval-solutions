#!/usr/bin/env php
<?php

$inputFile = new SplFileObject($argv[1]);

$inputFile->setFlags(SplFileObject::READ_CSV   |
                     SplFileObject::READ_AHEAD |
                     SplFileObject::SKIP_EMPTY |
                     SplFileObject::DROP_NEW_LINE);

$inputFile->setCsvControl(' ');

foreach($inputFile as $tokens)
{
  $values = [];

  foreach(array_reverse($tokens) as $token)
  {
    if(is_numeric($token))
    {
      array_push($values, $token);
      continue;
    }

    $x = array_pop($values);
    $y = array_pop($values);

    switch($token)
    {
      case '*': array_push($values, ($x * $y)); break;
      case '+': array_push($values, ($x + $y)); break;
      case '/': array_push($values, ($x / $y)); break;
      case '-': array_push($values, ($x - $y)); break;
    }
  }

  echo (int)$values[0], PHP_EOL;
}
