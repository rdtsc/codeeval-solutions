#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

// Non-STDC.
#include <fnmatch.h>

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  static char stdoutBuffer[8192] = "";

  // Turn on full output buffering for stdout.
  setvbuf(stdout, stdoutBuffer, _IOFBF, sizeof stdoutBuffer);

  FILE* inputStream = fopen(argv[1], "r");

  assert(inputStream && "Failed to open input stream.");

  for(char lineBuffer[2048] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    char* chunk = lineBuffer;

    // Isolate pattern.
    for(;*chunk; ++chunk) if(*chunk == ' ')
    {
      *chunk++ = '\0';
      break;
    }

    const char* const pattern = lineBuffer;

    assert(pattern);

    bool foundMatches = false;

    for(char* filename = chunk; *chunk; filename = chunk)
    {
      // Isolate filename.
      for(;*chunk; ++chunk) if(*chunk == ' ' || *chunk == '\n')
      {
        *chunk++ = '\0';
        break;
      }

      // Non-STDC, but compact, runs quickly, and the supported pattern
      // lexicon fits like a glove.
      if(fnmatch(pattern, filename, 0) != FNM_NOMATCH)
      {
        foundMatches = true;
        fputs(filename, stdout);
        putchar(' ');
      }
    }

    if(!foundMatches) putchar('-');

    putchar('\n');
  }

  // The CRT takes care of cleanup.
}
