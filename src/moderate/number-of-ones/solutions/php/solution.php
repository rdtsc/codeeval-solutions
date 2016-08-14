#!/usr/bin/env php
<?php

$lines = file($argv[1], FILE_IGNORE_NEW_LINES);

array_walk($lines, function(&$line)
{
  $line = substr_count(decbin($line), '1');
});

foreach($lines as $line) echo $line, PHP_EOL;
