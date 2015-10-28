module.exports = function(grunt)
{
  'use strict';

  var fs   = require('fs');
  var path = require('path');
  var util = require('util');
  var slug = require('slug');
  var exec = require('child_process').exec;
  var spin = require('clui').Spinner;

  require('array.prototype.find');

  grunt.registerTask('watch', function(problemId, languageName)
  {
    this.requiresConfig([this.name, 'config', 'sourcePath']);
    this.requiresConfig([this.name, 'config', 'problemListPath']);
    this.requiresConfig([this.name, 'config', 'pollingInterval']);
    this.requiresConfig([this.name, 'config', 'clearScreen']);
    this.requiresConfig([this.name, 'config', 'measureTime']);

    var config = grunt.config([this.name, 'config']);

    if(typeof problemId === 'undefined')
    {
      grunt.fatal('Missing problem id.');
    }

    if(typeof languageName === 'undefined')
    {
      grunt.fatal('Missing solution language.');
    }

    // Trim whitespace and discard any leading zeros.
    problemId = problemId.trim().replace(/^0+/, '');

    // Ensure the problem id is numeric.
    if(!problemId.match(/^\d+$/))
    {
      grunt.fatal('Problem id is expected to be a uint.');
    }

    else if(!grunt.file.isFile(config.problemListPath))
    {
      grunt.fatal('Local problem list not found. ' +
                  'Try running `grunt update-problem-list` first!');
    }

    var problem = grunt.file.readJSON(config.problemListPath).find(function(obj)
    {
      return (+obj.id === +problemId);
    });

    if(!problem)
    {
      grunt.fatal('Problem ' + problemId + ' not found in local list. ' +
                  'Try running `grunt update-problem-list` first!');
    }

    var language = config.languages[languageName];

    if(!language)
    {
      grunt.fatal('Language "' + languageName + '" is unregistered.');
    }

    var solutionDir = path.join(config.sourcePath,
                                problem.category.toLowerCase(),
                                slug(problem.name).toLowerCase(),
                                'solutions',
                                languageName);

    var solutionFile = path.join(solutionDir, language.sourceName);

    if(!grunt.file.isFile(solutionFile))
    {
      grunt.fatal('Failed to find ' + solutionFile);
    }

    // This tasks will run until we get SIGINTed.
    this.async();

    var runCommand = (function()
    {
      var command = (function()
      {
        var inputFile =
          path.join(process.cwd(), solutionDir, language.inputFile);

        if(grunt.file.exists(inputFile))
        {
          return util.format('%s %s < %s', language.run,
                                           language.inputFile,
                                           language.inputFile);
        }

        return language.run;
      })();

      if(!!config.measureTime)
      {
        return util.format('/bin/bash -c "time %s"', command);
      }

      return command;
    })();

    var spinner = new spin();

    fs.watchFile(solutionFile, {interval: config.pollingInterval}, function()
    {
      if(!!config.clearScreen) grunt.log.write("\u001bc");

      var execOptions = {cwd: solutionDir};

      var separator = Array((process.stdout.columns || 80) + 1).join('-');

      spinner.message('Compiling...');
      spinner.start();

      exec(language.compile, execOptions, function(error, stdout, stderr)
      {
        spinner.stop();

        if(stdout || stderr)
        {
          if(!config.clearScreen) grunt.log.writeln(separator);
          if(stdout) grunt.log.error(stdout);
          if(stderr) grunt.log.error(stderr);
        }

        if(!error)
        {
          spinner.message('Executing...');
          spinner.start();

          exec(runCommand, execOptions, function(error, stdout, stderr)
          {
            spinner.stop();

            if(stdout)
            {
              grunt.log.writeln(separator);
              grunt.log.write(stdout);
            }

            if(stderr)
            {
              grunt.log.writeln(separator);
              grunt.log.error(stderr);
            }

            grunt.log.write('\n\n');

            fs.unlinkSync(path.join(solutionDir, language.binaryName));
          });
        }
      });
    });
  });
};
