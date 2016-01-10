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

  static char stdoutBuffer[512] = "";

  // Turn on full output buffering for stdout.
  setvbuf(stdout, stdoutBuffer, _IOFBF, sizeof stdoutBuffer);

  FILE* inputStream = fopen(argv[1], "r");

  assert(inputStream && "Failed to open input stream.");

  for(char lineBuffer[256] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    size_t width = strlen(lineBuffer);

    // Trim trailing newline, if applicable.
    if(width && lineBuffer[width - 1] == '\n') lineBuffer[--width] = '\0';

    bool foundMatch = false;
    char match[sizeof lineBuffer] = "";

    assert(width < sizeof match);

    // The supplied strings are pretty short in this challenge.
    // Dumb brute force should be adequate.
    for(size_t i = width; i && !foundMatch; --i)
    for(size_t j = 0; j < (width - i); ++j)
    {
      // As per the problem statement.
      if(!strncmp(&lineBuffer[j], " ", i)) continue;

      // Save candidate.
      memcpy(match, &lineBuffer[j], i);
      match[i] = '\0';

      const char* const cursor = strstr(lineBuffer, match);

      assert(cursor);

      // Found a dupe.
      if(strstr(&lineBuffer[cursor - lineBuffer + i], match))
      {
        foundMatch = true;
        break;
      }
    }

    puts(foundMatch ? match : "NONE");
  }

  // The CRT takes care of cleanup.
}
