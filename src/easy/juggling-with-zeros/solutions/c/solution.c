#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  static char stdoutBuffer[384] = "";

  // Turn on full output buffering for stdout.
  setvbuf(stdout, stdoutBuffer, _IOFBF, sizeof stdoutBuffer);

  FILE* inputStream = fopen(argv[1], "r");

  assert(inputStream && "Failed to open input stream.");

  for(char lineBuffer[256] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    uint_fast64_t result = 0;

    for(const char* flag = strtok(lineBuffer, " ");
        flag; flag = strtok(NULL, " "))
    {
      // "00" indicates that the following sequence of zeros should be
      // "transformed" into ones.
      const bool shouldInvertSequence = (flag[1] != '\0');

      // Process the sequence of zeros and invert it, if required.
      for(const char* c = strtok(NULL, " \n"); c && *c; ++c)
      {
        result <<= 1;
        result += shouldInvertSequence;
      }
    }

    printf("%" PRIuFAST64 "\n", result);
  }

  // The CRT takes care of cleanup.
}
