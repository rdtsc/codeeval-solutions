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
    }
  });

  grunt.loadTasks('./tools/tasks');
};
