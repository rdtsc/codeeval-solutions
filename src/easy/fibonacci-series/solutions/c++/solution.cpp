#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <iostream>

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  // Fibonacci sequence terms. Terms above 93 are wider than 64 bits.
  const auto sequence = []
  {
    std::array<std::uint_fast64_t, 94> result = {0, 1};

    constexpr auto sequenceSize = result.size();

    for(std::size_t i = 2; i < sequenceSize; ++i)
    {
      result[i] = result[i - 1] + result[i - 2];
    }

    return result;
  }();

  for(std::size_t n = 0; inputStream >> n;)
  {
    assert(n < sequence.size());

    std::cout << sequence[n] << '\n';
  }
}
