module.exports = function(grunt)
{
  'use strict';

  grunt.initConfig(
  {
    'update-problem-list':
    {
      config:
      {
        authUrl: 'https://www.codeeval.com/accounts/login/',
        listPath: '/open_challenges/',
        storage:
        {
          path: './storage/problems.json',
          humanFriendly: true
        }
      }
    },

    'init-solution':
    {
      config:
      {
        sourcePath: './src/',
        problemListPath: './storage/problems.json',
        problemUrlFormat: 'https://www.codeeval.com/browse/%s/',
        problemUrlFormatAlt: 'https://www.codeeval.com/public_sc/%s/',
        files:
        {
          meta: 'meta.yaml',
          input: 'input.txt',
          readme: 'readme.md',
          mirror: 'readme.pdf',
          languages:
          {
            c:
            {
              args: './templates/c/main-args.c',
              argless: './templates/c/main.c'
            },

            'c++':
            {
              args: './templates/c++/main-args.cpp',
              argless: './templates/c++/main.cpp'
            },

            javascript:
            {
              args: './templates/javascript/main-args.js',
              argless: './templates/javascript/main.js'
            },

            php:
            {
              args: './templates/php/main-args.php',
              argless: './templates/php/main.php'
            },

            python:
            {
              args: './templates/python/main-args.py',
              argless: './templates/python/main.py'
            }
          }
        }
      }
    },

    watch:
    {
      config:
      {
        sourcePath: './src/',
        problemListPath: './storage/problems.json',
        pollingInterval: 500,
        clearScreen: true,
        measureTime: true,
        languages:
        {
          c:
          {
            sourceName: 'solution.c',
            binaryName: 'solution',
            inputFile: '../../input.txt',
            compile:
            [
              'gcc -std=c99',
              '-Wall -Wextra -pedantic',
              '-O1 -o solution',
              'solution.c -lm'
            ].join(' '),
            run: './solution'
          },

          'c++':
          {
            sourceName: 'solution.cpp',
            binaryName: 'solution',
            inputFile: '../../input.txt',
            compile:
            [
              'g++ -std=c++11',
              '-Wall -Wextra -pedantic',
              '-O1 -o solution',
              'solution.cpp -lm'
            ].join(' '),
            run: './solution'
          },

          javascript:
          {
            sourceName: 'solution.js',
            binaryName: null,
            inputFile: '../../input.txt',
            compile: false,
            run: 'node ./solution.js'
          },

          php:
          {
            sourceName: 'solution.php',
            binaryName: null,
            inputFile: '../../input.txt',
            compile: false,
            run: 'php ./solution.php'
          },

          python:
          {
            sourceName: 'solution.py',
            binaryName: null,
            inputFile: '../../input.txt',
            compile: false,
            run: 'python3 ./solution.py'
          }
        }
      }
    }
  });

  grunt.loadTasks('./tasks');
};
