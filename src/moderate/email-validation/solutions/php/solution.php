#!/usr/bin/env php
<?php

$inputFile = new SplFileObject($argv[1]);

$inputFile->setFlags(SplFileObject::READ_AHEAD |
                     SplFileObject::SKIP_EMPTY |
                     SplFileObject::DROP_NEW_LINE);

$wc   = '[a-z0-9]';
$wcr  = '[a-z0-9-]+';
$uca  = '[a-zA-Z0-9.!#$%&\'*+\/=?^_`{|}~-]+';
$ucb  = '[a-zA-Z0-9.!#$%&\'*+\/=?^_`{|}~@-]+';
$user = "($uca|\"$ucb\")";
$host = "$wc($wcr$wc)?(\\.$wc($wcr$wc)?)*";

foreach($inputFile as $line)
{
  echo (preg_match("/^$user@$host$/i", $line) ? 'true' : 'false'), PHP_EOL;
}
