#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

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

  for(char lineBuffer[256] = "", * line = lineBuffer;
      fgets(line, sizeof lineBuffer, inputStream);
      line = lineBuffer)
  {
    size_t lineLength = strlen(line);

    // Get line length, less trailing whitespace.
    for(const char* c = line + lineLength - 1; isspace(*c); --c, --lineLength);

    assert(lineLength);

    // Elements in the matrix.
    const size_t elementCount = (lineLength / 2) + 1;

    // N in the N-by-N matrix.
    const size_t n = sqrt(elementCount);

    assert(n * n == elementCount);

    for(size_t i = 0; i < n; ++i)
    {
      for(size_t j = 1; j <= n; ++j)
      {
        /*
          |0 1 2|    |6 3 0|
          |3 4 5| -> |7 4 1| -> "6 3 0 7 4 1 8 5 2"
          |6 7 8|    |8 5 2|

          L = 9; element count.
          N = 3; side length.

          '6' -> (L - 1N + 0)  |  '3' -> (L - 2N + 0)  |  '0' -> (L - 3N + 0)
          '7' -> (L - 1N + 1)  |  '4' -> (L - 2N + 1)  |  '1' -> (L - 3N + 1)
          '8' -> (L - 1N + 2)  |  '5' -> (L - 2N + 2)  |  '2' -> (L - 3N + 2)
        */

        putchar(line[(elementCount - (n * j) + i) * 2]);
        putchar(' ');
      }
    }

    putchar('\n');
  }

  // The CRT takes care of cleanup.
}
