#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

// Number of 0's in the binary representation of N.
static unsigned getZeroCount(const unsigned n)
{
  #define CACHE_SIZE 1001

  static unsigned resultCache[CACHE_SIZE] = {0};
  static bool resultHistory[CACHE_SIZE] = {false};

  assert(n < CACHE_SIZE);

  #undef CACHE_SIZE

  if(resultHistory[n]) return resultCache[n];

  unsigned result = 0;

  for(unsigned k = n; k; k >>= 1) if(!(k & 1)) ++result;

  resultCache[n] = result;
  resultHistory[n] = true;

  return result;
}

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

  for(unsigned lhs = 0, rhs = 0;
      fscanf(inputStream, "%u %u", &lhs, &rhs) == 2;)
  {
    size_t occurrences = 0;

    // Search space is [1, upperBound].
    for(unsigned i = 1; i <= rhs; ++i)
    {
      if(getZeroCount(i) == lhs) ++occurrences;
    }

    printf("%zu\n", occurrences);
  }

  // The CRT takes care of cleanup.
}
