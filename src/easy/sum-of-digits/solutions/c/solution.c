#include <assert.h>
#include <stdio.h>

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

  for(unsigned n = 0; fscanf(inputStream, "%u", &n) == 1;)
  {
    unsigned digitSum = 0;

    for(;n; n /= 10) digitSum += n % 10;

    printf("%u\n", digitSum);
  }

  // The CRT takes care of cleanup.
}
