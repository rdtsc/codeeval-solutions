#!/usr/bin/env php
<?php

$inputFile = new SplFileObject($argv[1]);

$inputFile->setFlags(SplFileObject::READ_CSV   |
                     SplFileObject::READ_AHEAD |
                     SplFileObject::SKIP_EMPTY |
                     SplFileObject::DROP_NEW_LINE);

$inputFile->setCsvControl(' ');

const MAPPING =
[
  '.-'    => 'A', '-...' => 'B', '-.-.' => 'C', '-..'  => 'D',
  '.'     => 'E', '..-.' => 'F', '--.'  => 'G', '....' => 'H',
  '..'    => 'I', '.---' => 'J', '-.-'  => 'K', '.-..' => 'L',
  '--'    => 'M', '-.'   => 'N', '---'  => 'O', '.--.' => 'P',
  '--.-'  => 'Q', '.-.'  => 'R', '...'  => 'S', '-'    => 'T',
  '..-'   => 'U', '...-' => 'V', '.--'  => 'W', '-..-' => 'X',
  '-.--'  => 'Y', '--..' => 'Z', '-----'=> '0', '.----'=> '1',
  '..---' => '2', '...--'=> '3', '....-'=> '4', '.....'=> '5',
  '-....' => '6', '--...'=> '7', '---..'=> '8', '----.'=> '9',
  ''      => ' '
];

foreach($inputFile as $line)
{
  $line = array_map(function($code)
  {
    return MAPPING[$code];
  }, $line);

  echo join('', $line), PHP_EOL;
}
