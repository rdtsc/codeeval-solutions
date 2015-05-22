#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

static inline size_t appendTo(char* buffer, const unsigned n)
{
  assert(buffer);

  const char* const bufferBegin = buffer;

  #define THRESHOLD(Value) n < (1e##Value) ? (1e##Value) / 10

  const unsigned divisor = THRESHOLD(1) : THRESHOLD(2) :
                           THRESHOLD(3) : THRESHOLD(4) :
                           THRESHOLD(5) : 0;

  #undef THRESHOLD

  for(unsigned i = divisor; i; i /= 10)
  {
    *buffer++ = ('0' + (n / i) % 10);
  }

  *buffer++ = ',';

  return (buffer - bufferBegin);
}

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  static char stdoutBuffer[131072] = "";

  // Turn on full output buffering for stdout.
  setvbuf(stdout, stdoutBuffer, _IOFBF, sizeof stdoutBuffer);

  FILE* inputStream = fopen(argv[1], "r");

  assert(inputStream && "Failed to open input stream.");

  unsigned terms[40] = {0}, maxTerm = 0;
  size_t termCount = 0;

  // Fetch all terms and record the highest one.
  for(;fscanf(inputStream, "%u", &terms[termCount]) == 1; ++termCount)
  {
    assert(termCount < (sizeof terms / sizeof *terms));
    assert(terms[termCount] >= 1);

    if(terms[termCount] > maxTerm) maxTerm = terms[termCount];
  }

  // Sanity check. Unrestricted VLAs can lead to UB if allocation fails.
  assert(termCount && maxTerm >= 3 && maxTerm <= 32768);

  // VLAs yield a slightly better memory score on CodeEval than straight
  // [m,c]alloc. Shouldn't rely on this in C11 and onwards, though.
  bool isPrime[maxTerm];
  memset(isPrime, true, sizeof isPrime);

  // Sieve of Eratosthenes.
  {
    const unsigned limit = sqrt(maxTerm);

    for(unsigned i = 2; i <= limit; ++i) if(isPrime[i])
    {
      for(unsigned j = (i * i); j <= maxTerm; j += i)
      {
        isPrime[j] = false;
      }
    }
  }

  static char primeList[16384] = "";
  static size_t breakpoints[16384] = {0};

  // Pre-format the result set for increased performance.
  {
    char* out = primeList;

    for(size_t i = 2, breakpoint = 0; i <= maxTerm; ++i)
    {
      if(isPrime[i])
      {
        out += appendTo(out, i);

        // Commas act as snip guidelines.
        breakpoints[i] = breakpoint = (out - primeList - 1);
      }

      else breakpoints[i] = breakpoint;
    }

    // Patch trailing comma.
    if(out > primeList) *--out = '\0';
  }

  // Input playback.
  for(size_t i = 0; i < termCount; ++i)
  {
    assert(terms[i] < (sizeof breakpoints / sizeof *breakpoints));

    const size_t end = breakpoints[terms[i] - 1];
    const char termSeparator = primeList[end];

    // Temporarily snip the end of the sequence.
    primeList[end] = '\0';

    puts(primeList);

    // Glue the sequence back together.
    primeList[end] = termSeparator;
  }

  // The CRT takes care of cleanup.
}
