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
    }
  });

  grunt.loadTasks('./tools/tasks');
};
