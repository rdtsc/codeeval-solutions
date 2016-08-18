#!/usr/bin/env php
<?php

$track = file($argv[1], FILE_IGNORE_NEW_LINES | FILE_SKIP_EMPTY_LINES);

$lastPos = strpos($track[0], '_');

$track[0][$lastPos] = '|';

for($y = 1, $trackLength = count($track); $y < $trackLength; ++$y)
{
  $x = strpos($track[$y], 'C');

  if($x === false) $x = strpos($track[$y], '_');

  if($lastPos === $x) $track[$y][$x] = '|';

  else $track[$y][$x] = ($lastPos > $x ? '/' : '\\');

  $lastPos = $x;
}

echo join(PHP_EOL, $track), PHP_EOL;
