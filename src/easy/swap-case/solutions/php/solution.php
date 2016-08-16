#!/usr/bin/env php
<?php

$lo = join('', range('a', 'z'));
$hi = join('', range('A', 'Z'));

echo strtr(file_get_contents($argv[1]), "$hi$lo", "$lo$hi");
