#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

static inline int strictWeakOrdering(const void* const lhs,
                                     const void* const rhs)
{
  assert(lhs && rhs);

  const unsigned x = *(unsigned*)lhs,
                 y = *(unsigned*)rhs;

  return (x < y) ? (-1) : (x > y);
}

static inline void dump(const unsigned n, const bool prefixComma)
{
  if(prefixComma) putchar(',');

  // One and two digit results appear to be most common.
  if(n < 100)
  {
    if(n >= 10) putchar('0' + (n / 10) % 10);

    putchar('0' + n % 10);

    return;
  }

  #define THRESHOLD(value) n < (value) ? (value) / 10

  const unsigned divisor = THRESHOLD(1e3) :
                           THRESHOLD(1e4) :
                           THRESHOLD(1e5) : 0;

  #undef THRESHOLD

  for(unsigned i = divisor; i; i /= 10)
  {
    putchar('0' + (n / i) % 10);
  }
}

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  static char stdoutBuffer[32768] = "";

  // Turn on full output buffering for stdout.
  setvbuf(stdout, stdoutBuffer, _IOFBF, sizeof stdoutBuffer);

  FILE* inputStream = fopen(argv[1], "r");

  assert(inputStream && "Failed to open input stream.");

  unsigned distances[600] = {0}, // All distances from the origin.
             distance = 0;       // Current distance from the origin.

  for(char lineBuffer[8192] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    // Total number of waypoints, including the origin.
    size_t distanceCount = 0;

    // Fetch waypoint distances. HACK: replacing sscanf with this yields a
    // better score on CodeEval in terms of running time and memory usage.
    for(const char* c = lineBuffer; *c;)
    {
      // Jump to the next distance separator.
      while(*c && *c++ != ',');

      for(distance = 0; *c && *c != ';'; ++c)
      {
        distance = (distance * 10) + (*c - '0');
      }

      distances[distanceCount++] = distance;
    }

    qsort(distances, --distanceCount, sizeof *distances, strictWeakOrdering);

    // HACK: A bit faster than printf in this use case.
    dump(*distances, false);

    for(size_t i = 1; i < distanceCount; ++i)
    {
      // Ditto.
      dump(distances[i] - distances[i - 1], true);
    }

    putchar('\n');
  }

  // The CRT takes care of cleanup.
}
