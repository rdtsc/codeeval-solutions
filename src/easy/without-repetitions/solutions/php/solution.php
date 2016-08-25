#!/usr/bin/env php
<?= preg_replace('/(.)\1+/', '$1', file_get_contents($argv[1]));
