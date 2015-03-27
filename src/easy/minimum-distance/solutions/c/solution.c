#include <assert.h>
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

  for(char lineBuffer[512] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    const char* c = lineBuffer;
    size_t friendCount = 0;

    // Get friend count.
    for(;*c && *c != ' '; ++c) friendCount = (friendCount * 10) + (*c - '0');

    assert(friendCount >= 2);

    // Eat trailing whitespace.
    while(*c && *c == ' ') ++c;

    unsigned addresses[100] = {0};

    for(unsigned n = 0, i = 0;; ++c)
    {
      if(*c == ' ' || *c == '\n' || !*c)
      {
        addresses[i++] = n;
        n = 0;

        if(*c == '\n' || !*c) break;
      }

      else n = (n * 10) + (*c - '0');
    }

    qsort(addresses, friendCount, sizeof *addresses, strictWeakOrdering);

    #define MID(offset) (addresses[friendCount / 2 + (offset)])

    const unsigned ourAddress =
      (friendCount & 1) ? (MID(0)) : ((MID(-1) + MID(0)) / 2);

    #undef MID

    unsigned totalDistance = 0;

    // Get the total distance from our address to all of our friends' houses.
    for(size_t i = 0; i < friendCount; ++i)
    {
      #define DISTANCE(x, y) ((x)>(y) ? (x)-(y) : (y)-(x))

      totalDistance += DISTANCE(ourAddress, addresses[i]);

      #undef DISTANCE
    }

    printf("%u\n", totalDistance);
  }

  // The CRT takes care of cleanup.
}
