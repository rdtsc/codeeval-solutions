#include <assert.h>
#include <inttypes.h>
#include <stddef.h>
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

  for(uint_fast64_t n = 0; fscanf(inputStream, "%" SCNuFAST64, &n) == 1;)
  {
    size_t setBitCount = 0;

    for(;n; n &= n - 1) ++setBitCount;

    printf("%zu\n", setBitCount);
  }

  // The CRT takes care of cleanup.
}
