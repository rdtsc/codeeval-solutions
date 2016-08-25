#!/usr/bin/env php
<?= strtr(file_get_contents($argv[1]), join('', range('a', 'z')),
                                       'uvwxyznopqrstghijklmabcdef');
