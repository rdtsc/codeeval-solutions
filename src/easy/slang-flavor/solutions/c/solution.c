#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  static char stdoutBuffer[16384] = "";

  // Turn on full output buffering for stdout.
  setvbuf(stdout, stdoutBuffer, _IOFBF, sizeof stdoutBuffer);

  FILE* inputStream = fopen(argv[1], "r");

  assert(inputStream && "Failed to open input stream.");

  static const char* const slang[] =
  {
    ", yeah!", ", this is crazy, I tell ya.", ", can U believe this?",
    ", eh?", ", aw yea.", ", yo.", "? No way!", ". Awesome!"
  };

  static const size_t slangTermCount = sizeof slang / sizeof *slang;

  size_t slangTermIndex = 0;
  bool shouldInjectSlang = false;

  for(char lineBuffer[512] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    for(const char* c = lineBuffer; *c; ++c)
    {
      if(*c == '.' || *c == '!' || *c == '?')
      {
        if(shouldInjectSlang)
        {
          fputs(slang[slangTermIndex++ % slangTermCount], stdout);
        }

        else putchar(*c);

        // Every other occurance of the above should be replaced.
        shouldInjectSlang = !shouldInjectSlang;
      }

      else putchar(*c);
    }
  }

  // The CRT takes care of cleanup.
}
