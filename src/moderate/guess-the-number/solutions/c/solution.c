#include <assert.h>
#include <stdio.h>
#include <string.h>

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

  for(char lineBuffer[256] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    unsigned upperBound = 0, lowerBound = 0, guess = 0;

    char* chunk = lineBuffer;

    // Get upper bound.
    for(;*chunk >= '0' && *chunk <= '9'; ++chunk)
    {
      upperBound = (upperBound * 10) + (*chunk - '0');
    }

    // Adjust ballpark based on responses.
    for(chunk = strtok(chunk, " \n"); chunk; chunk = strtok(NULL, " \n"))
    {
      const unsigned delta = upperBound - lowerBound;

      guess = lowerBound + (delta & 1) + (delta / 2);

      switch(*chunk)
      {
        case 'H': lowerBound = guess + 1; break;
        case 'L': upperBound = guess - 1; break;
      }
    }

    printf("%u\n", guess);
  }

  // The CRT takes care of cleanup.
}
