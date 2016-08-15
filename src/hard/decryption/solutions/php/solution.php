#!/usr/bin/env php
<?php

const KEY = 'OAGSFUKBCVWPJTEXYHDIQNLZMR';

const MESSAGE = '012222 1114142503 0313012513 03141418192' .
                '102 0113 2419182119021713 06131715070119';

$payload = [];

for($i = 0, $j = strlen(MESSAGE); $i < $j; ++$i)
{
  if(MESSAGE[$i] === ' ')
  {
    array_push($payload, ' ');
    continue;
  }

  $index = (MESSAGE[$i] * 10 + MESSAGE[$i + 1]);

  array_push($payload, KEY[$index]);

  ++$i;
}

echo join('', $payload), PHP_EOL;
