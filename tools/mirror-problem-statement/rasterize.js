'use strict';

var fs   = require('fs'),
    sys  = require('system'),
    page = require('webpage').create();

function die()
{
  sys.stderr.write(Array.prototype.join.call(arguments, ' ') + '\n');
  phantom.exit(1);
}

if(sys.args.length < 3)
{
  die(['Usage:', sys.args[0], 'URL', 'file'].join(' '));
}

page.paperSize =
{
  format: 'Letter',
  orientation: 'portrait',
  margin: '10mm'
};

page.onError = function() {};

page.onResourceRequested = function(data, request)
{
  // Intercept and discard the Ace editor library since
  // it renders code blocks as garbage in the PDF.
  if(/ace_editor/i.test(data.url)) request.abort();
};

page.open(sys.args[1], function(status)
{
  if(status !== 'success')
  {
    die('Failed to load remote target.');
  }

  var styles = fs.read(fs.absolute(phantom.libraryPath +
                                   fs.separator        +
                                   'patch.css'));

  page.evaluate(function(css)
  {
    // Inject override stylesheet.
    $('<style>').text(css.replace(/;/g, '!important;')).appendTo('head');

    // Remove empty elements that mess with vertical flow from some pages.
    $('body *').filter(function()
    {
      var isEmpty  = (this.innerHTML.trim() === ''),
          isTarget = (this.tagName.toLowerCase() in {h3: true, p: true});

      return (isEmpty && isTarget);
    }).remove();

    // Remove challenge sponsor branding.
    $('h3:contains("Sponsoring") + p').remove();
    $('h3:contains("Sponsoring")').remove();

    // Patch I/O sample blocks since we're not using Ace.
    $('.description-input-output').contents().unwrap().wrap('<pre/>');
  }, styles);

  window.setTimeout(function()
  {
    page.render(sys.args[2]);
    phantom.exit();
  }, 1e3);
});
