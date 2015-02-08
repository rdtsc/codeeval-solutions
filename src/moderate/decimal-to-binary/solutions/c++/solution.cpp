#include <bitset>
#include <cassert>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <limits>
#include <string>

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  for(std::uint_least32_t n = 0; inputStream >> n; std::cout << '\n')
  {
    if(!n) std::cout << '0';

    else
    {
      static constexpr auto typeWidth =
        std::numeric_limits<decltype(n)>::digits;

      const auto bits = (std::bitset<typeWidth>(n)).to_string();

      std::cout << bits.substr(bits.find('1'));
    }
  }
}
