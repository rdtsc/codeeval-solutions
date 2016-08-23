module.exports = function(grunt)
{
  'use strict';

  var path     = require('path');
  var util     = require('util');
  var execFile = require('child_process').execFileSync;
  var request  = require('request');
  var cheerio  = require('cheerio');
  var phantom  = require('phantomjs-prebuilt').path;
  var yaml     = require('js-yaml');
  var slug     = require('slug');
  var wrap     = require('word-wrap');

  require('array.prototype.find');
  require('./lib/to-title-case');
  require('./lib/capitalize');
  require('./lib/full-stop');

  grunt.registerTask('init-solution', function(problemId, solutionLanguage)
  {
    this.requiresConfig([this.name, 'config', 'sourcePath']);
    this.requiresConfig([this.name, 'config', 'problemListPath']);
    this.requiresConfig([this.name, 'config', 'problemUrlFormat']);
    this.requiresConfig([this.name, 'config', 'files', 'input']);
    this.requiresConfig([this.name, 'config', 'files', 'meta']);
    this.requiresConfig([this.name, 'config', 'files', 'readme']);
    this.requiresConfig([this.name, 'config', 'files', 'mirror']);

    var config = grunt.config([this.name, 'config']);

    if(typeof problemId === 'undefined')
    {
      grunt.fatal('Missing problem id.');
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

    // Ensure the provided solution language, if any, actually exists
    // in the Grunt config.
    else if(!(solutionLanguage in config.files.languages))
    {
      if(typeof solutionLanguage !== 'undefined')
      {
        grunt.fatal('Specified language config not found in the Grunt file.');
      }
    }

    var problem = grunt.file.readJSON(config.problemListPath).find(function(obj)
    {
      return +obj.id === +problemId;
    });

    if(!problem)
    {
      grunt.fatal('Problem ' + problemId + ' not found in local list. ' +
                  'Try running `grunt update-problem-list` first!');
    }

    var solutionDir = path.join(config.sourcePath,
                                problem.category.toLowerCase(),
                                slug(problem.name).toLowerCase());

    var paths =
    {
      meta:      path.join(solutionDir, config.files.meta),
      readme:    path.join(solutionDir, config.files.readme),
      mirror:    path.join(solutionDir, config.files.mirror),
      input:     path.join(solutionDir, config.files.input),
      solutions: path.join(solutionDir, 'solutions')
    };

    var metadata =
    {
      problemId: +problemId,
      name:      problem.name.toTitleCase(),
      category:  problem.category.capitalize(),
      summary:   problem.summary.fullStop(),
      url:       util.format(config.problemUrlFormat, problemId)
    };

    // Make solutions directory.
    (function()
    {
      if(!grunt.option('overwrite') && grunt.file.isDir(paths.solutions))
      {
        grunt.log.writeln('Skipped "solutions" directory creation' +
                          ' -- already exists in: ' + solutionDir);

        return;
      }

      grunt.file.mkdir(paths.solutions);
      grunt.log.ok('Created "solutions" directory in: ' + solutionDir);
    })();

    // Make metadata file.
    (function()
    {
      if(!grunt.option('overwrite') && grunt.file.isFile(paths.meta))
      {
        grunt.log.writeln('Skipped ' + config.files.meta +
                          ' -- already exists in: ' + solutionDir);

        return;
      }

      grunt.file.write(paths.meta, yaml.dump(metadata));
      grunt.log.ok('Created ' + config.files.meta + ' in: ' + solutionDir);
    })();

    // Make readme file.
    (function()
    {
      if(!grunt.option('overwrite') && grunt.file.isFile(paths.readme))
      {
        grunt.log.writeln('Skipped ' + config.files.readme +
                          ' -- already exists in: ' + solutionDir);

        return;
      }

      var readme =
      [
        '[%s][ce]\n%s',
        '**Problem %s**',
        '%s',
        'Full problem statement is available on [CodeEval][ce].',
        '[ce]: %s\n      "View problem statement on CodeEval"\n'
      ].join('\n\n');

      readme = util.format(readme, metadata.name,
                                   Array(metadata.name.length + 7).join('-'),
                                   metadata.problemId,
                                   wrap(metadata.summary,
                                   {
                                     indent: '> ',
                                     trim: true,
                                     width: 78
                                   }),
                                   metadata.url);

      grunt.file.write(paths.readme, readme);
      grunt.log.ok('Created ' + config.files.readme + ' in: ' + solutionDir);
    })();

    // Mirror problem statement.
    (function()
    {
      if(!grunt.option('overwrite') && grunt.file.isFile(paths.mirror))
      {
        grunt.log.writeln('Skipped ' + config.files.mirror +
                          ' -- already exists in: ' + solutionDir);

        return;
      }

      try
      {
        var job =
        [
          './tools/mirror-problem-statement/rasterize.js',
          util.format(config.problemUrlFormatAlt, problemId),
          path.resolve(paths.mirror)
        ];

        execFile(phantom, job);

        grunt.log.ok('Created ' + config.files.mirror + ' in: ' + solutionDir);
      }

      catch(error)
      {
        grunt.fatal('Failed to mirror problem statement; ' +
                    'rasterizer return code -> ' + error.status);
      }
    })();

    // Make solution templates.
    function makeSolutionTemplates(problemHasInput)
    {
      var languages = config.files.languages;

      if(typeof solutionLanguage !== 'undefined')
      {
        languages = {};
        languages[solutionLanguage] = true;
      }

      for(var language in languages)
      {
        var filename = 'solution' +
                       path.extname(config.files.languages[language].argless);

        var solutionFile = path.join(solutionDir,
                                     'solutions',
                                     language.toLowerCase(),
                                     filename);

        if(!grunt.option('overwrite') && grunt.file.isFile(solutionFile))
        {
          grunt.log.writeln('Skipped ' + filename + ' -- already exists in: ' +
                            path.dirname(solutionFile));

          continue;
        }

        var template = config.files.languages[language];

        grunt.file.copy(!!problemHasInput ? template.args : template.argless,
                        solutionFile);

        grunt.log.ok('Created ' + filename + ' in: ' +
                     path.dirname(solutionFile));
      }
    }

    // Make input file.
    (function(task)
    {
      if(!grunt.option('overwrite') && grunt.file.isFile(paths.input))
      {
        grunt.log.writeln('Skipped ' + config.files.input +
                          ' -- already exists in: ' + solutionDir);

        makeSolutionTemplates(true);

        return;
      }

      // Tell Grunt our upcoming GET request is async.
      var done = task.async();

      request(metadata.url, function(error, response, body)
      {
        if(error || response.statusCode !== 200)
        {
          grunt.fatal('Failed to fetch problem statement -> ' + error);
        }

        // HACK: Some problem statements have unescaped HTML entities. Cheerio
        // (or maybe its dependency, htmlparser2) fails to escape less-than
        // signs which are immediately followed by an equals sign. This results
        // in the introduction of additional DOM nodes and breaks the extraction
        // of sample input from problem statement pages.
        body = body.replace(/<=/g, '&gt;=');

        var $ = cheerio.load(body);

        var isInputHeader =
          /^(?:input\s*|input\s+sample\s*|sample\s+input\s*):/i;

        var $input = $('#ce-content h3').filter(function()
        {
          return isInputHeader.test($(this).text().trim());
        }).nextUntil('h3', 'pre, .description-input-output');

        if(!$input.length)
        {
          grunt.log.writeln('Skipped ' + config.files.input +
                            ' -- no input found in problem statement.');
        }

        else
        {
          grunt.file.write(paths.input, $input.first().text().trim() + '\n');
          grunt.log.ok('Created ' + config.files.input + ' in: ' + solutionDir);
        }

        makeSolutionTemplates(!!$input.length);

        done();
      });
    })(this);
  });
};
