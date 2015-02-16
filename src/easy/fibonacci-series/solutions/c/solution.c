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

  static char stdoutBuffer[512] = "";

  // Turn on full output buffering for stdout.
  setvbuf(stdout, stdoutBuffer, _IOFBF, sizeof stdoutBuffer);

  FILE* inputStream = fopen(argv[1], "r");

  assert(inputStream && "Failed to open input stream.");

  // Fibonacci sequence terms. Terms above 93 are wider than 64 bits.
  uint_fast64_t sequence[94] = {0, 1};

  for(size_t i = 2; i < sizeof sequence / sizeof *sequence; ++i)
  {
    sequence[i] = sequence[i - 1] + sequence[i - 2];
  }

  for(size_t n = 0; fscanf(inputStream, "%zu", &n) == 1;)
  {
    assert(n < sizeof sequence / sizeof *sequence);

    printf("%" PRIuFAST64 "\n", sequence[n]);
  }

  // The CRT takes care of cleanup.
}
