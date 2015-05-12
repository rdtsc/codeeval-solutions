#include <cassert>
#include <cstddef>
#include <iostream>

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

  const std::size_t index = (n < 0 ? -n : n);

  assert(index < (sizeof lut / sizeof *lut));

  return lut[index];
}

int main()
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  static bool grid[640][640] = {};

  static_assert((sizeof grid / sizeof *grid) == (sizeof *grid / sizeof **grid),
    "The grid should be square.");

  static constexpr auto gridCenter = ((sizeof grid / sizeof *grid) / 2);

  static struct {int x, y;} moves[30000] = {};

  std::size_t result = 0;

  for(std::size_t i = 1; i--;)
  {
    const auto x = moves[i].x, y = moves[i].y;

    const auto isCellEmpty = [&](const int xOffset, const int yOffset)
    {
      // Patch the coordinates because the origin is in the middle of the grid.
      return !grid[y + (yOffset) + gridCenter][x + (xOffset) + gridCenter];
    };

    const auto addMove = [&](const int xOffset, const int yOffset)
    {
      if(isCellEmpty(xOffset, yOffset))
      {
        assert(i < (sizeof moves / sizeof *moves));

        moves[i++] = {x + xOffset, y + yOffset};
      }
    };

    // Arbitrary criteria, as per the problem statement.
    if(isCellEmpty(0, 0) && (::getDigitSum(x) + ::getDigitSum(y) <= 19))
    {
      ++result;

      // Make sure we don't visit here again.
      grid[y + gridCenter][x + gridCenter] = true;

      // Visit our immediate neighbors, if necessary.
      addMove(+1, 0); addMove(0, +1);
      addMove(-1, 0); addMove(0, -1);
    }
  }

  std::cout << result << '\n';
}
