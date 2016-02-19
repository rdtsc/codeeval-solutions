#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  static char stdoutBuffer[1024] = "";

  // Turn on full output buffering for stdout.
  setvbuf(stdout, stdoutBuffer, _IOFBF, sizeof stdoutBuffer);

  FILE* inputStream = fopen(argv[1], "r");

  assert(inputStream && "Failed to open input stream.");

  for(char lineBuffer[128] = "";
    fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    for(const char* chunk = strtok(lineBuffer, "|"); chunk;)
    {
      unsigned long value = strtoul(chunk, NULL, 2);

      assert(value && (value <= 100));

      // Low upper bound, as per the problem statement.
      value ^= (value >> 1);
      value ^= (value >> 2);
      value ^= (value >> 4);

      printf("%lu", value);

      if((chunk = strtok(NULL, "|"))) fputs(" | ", stdout);
    }

    putchar('\n');
  }

  // The CRT takes care of cleanup.
}
