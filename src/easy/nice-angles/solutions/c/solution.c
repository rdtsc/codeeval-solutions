#include <assert.h>
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

  for(double n = 0; fscanf(inputStream, "%lf", &n) == 1;)
  {
    const unsigned degrees = n,
                   minutes = (n - degrees) * 60,
                   seconds = ((n - degrees) * 60 - minutes) * 60;

    printf("%u.%02u'%02u\"\n", degrees, minutes , seconds);
  }

  // The CRT takes care of cleanup.
}
