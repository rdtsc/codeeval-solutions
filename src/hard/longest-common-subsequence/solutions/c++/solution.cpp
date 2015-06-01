#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <string>

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  for(std::string lhs, rhs; std::getline(inputStream, lhs, ';') &&
                            std::getline(inputStream, rhs);)
  {
    std::cout << [&]
    {
      const auto lhsSize = lhs.size(),
                 rhsSize = rhs.size();

      // Input strings aren't too long in CE's test cases. This yields a
      // slightly higher memory score vs the default std::allocator used by
      // standard containers.
      unsigned table[256][256] = {};

      assert(lhsSize < (sizeof  table / sizeof  *table));
      assert(rhsSize < (sizeof *table / sizeof **table));

      // Per: https://en.wikipedia.org/wiki/Longest_common_subsequence_problem
      {
        const auto lhsBegin = lhs.cbegin(), lhsEnd = lhs.cend(),
                   rhsBegin = rhs.cbegin(), rhsEnd = rhs.cend();

        for(auto a = lhsBegin; a != lhsEnd; ++a)
        for(auto b = rhsBegin; b != rhsEnd; ++b)
        {
          #define VAL(xOffset, yOffset) \
            table[(a - lhsBegin) + (yOffset)][(b - rhsBegin) + (xOffset)]

          if(*a != *b) VAL(1, 1) = std::max(VAL(1, 0), VAL(0, 1));

          else VAL(1, 1) = VAL(0, 0) + 1;

          #undef VAL
        }
      }

      // Repurposing as an output buffer.
      rhs.clear();

      // Reconstruct the subsequence from the bottom-right to the top-left.
      for(int y = lhsSize, x = rhsSize; (y > 0 && x > 0); --y, --x)
      {
        const auto cell = table[y][x],
                   top  = table[y - 1][x],
                   left = table[y][x - 1];

        if(cell == top) ++x;

        else if(cell == left) ++y;

        else rhs.insert(0, 1, lhs[y - 1]);
      }

      return rhs;
    }() << '\n';
  }
}
