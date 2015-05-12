#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

static inline unsigned getDigitSum(const int n)
{
  static const unsigned lut[] =
  {
    /*
      #!/usr/bin/env python

      for i in range(0, 501):
        if i and not (i % 15): print()

        digitSum = sum(map(int, str(i)))

        print((' ' if (digitSum < 10) else '') + str(digitSum), end=', ')
    */

    #include "sequence.dat"
  };

  const size_t index = (n < 0 ? -n : n);

  assert(index < (sizeof lut / sizeof *lut));

  return lut[index];
}

int main(void)
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  static bool grid[640][640] = {{false}};

  assert((sizeof grid / sizeof *grid) == (sizeof *grid / sizeof **grid));

  static const size_t gridCenter = ((sizeof grid / sizeof *grid) / 2);

  static struct {int x, y;} moves[30000] = {{0}};

  size_t result = 0;

  for(size_t i = 1; i--;)
  {
    const int x = moves[i].x, y = moves[i].y;

    // Patch the coordinates because the origin is in the middle of the grid.
    #define IS_CELL_EMPTY(xOffset, yOffset) \
      (!grid[y + (yOffset) + gridCenter][x + (xOffset) + gridCenter])

    // Arbitrary criteria, as per the problem statement.
    if(IS_CELL_EMPTY(0, 0) && (getDigitSum(x) + getDigitSum(y) <= 19))
    {
      ++result;

      // Make sure we don't visit here again.
      grid[y + gridCenter][x + gridCenter] = true;

      #define ADD_MOVE(X, Y) if(IS_CELL_EMPTY((X), (Y))) { \
        assert(i < (sizeof moves / sizeof *moves));        \
        moves[i].x = x + (X);                              \
        moves[i].y = y + (Y);                              \
        ++i;                                               \
      }

      // Visit our immediate neighbors, if necessary.
      ADD_MOVE(+1, 0); ADD_MOVE(0, +1);
      ADD_MOVE(-1, 0); ADD_MOVE(0, -1);

      #undef ADD_MOVE
      #undef IS_CELL_EMPTY
    }
  }

  printf("%zu\n", result);
}
