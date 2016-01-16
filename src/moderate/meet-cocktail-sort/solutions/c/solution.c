#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  static char stdoutBuffer[4096] = "";

  // Turn on full output buffering for stdout.
  setvbuf(stdout, stdoutBuffer, _IOFBF, sizeof stdoutBuffer);

  FILE* inputStream = fopen(argv[1], "r");

  assert(inputStream && "Failed to open input stream.");

  for(char lineBuffer[384] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    unsigned values[64] = {0};
    size_t valueCount = 0;
    int bytesRead = 0;

    // Fetch the values.
    for(const char* cursor = strtok(lineBuffer, "|");
        sscanf(cursor, "%u%n", &values[valueCount], &bytesRead) == 1;
        ++valueCount, cursor += bytesRead);

    size_t iterationCount = 0;

    // Fetch the number of iterations to perform.
    sscanf(strtok(NULL, "\n"), "%zu", &iterationCount);

    assert(valueCount);

    // As per the problem statement.
    assert((iterationCount >= 1) && (iterationCount <= 30));

    for(size_t i = 0; i < iterationCount; ++i)
    {
      #define SWAP(a, b) \
        do {(a) ^= (b); (b) ^= (a); (a) ^= (b);} while(false)

      for(size_t j = 0; j < (valueCount - 1); ++j)
      {
        if(values[j] > values[j + 1]) SWAP(values[j], values[j + 1]);
      }

      for(size_t j = (valueCount - 1); j; --j)
      {
        if(values[j] < values[j - 1]) SWAP(values[j], values[j - 1]);
      }

      #undef SWAP
    }

    for(size_t i = 0; i < valueCount; ++i) printf("%u ", values[i]);

    putchar('\n');
  }

  // The CRT takes care of cleanup.
}
