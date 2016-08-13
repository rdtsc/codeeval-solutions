#!/usr/bin/env php
<?php

$lines = file($argv[1], FILE_IGNORE_NEW_LINES);
$count = array_shift($lines);

usort($lines, function($lhs, $rhs)
{
  return strlen($rhs) - strlen($lhs);
});

echo join(PHP_EOL, array_slice($lines, 0, $count)), PHP_EOL;
