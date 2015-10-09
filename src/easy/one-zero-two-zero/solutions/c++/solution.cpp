#include <cassert>
#include <cstddef>
#include <fstream>
#include <iostream>

// Number of 0's in the binary representation of N.
template<std::size_t CacheSize = 1001, typename T>
static unsigned getZeroCount(const T n)
{
  assert(n < CacheSize);

  static unsigned resultCache[CacheSize] = {};
  static bool resultHistory[CacheSize] = {};

  if(resultHistory[n]) return resultCache[n];

  unsigned result = 0;

  for(auto k = n; k; k >>= 1) if(!(k & 1)) ++result;

  resultCache[n] = result;
  resultHistory[n] = true;

  return result;
}

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  for(unsigned lhs = 0, rhs = 0; inputStream >> lhs >> rhs;)
  {
    std::size_t occurrences = 0;

    // Search space is [1, rhs].
    for(auto i = decltype(rhs){1}; i <= rhs; ++i)
    {
      if(::getZeroCount(i) == lhs) ++occurrences;
    }

    std::cout << occurrences << '\n';
  }
}
