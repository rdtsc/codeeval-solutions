#!/usr/bin/env php
<?php

$inputFile = new SplFileObject($argv[1]);

$inputFile->setFlags(SplFileObject::READ_AHEAD |
                     SplFileObject::SKIP_EMPTY |
                     SplFileObject::DROP_NEW_LINE);

const TERMS = [1, 2, 2, 4, 96, 1024, 2880, 81024, 770144];

foreach($inputFile as $n)
{
  echo ((!$n || ($n & 1)) ? 0 : TERMS[($n >> 1) - 1]), PHP_EOL;
}
