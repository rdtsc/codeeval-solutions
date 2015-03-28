#include <assert.h>
#include <stdio.h>
#include <string.h>

// Non-STDC.
#include <fnmatch.h>

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  static char stdoutBuffer[256] = "";

  // Turn on full output buffering for stdout.
  setvbuf(stdout, stdoutBuffer, _IOFBF, sizeof stdoutBuffer);

  FILE* inputStream = fopen(argv[1], "r");

  assert(inputStream && "Failed to open input stream.");

  for(char lineBuffer[512] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    char* chunk = strchr(lineBuffer, ',');

    assert(chunk);

    // Clean up raw pattern.
    {
      *chunk++ = '\0';

      for(char* c = chunk; *c; ++c) if(*c == '\n')
      {
        *c = '\0';
        break;
      }
    }

    char pattern[sizeof lineBuffer / 2] = "*";

    // Enclose raw pattern with asterisks.
    {
      char* c = pattern + 1;

      while(*chunk) *c++ = *chunk++;

      *c = '*';
    }

    // Non-STDC. May have to implement a lightweight DIY equivalent depending
    // on the score this yields on CodeEval.
    puts(fnmatch(pattern, lineBuffer, 0) ? "false" : "true");
  }

  // The CRT takes care of cleanup.
}
