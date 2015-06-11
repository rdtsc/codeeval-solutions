#include <algorithm>
#include <bitset>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <iterator>
#include <limits>

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  using input_iterator  = std::istream_iterator<std::uint_fast64_t>;
  using output_iterator = std::ostream_iterator<unsigned>;

  std::transform(input_iterator(inputStream),
                 input_iterator(),
                 output_iterator(std::cout, "\n"),
                 [](const input_iterator::value_type n)
  {
    /*
      A = {0, 1, 2};

      0_10     = 0_2               -> (0 % |A|) = 0;
      5_10     = 101_2             -> (2 % |A|) = 2;
      101_10   = 1100101_2         -> (4 % |A|) = 1;
      25684_10 = 110010001010100_2 -> (6 % |A|) = 0;
    */

    static constexpr auto typeWidth =
      std::numeric_limits<decltype(n)>::digits;

    return (std::bitset<typeWidth>(n).count() % 3);
  });
}
