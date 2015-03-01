#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

static inline unsigned reduceDigits(uint_fast32_t n)
{
  static const unsigned digitSquares[] = {0, 1, 4, 9, 16, 25, 36, 49, 64, 81};

  unsigned result = 0;

  for(;n; n /= 10) result += digitSquares[n % 10];

  return result;
}

static inline bool isHappy(const uint_fast32_t n)
{
  unsigned chain[20] = {0};
  size_t linkCount = 0;

  // We should terminate if we've arrived at one...
  for(unsigned  i = reduceDigits(n); i != 1; i = reduceDigits(i))
  {
    // ...or have encountered a previously repeated term.
    for(size_t j = 0; j < linkCount; ++j) if(chain[j] == i) return false;

    chain[linkCount++] = i;

    assert(linkCount < sizeof chain / sizeof *chain);
  }

  return true;
}

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

  for(uint_fast32_t n = 0; fscanf(inputStream, "%" SCNuFAST32, &n) == 1;)
  {
    puts(isHappy(n) ? "1" : "0");
  }

  // The CRT takes care of cleanup.
}
