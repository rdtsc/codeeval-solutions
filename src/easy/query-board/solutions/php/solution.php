#!/usr/bin/env php
<?php

$inputFile = new SplFileObject($argv[1]);

$inputFile->setFlags(SplFileObject::READ_CSV   |
                     SplFileObject::READ_AHEAD |
                     SplFileObject::SKIP_EMPTY |
                     SplFileObject::DROP_NEW_LINE);

$inputFile->setCsvControl(' ');

const BOARD_WIDTH = 256;

$board = array_fill(0, BOARD_WIDTH, array_fill(0, BOARD_WIDTH, 0));

foreach($inputFile as $line)
{
  if(strpos($line[0], 'Query') === 0)
  {
    list($command, $index) = $line;

    if($command === 'QueryRow')
    {
      echo array_sum($board[$index]), PHP_EOL;
    }

    else if($command === 'QueryCol')
    {
      echo array_sum(array_column($board, (int)$index)), PHP_EOL;
    }
  }

  else if(strpos($line[0], 'Set') === 0)
  {
    list($command, $index, $value) = $line;

    if($command === 'SetRow')
    {
      $board[$index] = array_fill(0, BOARD_WIDTH, $value);
    }

    else if($command === 'SetCol')
    {
      for($i = 0; $i < BOARD_WIDTH; ++$i)
      {
        $board[$i][$index] = $value;
      }
    }
  }
}
