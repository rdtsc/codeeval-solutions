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
          readme: 'readme.md'
        }
      }
    }
  });

  grunt.loadTasks('./tools/tasks');
};
