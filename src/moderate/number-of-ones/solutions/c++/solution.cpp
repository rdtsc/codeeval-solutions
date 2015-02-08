#include <bitset>
#include <cassert>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <limits>

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  for(std::uint_fast64_t n = 0; inputStream >> n;)
  {
    static constexpr auto typeWidth = std::numeric_limits<decltype(n)>::digits;

    std::cout << (std::bitset<typeWidth>(n)).count() << '\n';
  }
}
