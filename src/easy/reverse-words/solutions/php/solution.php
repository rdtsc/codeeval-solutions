#!/usr/bin/env php
<?php

$lines = file($argv[1], FILE_IGNORE_NEW_LINES);

array_walk($lines, function(&$line)
{
  $line = join(' ', array_reverse(explode(' ', $line)));
});

foreach($lines as $line) echo $line, PHP_EOL;
