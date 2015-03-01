#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

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

  for(char lineBuffer[384] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);
      putchar('\n'))
  {
    bool hasLetter['z' - 'a' + 1] = {false};

    for(const char* c = lineBuffer; *c; ++c) if(isalpha(*c))
    {
      hasLetter[tolower(*c) - 'a'] = true;
    }

    bool isPangram = true;

    for(size_t i = 0; i < sizeof hasLetter / sizeof *hasLetter; ++i)
    {
      if(!hasLetter[i])
      {
        isPangram = false;
        putchar(i + 'a');
      }
    }

    if(isPangram) fputs("NULL", stdout);
  }

  // The CRT takes care of cleanup.
}
