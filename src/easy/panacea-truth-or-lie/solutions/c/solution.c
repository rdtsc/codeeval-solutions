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

  static char stdoutBuffer[256] = "";

  // Turn on full output buffering for stdout.
  setvbuf(stdout, stdoutBuffer, _IOFBF, sizeof stdoutBuffer);

  FILE* inputStream = fopen(argv[1], "r");

  assert(inputStream && "Failed to open input stream.");

  for(char lineBuffer[256] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    unsigned lhs = 0, rhs = 0;

    // Tally lhs in base 16.
    {
      const char* chunk = strtok(lineBuffer, "|");

      assert(chunk);

      unsigned value = 0;
      int bytesRead  = 0;

      while(sscanf(chunk, "%x%n", &value, &bytesRead) == 1)
      {
        lhs   += value;
        chunk += bytesRead;
      }
    }

    // Tally rhs in base 2.
    {
      const char* chunk = strtok(NULL, " ");

      assert(chunk);

      while(chunk)
      {
        rhs  += strtoul(chunk, NULL, 2);
        chunk = strtok(NULL, " \n");
      }
    }

    // As per the problem statement.
    assert((lhs >= 60) && (lhs <= 1500));
    assert((rhs >= 60) && (rhs <= 1500));

    puts(lhs <= rhs ? "True" : "False");
  }

  // The CRT takes care of cleanup.
}
