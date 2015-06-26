#include <assert.h>
#include <stddef.h>
#include <stdio.h>

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

  for(char lineBuffer[256] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    char* longestWordBegin = lineBuffer,
        * longestWordEnd   = lineBuffer;

    for(char* wordBegin = lineBuffer, * wordEnd = wordBegin;; ++wordEnd)
    {
      const char c = (*wordEnd);

      if(c == ' ' || c == '\n' || !c)
      {
        if((wordEnd - wordBegin) > (longestWordEnd - longestWordBegin))
        {
          longestWordBegin = wordBegin;
          longestWordEnd   = wordEnd;
        }

        wordBegin = ++wordEnd;

        if(c == '\n' || !c) break;
      }
    }

    assert(longestWordEnd > longestWordBegin);

    for(char* c = longestWordBegin; c < longestWordEnd; ++c)
    {
      char outputBuffer[16] = "***************";

      const size_t offset = (c - longestWordBegin);

      outputBuffer[offset] = *c;
      outputBuffer[offset + 1] = '\0';

      fputs(outputBuffer, stdout);
      putchar(' ');
    }

    putchar('\n');
  }

  // The CRT takes care of cleanup.
}
