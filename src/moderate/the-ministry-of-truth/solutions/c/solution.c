#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  static char stdoutBuffer[2048] = "";

  // Turn on full output buffering for stdout.
  setvbuf(stdout, stdoutBuffer, _IOFBF, sizeof stdoutBuffer);

  FILE* inputStream = fopen(argv[1], "r");

  assert(inputStream && "Failed to open input stream.");

  for(char lineBuffer[1024] = "", outputBuffer[1024] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    char* lhs = lineBuffer,
        * rhs = lineBuffer,
        * out = outputBuffer;

    // Isolate the left and right chunks.
    {
      char* delimiter = strchr(lineBuffer, ';');

      assert(delimiter && *delimiter == ';');

      // Kill the semicolon.
      *delimiter++ = '\0';

      rhs = delimiter;

      // Trim the right chunk.
      for(char* c = delimiter; *c; ++c) if(*c == '\n') *c = '\0';
    }

    // Supplied filter is empty. All words should be censored.
    if(!*rhs)
    {
      // Replace: /\s{2,}/ -> " "
      for(char* c = lhs, cPrev = *c; *c; ++c)
      {
        // Copy non-space characters verbatim.
        if(*c != ' ') *out++ = cPrev = '_';

        // Copy only the first space between adjacent words.
        else if(cPrev != ' ') *out++ = cPrev = ' ';
      }

      *out = '\0';

      puts(outputBuffer);

      continue;
    }

    char* filters[256] = {rhs};
    size_t filterCount = 1;

    // Explode the filter.
    for(char* c = rhs; *c; ++c) if(*c == ' ')
    {
      assert(filterCount < (sizeof filters / sizeof *filters));

      filters[filterCount++] = (c + 1);

      *c = '\0';
    }

    // Attempt to redact the left chunk against the acquired filters.
    {
      size_t filterIndex = 0;

      for(char* word = strtok(lhs, " "); word; word = strtok(NULL, " "))
      {
        char* const cursor = (filterIndex < filterCount ?
                              strstr(word, filters[filterIndex]) :
                              NULL);

        const size_t wordSize = strlen(word);

        // Completely censor the word.
        memset(word, '_', wordSize);

        // Match. Partial or otherwise.
        if(cursor)
        {
          const size_t replacementSize = strlen(filters[filterIndex]);

          // Inject the match on top of the redaction mask.
          memcpy(cursor, filters[filterIndex], replacementSize);

          ++filterIndex;
        }

        assert((size_t)(out - outputBuffer) < sizeof outputBuffer);

        // Dump the redacted word into the output buffer.
        out = (char*)memcpy(out, word, wordSize) + wordSize;

        *out++ = ' ';
      }

      *out = '\0';

      const bool isFullyCensored = (filterIndex == filterCount);

      if(!isFullyCensored) puts("I cannot fix history");

      else puts(outputBuffer);
    }
  }

  // The CRT takes care of cleanup.
}
