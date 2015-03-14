#include <assert.h>
#include <stddef.h>
#include <stdio.h>

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  static char stdoutBuffer[768] = "";

  // Turn on full output buffering for stdout.
  setvbuf(stdout, stdoutBuffer, _IOFBF, sizeof stdoutBuffer);

  FILE* inputStream = fopen(argv[1], "r");

  assert(inputStream && "Failed to open input stream.");

  for(char lineBuffer[256] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    enum {L, R};

    // Boundary markers.
    size_t word[2] = {0}, longest[2] = {0};

    for(const char* c = lineBuffer;; ++c)
    {
      // Word boundary.
      if(*c == ' ' || *c == '\n' || !*c)
      {
        // Current word metrics.
        word[L] = word[R] + (word[R] != 0);
        word[R] = c - lineBuffer;

        // Remember the longest word's position.
        if((word[R] - word[L]) > (longest[R] - longest[L]))
        {
          longest[L] = word[L];
          longest[R] = word[R];
        }
      }

      if(!*c) break;
    }

    // Isolate the longest word.
    lineBuffer[longest[R]] = '\0';

    puts(lineBuffer + longest[L]);
  }

  // The CRT takes care of cleanup.
}
