#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <iterator>

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  using input_iterator  = std::istream_iterator<std::uint_fast64_t>;
  using output_iterator = std::ostream_iterator<const char*>;

  std::transform(input_iterator(inputStream),
                 input_iterator(),
                 output_iterator(std::cout, "\n"),
                 [](const input_iterator::value_type n)
  {
    const unsigned digitCount = std::log10(n) + 1;

    auto armstrongSum = decltype(n){};

    for(auto i = n; i; i /= 10)
    {
      armstrongSum += std::pow(i % 10, digitCount);
    }

    return (armstrongSum == n) ? ("True") : ("False");
  });
}
