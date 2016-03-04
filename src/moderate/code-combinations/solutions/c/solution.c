#include <assert.h>
#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

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

  for(char lineBuffer[256] = "";
    fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    size_t w = 0, h = 0;
    char cells[11][11] = {""};

    for(const char* slice = strtok(lineBuffer, "| \n"); slice;
      slice = strtok(NULL, "| \n"))
    {
      // As per the problem statement.
      assert((strlen(slice) >= 2) && (strlen(slice) <= 10));

      strncpy(cells[h++], slice, (sizeof *cells / sizeof **cells - 1));
    }

    w = strlen(*cells);

    unsigned matchCount = 0;

    for(size_t y = 0; y < (h - 1); ++y)
    for(size_t x = 0; x < (w - 1); ++x)
    {
      unsigned char histogram[CHAR_MAX] = {0};

      /*
                     (0, 0) +--+ (1, 0)
                            |  |
                     (0, 1) +--+ (1, 1)
      */

      ++histogram[(unsigned char)cells[y + 0][x + 0]];
      ++histogram[(unsigned char)cells[y + 0][x + 1]];
      ++histogram[(unsigned char)cells[y + 1][x + 0]];
      ++histogram[(unsigned char)cells[y + 1][x + 1]];

      matchCount += (histogram['c'] == 1) &&
                    (histogram['o'] == 1) &&
                    (histogram['d'] == 1) &&
                    (histogram['e'] == 1);
    }

    printf("%u\n", matchCount);
  }

  // The CRT takes care of cleanup.
}
