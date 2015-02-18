#include <assert.h>
#include <stddef.h>
#include <stdio.h>

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  static char stdoutBuffer[128] = "";

  // Turn on full output buffering for stdout.
  setvbuf(stdout, stdoutBuffer, _IOFBF, sizeof stdoutBuffer);

  FILE* inputStream = fopen(argv[1], "r");

  assert(inputStream && "Failed to open input stream.");

  for(char lineBuffer[128] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    size_t n = 0, // Number of character elements.
           m = 0; // Mth element that we're supposed to output.

    // N is directly proportional to the number of spaces in the input string.
    for(const char* c = lineBuffer; *c; ++c)
    {
      if(*c == ' ') ++n;

      else if((unsigned)*c - '0' <= 9U)
      {
        m = (m * 10) + (*c - '0');
      }
    }

    if(n >= m)
    {
      putchar(lineBuffer[(n - m) * 2]);
      putchar('\n');
    }
  }

  // The CRT takes care of cleanup.
}
