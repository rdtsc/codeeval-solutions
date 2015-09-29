#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>

static std::uint_least64_t getVariantCount(const std::string& data)
{
  assert(!(data.size() & 1));

  assert(std::all_of(data.cbegin(), data.cend(), [](const char c)
  {
    return (c == 'A' || c == 'B' || c == '*');
  }));

  const auto dataLeft  = data.cbegin(),
             dataRight = std::next(dataLeft, (data.size() / 2));

  unsigned asteriskCount = 0;

  for(auto lhs = dataLeft, rhs = dataRight; lhs != dataRight; ++lhs, ++rhs)
  {
    // Wildcard pair.
    if((*lhs == '*') && (*rhs == '*')) ++asteriskCount;

    // Complete mismatch between letters.
    else if((*lhs != *rhs) && !(*lhs == '*' || *rhs == '*')) return 0;
  }

  // |{A, B}| = 2.
  return ::pow(2, asteriskCount);
}

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  for(std::string line; std::getline(inputStream, line);)
  {
    std::cout << ::getVariantCount(line) << '\n';
  }
}
