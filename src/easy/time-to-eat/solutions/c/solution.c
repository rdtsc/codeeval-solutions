#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct
{
  unsigned h, m, s;
  unsigned value;
} Timestamp;

static int timestampOrdering(const void* const lhs,
                             const void* const rhs)
{
  const unsigned x = ((const Timestamp*)lhs)->value,
                 y = ((const Timestamp*)rhs)->value;

  return (x > y) ? (-1) : (x < y);
}

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  static char stdoutBuffer[8192] = "";

  // Turn on full output buffering for stdout.
  setvbuf(stdout, stdoutBuffer, _IOFBF, sizeof stdoutBuffer);

  FILE* inputStream = fopen(argv[1], "r");

  assert(inputStream && "Failed to open input stream.");

  for(char lineBuffer[256] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    Timestamp timestamps[32] = {{0}};
    size_t timestampCount = 0;

    Timestamp* timestamp = timestamps;
    int bytesRead = 0;

    #define ARGUMENT_PACK &timestamp->h, &timestamp->m, &timestamp->s

    // The supplied timestamps are in ISO 8601/Extended format.
    for(const char* cursor = lineBuffer;
        sscanf(cursor, "%u:%u:%u%n", ARGUMENT_PACK, &bytesRead) == 3;
        cursor += bytesRead)
    {
      timestamp->value = (timestamp->h * 3600) +
                         (timestamp->m *   60) +
                         (timestamp->s *    1);

      timestamp = &timestamps[++timestampCount];

      assert(timestampCount < (sizeof timestamps / sizeof *timestamps));
    }

    #undef ARGUMENT_PACK

    qsort(timestamps, timestampCount, sizeof(Timestamp), timestampOrdering);

    for(size_t i = 0; i < timestampCount; ++i)
    {
      const Timestamp* const timestamp = &timestamps[i];

      printf("%02u:%02u:%02u ", timestamp->h, timestamp->m, timestamp->s);
    }

    putchar('\n');
  }

  // The CRT takes care of cleanup.
}
