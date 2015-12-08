#include <assert.h>
#include <stdbool.h>
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

  // Pre-computed by the C++ version of this solution via -DGENERATE_LUT.
  static const unsigned terms[] =
  {
    1, 2, 2, 4, 96, 1024, 2880, 81024, 770144
  };

  static const size_t termCount = (sizeof terms / sizeof *terms);

  for(size_t n = 0; fscanf(inputStream, "%zu", &n) == 1;)
  {
    assert(n <= (termCount * 2));

    const bool isOdd = (!n || (n & 1));

    const unsigned term = isOdd ? 0 : terms[(n / 2) - 1];

    printf("%u\n", term);
  }

  // The CRT takes care of cleanup.
}
