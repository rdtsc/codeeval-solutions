module.exports = function(grunt)
{
  'use strict';

  var request  = require('request');
  var cheerio  = require('cheerio');
  var readline = require('readline-sync');

  require('./lib/full-stop');

  grunt.registerTask('update-problem-list', function()
  {
    this.requiresConfig([this.name, 'config', 'authUrl']);
    this.requiresConfig([this.name, 'config', 'listPath']);
    this.requiresConfig([this.name, 'config', 'storage', 'path']);

    var config = grunt.config([this.name, 'config']);

    // The full problem list is behind an auth wall and is only available to
    // registered users. Note that CodeEval also supports authentication via
    // Facebook and GitHub, though this is not implemented below.
    var requestOptions = (function()
    {
      var username = readline.question('CodeEval username: ');

      var options =
      {
        url: config.authUrl,
        followAllRedirects: true,
        jar: request.jar(),
        form:
        {
          next: config.listPath,
          email_not_activated: 'email',
          username: username,
          password: readline.question(username + "'s password: ",
          {
            noTrim: true,
            noEchoBack: true
          })
        }
      };

      return options;
    })();

    // Tell Grunt our upcoming POST request is async.
    var done = this.async();

    request.post(requestOptions, function(error, response, body)
    {
      if(error || response.statusCode !== 200)
      {
        grunt.fatal('Failed to fetch problem list -> ' + error);
      }

      var $ = cheerio.load(body);
      var problemSet = [];

      // The problem data is split among three variables in the source,
      // namely: `var (easy|moderate|hard)_challanges = []` -- note the
      // misspelling of "challenges".
      $('script:not([src])').each(function()
      {
        var script = $(this).text();

        // Is this the script block we're looking for?
        if(!/var\s+easy_chall/i.test(script)) return;

        ['easy', 'moderate', 'hard'].forEach(function(category)
        {
          var distiller = 'var\\s+' + category + '_chall.*?=\\s*([^;]+)';
          var problems  = (new RegExp(distiller, 'im')).exec(script);

          if(!problems)
          {
            grunt.fatal('Failed to extract ' + category + ' problem list. ' +
                        "Check the site's DOM.");
          }

          problems = JSON.parse(problems[1]).map(function(attributes)
          {
            attributes =
            {
              id:       attributes[0],
              name:     attributes[1].trim().replace(/\s{2,}/g, ' '),
              summary:  attributes[3].trim().replace(/\s{2,}/g, ' ').fullStop(),
              category: category.toLowerCase(),
            };

            return attributes;
          });

          problemSet.push.apply(problemSet, problems);
        });

        problemSet.sort(function(lhs, rhs)
        {
          var lhsId = +lhs.id,
              rhsId = +rhs.id;

          return (lhsId < rhsId) ? (-1) : (lhsId > rhsId);
        });

        // Break out of the Cheerio loop.
        return false;
      });

      var problemCount = problemSet.length;

      if(!problemCount)
      {
        grunt.fatal('Failed to extract any problems. ' +
                    "Check your credentials or the site's DOM.");
      }

      var humanFriendlyJson = !!config.storage.humanFriendly;

      problemSet = humanFriendlyJson ? JSON.stringify(problemSet, null, 2) :
                                       JSON.stringify(problemSet);

      grunt.file.write(config.storage.path, problemSet);

      grunt.log.ok('Saved ' + problemCount + ' problem descriptions in: ' +
                   config.storage.path);

      // Notify Grunt that we're done.
      done();
    });
  });
};
