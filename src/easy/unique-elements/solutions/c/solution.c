#include <assert.h>
#include <limits.h>
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

  unsigned n = 0, nPrevious = UINT_MAX;
  char delimiter = '\0';

  while(fscanf(inputStream, "%u%c", &n, &delimiter) == 2)
  {
    if(n != nPrevious)
    {
      printf((nPrevious == UINT_MAX) ? ("%u") : (",%u"), n);
      nPrevious = n;
    }

    // Reset for new line.
    if(delimiter == '\n')
    {
      putchar(delimiter);
      nPrevious = UINT_MAX;
    }
  }

  if(delimiter == ',')
  {
    printf((nPrevious == UINT_MAX) ? ("%u") : (",%u"), n);
  }

  // The CRT takes care of cleanup.
}
