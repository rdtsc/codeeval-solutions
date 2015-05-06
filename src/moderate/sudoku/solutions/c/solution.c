#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

static bool isValidSudokuSlice(unsigned char grid[][9],
                               const size_t index,
                               const size_t gridSize)
{
  assert(grid && (index < 9) && (gridSize == 4 || gridSize == 9));
  assert((sizeof(unsigned) * CHAR_BIT) > gridSize);

  unsigned rowHistory = 0,
           colHistory = 0;

  // Check row and column uniqueness at the current index.
  for(size_t i = 0; i < gridSize; ++i)
  {
    const unsigned rowDigit = (1 << grid[index][i]),
                   colDigit = (1 << grid[i][index]);

    if((rowHistory & rowDigit) || (colHistory & colDigit)) return false;

    rowHistory |= rowDigit;
    colHistory |= colDigit;
  }

  return true;
}

static bool isValidSudokuSubGrid(unsigned char grid[][9],
                                 const size_t index,
                                 const size_t gridSize)
{
  assert(grid && (index < 9) && (gridSize == 4 || gridSize == 9));
  assert((sizeof(unsigned) * CHAR_BIT) > gridSize);

  // Square root.
  const size_t subGridSize = (gridSize == 9 ? 3 : 2);

  const size_t yOffset = (index / subGridSize) * subGridSize,
               xOffset = (index % subGridSize) * subGridSize;

  unsigned history = 0;

  // Check digit uniqueness in current sub-grid.
  for(size_t y = yOffset; y < (yOffset + subGridSize); ++y)
  for(size_t x = xOffset; x < (xOffset + subGridSize); ++x)
  {
    const unsigned digit = (1 << grid[y][x]);

    if(history & digit) return false;

    history |= digit;
  }

  return true;
}

static bool isValidSudokuGrid(unsigned char grid[][9], const size_t gridSize)
{
  assert((gridSize == 4 || gridSize == 9) && grid);

  // Check for digit uniqueness in all rows, columns, and sub-grids.
  for(size_t i = 0; i < gridSize; ++i)
  {
    if(!isValidSudokuSlice(grid, i, gridSize) ||
       !isValidSudokuSubGrid(grid, i, gridSize))
    {
      return false;
    }
  }

  return true;
}

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
    // Guaranteed to only be dealing with 4x4 or 9x9 grids.
    assert(*lineBuffer == '4' || *lineBuffer == '9');

    unsigned char solution[9][9] = {{0}};
    const size_t gridSize = (*lineBuffer - '0');

    // Skip grid width hint.
    const char* c = (lineBuffer + 2);

    for(size_t y = 0; y < gridSize; ++y)
    for(size_t x = 0; x < gridSize; ++x, c += 2)
    {
      assert(isdigit(*c));
      assert(*(c + 1) == ',' || *(c + 1) == '\n' || !*(c + 1));

      solution[y][x] = (*c - '0');
    }

    puts(isValidSudokuGrid(solution, gridSize) ? "True" : "False");
  }

  // The CRT takes care of cleanup.
}
