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
          path: './tools/storage/problems.json',
          humanFriendly: true
        }
      }
    },

    'init-solution':
    {
      config:
      {
        sourcePath: './src/',
        problemListPath: './tools/storage/problems.json',
        problemUrlFormat: 'https://www.codeeval.com/browse/%s/',
        files:
        {
          meta: 'meta.yaml',
          input: 'input.txt',
          readme: 'readme.md',
          languages:
          {
            c:
            {
              args: './tools/templates/c/main-args.c',
              argless: './tools/templates/c/main.c'
            },

            'c++':
            {
              args: './tools/templates/c++/main-args.cpp',
              argless: './tools/templates/c++/main.cpp'
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
        problemListPath: './tools/storage/problems.json',
        pollingInterval: 500,
        clearScreen: true,
        measureTime: true,
        languages:
        {
          c:
          {
            sourceName: 'solution.c',
            binaryName: 'solution',
            compile:
            [
              'gcc -std=c99',
              '-Wall -Wextra -pedantic',
              '-O1 -o solution',
              'solution.c -lm'
            ].join(' '),
            run:
            [
              '[ ! -f ../../input.txt ]',
              '&& ./solution',
              '|| ./solution ../../input.txt < ../../input.txt'
            ].join(' ')
          },

          'c++':
          {
            sourceName: 'solution.cpp',
            binaryName: 'solution',
            compile:
            [
              'g++ -std=c++11',
              '-Wall -Wextra -pedantic',
              '-O1 -o solution',
              'solution.cpp -lm'
            ].join(' '),
            run:
            [
              '[ ! -f ../../input.txt ]',
              '&& ./solution',
              '|| ./solution ../../input.txt < ../../input.txt'
            ].join(' ')
          }
        }
      }
    }
  });

  grunt.loadTasks('./tools/tasks');
};
