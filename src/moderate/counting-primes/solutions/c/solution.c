#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Brute-force search. Fast enough for this problem.
static inline bool isPrime(const uint_fast64_t n)
{
  if(n <= 1)        return false; // 0 and 1 -> n/a.
  else if(n < 4)    return true;  // 2 and 3 -> prime.
  else if(!(n & 1)) return false; // Even numbers -> composite.
  else if(n < 9)    return true;  // 5 and 7 -> prime.
  else if(!(n % 3)) return false; // Multiples of 3 -> composite.

  const uint_fast64_t limit = sqrt(n);

  // Primes >3 are of the form (6n +/- 1).
  for(uint_fast64_t i = 5; i <= limit; i += 6)
  {
    if(!(n % i) || !(n % (i + 2))) return false;
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

  struct
  {
    unsigned lower, upper;
  } ranges[30]   = {{0}},
    globalBounds = {.lower = UINT_MAX};

  size_t rangeCount = 0;

  // Fetch all the range pairs and get the lowest and highest values.
  for(;fscanf(inputStream, "%u,%u", &ranges[rangeCount].lower,
                                    &ranges[rangeCount].upper) == 2;
                                    ++rangeCount)
  {
    assert(rangeCount < sizeof ranges / sizeof *ranges);
    assert(ranges[rangeCount].lower < ranges[rangeCount].upper);

    #define MIN(x, y) ((x) < (y) ? (x) : (y))
    #define MAX(x, y) ((x) > (y) ? (x) : (y))

    globalBounds.lower = MIN(globalBounds.lower, ranges[rangeCount].lower);
    globalBounds.upper = MAX(globalBounds.upper, ranges[rangeCount].upper);

    #undef MIN
    #undef MAX
  }

  assert(globalBounds.upper && (globalBounds.upper > globalBounds.lower));

  const size_t termCount = globalBounds.upper - globalBounds.lower + 1;

  // Sanity check. Unrestricted VLAs can lead to UB if allocation fails.
  assert(termCount <= 1e3);

  // VLAs yield a slightly better memory score on CodeEval than straight
  // [m,c]alloc. Shouldn't rely on this in C11 and onwards, though.
  bool prime[termCount];
  memset(prime, false, sizeof prime);

  // Brute force. A sieve makes more sense, especially for large ranges, but it
  // looks like we'll only be dealing with a bunch of overlapping ranges all of
  // which seem to max out at ~500.
  for(unsigned i = 0; i < termCount; ++i)
  {
    if(isPrime(globalBounds.lower + i)) prime[i] = true;
  }

  // Input range playback.
  for(size_t i = 0; i < rangeCount; ++i)
  {
    const unsigned lower = ranges[i].lower - globalBounds.lower,
                   upper = ranges[i].upper - globalBounds.lower;

    unsigned primeCount = 0;

    // Rage bounds are inclusive.
    for(size_t n = lower; n <= upper; ++n) if(prime[n]) ++primeCount;

    printf("%u\n", primeCount);
  }

  // The CRT takes care of cleanup.
}
