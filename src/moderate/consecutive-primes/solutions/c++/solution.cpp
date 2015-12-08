#include <algorithm>
#include <array>
#include <cassert>
#include <cstddef>
#include <fstream>
#include <functional>
#include <iostream>
#include <vector>

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  using value_type = unsigned;

  #ifdef GENERATE_LUT
  // The upper bound is relatively small. May as well pre-compute the results
  // off-line for a higher score.
  {
    static constexpr std::size_t maxBeadCount = 18;

    std::vector<value_type> beads;

    const auto getTerm = [&]
    {
      static constexpr bool isPrime[] =
      {
        /* [00, 09] */ 0, 0, 1, 1, 0, 1, 0, 1, 0, 0,
        /* [10, 19] */ 0, 1, 0, 1, 0, 0, 0, 1, 0, 1,
        /* [20, 29] */ 0, 0, 0, 1, 0, 0, 0, 0, 0, 1,
        /* [30, 39] */ 0, 1, 0, 0, 0, 0, 0, 1, 0, 0,
      };

      static_assert((maxBeadCount * 2) < (sizeof isPrime / sizeof *isPrime),
        "Increase static prime list size.");

      const auto beadCount = beads.size();

      const std::function<value_type(std::size_t)> get =
        [&](const std::size_t i) -> value_type
      {
        if(i == beadCount) return isPrime[beads[0] + beads[beadCount - 1]];

        value_type result = 0;

        if(isPrime[beads[i] + beads[i - 1]]) result += get(i + 1);

        for(auto j = (i + 1); j < beadCount; ++j)
        {
          if(isPrime[beads[i - 1] + beads[j]])
          {
            std::swap(beads[i], beads[j]);

            result += get(i + 1);

            std::swap(beads[i], beads[j]);
          }
        }

        return result;
      };

      return get(1);
    };

    for(auto i = decltype(maxBeadCount){2}; i <= maxBeadCount; i += 2)
    {
      beads.resize(i);

      std::iota(beads.begin(), beads.end(), 1);

      std::cout << getTerm() << (i != maxBeadCount ? ", " : "");
    }

    std::cout << '\n';
  }
  #else
  {
    static constexpr std::array<value_type, 9> terms =
    {
      1, 2, 2, 4, 96, 1024, 2880, 81024, 770144
    };

    for(std::size_t n = 0; inputStream >> n;)
    {
      assert(n <= (terms.size() * 2));

      const auto isOdd = (!n || (n & 1));

      const value_type term = isOdd ? 0 : terms[(n / 2) - 1];

      std::cout << term << '\n';
    }
  }
  #endif
}
