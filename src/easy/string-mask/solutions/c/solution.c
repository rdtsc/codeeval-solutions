#include <assert.h>
#include <ctype.h>
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

  for(char lineBuffer[128] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    char* word = lineBuffer,
        * mask = lineBuffer;

    // Isolate the word and mask.
    {
      char* delimiter = strchr(lineBuffer, ' ');

      assert(delimiter && *delimiter == ' ');

      // Kill the space.
      *delimiter++ = '\0';

      mask = delimiter;

      // Trim the mask.
      for(char* c = delimiter; *c; ++c) if(*c == '\n') *c = '\0';
    }

    // Guaranteed by the problem statement.
    assert(strlen(word) == strlen(mask));

    for(;*mask; ++mask, ++word) if(*mask == '1') *word = toupper(*word);

    puts(lineBuffer);
  }

  // The CRT takes care of cleanup.
}
