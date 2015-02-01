#include <assert.h>
#include <ctype.h>
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

  for(size_t n = 0; fscanf(inputStream, "%zu", &n) == 1;)
  {
    uint_fast32_t matrix[32][32] = {{0}};
    size_t lineCount = 0;

    assert(n >= 2 && n <= sizeof matrix / sizeof *matrix);

    for(char lineBuffer[1024] = "", * line = lineBuffer;
        lineCount < n && fgets(line, sizeof lineBuffer, inputStream);
        line = lineBuffer)
    {
      if(*line == '\n') continue;

      uint_fast32_t term = 0;
      size_t colCount = 0;

      // Fetch matrix terms.
      for(;*line && *line != '\n'; ++line)
      {
        if(isdigit(*line)) term = (term * 10) + (*line - '0');

        // We're at a term boundary.
        else if(*line == ',')
        {
          matrix[lineCount][colCount++] = term;
          term = 0;
        }
      }

      // Record the last term in each row.
      matrix[lineCount][colCount++] = term;

      ++lineCount;
    }

    // Patch the bottom row and rightmost column with local sums moving
    // from the destination at the bottom right.
    for(size_t y = --n, x = y; y--;)
    {
      matrix[y][x] += matrix[y + 1][x];
      matrix[x][y] += matrix[x][y + 1];
    }

    // Start from the destination at the bottom right and work our way up to the
    // top left by selecting and recording the smallest local sum at each hop.
    for(size_t y = n; y--;) for(size_t x = n; x--;)
    {
      matrix[y][x] += matrix[y + 1][x] < matrix[y][x + 1] ? matrix[y + 1][x] :
                                                            matrix[y][x + 1];
    }

    // We're at the origin.
    printf("%" PRIuFAST32 "\n", **matrix);
  }

  // The CRT takes care of cleanup.
}
