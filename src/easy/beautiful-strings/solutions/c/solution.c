#include <assert.h>
#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct
{
  char ordinal;
  unsigned count;
} Probe;

static inline int beautyOrdering(const void* const lhs, const void* const rhs)
{
  assert(lhs && rhs);

  const unsigned x = ((const Probe*)lhs)->count,
                 y = ((const Probe*)rhs)->count;

  return (x > y) ? (-1) : (x < y);
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

  for(char lineBuffer[768] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    Probe probes[26] = {{'\0'}};

    // Store letter frequencies, disregarding case.
    for(const char* c = lineBuffer; *c && *c != '\n'; ++c) if(isalpha(*c))
    {
      const size_t offset = tolower(*c) - 'a';

      probes[offset].ordinal = *c;
      probes[offset].count += 1;
    }

    static const size_t probeCount = sizeof probes / sizeof *probes;

    qsort(probes, probeCount, sizeof(Probe), beautyOrdering);

    unsigned beautyScore = 0,          // Total beauty score of the string.
             beautyIndex = probeCount; // Maximum letter score.

    for(size_t i = 0; i < probeCount; ++i)
    {
      beautyScore += (beautyIndex--) * probes[i].count;
    }

    printf("%u\n", beautyScore);
  }

  // The CRT takes care of cleanup.
}
