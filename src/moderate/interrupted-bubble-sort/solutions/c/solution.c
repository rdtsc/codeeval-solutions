#include <assert.h>
#include <inttypes.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  static char stdoutBuffer[512] = "";

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
    for(const char* lhs = strtok(lineBuffer, "|");
        sscanf(lhs, "%u%n", &values[valueCount], &bytesRead) == 1;
        ++valueCount, lhs += bytesRead);

    uint_fast64_t requestedIterations = 0;

    // Fetch the number of iterations to perform.
    sscanf(strtok(NULL, "\n"), "%" SCNuFAST64, &requestedIterations);

    assert(valueCount && requestedIterations);

    requestedIterations = requestedIterations < valueCount ?
                          requestedIterations : valueCount;

    // Apply bubble sort for N iterations.
    for(size_t i = 0; i < requestedIterations; ++i)
    {
      for(size_t j = 1, k = 0; k < valueCount - 1; ++j, ++k)
      {
        if(values[j] < values[k])
        {
          values[j] ^= values[k];
          values[k] ^= values[j];
          values[j] ^= values[k];
        }
      }
    }

    for(size_t i = 0; i < valueCount; ++i) printf("%u ", values[i]);

    putchar('\n');
  }

  // The CRT takes care of cleanup.
}
