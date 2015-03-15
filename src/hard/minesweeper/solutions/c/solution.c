#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  static char stdoutBuffer[1536] = "";

  // Turn on full output buffering for stdout.
  setvbuf(stdout, stdoutBuffer, _IOFBF, sizeof stdoutBuffer);

  FILE* inputStream = fopen(argv[1], "r");

  assert(inputStream && "Failed to open input stream.");

  for(char lineBuffer[256] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    // Size of the minefield: M rows by N columns.
    size_t m = 0, n = 0;

    char* line = lineBuffer;

    // Get M.
    for(;*line != ','; ++line)
    {
      assert(*line >= '0' && *line <= '9');

      m = (m * 10) + (*line - '0');
    }

    // Get N.
    for(++line; *line != ';'; ++line)
    {
      assert(*line >= '0' && *line <= '9');

      n = (n * 10) + (*line - '0');
    }

    assert(*line == ';');

    // Eat the delimiter.
    ++line;

    bool minefield[10][10] = {{false}};

    assert(m < sizeof minefield / sizeof *minefield);
    assert(n < sizeof *minefield / sizeof **minefield);

    // Reconstruct the minefield. Asterisks represent mines.
    for(size_t y = 0; y < m; ++y)
    for(size_t x = 0; x < n; ++x)
    {
      assert(*line == '.' || *line == '*');

      minefield[y][x] = (*line++ == '*');
    }

    // Reuse the input buffer for output.
    line = lineBuffer;
    line[m * n] = '\0';

    for(size_t y = 0; y < m; ++y)
    for(size_t x = 0; x < n; ++x, ++line)
    {
      // Mines get dumped verbatim.
      if(minefield[y][x]) *line = '*';

      // Empty cells display the total number of mines in all eight of
      // their adjacent fields.
      else
      {
        #define IS_MINE(xOffset, yOffset)                     \
          (((!x && (xOffset) < 0) || (x + (xOffset) >= n)  || \
          (  !y && (yOffset) < 0) || (y + (yOffset) >= m)) ?  \
          (0) : (minefield[y + (yOffset)][x + (xOffset)]))

        /*
          Possible neighbor position offsets.

          <A> -1 + + +
          <B>  0 + @ +
          <C>  1 + + +
                -1 0 1
        */

        const unsigned adjacentMineCount =
          IS_MINE(-1, -1) + IS_MINE(0, -1) + IS_MINE(+1, -1) + // <A>
          IS_MINE(-1,  0) + (000000000000) + IS_MINE(+1,  0) + // <B>
          IS_MINE(-1, +1) + IS_MINE(0, +1) + IS_MINE(+1, +1);  // <C>

        *line = adjacentMineCount + '0';

        #undef IS_MINE
      }
    }

    puts(lineBuffer);
  }

  // The CRT takes care of cleanup.
}
